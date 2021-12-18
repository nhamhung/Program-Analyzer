#include "QueryParser.h"

#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>

#include "parser_utils/ExpressionParser.h"
#include "query_processor/commons/BooleanSemanticError.h"
#include "query_processor/commons/query/clause/Clause.h"
#include "query_processor/commons/query/clause/ClauseParam.h"
#include "query_processor/commons/query/entities/DesignEntity.h"
#include "query_processor/commons/query/entities/SelectedEntity.h"
#include "query_processor/commons/query/utils/QueryUtils.h"
#include "utils/QueryParserUtils.h"
#include "utils/QueryRegex.h"
#include "utils/Extension.h"


namespace query_processor {

bool QueryParser::is_boolean_query = false;
bool QueryParser::is_semantic_error = false;
bool QueryParser::has_declaration_error = false;

void QueryParser::ResetBooleans() {
  QueryParser::is_boolean_query = false;
  QueryParser::is_semantic_error = false;
  QueryParser::has_declaration_error = false;
}

Query QueryParser::ParseQuery(std::string& query_string) {
  try {
    Query return_query = QueryParser::GenerateQuery(query_string);
    return return_query;
  } catch (std::runtime_error& error) {
    if (QueryParser::is_boolean_query && QueryParser::is_semantic_error) {
      throw BooleanSemanticError(error.what(), 0, 0);
    } else {
      throw error;
    }
  }
}

Query QueryParser::GenerateQuery(std::string& query_string) {
  /*
		Generates a Query using the input query string.
		
		A valid query string follows 4 main rules:
			- It starts with some number of declarations, and ends with a Select clause.
			- All design entities in the Select clause must have been declared.
			- All synonyms in declarations must be unique.
			- Each design entity must only be declared once (but can have multiple synonyms).

		Generally, any amount and type of whitespace is allowed, at any point in the query.
	*/

  QueryParser::ResetBooleans();

  std::string& remaining_query_string = QueryParserUtils::StripWhitespace(query_string);

  // Maintain a mapping of synonyms and their design entity types to check for duplicates
  std::map<std::string, DesignEntityType> synonym_to_de_type_mappings;

  QueryParser::ParseDeclarationClauses(remaining_query_string, synonym_to_de_type_mappings);

  Query return_query = QueryParser::ParseSelectPhrase(remaining_query_string, synonym_to_de_type_mappings);

  return_query = QueryParser::ParseConditionalClauses(remaining_query_string, synonym_to_de_type_mappings, return_query);

  return return_query;
}

void QueryParser::ParseDeclarationClauses(std::string& remaining_query_string, std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  while (std::regex_search(remaining_query_string, QueryRegex::STARTS_WITH_VALID_DECLARATION_FIRST_SYNONYM_REGEX)) {
    // First, pop the first synonym (which comes with the design entity type)
    std::pair<std::string, std::string> split_query_string = QueryParserUtils::PopMatchFromFront(
        remaining_query_string, QueryRegex::STARTS_WITH_VALID_DECLARATION_FIRST_SYNONYM_REGEX);
    std::string declaration_type_and_first_synonym = QueryParserUtils::StripWhitespace(split_query_string.first);
    remaining_query_string = split_query_string.second;

    // Identify the design entity as the first token separated by spaces
    std::string design_entity_type_as_string = QueryParserUtils::SplitStringAlongChar(declaration_type_and_first_synonym, ' ').at(0);
    DesignEntityType design_entity_type = QueryUtils::ConvertStringToDesignEntityType(design_entity_type_as_string);

    // Identify the first synonym and initialize the vector used to hold all the synonyms for this declaration clause
    std::string declaration_first_synonym = QueryParserUtils::SplitStringAlongChar(declaration_type_and_first_synonym, ' ').at(1);
    std::vector<std::string> synonyms = std::vector<std::string>{declaration_first_synonym};

    // Then pop each following synonym in turn
    while (std::regex_search(remaining_query_string, QueryRegex::STARTS_WITH_VALID_DECLARATION_OTHER_SYNONYM_REGEX)) {
      std::pair<std::string, std::string> split_declaration_string = QueryParserUtils::PopMatchFromFront(
          remaining_query_string, QueryRegex::STARTS_WITH_VALID_DECLARATION_OTHER_SYNONYM_REGEX);
      std::string next_declaration_synonym_with_comma = split_declaration_string.first;
      std::string next_declaration_synonym = QueryParserUtils::GetAllMatchesFromString(next_declaration_synonym_with_comma,
                                                                                       QueryRegex::VALID_SYNONYM_REGEX)
                                                 .at(0);
      remaining_query_string = split_declaration_string.second;
      synonyms.push_back(QueryParserUtils::StripWhitespace(next_declaration_synonym));
    }

    // Ensure that the declaration clause ends as it should (with a semicolon)
    if (!std::regex_search(remaining_query_string, QueryRegex::STARTS_WITH_VALID_DECLARATION_CLAUSE_ENDING_REGEX)) {
      throw std::runtime_error("Query does not satisfy the valid PQL concrete grammar for declaration clauses.");
    } else {
      std::pair<std::string, std::string> split_declaration_string = QueryParserUtils::PopMatchFromFront(
          remaining_query_string, QueryRegex::STARTS_WITH_VALID_DECLARATION_CLAUSE_ENDING_REGEX);
      remaining_query_string = split_declaration_string.second;
    }

    // Ensure that no duplicates exist between synonym names
    for (std::string synonym : synonyms) {
      // Remove leading and trailing whitespace to ensure proper comparison
      synonym = QueryParserUtils::StripWhitespace(synonym);

      if (synonym_to_design_entity_type_mappings.count(synonym) == 0) {
        synonym_to_design_entity_type_mappings.insert(std::pair<std::string, DesignEntityType>(synonym, design_entity_type));
      } else {
        // Synonym already used; set to error
        // A disappointing way of resolving the one failure case where
        //   re-declaration of synonym (a semantic error) doesn't return
        //   FALSE upon Select BOOLEAN because the parser hasn't reached there yet.
        QueryParser::has_declaration_error = true;
        break;
      }
    }

    if (QueryParser::has_declaration_error) {
      break;
    }
  }
}

Query QueryParser::ParseSelectPhrase(std::string& remaining_query_string, std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  // Ensure that exactly one match of the Select phrase (i.e. "Select (BOOLEAN/synonym/tuple/attribute)") exists
  if (!std::regex_search(remaining_query_string, QueryRegex::STARTS_WITH_VALID_SELECT_PHRASE_REGEX)) {
    throw std::runtime_error("Query does not satisfy the PQL concrete grammar.");
  }

  std::pair<std::string, std::string> select_clause_components = QueryParserUtils::PopMatchFromFront(remaining_query_string, QueryRegex::STARTS_WITH_VALID_SELECT_PHRASE_REGEX);
  std::string select_phrase = select_clause_components.first;
  remaining_query_string = select_clause_components.second;

  // Obtain the selected entity from the 'Select' phrase
  std::string select_phrase_synonym = QueryParserUtils::PopMatchFromFront(select_phrase, QueryRegex::STARTS_WITH_SELECT_STRING_REGEX).second;
  select_phrase_synonym = QueryParserUtils::StripWhitespace(select_phrase_synonym);

  Query return_query;

  if (std::regex_match(select_phrase_synonym, QueryRegex::STARTS_WITH_BOOLEAN_STRING_REGEX)) {
    // This is to ensure that 'Select BOOLEANwith 1 = 1' is treated as syntactic and not semantic
    if (std::regex_match(select_phrase_synonym, QueryRegex::STARTS_WITH_VALID_SELECT_BOOLEAN_REGEX)) {
      QueryParser::is_boolean_query = true;

      // This is used to resolve the one case where a re-declaration of synonym
      //	 is not considered as a semantic BOOLEAN error, because the parser hasn't
      //	 reached this part yet.
      if (QueryParser::has_declaration_error) {
        QueryParser::ThrowSemanticError("Synonym re-declared in declaration clauses.");
      }

      return Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    } else {
      throw std::runtime_error("Select phrase entity starts with 'BOOLEAN' but is not 'BOOLEAN'.");
    }
  }

  // Fail-fast, since there is no point postponing it any longer.
  if (QueryParser::has_declaration_error) {
    QueryParser::ThrowSemanticError("Synonym re-declared in declaration clauses.");
  }

  if (std::regex_match(select_phrase_synonym, QueryRegex::STARTS_WITH_VALID_ATTRIBUTE_REFERENCE_REGEX)) {
    return_query = Query(SelectedEntity(QueryParser::CreateAttributeSelectedEntityFromString(select_phrase_synonym, synonym_to_design_entity_type_mappings)));
  } else if (std::regex_match(select_phrase_synonym, QueryRegex::STARTS_WITH_VALID_SELECT_TUPLE_REGEX)) {
    std::vector<SelectedEntity> tuple_entities{};

    // Clean up the angle brackets from the start and end
    select_phrase_synonym = std::regex_replace(select_phrase_synonym, QueryRegex::STARTS_OR_ENDS_WITH_VALID_TUPLE_ANGLE_BRACKETS_REGEX, "");

    std::vector<std::string> tuple_elements = QueryParserUtils::SplitStringAlongChar(select_phrase_synonym, ',');
    for (std::string& element : tuple_elements) {
      element = QueryParserUtils::StripWhitespace(element);

      if (std::regex_match(element, QueryRegex::STARTS_WITH_VALID_SYNONYM_REGEX)) {
        if (synonym_to_design_entity_type_mappings.count(element) == 1) {
          tuple_entities.push_back(SelectedEntity(DesignEntity(synonym_to_design_entity_type_mappings[element], element)));
        } else {
          QueryParser::ThrowSemanticError("Synonym " + element + " used in Select clause but not declared previously.");
        }
      } else {
        // Previous regex validation ensures that it is an attribute, if not a synonym
        tuple_entities.push_back(SelectedEntity(QueryParser::CreateAttributeSelectedEntityFromString(element, synonym_to_design_entity_type_mappings)));
      }
    }

    return_query = Query(tuple_entities);

  } else if (synonym_to_design_entity_type_mappings.count(select_phrase_synonym) == 1) {
    // If none of the above are true, assume that it is a synonym
    return_query = Query(SelectedEntity(DesignEntity(synonym_to_design_entity_type_mappings[select_phrase_synonym], select_phrase_synonym)));
  } else {
    QueryParser::ThrowSemanticError("Synonym " + select_phrase_synonym + " used in Select clause but not declared previously.");
  }

  return return_query;
}

Query QueryParser::ParseConditionalClauses(std::string& remaining_conditional_clauses, std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings, Query query) {
  ClauseType most_recent_clause_type = ClauseType::UNDEFINED;

  while (std::regex_search(remaining_conditional_clauses, QueryRegex::STARTS_WITH_VALID_CONDITION_CLAUSE_REGEX)) {
    // Substitute out the syntactic sugar 'and' between each clause
    remaining_conditional_clauses = QueryParser::ReplaceKeywordAnd(remaining_conditional_clauses, most_recent_clause_type);

    std::pair<std::string, std::string> split_conditional_clauses = QueryParserUtils::PopMatchFromFront(
        remaining_conditional_clauses, QueryRegex::STARTS_WITH_VALID_CONDITION_CLAUSE_REGEX);
    std::string next_conditional_clause = split_conditional_clauses.first;
    remaining_conditional_clauses = split_conditional_clauses.second;

    if (std::regex_match(next_conditional_clause, QueryRegex::STARTS_WITH_VALID_WITH_CLAUSE_REGEX)) {
      WithClause with_clause = QueryParser::ParseWithClause(next_conditional_clause, synonym_to_design_entity_type_mappings);
      query.AddClause(Clause(with_clause));
      most_recent_clause_type = ClauseType::WITH;
    } else if (std::regex_match(next_conditional_clause, QueryRegex::STARTS_WITH_VALID_PATTERN_CLAUSE_REGEX)) {
      PatternClause pattern_clause = QueryParser::ParsePatternClause(next_conditional_clause, synonym_to_design_entity_type_mappings);
      query.AddClause(Clause(pattern_clause));
      most_recent_clause_type = ClauseType::PATTERN;
    } else if (std::regex_match(next_conditional_clause, QueryRegex::STARTS_WITH_VALID_SUCH_THAT_CLAUSE_REGEX)) {
        SuchThatClause such_that_clause = QueryParser::ParseSuchThatClause(next_conditional_clause,
                                                                           synonym_to_design_entity_type_mappings);
        query.AddClause(Clause(such_that_clause));
        most_recent_clause_type = ClauseType::SUCHTHAT;
    }
  }

  remaining_conditional_clauses = QueryParserUtils::StripWhitespace(remaining_conditional_clauses);
  if (remaining_conditional_clauses != "") {
    throw std::runtime_error("Query does not satisfy the valid PQL concrete grammar for conditional clauses.");
  }

  return query;
}

SuchThatClause QueryParser::ParseSuchThatClause(std::string& such_that_clause, std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  // Identify if each synonym in the 'such that' clause is valid, based on whether it exists in synonym_to_design_entity_type_mappings

  // Identify the design abstraction involved
  std::string design_abstraction_string = QueryParserUtils::GetAllMatchesFromString(such_that_clause,
                                                                                    QueryRegex::VALID_DESIGN_ABSTRACTION_REGEX)
                                              .at(0);
  DesignAbstraction design_abstraction = QueryUtils::ConvertStringToDesignAbstraction(design_abstraction_string);

  // First identify the entire chunk of parameters for the design abstraction, so you can isolate the parameters
  std::string design_abstraction_inputs = QueryParserUtils::GetAllMatchesFromString(such_that_clause,
                                                                                    QueryRegex::VALID_DESIGN_ABSTRACTION_PARAMETERS_REGEX)
                                              .at(0);

  // Obfuscate synonym strings to prevent them from matching the synonym regex specifically,
  //		since they are not subject to the same logic-based checking.
  std::string obfuscated_design_abstraction_inputs = std::regex_replace(design_abstraction_inputs, QueryRegex::VALID_SYNONYM_STRING_REGEX, "~");
  std::vector<std::string> design_abstraction_synonyms = QueryParserUtils::GetAllMatchesFromString(obfuscated_design_abstraction_inputs,
                                                                                                   QueryRegex::VALID_SYNONYM_REGEX);

  // Then, verify that any synonyms were declared.
  for (const std::string& synonym : design_abstraction_synonyms) {
    if (synonym_to_design_entity_type_mappings.count(synonym) == 0) {
      QueryParser::ThrowSemanticError("Synonym " + synonym + " used in 'such that' clause but not declared previously.");
    }
  }

  // Translate the parameters of the design abstraction into a SuchThatClause
  std::vector<std::string> design_abstraction_parameters = QueryParserUtils::GetAllMatchesFromString(design_abstraction_inputs,
                                                                                                     QueryRegex::VALID_DESIGN_ABSTRACTION_PARAMETER_REGEX);
  ClauseParam lhs_clause_param = QueryParser::CreateClauseParamFromString(design_abstraction_parameters.at(0), synonym_to_design_entity_type_mappings);
  ClauseParam rhs_clause_param = QueryParser::CreateClauseParamFromString(design_abstraction_parameters.at(1), synonym_to_design_entity_type_mappings);

  return SuchThatClause(design_abstraction, lhs_clause_param, rhs_clause_param);
}

PatternClause QueryParser::ParsePatternClause(std::string& pattern_clause, std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  // Identify if each synonym in the 'pattern' clause is valid, based on whether it exists in synonym_to_design_entity_type_mappings

  /*
		It is known (via regex validation) that the format is:
			- Synonym
			- "("
			- First parameter
			- ","
			- Second parameter
			- ")"
		Or, if it is an if pattern clauses, then:
			- Synonym
			- "("
			- First parameter
			- ","
			- Second parameter
			- ","
			- Third parameter
			- ")"
		This knowledge is used to split the query appropriately.
	*/

  std::string pattern_parameters = QueryParserUtils::GetAllMatchesFromString(pattern_clause,
                                                                             QueryRegex::VALID_PATTERN_PARAMETERS_REGEX)
                                       .at(0);

  // Split the parameters along the notable tokens (which are guaranteed because the string has been syntactically validated)
  // The strange ordering helps protect against splitting along a parenthesis in the pattern expression instead.
  std::vector<std::string> parameters_split_by_comma = QueryParserUtils::SplitStringAlongChar(pattern_parameters, ',');
  std::vector<std::string> parameters_split_by_left_bracket = QueryParserUtils::SplitStringAlongChar(parameters_split_by_comma.front(), '(');

  // Identify the design entity type of the synonym at the front
  std::string pattern_clause_synonym = QueryParserUtils::StripWhitespace(parameters_split_by_left_bracket.at(0));

  // Verify that the synonym used is a declared variable
  if (synonym_to_design_entity_type_mappings.count(pattern_clause_synonym) == 0) {
    QueryParser::ThrowSemanticError("Synonym " + pattern_clause_synonym + " used at front of 'pattern' clause but not declared previously.");
  }

  // The value of the first parameter is shared between all variants. The second and third are not.
  std::string pattern_clause_first_param = QueryParserUtils::StripWhitespace(parameters_split_by_left_bracket.at(1));
  std::string pattern_clause_second_param;
  std::string pattern_clause_third_param;
  std::vector<std::string> parameters_split_by_right_bracket = QueryParserUtils::SplitStringAlongChar(parameters_split_by_comma.back(), ')');

  // Verify that the synonym used is of assign/if/while type
  switch (synonym_to_design_entity_type_mappings[pattern_clause_synonym]) {
    case DesignEntityType::ASSIGN:
      pattern_clause_second_param = QueryParserUtils::GetAllMatchesFromString(parameters_split_by_comma.at(1),
                                                                              QueryRegex::VALID_ASSIGN_PATTERN_SECOND_PARAMETER_REGEX)
                                        .at(0);
      break;
    case DesignEntityType::WHILE:
      pattern_clause_second_param = QueryParserUtils::StripWhitespace(parameters_split_by_right_bracket.at(0));
      if (pattern_clause_second_param != "_") {
        QueryParser::ThrowSemanticError("'pattern' clause of 'while' type contains a non-wildcard as second parameter.");
      }
      break;
    case DesignEntityType::IF:
      pattern_clause_second_param = QueryParserUtils::StripWhitespace(parameters_split_by_comma.at(1));
      pattern_clause_third_param = QueryParserUtils::StripWhitespace(parameters_split_by_right_bracket.at(0));
      if (pattern_clause_second_param != "_" || pattern_clause_third_param != "_") {
        QueryParser::ThrowSemanticError("'pattern' clause of 'if' type contains a non-wildcard as second or third parameter.");
      }
      break;
    default:
      QueryParser::ThrowSemanticError("Synonym " + pattern_clause_synonym + " used at front of 'pattern' clause but not of 'assign', 'if', or 'while' type.");
  }

  // Obfuscate synonym strings to prevent them from matching the synonym regex specifically,
  //		since they are not subject to the same logic-based checking.
  std::string obfuscated_first_parameter = std::regex_replace(pattern_clause_first_param, QueryRegex::VALID_SYNONYM_STRING_REGEX, "~");
  // Ensure that what remains has been declared
  std::vector<std::string> first_param_synonyms = QueryParserUtils::GetAllMatchesFromString(obfuscated_first_parameter,
                                                                                            QueryRegex::VALID_SYNONYM_REGEX);

  for (const std::string& synonym : first_param_synonyms) {
    if (synonym_to_design_entity_type_mappings.count(synonym) == 0) {
      QueryParser::ThrowSemanticError("Synonym " + synonym + " used in 'pattern' clause but not declared previously.");
    }
  }

  // If the second parameter is a wildcard, insert that.
  // Else, parse and validate the pattern expression via helper methods.
  // Note again that the third parameter of the 'if' pattern clause is not considered, since it is known to be a wildcard only.
  return PatternClause(
      DesignEntity(synonym_to_design_entity_type_mappings[pattern_clause_synonym], pattern_clause_synonym),
      QueryParser::CreateClauseParamFromString(pattern_clause_first_param, synonym_to_design_entity_type_mappings),
      QueryParser::CreatePatternExpressionClauseParamFromString(pattern_clause_second_param));
}

WithClause QueryParser::ParseWithClause(std::string& with_clause, std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  std::string with_clause_body = QueryParserUtils::PopMatchFromFront(with_clause, QueryRegex::STARTS_WITH_WITH_STRING_REGEX).second;

  std::vector<std::string> with_clause_parameters = QueryParserUtils::GetAllMatchesFromString(with_clause_body,
                                                                                              QueryRegex::VALID_WITH_CLAUSE_PARAMETER_REGEX);

  std::string first_parameter = with_clause_parameters.at(0);
  std::string second_parameter = with_clause_parameters.at(1);

  // Convert synonym attribute parameters to their correct forms and create 'with' clause
  return WithClause(
      QueryParser::CreateWithClauseParamFromString(first_parameter, synonym_to_design_entity_type_mappings),
      QueryParser::CreateWithClauseParamFromString(second_parameter, synonym_to_design_entity_type_mappings));
}

ClauseParam QueryParser::CreateClauseParamFromString(std::string& clause_param_string, std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_WILDCARD_TOKEN_REGEX)) {
    return ClauseParam(DesignEntity(DesignEntityType::WILDCARD, clause_param_string));
  } else if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_SYNONYM_STRING_REGEX)) {
    std::string clause_param_string_quotes_removed = std::regex_replace(clause_param_string, QueryRegex::QUOTES_REGEX, "");
    return ClauseParam(clause_param_string_quotes_removed);
  } else if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_SYNONYM_REGEX)) {
    DesignEntityType synonym_type = synonym_to_design_entity_type_mappings[clause_param_string];
    return ClauseParam(DesignEntity(synonym_type, clause_param_string));
  } else if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_INTEGER_REGEX)) {
    return ClauseParam(std::stoi(clause_param_string));
  } else {
    throw std::runtime_error("String to be constructed into ClauseParam does not fit any of the available regexes.");
  }
}

