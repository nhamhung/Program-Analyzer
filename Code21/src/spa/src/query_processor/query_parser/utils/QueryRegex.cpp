#include "QueryRegex.h"

#include <string>

namespace query_processor {

// General constants

const std::string& QueryRegex::OPTIONAL_SPACE = " ?";
const std::string& QueryRegex::ONE_SPACE = " ";
const std::string& QueryRegex::ONE_OR_MORE_WHITESPACE = "\\s+";
const std::string& QueryRegex::LEADING_OR_TRAILING_SPACE = "^( )+|( )+$";
const std::string& QueryRegex::QUOTES = "\"";
const std::string& QueryRegex::OR = "|";
const std::string& QueryRegex::ANY_CHARS = ".*?";
const std::string& QueryRegex::AT_STRING_START = "^";
const std::string& QueryRegex::AT_STRING_END = "$";

const std::string& QueryRegex::VALID_SYNONYM = "[a-zA-Z][a-zA-Z0-9]*";
const std::string& QueryRegex::VALID_SYNONYM_STRING = QUOTES + VALID_SYNONYM + QUOTES;
const std::string& QueryRegex::VALID_INTEGER = "(0|[1-9]\\d*)";
const std::string& QueryRegex::VALID_WILDCARD_TOKEN = "_";
const std::string& QueryRegex::VALID_STATEMENT_REFERENCE = "(" + VALID_SYNONYM + OR + VALID_WILDCARD_TOKEN + OR + VALID_INTEGER + ")";
const std::string& QueryRegex::VALID_ENTITY_REFERENCE = "(" + VALID_SYNONYM + OR + VALID_WILDCARD_TOKEN + OR + VALID_SYNONYM_STRING + ")";
const std::string& QueryRegex::VALID_LINE_REFERENCE = "(" + VALID_SYNONYM + OR + VALID_WILDCARD_TOKEN + OR + VALID_INTEGER + ")";
const std::string& QueryRegex::VALID_STATEMENT_OR_ENTITY_REFERENCE = "(" + VALID_WILDCARD_TOKEN + OR + VALID_SYNONYM + OR + VALID_INTEGER + OR + VALID_SYNONYM_STRING + ")";
const std::string& QueryRegex::SELECT_STRING = "Select";
const std::string& QueryRegex::BOOLEAN_STRING = "BOOLEAN";
const std::string& QueryRegex::SUCH_THAT_STRING = "such that";
const std::string& QueryRegex::PATTERN_STRING = "pattern";
const std::string& QueryRegex::WITH_STRING = "with";
const std::string& QueryRegex::AND_STRING = "and";
const std::string& QueryRegex::OPTIONAL_SPACE_AND_AND_STRING = OPTIONAL_SPACE + AND_STRING;

// Attribute constants
const std::string& QueryRegex::VALID_ATTRIBUTE_NAME = "(procName|varName)";
const std::string& QueryRegex::VALID_ATTRIBUTE_INTEGER = "(value|stmt#)";
const std::string& QueryRegex::VALID_ATTRIBUTE_TYPE = "(" + VALID_ATTRIBUTE_NAME + OR + VALID_ATTRIBUTE_INTEGER + ")";
const std::string& QueryRegex::VALID_ATTRIBUTE_REFERENCE = VALID_SYNONYM + "\\." + VALID_ATTRIBUTE_TYPE;

// Declaration constants

const std::string& QueryRegex::VALID_DESIGN_ENTITY_TYPE = "(stmt|read|print|while|if|assign|call|variable|constant|procedure|prog_line)";

const std::string& QueryRegex::VALID_DECLARATION_FIRST_SYNONYM = OPTIONAL_SPACE + VALID_DESIGN_ENTITY_TYPE + ONE_SPACE + VALID_SYNONYM + OPTIONAL_SPACE;

const std::string& QueryRegex::VALID_DECLARATION_OTHER_SYNONYM = OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_SYNONYM + OPTIONAL_SPACE;

const std::string& QueryRegex::VALID_DECLARATION_CLAUSE_ENDING = OPTIONAL_SPACE + ";" + OPTIONAL_SPACE;

const std::string& QueryRegex::VALID_DECLARATION_CLAUSE = OPTIONAL_SPACE + VALID_DECLARATION_FIRST_SYNONYM + "(" + VALID_DECLARATION_OTHER_SYNONYM + ")*" + OPTIONAL_SPACE + ";" + OPTIONAL_SPACE;

// Select phrase constants

const std::string& QueryRegex::VALID_SELECT_BOOLEAN = "(" + BOOLEAN_STRING + AT_STRING_END + OR + BOOLEAN_STRING + ONE_SPACE + ")";

const std::string& QueryRegex::VALID_SELECT_TUPLE_PARAMETER = "(" + VALID_SYNONYM + OR + VALID_ATTRIBUTE_REFERENCE + ")";

const std::string& QueryRegex::VALID_SELECT_TUPLE = "<" + OPTIONAL_SPACE + VALID_SELECT_TUPLE_PARAMETER + OPTIONAL_SPACE + "(" + "," + OPTIONAL_SPACE + VALID_SELECT_TUPLE_PARAMETER + OPTIONAL_SPACE + ")*" + ">";

const std::string& QueryRegex::VALID_TUPLE_ANGLE_BRACKETS = "(" + AT_STRING_START + "<" + OR + ">" + AT_STRING_END + ")";

const std::string& QueryRegex::VALID_SELECTED_ENTITY = "(" + VALID_SELECT_TUPLE + OR + VALID_ATTRIBUTE_REFERENCE + OR + VALID_SELECT_BOOLEAN + OR + VALID_SYNONYM + ")";

const std::string& QueryRegex::VALID_SELECT_PHRASE = OPTIONAL_SPACE + SELECT_STRING + ONE_SPACE + VALID_SELECTED_ENTITY + OPTIONAL_SPACE;

// 'such that' constants

// IMPORTANT: The regexes below have the transitive counterparts before the non-transitive ones. This is to ensure the correct one is matched.

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_FOLLOWS = "(Follows\\*|Follows)" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_STATEMENT_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_STATEMENT_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_PARENT = "(Parent\\*|Parent)" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_STATEMENT_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_STATEMENT_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_MODIFIES = "Modifies" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_STATEMENT_OR_ENTITY_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_ENTITY_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_USES = "Uses" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_STATEMENT_OR_ENTITY_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_ENTITY_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_CALLS = "(Calls\\*|Calls)" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_ENTITY_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_ENTITY_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_NEXT = "(Next\\*|Next)" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_LINE_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_LINE_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_AFFECTS = "(Affects\\*|Affects)" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_STATEMENT_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_STATEMENT_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_NEXT_BIP = "(NextBip\\*|NextBip)" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_LINE_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_LINE_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE_AFFECTS_BIP = "(AffectsBip\\*|AffectsBip)" + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_STATEMENT_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_STATEMENT_REFERENCE + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_DESIGN_ABSTRACTION = "(Follows\\*|Follows|Parent\\*|Parent|Modifies|Uses|Calls\\*|Calls|NextBip\\*|NextBip|Next\\*|Next|AffectsBip\\*|AffectsBip|Affects\\*|Affects)";
const std::string& QueryRegex::VALID_DESIGN_ABSTRACTION_PARAMETER = "(" + VALID_STATEMENT_REFERENCE + OR + VALID_ENTITY_REFERENCE + OR + VALID_LINE_REFERENCE + ")";
const std::string& QueryRegex::VALID_DESIGN_ABSTRACTION_PARAMETERS = "\\(" + OPTIONAL_SPACE + VALID_DESIGN_ABSTRACTION_PARAMETER + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_DESIGN_ABSTRACTION_PARAMETER + OPTIONAL_SPACE + "\\)";
const std::string& QueryRegex::VALID_SUCH_THAT_CLAUSE = OPTIONAL_SPACE + "(" + SUCH_THAT_STRING + OR + AND_STRING + ")" + ONE_SPACE + "(" + VALID_SUCH_THAT_CLAUSE_FOLLOWS + OR + VALID_SUCH_THAT_CLAUSE_PARENT + OR + VALID_SUCH_THAT_CLAUSE_MODIFIES + OR + VALID_SUCH_THAT_CLAUSE_USES + OR + VALID_SUCH_THAT_CLAUSE_CALLS + OR + VALID_SUCH_THAT_CLAUSE_NEXT + OR + VALID_SUCH_THAT_CLAUSE_AFFECTS + OR + VALID_SUCH_THAT_CLAUSE_NEXT_BIP + OR + VALID_SUCH_THAT_CLAUSE_AFFECTS_BIP + ")" + OPTIONAL_SPACE;

// 'pattern' constants

// It is not the job of the regex to validate the pattern expression, so a nonempty placeholder is used.
// This regex ensures that no wildcard tokens or quotes exist within the expression.
// The reason is to ensure that wildcard tokens and quotes exist only outside the pattern expression, to be used in string splitting.

const std::string& QueryRegex::VALID_ASSIGN_PATTERN_EXPRESSION = "[^_\"]+";

const std::string& QueryRegex::VALID_ASSIGN_PATTERN_EXPRESSION_WITH_QUOTES = QUOTES + OPTIONAL_SPACE + VALID_ASSIGN_PATTERN_EXPRESSION + OPTIONAL_SPACE + QUOTES;

const std::string& QueryRegex::VALID_ASSIGN_PATTERN_EXPRESSION_WILDCARD = VALID_WILDCARD_TOKEN + OPTIONAL_SPACE + VALID_ASSIGN_PATTERN_EXPRESSION_WITH_QUOTES + OPTIONAL_SPACE + VALID_WILDCARD_TOKEN;

const std::string& QueryRegex::VALID_ASSIGN_PATTERN_SECOND_PARAMETER = "(" + VALID_ASSIGN_PATTERN_EXPRESSION_WILDCARD + OR + VALID_ASSIGN_PATTERN_EXPRESSION_WITH_QUOTES + OR + VALID_WILDCARD_TOKEN + ")";

const std::string& QueryRegex::VALID_ASSIGN_PATTERN_PARAMETERS = VALID_SYNONYM + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_ENTITY_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_ASSIGN_PATTERN_SECOND_PARAMETER + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_IF_PATTERN_PARAMETERS = VALID_SYNONYM + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_ENTITY_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_WILDCARD_TOKEN + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_WILDCARD_TOKEN + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_WHILE_PATTERN_PARAMETERS = VALID_SYNONYM + OPTIONAL_SPACE + "\\(" + OPTIONAL_SPACE + VALID_ENTITY_REFERENCE + OPTIONAL_SPACE + "," + OPTIONAL_SPACE + VALID_WILDCARD_TOKEN + OPTIONAL_SPACE + "\\)";

const std::string& QueryRegex::VALID_PATTERN_PARAMETERS = "(" + VALID_ASSIGN_PATTERN_PARAMETERS + OR + VALID_IF_PATTERN_PARAMETERS + OR + VALID_WHILE_PATTERN_PARAMETERS + ")";

const std::string& QueryRegex::VALID_PATTERN_CLAUSE = OPTIONAL_SPACE + "(" + PATTERN_STRING + OR + AND_STRING + ")" + ONE_SPACE + VALID_PATTERN_PARAMETERS + OPTIONAL_SPACE;

// 'with' constants

const std::string& QueryRegex::VALID_WITH_CLAUSE_PARAMETER = "(" + VALID_ATTRIBUTE_REFERENCE + OR + VALID_SYNONYM_STRING + OR + VALID_SYNONYM + OR + VALID_INTEGER + ")";

const std::string& QueryRegex::VALID_WITH_CLAUSE = OPTIONAL_SPACE + "(" + WITH_STRING + OR + AND_STRING + ")" + ONE_SPACE + VALID_WITH_CLAUSE_PARAMETER + OPTIONAL_SPACE + "=" + OPTIONAL_SPACE + VALID_WITH_CLAUSE_PARAMETER + OPTIONAL_SPACE;

// Overall constants

const std::string& QueryRegex::VALID_CONDITION_CLAUSE = "(" + VALID_WITH_CLAUSE + OR + VALID_PATTERN_CLAUSE + OR + VALID_SUCH_THAT_CLAUSE + ")";

const std::string& QueryRegex::VALID_SELECT_CLAUSE = VALID_SELECT_PHRASE + "(" + VALID_CONDITION_CLAUSE + ")*" + OPTIONAL_SPACE;

// Regex versions

const std::regex& QueryRegex::ONE_OR_MORE_WHITESPACE_REGEX = std::regex(ONE_OR_MORE_WHITESPACE);
const std::regex& QueryRegex::LEADING_OR_TRAILING_SPACE_REGEX = std::regex(LEADING_OR_TRAILING_SPACE);

const std::regex& QueryRegex::VALID_SYNONYM_STRING_REGEX = std::regex(VALID_SYNONYM_STRING);
const std::regex& QueryRegex::VALID_SYNONYM_REGEX = std::regex(VALID_SYNONYM);
const std::regex& QueryRegex::QUOTES_REGEX = std::regex(QUOTES);
const std::regex& QueryRegex::WILDCARD_TOKEN_REGEX = std::regex(VALID_WILDCARD_TOKEN);

const std::regex& QueryRegex::VALID_DESIGN_ABSTRACTION_REGEX = std::regex(VALID_DESIGN_ABSTRACTION);
const std::regex& QueryRegex::VALID_DESIGN_ABSTRACTION_PARAMETER_REGEX = std::regex(VALID_DESIGN_ABSTRACTION_PARAMETER);
const std::regex& QueryRegex::VALID_DESIGN_ABSTRACTION_PARAMETERS_REGEX = std::regex(VALID_DESIGN_ABSTRACTION_PARAMETERS);

const std::regex& QueryRegex::VALID_PATTERN_PARAMETERS_REGEX = std::regex(VALID_PATTERN_PARAMETERS);
const std::regex& QueryRegex::VALID_ASSIGN_PATTERN_SECOND_PARAMETER_REGEX = std::regex(VALID_ASSIGN_PATTERN_SECOND_PARAMETER);

const std::regex& QueryRegex::VALID_WITH_CLAUSE_PARAMETER_REGEX = std::regex(VALID_WITH_CLAUSE_PARAMETER);

const std::regex& QueryRegex::STARTS_WITH_VALID_SYNONYM_REGEX = std::regex(AT_STRING_START + VALID_SYNONYM);
const std::regex& QueryRegex::STARTS_WITH_VALID_INTEGER_REGEX = std::regex(AT_STRING_START + VALID_INTEGER);
const std::regex& QueryRegex::STARTS_WITH_VALID_SYNONYM_STRING_REGEX = std::regex(AT_STRING_START + VALID_SYNONYM_STRING);
const std::regex& QueryRegex::STARTS_WITH_VALID_ATTRIBUTE_REFERENCE_REGEX = std::regex(AT_STRING_START + VALID_ATTRIBUTE_REFERENCE);
const std::regex& QueryRegex::STARTS_WITH_VALID_WILDCARD_TOKEN_REGEX = std::regex(AT_STRING_START + VALID_WILDCARD_TOKEN);

const std::regex& QueryRegex::STARTS_WITH_OPTIONAL_SPACE_AND_AND_STRING_REGEX = std::regex(AT_STRING_START + OPTIONAL_SPACE_AND_AND_STRING);

const std::regex& QueryRegex::STARTS_WITH_VALID_DECLARATION_FIRST_SYNONYM_REGEX = std::regex(AT_STRING_START + VALID_DECLARATION_FIRST_SYNONYM);
const std::regex& QueryRegex::STARTS_WITH_VALID_DECLARATION_OTHER_SYNONYM_REGEX = std::regex(AT_STRING_START + VALID_DECLARATION_OTHER_SYNONYM);
const std::regex& QueryRegex::STARTS_WITH_VALID_DECLARATION_CLAUSE_ENDING_REGEX = std::regex(AT_STRING_START + VALID_DECLARATION_CLAUSE_ENDING);

const std::regex& QueryRegex::STARTS_WITH_SELECT_STRING_REGEX = std::regex(AT_STRING_START + SELECT_STRING);
const std::regex& QueryRegex::STARTS_WITH_VALID_SELECT_BOOLEAN_REGEX = std::regex(AT_STRING_START + VALID_SELECT_BOOLEAN);
const std::regex& QueryRegex::STARTS_WITH_BOOLEAN_STRING_REGEX = std::regex(AT_STRING_START + BOOLEAN_STRING);
const std::regex& QueryRegex::STARTS_OR_ENDS_WITH_VALID_TUPLE_ANGLE_BRACKETS_REGEX = std::regex(VALID_TUPLE_ANGLE_BRACKETS);
const std::regex& QueryRegex::STARTS_WITH_VALID_SELECT_TUPLE_REGEX = std::regex(AT_STRING_START + VALID_SELECT_TUPLE);
const std::regex& QueryRegex::STARTS_WITH_VALID_SELECT_PHRASE_REGEX = std::regex(AT_STRING_START + VALID_SELECT_PHRASE);

const std::regex& QueryRegex::STARTS_WITH_VALID_CONDITION_CLAUSE_REGEX = std::regex(AT_STRING_START + VALID_CONDITION_CLAUSE);

const std::regex& QueryRegex::STARTS_WITH_VALID_SUCH_THAT_CLAUSE_REGEX = std::regex(AT_STRING_START + VALID_SUCH_THAT_CLAUSE);

const std::regex& QueryRegex::STARTS_WITH_VALID_ASSIGN_PATTERN_EXPRESSION_WILDCARD_REGEX = std::regex(AT_STRING_START + VALID_ASSIGN_PATTERN_EXPRESSION_WILDCARD);
const std::regex& QueryRegex::STARTS_WITH_VALID_ASSIGN_PATTERN_EXPRESSION_WITH_QUOTES_REGEX = std::regex(AT_STRING_START + VALID_ASSIGN_PATTERN_EXPRESSION_WITH_QUOTES);
const std::regex& QueryRegex::STARTS_WITH_VALID_PATTERN_CLAUSE_REGEX = std::regex(AT_STRING_START + VALID_PATTERN_CLAUSE);

const std::regex& QueryRegex::STARTS_WITH_WITH_STRING_REGEX = std::regex(AT_STRING_START + WITH_STRING);
const std::regex& QueryRegex::STARTS_WITH_VALID_WITH_CLAUSE_REGEX = std::regex(AT_STRING_START + VALID_WITH_CLAUSE);

}  // namespace query_processor
