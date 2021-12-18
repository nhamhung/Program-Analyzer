#include <string>

#include "catch.hpp"
#include "parser_utils/ExpressionParser.h"
#include "query_processor/commons/query/Query.h"
#include "query_processor/query_parser/QueryParser.h"

using namespace std;
using namespace query_processor;

SCENARIO("Testing GenerateQuery() on invalid queries with both 'such that', 'pattern', and 'with' clauses.", "[query_parser]") {
  WHEN("The 'such that' clause is valid but the 'pattern' clause is invalid, in that order.") {
    string VALID_SUCH_THAT_INVALID_PATTERN_STRING = "stmt s, s1; assign a; Select a such that Follows(s, s1) pattern a(_, a+b)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(VALID_SUCH_THAT_INVALID_PATTERN_STRING));
    }
  }

  WHEN("The 'such that' clause is invalid but the 'pattern' clause is valid, in that order.") {
    string INVALID_SUCH_THAT_VALID_PATTERN_STRING = "stmt s, s1; assign a; Select a such that Follows(1s, s) pattern a(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SUCH_THAT_VALID_PATTERN_STRING));
    }
  }

  WHEN("The 'pattern' clause is valid but the 'such that' clause is invalid, in that order.") {
    string VALID_PATTERN_INVALID_SUCH_THAT_STRING = "stmt s, s1; assign a; Select a pattern a(_, _) such that Follows(1s, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(VALID_PATTERN_INVALID_SUCH_THAT_STRING));
    }
  }

  WHEN("The 'pattern' clause is invalid but the 'such that' clause is valid, in that order.") {
    string INVALID_PATTERN_VALID_SUCH_THAT_STRING = "stmt s, s1; assign a; Select a pattern a(_, a+b) such that Follows(s, s1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PATTERN_VALID_SUCH_THAT_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query with invalid semantics with a 'such that' followed by a 'pattern' clause.", "[query_parser]") {
  WHEN("The synonym in the 'such that' clause is not declared.") {
    string UNDECLARED_SUCH_THAT_SYNONYM = "stmt s; assign a; Select s such that Follows(n, _) pattern a(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_SUCH_THAT_SYNONYM));
    }
  }

  WHEN("The synonym in the 'pattern' clause is not declared.") {
    string UNDECLARED_PATTERN_SYNONYM = "stmt s; assign a; Select s such that Follows(s, _) pattern n(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PATTERN_SYNONYM));
    }
  }

  WHEN("The synonym in the 'pattern' clause is not an assign synonym.") {
    string NOT_ASSIGN_PATTERN_SYNONYM = "stmt s; assign a; Select s such that Follows(s, _) pattern s(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NOT_ASSIGN_PATTERN_SYNONYM));
    }
  }

  WHEN("The first parameter of the 'pattern' clause is not declared.") {
    string UNDECLARED_FIRST_PARAM = "stmt s; assign a; Select s such that Follows(s, _) pattern a(n, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_FIRST_PARAM));
    }
  }

  WHEN("The second parameter of the 'pattern' clause contains invalid characters.") {
    string INVALID_CHARS_SECOND_PARAM = "stmt s; assign a; Select s such that Follows(_, s) pattern a(s, _\" a + b_\"_)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_CHARS_SECOND_PARAM));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a query with invalid semantics with a 'pattern' followed by a 'such that' clause.", "[query_parser]") {
  WHEN("The synonym in the 'such that' clause is not declared.") {
    string UNDECLARED_SUCH_THAT_SYNONYM = "stmt s; assign a; Select s pattern a(_, _) such that Follows(n, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_SUCH_THAT_SYNONYM));
    }
  }

  WHEN("The synonym in the 'pattern' clause is not declared.") {
    string UNDECLARED_PATTERN_SYNONYM = "stmt s; assign a; Select s pattern n(_, _) such that Follows(s, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PATTERN_SYNONYM));
    }
  }

  WHEN("The synonym in the 'pattern' clause is not an assign synonym.") {
    string NOT_ASSIGN_PATTERN_SYNONYM = "stmt s; assign a; Select s pattern s(_, _) such that Follows(s, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NOT_ASSIGN_PATTERN_SYNONYM));
    }
  }

  WHEN("The first parameter of the 'pattern' clause is not declared.") {
    string UNDECLARED_FIRST_PARAM = "stmt s; assign a; Select s pattern a(n, _) such that Follows(s, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_FIRST_PARAM));
    }
  }

  WHEN("The second parameter of the 'pattern' clause contains invalid characters.") {
    string INVALID_CHARS_SECOND_PARAM = "stmt s; assign a; Select s pattern a(s, _\" a + b_\"_) such that Follows(_, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_CHARS_SECOND_PARAM));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with a 'such that' clause followed by a 'pattern' clause.", "[query_parser]") {
  WHEN("All synonyms are wildcards.") {
    string ALL_WILDCARDS = "assign a; Select a such that Follows(_, _) pattern a(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(ALL_WILDCARDS);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("All values are populated with tricky words.") {
    string TRICKY_WORDS = "stmt Select, Follows; assign while, pattern; Select pattern such that Follows(Follows, Follows) pattern pattern (pattern, _\" pattern + assign + while \"_)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "pattern")));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "Follows")),
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "Follows")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(DesignEntity(DesignEntityType::ASSIGN, "pattern"),
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "pattern")),
                                                  ClauseParam(PatternExpression(
                                                      parser_utils::ExpressionParser::ParseExpression(
                                                          "pattern + assign + while"),
                                                      true)))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("All values are populated with tricky words and excessive valid whitespace is present.") {
    string TRICKY_WORDS_AND_WHITESPACES = "stmt Select, Follows; assign while, pattern; Select pattern\n\nsuch that Follows  \t\r\n\n\n(Follows,    Follows)\n\n\npattern pattern      \n\n\n (pattern, _    \t\r\n\" pattern\n\n\n\r\n+ assign + \n\nwhile \"_ \t\n\r  )";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_AND_WHITESPACES);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "pattern")));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "Follows")),
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "Follows")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(DesignEntity(DesignEntityType::ASSIGN, "pattern"),
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "pattern")),
                                                  ClauseParam(PatternExpression(
                                                      parser_utils::ExpressionParser::ParseExpression(
                                                          "pattern + assign + while"),
                                                      true)))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with a 'pattern' clause followed by a 'such that' clause.", "[query_parser]") {
  WHEN("All synonyms are wildcards.") {
    string ALL_WILDCARDS = "assign a; Select a pattern a(_, _) such that Follows(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(ALL_WILDCARDS);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("All values are populated with tricky words.") {
    string TRICKY_WORDS = "stmt Select, Follows; assign while, pattern; Select pattern pattern pattern (pattern, _\" pattern + assign + while \"_) such that Follows(Follows, Follows)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "pattern")));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(DesignEntity(DesignEntityType::ASSIGN, "pattern"),
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "pattern")),
                                                  ClauseParam(PatternExpression(
                                                      parser_utils::ExpressionParser::ParseExpression(
                                                          "pattern + assign + while"),
                                                      true)))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "Follows")),
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "Follows")))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("All values are populated with tricky words and excessive valid whitespace is present.") {
    string TRICKY_WORDS_AND_WHITESPACES = "stmt Select, Follows; assign while, pattern; Select pattern \t\t\n\npattern pattern      \n\n\n (pattern, _    \t\r\n\" pattern\n\n\n\r\n+ assign + \n\nwhile \"_ \t\n\r )\n\nsuch that Follows  \t\r\n\n\n(Follows,    Follows)\n\n\n ";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TRICKY_WORDS_AND_WHITESPACES);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "pattern")));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(DesignEntity(DesignEntityType::ASSIGN, "pattern"),
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "pattern")),
                                                  ClauseParam(PatternExpression(
                                                      parser_utils::ExpressionParser::ParseExpression(
                                                          "pattern + assign + while"),
                                                      true)))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "Follows")),
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "Follows")))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with multiple 'such that', 'pattern', and 'with' clauses.", "[query_parser]") {
  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses, and a synonym is selected.") {
    string MULTIPLE_ALL_CLAUSE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select v with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) pattern ifs(v, _, _) with 20 = a.stmt# such that Follows*(s2, ifs1) pattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) such that Modifies(p2, v1) with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) with n = 3 with 2 = 1 such that Calls*(p1, p2) pattern w(_, _) such that Parent(s1, a1) such that Affects*(s1, 3) such that Next(5, n)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_ALL_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::USES,
                                                   ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses, a synonym is selected, and excessive valid whitespace exists.") {
    string MULTIPLE_ALL_CLAUSE_WHITESPACE_STRING = " \n\n stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select	 v with  \n\n  c.value \t\r\n =2	 such that   Uses(w1, v) \r\n pattern		 BOOLEAN(\"valid\", _) pattern ifs(v, _, _)with 20 = a.stmt# such that Follows*(s2, ifs1)\n\npattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) such that Modifies(p2, v1)			with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) with n = 3 with 2 = 1 \t\r\n  \n\nsuch that Calls*(p1, p2) pattern w(_, _) such that Parent(s1, a1) such that Affects*(s1, 3) such that Next(5, n)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_ALL_CLAUSE_WHITESPACE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::USES,
                                                   ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));

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