std::pair<DesignEntity, AttributeType> QueryParser::CreateAttributeSelectedEntityFromString(std::string& clause_param_string,
                                                                                            std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_ATTRIBUTE_REFERENCE_REGEX)) {
    std::pair<std::string, AttributeType> synonym_attribute_pair = QueryParser::ParseSynonymAttribute(clause_param_string,
                                                                                                      synonym_to_design_entity_type_mappings);
    std::string synonym = synonym_attribute_pair.first;
    AttributeType attribute_type = synonym_attribute_pair.second;

    return std::pair<DesignEntity, AttributeType>(DesignEntity(synonym_to_design_entity_type_mappings[synonym], synonym), attribute_type);
  } else {
    throw std::runtime_error("String passed to method is not a valid attribute.");
  }
}

ClauseParam QueryParser::CreatePatternExpressionClauseParamFromString(std::string& clause_param_string) {
  clause_param_string = QueryParserUtils::StripWhitespace(clause_param_string);
  if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_WILDCARD_TOKEN_REGEX)) {
    return ClauseParam(DesignEntity(DesignEntityType::WILDCARD, clause_param_string));
  } else if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_ASSIGN_PATTERN_EXPRESSION_WILDCARD_REGEX)) {
    std::string clause_param_string_quotes_removed = std::regex_replace(clause_param_string, QueryRegex::QUOTES_REGEX, "");
    std::string clause_param_string_wildcards_removed = std::regex_replace(clause_param_string_quotes_removed, QueryRegex::WILDCARD_TOKEN_REGEX, "");
    return ClauseParam(PatternExpression(
        parser_utils::ExpressionParser::ParseExpression(clause_param_string_wildcards_removed), true));
  } else if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_ASSIGN_PATTERN_EXPRESSION_WITH_QUOTES_REGEX)) {
    std::string clause_param_string_quotes_removed = std::regex_replace(clause_param_string, QueryRegex::QUOTES_REGEX, "");
    return ClauseParam(PatternExpression(
        parser_utils::ExpressionParser::ParseExpression(clause_param_string_quotes_removed), false));
  } else {
    throw std::runtime_error("String to be constructed into ClauseParam does not fit any of the available pattern regexes.");
  }
}

