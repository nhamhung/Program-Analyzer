#pragma once

#include <regex>
#include <string>

namespace query_processor {

class QueryRegex {
 public:
  const static std::string& VALID_DESIGN_ENTITY_TYPE;
  const static std::string& VALID_SYNONYM;
  const static std::string& VALID_SYNONYM_STRING;
  const static std::string& VALID_INTEGER;
  const static std::string& VALID_WILDCARD_TOKEN;
  const static std::string& SELECT_STRING;
  const static std::string& BOOLEAN_STRING;
  const static std::string& SUCH_THAT_STRING;
  const static std::string& PATTERN_STRING;
  const static std::string& WITH_STRING;
  const static std::string& AND_STRING;
  const static std::string& OPTIONAL_SPACE_AND_AND_STRING;

  const static std::string& OPTIONAL_SPACE;
  const static std::string& ONE_SPACE;
  const static std::string& ONE_OR_MORE_WHITESPACE;
  const static std::string& LEADING_OR_TRAILING_SPACE;
  const static std::string& QUOTES;
  const static std::string& OR;
  const static std::string& ANY_CHARS;
  const static std::string& AT_STRING_START;
  const static std::string& AT_STRING_END;
  const static std::string& VALID_STATEMENT_REFERENCE;
  const static std::string& VALID_ENTITY_REFERENCE;
  const static std::string& VALID_LINE_REFERENCE;
  const static std::string& VALID_STATEMENT_OR_ENTITY_REFERENCE;

  const static std::string& VALID_ATTRIBUTE_NAME;
  const static std::string& VALID_ATTRIBUTE_INTEGER;
  const static std::string& VALID_ATTRIBUTE_TYPE;
  const static std::string& VALID_ATTRIBUTE_REFERENCE;

  const static std::string& VALID_DECLARATION_FIRST_SYNONYM;
  const static std::string& VALID_DECLARATION_OTHER_SYNONYM;
  const static std::string& VALID_DECLARATION_CLAUSE_ENDING;
  const static std::string& VALID_DECLARATION_CLAUSE;

  const static std::string& VALID_SELECT_BOOLEAN;
  const static std::string& VALID_SELECT_TUPLE_PARAMETER;
  const static std::string& VALID_SELECT_TUPLE;
  const static std::string& VALID_TUPLE_ANGLE_BRACKETS;
  const static std::string& VALID_SELECTED_ENTITY;
  const static std::string& VALID_SELECT_PHRASE;

  const static std::string& VALID_SUCH_THAT_CLAUSE_FOLLOWS;
  const static std::string& VALID_SUCH_THAT_CLAUSE_PARENT;
  const static std::string& VALID_SUCH_THAT_CLAUSE_MODIFIES;
  const static std::string& VALID_SUCH_THAT_CLAUSE_USES;
  const static std::string& VALID_SUCH_THAT_CLAUSE_CALLS;
  const static std::string& VALID_SUCH_THAT_CLAUSE_NEXT;
  const static std::string& VALID_SUCH_THAT_CLAUSE_AFFECTS;
  const static std::string& VALID_SUCH_THAT_CLAUSE_NEXT_BIP;
  const static std::string& VALID_SUCH_THAT_CLAUSE_AFFECTS_BIP;
  const static std::string& VALID_DESIGN_ABSTRACTION;
  const static std::string& VALID_DESIGN_ABSTRACTION_PARAMETER;
  const static std::string& VALID_DESIGN_ABSTRACTION_PARAMETERS;
  const static std::string& VALID_SUCH_THAT_CLAUSE;

  const static std::string& VALID_ASSIGN_PATTERN_EXPRESSION;
  const static std::string& VALID_ASSIGN_PATTERN_EXPRESSION_WITH_QUOTES;
  const static std::string& VALID_ASSIGN_PATTERN_EXPRESSION_WILDCARD;
  const static std::string& VALID_ASSIGN_PATTERN_SECOND_PARAMETER;
  const static std::string& VALID_ASSIGN_PATTERN_PARAMETERS;
  const static std::string& VALID_IF_PATTERN_PARAMETERS;
  const static std::string& VALID_WHILE_PATTERN_PARAMETERS;
  const static std::string& VALID_PATTERN_PARAMETERS;
  const static std::string& VALID_PATTERN_CLAUSE;

