#include <string>

#include "catch.hpp"
#include "parser_utils/ExpressionParser.h"
#include "query_processor/commons/query/Query.h"
#include "query_processor/query_parser/QueryParser.h"

using namespace std;
using namespace query_processor;

SCENARIO("Testing GenerateQuery() when query's assign pattern clause has invalid syntax.", "[query_parser]") {
  WHEN("'pattern' is not in one word.") {
    string NOT_ONE_WORD_PATTERN_STRING = "stmt s; assign a; Select s pat tern a(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NOT_ONE_WORD_PATTERN_STRING));
    }
  }

  WHEN("There is no space between 'pattern' and the assign variable.") {
    string NO_SPACE_PATTERN_STRING = "stmt s; assign a; Select s patterna(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_PATTERN_STRING));
    }
  }

  WHEN("There are no parentheses around the pattern parameters.") {
    string NO_PARENTHESES_PATTERN_STRING = "stmt s; assign a; Select s pattern a _, _ ";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_PARENTHESES_PATTERN_STRING));
    }
  }

  WHEN("There are no comma between the pattern parameters.") {
    string NO_PARENTHESES_PATTERN_STRING = "stmt s; assign a; Select s pattern a(_ _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_PARENTHESES_PATTERN_STRING));
    }
  }

  WHEN("The first parameter is an integer.") {
    string FIRST_PARAM_INTEGER_STRING = "stmt s; assign a; Select s pattern a(1, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_INTEGER_STRING));
    }
  }

  WHEN("The first parameter is a string that does not start with a letter.") {
    string FIRST_PARAM_INVALID_STRING_STRING = "stmt s; assign a; Select s pattern a(\"1s\", _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_INVALID_STRING_STRING));
    }
  }

  WHEN("The first parameter is a string that contains more than one word.") {
    string FIRST_PARAM_MULTIWORD_STRING_STRING = "stmt s; assign a; Select s pattern a(\"s n\", _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_MULTIWORD_STRING_STRING));
    }
  }

  WHEN("The second parameter is an expression without surrounding quotes.") {
    string SECOND_PARAM_NO_QUOTES_STRING = "stmt s; assign a; Select s pattern a(_, a + s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_NO_QUOTES_STRING));
    }
  }

  WHEN("The second parameter is an expression with wildcards but no surrounding quotes.") {
    string SECOND_PARAM_WILDCARD_NO_QUOTES_STRING = "stmt s; assign a; Select s pattern a(_, _a + s_)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_WILDCARD_NO_QUOTES_STRING));
    }
  }

  WHEN("The second parameter is an expression with surrounding quotes but with only one surrounding wildcard.") {
    string SECOND_PARAM_MISSING_WILDCARD_STRING = "stmt s; assign a; Select s pattern a(_, _\"a + s\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_MISSING_WILDCARD_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() when query's if pattern clause has invalid syntax.", "[query_parser]") {
  WHEN("'pattern' is not in one word.") {
    string NOT_ONE_WORD_PATTERN_STRING = "stmt s; if ifs; Select s pat tern ifs(_, _, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NOT_ONE_WORD_PATTERN_STRING));
    }
  }

  WHEN("There is no space between 'pattern' and the if variable.") {
    string NO_SPACE_PATTERN_STRING = "stmt s; if ifs; Select s patternifs(_, _, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_PATTERN_STRING));
    }
  }

  WHEN("There are no parentheses around the pattern parameters.") {
    string NO_PARENTHESES_PATTERN_STRING = "stmt s; if ifs; Select s pattern ifs _, _, _";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_PARENTHESES_PATTERN_STRING));
    }
  }

  WHEN("There are no comma between the pattern parameters.") {
    string NO_PARENTHESES_PATTERN_STRING = "stmt s; if ifs; Select s pattern ifs(_, _ _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_PARENTHESES_PATTERN_STRING));
    }
  }

  WHEN("The first parameter is an integer.") {
    string FIRST_PARAM_INTEGER_STRING = "stmt s; if ifs; Select s pattern ifs(1, _, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_INTEGER_STRING));
    }
  }

  WHEN("The first parameter is a string that does not start with a letter.") {
    string FIRST_PARAM_INVALID_STRING_STRING = "stmt s; if ifs; Select s pattern ifs(\"1s\", _, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_INVALID_STRING_STRING));
    }
  }

  WHEN("The first parameter is a string that contains more than one word.") {
    string FIRST_PARAM_MULTIWORD_STRING_STRING = "stmt s; if ifs; Select s pattern ifs(\"s n\", _, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_MULTIWORD_STRING_STRING));
    }
  }

  WHEN("The second parameter is not a wildcard.") {
    string SECOND_PARAM_NOT_WILDCARD_STRING = "stmt s; if ifs; Select s pattern ifs(_, s, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_NOT_WILDCARD_STRING));
    }
  }

  WHEN("The second parameter is a pattern expression.") {
    string SECOND_PARAM_PATTERN_EXPRESSION_STRING = "assign a; if ifs; Select a pattern ifs(_, _\"a * (b/c)\"_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_PATTERN_EXPRESSION_STRING));
    }
  }

  WHEN("The third parameter is not a wildcard.") {
    string THIRD_PARAM_NOT_WILDCARD_STRING = "stmt s; if ifs; Select s pattern ifs(_, _, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(THIRD_PARAM_NOT_WILDCARD_STRING));
    }
  }

  WHEN("The third parameter is a pattern expression.") {
    string THIRD_PARAM_PATTERN_EXPRESSION_STRING = "assign a; if ifs; Select a pattern ifs(_, _, _\"a * (b/c)\"_)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(THIRD_PARAM_PATTERN_EXPRESSION_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() when query's while pattern clause has invalid syntax.", "[query_parser]") {
  WHEN("'pattern' is not in one word.") {
    string NOT_ONE_WORD_PATTERN_STRING = "stmt s; while w; Select s pat tern w(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NOT_ONE_WORD_PATTERN_STRING));
    }
  }

  WHEN("There is no space between 'pattern' and the while variable.") {
    string NO_SPACE_PATTERN_STRING = "stmt s; while w; Select s patternw(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_PATTERN_STRING));
    }
  }

  WHEN("There are no parentheses around the pattern parameters.") {
    string NO_PARENTHESES_PATTERN_STRING = "stmt s; while w; Select s pattern w _, _";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_PARENTHESES_PATTERN_STRING));
    }
  }

  WHEN("There are no comma between the pattern parameters.") {
    string NO_PARENTHESES_PATTERN_STRING = "stmt s; while w; Select s pattern w(_ _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_PARENTHESES_PATTERN_STRING));
    }
  }

  WHEN("The first parameter is an integer.") {
    string FIRST_PARAM_INTEGER_STRING = "stmt s; while w; Select s pattern w(1, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_INTEGER_STRING));
    }
  }

  WHEN("The first parameter is a string that does not start with a letter.") {
    string FIRST_PARAM_INVALID_STRING_STRING = "stmt s; while w; Select s pattern w(\"1s\", _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_INVALID_STRING_STRING));
    }
  }

  WHEN("The first parameter is a string that contains more than one word.") {
    string FIRST_PARAM_MULTIWORD_STRING_STRING = "stmt s; while w; Select s pattern w(\"s n\", _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_MULTIWORD_STRING_STRING));
    }
  }

  WHEN("The second parameter is not a wildcard.") {
    string SECOND_PARAM_NOT_WILDCARD_STRING = "stmt s; while w; Select s pattern w(_, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_NOT_WILDCARD_STRING));
    }
  }

  WHEN("The second parameter is a pattern expression.") {
    string SECOND_PARAM_PATTERN_EXPRESSION_STRING = "assign a; while w; Select a pattern w(_, _\"a * (b/c)\"_)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_PATTERN_EXPRESSION_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on assign 'pattern' clauses with invalid logic.", "[query_parser]") {
  WHEN("The synonym used before the parentheses was undeclared in the declaration clauses.") {
    string UNDECLARED_SYNONYM_STRING = "stmt s; assign a; Select a pattern n(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_SYNONYM_STRING));
    }
  }

  WHEN("The synonym used before the parentheses is not an assignment synonym.") {
    string NON_ASSIGNMENT_SYNONYM_STRING = "stmt s; assign a; Select a pattern s(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NON_ASSIGNMENT_SYNONYM_STRING));
    }
  }

  WHEN("The first parameter is an undeclared synonym.") {
    string FIRST_PARAM_UNDECLARED_SYNONYM_STRING = "stmt s; assign a; Select a pattern a(s1, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_UNDECLARED_SYNONYM_STRING));
    }
  }

  WHEN("The second parameter contains invalid characters.") {
    string SECOND_PARAM_INVALID_CHARS_STRING = "stmt s; assign a; Select s pattern a(_, \"a & b\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_INVALID_CHARS_STRING));
    }
  }

  WHEN("The second parameter contains invalid names.") {
    string SECOND_PARAM_INVALID_NAMES_STRING = "stmt s; assign a; Select s pattern a(_, \"a + b_\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_INVALID_NAMES_STRING));
    }
  }

  WHEN("The second parameter contains invalid parenthesis pairings and is a wildcard-type expression.") {
    string SECOND_PARAM_INVALID_PARENTHESES_STRING = "stmt s; assign a; Select s pattern a(_, _\"a ( + b )\"_)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_INVALID_PARENTHESES_STRING));
    }
  }

  WHEN("The second parameter has an empty expression.") {
    string SECOND_PARAM_EMPTY_STRING = "stmt s; assign a; Select s pattern a(_, \" \")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_PARAM_EMPTY_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on if 'pattern' clauses with invalid logic.", "[query_parser]") {
  WHEN("The synonym used before the parentheses was undeclared in the declaration clauses.") {
    string UNDECLARED_SYNONYM_STRING = "stmt s; if ifs; Select s pattern n(_, _, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_SYNONYM_STRING));
    }
  }

  WHEN("The synonym used before the parentheses is not an if synonym.") {
    string NON_IF_SYNONYM_STRING = "stmt s; if ifs; Select s pattern s(_, _, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NON_IF_SYNONYM_STRING));
    }
  }

  WHEN("The first parameter is an undeclared synonym.") {
    string FIRST_PARAM_UNDECLARED_SYNONYM_STRING = "stmt s; if ifs; Select s pattern ifs(s1, _, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_UNDECLARED_SYNONYM_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on while 'pattern' clauses with invalid logic.", "[query_parser]") {
  WHEN("The synonym used before the parentheses was undeclared in the declaration clauses.") {
    string UNDECLARED_SYNONYM_STRING = "stmt s; while w; Select s pattern n(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_SYNONYM_STRING));
    }
  }

  WHEN("The synonym used before the parentheses is not a while synonym.") {
    string NON_WHILE_SYNONYM_STRING = "stmt s; while w; Select s pattern s(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NON_WHILE_SYNONYM_STRING));
    }
  }

  WHEN("The first parameter is an undeclared synonym.") {
    string FIRST_PARAM_UNDECLARED_SYNONYM_STRING = "stmt s; while w; Select s pattern w(s1, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_PARAM_UNDECLARED_SYNONYM_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with an assign 'pattern' clause.", "[query_parser]") {
  WHEN("Synonym is a valid assign synonym, and parameters are wildcards.") {
    string VALID_ASSIGN_SYNONYM_STRING = "stmt s; assign a; Select s pattern a(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_ASSIGN_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid synonym.") {
    string FIRST_SYNONYM_STRING = "stmt s; assign a; Select s pattern a(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid string.") {
    string FIRST_SYNONYM_STRING = "stmt s; assign a; Select s pattern a(\"string\", _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam("string"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Second parameter is a valid normal expression.") {
    string SECOND_NORMAL_EXPR_STRING = "stmt s; assign a; Select s pattern a(_, \"a+b\")";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SECOND_NORMAL_EXPR_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), false))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Second parameter is a valid wildcard-type expression.") {
    string SECOND_WILDCARD_EXPR_STRING = "stmt s; assign a; Select s pattern a(_, _\"a+b\"_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SECOND_WILDCARD_EXPR_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid synonym and second parameter is a normal expression.") {
    string FIRST_SYNONYM_SECOND_NORMAL_STRING = "stmt s; assign a; Select s pattern a(s, \"a+b\")";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_SYNONYM_SECOND_NORMAL_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), false))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid string and second parameter is a wildcard-type expression.") {
    string FIRST_STRING_SECOND_WILDCARD_STRING = "stmt s; assign a; Select s pattern a(\"string\", _\"a+b\"_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_STRING_SECOND_WILDCARD_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam("string"),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Tricky words are used as parameter names.") {
    string TRICKY_WORDS_STRING = "stmt assign; assign stmt, pattern; Select assign pattern pattern(pattern, _\"stmt\"_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "assign")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "pattern"),
        ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "pattern")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("stmt"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("There is excessive valid whitespace.") {
    string EXCESSIVE_WHITESPACE_STRING = "\t\n\n  stmt \r\n\n \vs; assign\n\n\na; Select\t\r \n\ns\t\n\n\n\r\npattern\t\n\na     \t\n\r (\n\n\n_\n\n,\n\t\r_\n\"   \t\n\r\n  a  \n\n\n+    b   \t\n\n\"\n\n_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query with one assign 'pattern' clause, and BOOLEAN is selected.", "[query_parser]") {
  WHEN("First parameter is a valid synonym and second parameter is a normal expression.") {
    string FIRST_SYNONYM_SECOND_NORMAL_STRING = "stmt s; assign a; Select BOOLEAN pattern a(s, \"a+b\")";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_SYNONYM_SECOND_NORMAL_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), false))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid string and second parameter is a wildcard-type expression.") {
    string FIRST_STRING_SECOND_WILDCARD_STRING = "stmt s; assign a; Select BOOLEAN pattern a(\"string\", _\"a+b\"_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_STRING_SECOND_WILDCARD_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam("string"),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Tricky words are used as parameter names.") {
    string TRICKY_WORDS_STRING = "stmt assign, BOOlEAN; assign stmt, pattern; Select BOOLEAN pattern pattern(pattern, _\"stmt\"_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "pattern"),
        ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "pattern")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("stmt"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("There is excessive valid whitespace.") {
    string EXCESSIVE_WHITESPACE_STRING = "\t\n\n  stmt \r\n\n \vs \n\n\n,\t\r\nbOOLEAN\t\r\n; assign\n\n\na; Select\t\r \n\nBOOLEAN\t\n\n\n\r\npattern\t\n\na     \t\n\r (\n\n\n_\n\n,\n\t\r_\n\"   \t\n\r\n  a  \n\n\n+    b   \t\n\n\"\n\n_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with an if 'pattern' clause.", "[query_parser]") {
  WHEN("Synonym is a valid if synonym, and parameters are wildcards.") {
    string VALID_IF_SYNONYM_STRING = "stmt s; if ifs; Select s pattern ifs(_, _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_IF_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid synonym.") {
    string FIRST_SYNONYM_STRING = "stmt s; if ifs; variable v; Select s pattern ifs(v, _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid string.") {
    string FIRST_SYNONYM_STRING = "stmt s; if ifs; Select s pattern ifs(\"string\", _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam("string"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Tricky words are used as parameter names.") {
    string TRICKY_WORDS_STRING = "stmt if; if stmt, pattern; variable BOOLEAN; Select if pattern pattern(BOOLEAN, _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "if")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "pattern"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("There is excessive valid whitespace.") {
    string TRICKY_WORDS_STRING = "		\nstmt \n\n\n	if; if		stmt, pattern; variable		\n\n BOOLEAN	;	\t\t\r \n Select if pattern		pattern  \n\n\r\n		( \t\r\nBOOLEAN  \r\n \n,	_	, \r\n _  )		";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "if")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "pattern"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query with one if 'pattern' clause, and BOOLEAN is selected.", "[query_parser]") {
  WHEN("Tricky words are used as parameter names.") {
    string TRICKY_WORDS_STRING = "stmt if; if BOOLEAN, pattern; variable stmt; Select BOOLEAN pattern BOOLEAN(stmt, _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "stmt")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Excessive whitespace is present.") {
    string EXCESSIVE_WHITESPACE_STRING = " \n\n		stmt if; if			\t BOOLEAN \n, pattern; variable stmt; Select \t\r\n  \n\r\tBOOLEAN			 pattern	 BOOLEAN \n\n(stmt, _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "stmt")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with an while 'pattern' clause.", "[query_parser]") {
  WHEN("Synonym is a valid while synonym, and parameters are wildcards.") {
    string VALID_WHILE_SYNONYM_STRING = "stmt s; while w; Select s pattern w(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHILE_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid synonym.") {
    string FIRST_SYNONYM_STRING = "stmt s; while w; variable v; Select s pattern w(v, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("First parameter is a valid string.") {
    string FIRST_SYNONYM_STRING = "stmt s; while w; Select s pattern w(\"string\", _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(FIRST_SYNONYM_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam("string"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Tricky words are used as parameter names.") {
    string TRICKY_WORDS_STRING = "stmt while; while stmt, pattern; variable BOOLEAN; Select while pattern pattern(BOOLEAN, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "while")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "pattern"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("There is excessive valid whitespace.") {
    string TRICKY_WORDS_STRING = "		\nstmt \n\n\n	while; while		stmt, pattern; variable		\n\n BOOLEAN	;	\t\t\r \n Select while pattern		pattern  \n\n\r\n		( \t\r\nBOOLEAN  \r\n \n,	 \r\n _  )		";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "while")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "pattern"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query with one while 'pattern' clause, and BOOLEAN is selected.", "[query_parser]") {
  WHEN("Tricky words are used as parameter names.") {
    string TRICKY_WORDS_STRING = "stmt while; while BOOLEAN, pattern; variable stmt; Select BOOLEAN pattern BOOLEAN(stmt, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "stmt")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Excessive whitespace is present.") {
    string EXCESSIVE_WHITESPACE_STRING = " \n\n		stmt while; while			\t BOOLEAN \n, pattern; variable stmt; Select \t\r\n  \n\r\tBOOLEAN			 pattern	 BOOLEAN \n\n(stmt, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "stmt")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query with multiple assign 'pattern' clauses of the same type.", "[query_parser]") {
  WHEN("There are two assign 'pattern' clauses, and a synonym is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "stmt s; assign a, a1; variable v; Select a pattern a(\"string\", _\"a+b\"_) pattern a1(v, \"b+c/d\")";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam("string"),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("b+c/d"), false))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("There are two assign 'pattern' clauses, and BOOLEAN is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING_BOOLEAN = "stmt s; assign a, a1; variable v; Select BOOLEAN pattern a1(v, _) pattern a(_, _\"a*b+c/d-e\"_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING_BOOLEAN);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+c/d-e"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("There are multiple assign 'pattern' clauses, and a synonym is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "assign a, a1, a2, a3, BOOLEAN; variable v, v1; Select v pattern BOOLEAN(\"valid\", _\"a+b\"_) pattern a1(v1, \"b+c/d\") pattern BOOLEAN(v, _) pattern a3(_, _) pattern a2(v1, _\"1\"_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("b+c/d"), false))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a3"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a2"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("1"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("There are multiple assign 'pattern' clauses, a synonym is selected, and excessive valid whitespace exists.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "			assign				\n\n a		, a1, a2, a3, \n\nBOOLEAN	; variable v \n\n, v1; Select v pattern \r\r\n\nBOOLEAN(      \"valid\"   , _   \t\t\n\n\"a+b\"_    ) pattern a1(v1, \n\n\n\"b+c/d\")			 pattern BOOLEAN(v, _) pattern a3(_,		 _) pattern  \r\r\n\n\n\r a2(		v1 , _\"1\"			_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("b+c/d"), false))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a3"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a2"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("1"), true))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query ifsith multiple if 'pattern' clauses of the same type.", "[query_parser]") {
  WHEN("There are tifso if 'pattern' clauses, and a synonym is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "stmt s; if ifs, ifs1; variable v; Select ifs pattern ifs(\"string\", _, _) pattern ifs1(v, _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::IF, "ifs")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam("string"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("There are tifso if 'pattern' clauses, and BOOLEAN is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING_BOOLEAN = "stmt s; if ifs, ifs1; variable v; Select BOOLEAN pattern ifs1(v, _, _) pattern ifs(_, _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING_BOOLEAN);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("There are multiple if 'pattern' clauses, and a synonym is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "if ifs, ifs1, ifs2, ifs3, BOOLEAN; variable v, v1; Select v pattern BOOLEAN(\"valid\", _, _) pattern ifs1(v1, _, _) pattern BOOLEAN(v, _, _) pattern ifs3(_, _, _) pattern ifs2(v1, _, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs3"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs2"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("There are multiple if 'pattern' clauses, a synonym is selected, and excessive valid ifshitespace exists.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "			if				\n\n a		, ifs1, ifs2, ifs3, \n\nBOOLEAN	; variable v \n\n, v1; Select v pattern \r\r\n\nBOOLEAN(      \"valid\"   ,_,  _   \t\t\n\n    ) pattern ifs1(v1, _,  \n\n\n_)			 pattern BOOLEAN(v, _, _) pattern ifs3(_,		 _,   _) pattern  \r\r\n\n\n\r ifs2(		v1 , 	_, 		_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs3"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs2"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query with multiple while 'pattern' clauses of the same type.", "[query_parser]") {
  WHEN("There are two while 'pattern' clauses, and a synonym is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "stmt s; while w, w1; variable v; Select w pattern w(\"string\", _) pattern w1(v, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam("string"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("There are two while 'pattern' clauses, and BOOLEAN is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING_BOOLEAN = "stmt s; while w, w1; variable v; Select BOOLEAN pattern w1(v, _) pattern w(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING_BOOLEAN);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("There are multiple while 'pattern' clauses, and a synonym is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "while w, w1, w2, w3, BOOLEAN; variable v, v1; Select v pattern BOOLEAN(\"valid\", _) pattern w1(v1, _) pattern BOOLEAN(v, _) pattern w3(_, _) pattern w2(v1, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w3"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w2"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("There are multiple while 'pattern' clauses, a synonym is selected, and excessive valid whitespace exists.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "			while				\n\n a		, w1, w2, w3, \n\nBOOLEAN	; variable v \n\n, v1; Select v pattern \r\r\n\nBOOLEAN(      \"valid\"   , _   \t\t\n\n    ) pattern w1(v1, \n\n\n_)			 pattern BOOLEAN(v, _) pattern w3(_,		 _) pattern  \r\r\n\n\n\r w2(		v1 , 			_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w3"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w2"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query with multiple 'pattern' clauses of type assign, if, and while.", "[query_parser]") {
  WHEN("There are multiple 'pattern' clauses of all three types, and a synonym is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "assign a, a1; if ifs, ifs1; while w, BOOLEAN; variable v, v1; Select v pattern BOOLEAN(\"valid\", _) pattern a1(v1, _\"a*b+22*3\"_) pattern ifs(v, _, _) pattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) pattern w(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("There are multiple 'pattern' clauses of all three types, and BOOLEAN is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "assign a, a1; if ifs, ifs1; while w, BOOLEAN; variable v, v1; Select BOOLEAN pattern BOOLEAN(\"valid\", _) pattern a1(v1, _\"a*b+22*3\"_) pattern ifs(v, _, _) pattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) pattern w(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("There are multiple 'pattern' clauses of all three types, and excessive valid whitespace exists.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "assign a, \n\n a1; \r \t \n\n if	ifs		, \n\n ifs1; \n\n while			w,	 BOOLEAN; variable v, v1; Select v pattern BOOLEAN \n\n	(\"valid\", _) \r\n\n\r \t	pattern		a1	 (\r \r\rv1, _\"a*b+22*3\"_) pattern\n\nifs(v, _, _)	  pattern		a(_, \"test\") pattern ifs1		(\"fantastic\", _	, _) \r\n\n		pattern w	(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}