std::pair<ClauseParam, AttributeType> QueryParser::CreateWithClauseParamFromString(std::string& clause_param_string,
                                                                                   std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_ATTRIBUTE_REFERENCE_REGEX)) {
    std::pair<std::string, AttributeType> synonym_attribute_pair = QueryParser::ParseSynonymAttribute(clause_param_string,
                                                                                                      synonym_to_design_entity_type_mappings);
    std::string synonym = synonym_attribute_pair.first;
    AttributeType attribute_type = synonym_attribute_pair.second;

    return std::pair<ClauseParam, AttributeType>(QueryParser::CreateClauseParamFromString(synonym, synonym_to_design_entity_type_mappings), attribute_type);
  } else if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_SYNONYM_STRING_REGEX)) {
    return std::pair<ClauseParam, AttributeType>(QueryParser::CreateClauseParamFromString(clause_param_string, synonym_to_design_entity_type_mappings),
                                                 AttributeType::NAME);
  } else if (std::regex_match(clause_param_string, QueryRegex::STARTS_WITH_VALID_INTEGER_REGEX)) {
    return std::pair<ClauseParam, AttributeType>(QueryParser::CreateClauseParamFromString(clause_param_string, synonym_to_design_entity_type_mappings),
                                                 AttributeType::INTEGER);
  }
  // Otherwise, must be a synonym; validate it
  else {
    if (synonym_to_design_entity_type_mappings.count(clause_param_string) == 0) {
      QueryParser::ThrowSemanticError("Synonym " + clause_param_string + " used in 'with' clause but not declared previously.");
    }
    return std::pair<ClauseParam, AttributeType>(QueryParser::CreateClauseParamFromString(clause_param_string, synonym_to_design_entity_type_mappings),
                                                 AttributeType::NONE);
  }
}