SCENARIO("Testing GenerateQuery() given a valid query with multiple 'such that', 'pattern', and 'with' clauses, where BOOLEAN is selected.", "[query_parser]") {
  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses, and BOOLEAN is selected.") {
    string MULTIPLE_ALL_CLAUSE_BOOLEAN_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select BOOLEAN with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) pattern ifs(v, _, _) with 20 = a.stmt# such that Follows*(s2, ifs1) pattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) such that Modifies(p2, v1) with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) with n = 3 with 2 = 1 such that Calls*(p1, p2) pattern w(_, _) such that Parent(s1, a1) such that Affects*(s1, 3) such that Next(5, n)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_ALL_CLAUSE_BOOLEAN_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::USES,
                                                   ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with multiple 'such that', 'pattern', and 'with' clauses, where a synonym attribute is selected.", "[query_parser]") {
  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses, and a synonym attribute is selected.") {
    string MULTIPLE_ALL_CLAUSE_ATTRIBUTE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select BOOLEAN.stmt# with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) pattern ifs(v, _, _) with 20 = a.stmt# such that Follows*(s2, ifs1) pattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) such that Modifies(p2, v1) with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) with n = 3 with 2 = 1 such that Calls*(p1, p2) pattern w(_, _) such that Parent(s1, a1) such that Affects*(s1, 3) such that Next(5, n)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_ALL_CLAUSE_ATTRIBUTE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::WHILE, "BOOLEAN"), AttributeType::STMT_NO)));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::USES,
                                                   ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::ATTRIBUTE);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().attribute == EXPECTED_QUERY.GetSelectedEntity().attribute);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with multiple 'such that', 'pattern', and 'with' clauses, where a tuple is selected.", "[query_parser]") {
  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses, and a tuple with a single element is selected.") {
    string MULTIPLE_ALL_CLAUSE_TUPLE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select <BOOLEAN.stmt#> with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) pattern ifs(v, _, _) with 20 = a.stmt# such that Follows*(s2, ifs1) pattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) such that Modifies(p2, v1) with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) with n = 3 with 2 = 1 such that Calls*(p1, p2) pattern w(_, _) such that Parent(s1, a1) such that Affects*(s1, 3) such that Next(5, n)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_ALL_CLAUSE_TUPLE_STRING);

    Query EXPECTED_QUERY = Query(std::vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::WHILE, "BOOLEAN"), AttributeType::STMT_NO))});

    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::USES,
                                                   ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(0).entity_type == SelectedEntityType::ATTRIBUTE);
      REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(0).attribute == EXPECTED_QUERY.GetSelectedEntities().at(0).attribute);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses including duplicates, and a tuple with multiple elements of synonyms and synonym attributes, with duplicates, is selected.") {
    string MULTIPLE_ALL_CLAUSE_TUPLE_DUPLICATE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; print pn; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select <s1.stmt#, s1, c.value, c, pn.varName, c, pn.stmt#> with c.value = 2 with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) pattern ifs(v, _, _) with 20 = a.stmt# such that Follows*(s2, ifs1) pattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) such that Modifies(p2, v1) with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) with n = 3 with 2 = 1 such that Calls*(p1, p2) pattern w(_, _) such that Parent(s1, a1) such that Affects*(s1, 3) such that Next(5, n) pattern w(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_ALL_CLAUSE_TUPLE_DUPLICATE_STRING);

    Query EXPECTED_QUERY = Query(std::vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::STMT, "s1"), AttributeType::STMT_NO)),
        SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CONSTANT, "c"), AttributeType::VALUE)),
        SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "c")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::PRINT, "pn"), AttributeType::VAR_NAME)),
        SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "c")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::PRINT, "pn"), AttributeType::STMT_NO))});

    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::USES,
                                                   ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));

    THEN("GenerateQuery() returns the expected Query.") {
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetSelectedEntities().size(); i++) {
        if (i == 0 || i == 2 || i == 4 || i == 6) {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::ATTRIBUTE);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).attribute == EXPECTED_QUERY.GetSelectedEntities().at(i).attribute);
        } else {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::DESIGN_ENTITY);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).design_entity == EXPECTED_QUERY.GetSelectedEntities().at(i).design_entity);
        }
      }
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses including duplicates, and a tuple with multiple elements of synonyms and synonym attributes, with duplicates and excessive valid whitespace, is selected.") {
    string MULTIPLE_ALL_CLAUSE_TUPLE_DUPLICATE_WHITESPACE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; print pn; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; \n\n	Select		<\n\ns1.stmt#	,	s1\n,c.value ,c	,\npn.varName, c   , pn.stmt#	\n> \nwith c.value = 2		 with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) pattern ifs(v, _, _) with 20 = a.stmt# such that Follows*(s2, ifs1) pattern a(_, \"test\") pattern ifs1(\"fantastic\", _, _) such that Modifies(p2, v1) with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) with n = 3 with 2 = 1 such that Calls*(p1, p2) pattern w(_, _) such that Parent(s1, a1) such that Affects*(s1, 3) such that Next(5, n) pattern w(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_ALL_CLAUSE_TUPLE_DUPLICATE_WHITESPACE_STRING);

    Query EXPECTED_QUERY = Query(std::vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::STMT, "s1"), AttributeType::STMT_NO)),
        SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CONSTANT, "c"), AttributeType::VALUE)),
        SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "c")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::PRINT, "pn"), AttributeType::VAR_NAME)),
        SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "c")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::PRINT, "pn"), AttributeType::STMT_NO))});

    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::USES,
                                                   ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "BOOLEAN"),
        ClauseParam("valid"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));

    THEN("GenerateQuery() returns the expected Query.") {
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetSelectedEntities().size(); i++) {
        if (i == 0 || i == 2 || i == 4 || i == 6) {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::ATTRIBUTE);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).attribute == EXPECTED_QUERY.GetSelectedEntities().at(i).attribute);
        } else {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::DESIGN_ENTITY);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).design_entity == EXPECTED_QUERY.GetSelectedEntities().at(i).design_entity);
        }
      }
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}