  const static std::string& VALID_WITH_CLAUSE_PARAMETER;
  const static std::string& VALID_WITH_CLAUSE;

  const static std::string& VALID_CONDITION_CLAUSE;
  const static std::string& VALID_SELECT_CLAUSE;

  const static std::regex& ONE_OR_MORE_WHITESPACE_REGEX;
  const static std::regex& LEADING_OR_TRAILING_SPACE_REGEX;

  const static std::regex& VALID_SYNONYM_STRING_REGEX;
  const static std::regex& VALID_SYNONYM_REGEX;
  const static std::regex& QUOTES_REGEX;
  const static std::regex& WILDCARD_TOKEN_REGEX;

  const static std::regex& VALID_DESIGN_ABSTRACTION_REGEX;
  const static std::regex& VALID_DESIGN_ABSTRACTION_PARAMETER_REGEX;
  const static std::regex& VALID_DESIGN_ABSTRACTION_PARAMETERS_REGEX;

  const static std::regex& VALID_PATTERN_PARAMETERS_REGEX;
  const static std::regex& VALID_ASSIGN_PATTERN_SECOND_PARAMETER_REGEX;

  const static std::regex& VALID_WITH_CLAUSE_PARAMETER_REGEX;

  const static std::regex& STARTS_WITH_VALID_SYNONYM_REGEX;
  const static std::regex& STARTS_WITH_VALID_INTEGER_REGEX;
  const static std::regex& STARTS_WITH_VALID_SYNONYM_STRING_REGEX;
  const static std::regex& STARTS_WITH_VALID_ATTRIBUTE_REFERENCE_REGEX;
  const static std::regex& STARTS_WITH_VALID_WILDCARD_TOKEN_REGEX;

  const static std::regex& STARTS_WITH_OPTIONAL_SPACE_AND_AND_STRING_REGEX;

  const static std::regex& STARTS_WITH_VALID_DECLARATION_FIRST_SYNONYM_REGEX;
  const static std::regex& STARTS_WITH_VALID_DECLARATION_OTHER_SYNONYM_REGEX;
  const static std::regex& STARTS_WITH_VALID_DECLARATION_CLAUSE_ENDING_REGEX;

  const static std::regex& STARTS_WITH_VALID_SELECT_BOOLEAN_REGEX;
  const static std::regex& STARTS_WITH_BOOLEAN_STRING_REGEX;
  const static std::regex& STARTS_WITH_VALID_SELECT_TUPLE_REGEX;
  const static std::regex& STARTS_OR_ENDS_WITH_VALID_TUPLE_ANGLE_BRACKETS_REGEX;
  const static std::regex& STARTS_WITH_VALID_SELECT_PHRASE_REGEX;
  const static std::regex& STARTS_WITH_SELECT_STRING_REGEX;

  const static std::regex& STARTS_WITH_VALID_CONDITION_CLAUSE_REGEX;

  const static std::regex& STARTS_WITH_VALID_SUCH_THAT_CLAUSE_REGEX;

  const static std::regex& STARTS_WITH_VALID_ASSIGN_PATTERN_EXPRESSION_WILDCARD_REGEX;
  const static std::regex& STARTS_WITH_VALID_ASSIGN_PATTERN_EXPRESSION_WITH_QUOTES_REGEX;
  const static std::regex& STARTS_WITH_VALID_PATTERN_CLAUSE_REGEX;

  const static std::regex& STARTS_WITH_WITH_STRING_REGEX;
  const static std::regex& STARTS_WITH_VALID_WITH_CLAUSE_REGEX;
};

}  // namespace query_processor