std::pair<std::string, AttributeType> QueryParser::ParseSynonymAttribute(std::string& synonym_attribute, std::map<std::string, DesignEntityType>& synonym_to_design_entity_type_mappings) {
  std::vector<std::string> attribute_values = QueryParserUtils::SplitStringAlongChar(synonym_attribute, '.');
  std::string synonym = attribute_values.at(0);
  std::string type = attribute_values.at(1);

  // Check if this synonym has been declared
  if (synonym_to_design_entity_type_mappings.count(synonym) == 0) {
    QueryParser::ThrowSemanticError("Synonym " + synonym + " used but not declared previously.");
  }

  AttributeType attribute_type = QueryUtils::ConvertStringToAttributeType(type);

  return std::pair<std::string, AttributeType>(synonym, attribute_type);
}

std::string& QueryParser::ReplaceKeywordAnd(std::string& string_to_replace, ClauseType most_recent_clause_type) {
  if (!std::regex_search(string_to_replace, QueryRegex::STARTS_WITH_OPTIONAL_SPACE_AND_AND_STRING_REGEX)) {
    return string_to_replace;
  }

  if (most_recent_clause_type != ClauseType::UNDEFINED) {
    string_to_replace = QueryParserUtils::PopMatchFromFront(string_to_replace, QueryRegex::STARTS_WITH_OPTIONAL_SPACE_AND_AND_STRING_REGEX).second;
    string_to_replace = QueryUtils::ConvertClauseTypeToString(most_recent_clause_type) + string_to_replace;
  } else {
    throw std::runtime_error("Query uses 'and' as the first clause.");
  }

  return string_to_replace;
}

void QueryParser::ThrowSemanticError(std::string error_message) {
  QueryParser::is_semantic_error = true;
  throw std::runtime_error(error_message);
}

}  // namespace query_processor
