#include <string>

#include "catch.hpp"
#include "parser_utils/ExpressionParser.h"
#include "query_processor/commons/query/Query.h"
#include "query_processor/query_parser/QueryParser.h"

using namespace std;
using namespace query_processor;

SCENARIO("Testing GenerateQuery() when query contains only 'such that' clauses and uses 'and', but invalidly.", "[query_parser]") {
  WHEN("'and' is not in one word.") {
    string NOT_ONE_WORD_AND_STRING = "stmt s; Select s such that Follows(1, s) a nd Parent(1, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NOT_ONE_WORD_AND_STRING));
    }
  }

  WHEN("'and' is used for the first clause.") {
    string FIRST_CLAUSE_AND_STRING = "stmt s; Select s and Follows(1, s) such that Parent(1, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_CLAUSE_AND_STRING));
    }
  }

  WHEN("'and' is used in addition to 'such that'.") {
    string SUCH_THAT_INCLUDING_AND_STRING = "stmt s; Select s such that Follows(1, s) and such that Parent(1, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SUCH_THAT_INCLUDING_AND_STRING));
    }
  }

  WHEN("'and' is used twice.") {
    string AND_USED_TWICE_STRING = "stmt s; Select s such that Follows(1, s) and and Parent(1, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(AND_USED_TWICE_STRING));
    }
  }

  WHEN("'and' is missing.") {
    string MISSING_AND_STRING = "stmt s; Select s such that Follows(1, s) Parent(1, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(MISSING_AND_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() when query contains only 'pattern' clauses and uses 'and', but invalidly.", "[query_parser]") {
  WHEN("'and' is not in one word.") {
    string NOT_ONE_WORD_AND_STRING = "while w; Select w pattern w(_, _) a nd w(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NOT_ONE_WORD_AND_STRING));
    }
  }

  WHEN("'and' is used for the first clause.") {
    string FIRST_CLAUSE_AND_STRING = "while w; Select w and w(_, _) pattern w(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_CLAUSE_AND_STRING));
    }
  }

  WHEN("'and' is used in addition to 'pattern'.") {
    string PATTERN_INCLUDING_AND_STRING = "while w; Select w pattern w(_, _) and pattern w(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(PATTERN_INCLUDING_AND_STRING));
    }
  }

  WHEN("'and' is used twice.") {
    string AND_USED_TWICE_STRING = "while w; Select w pattern w(_, _) and and w(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(AND_USED_TWICE_STRING));
    }
  }

  WHEN("'and' is missing.") {
    string MISSING_AND_STRING = "while w; Select w pattern w(_, _) w(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(MISSING_AND_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() when query contains both 'such that' and 'pattern' clauses, and uses 'and' invalidly.", "[query_parser]") {
  WHEN("'and' is used to directly connect two different conditional clause types.") {
    string DIRECT_DIFFERENT_CLAUSES_STRING = "assign a; stmt s; Select s such that Follows(1, s) and a(_, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(DIRECT_DIFFERENT_CLAUSES_STRING));
    }
  }

  WHEN("'and' is used to indirectly connect two different conditional clause types.") {
    string INDIRECT_DIFFERENT_CLAUSES_STRING = "assign a; if ifs; stmt s; Select s pattern a(_, _) and ifs(_, _, _) and Uses(s, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INDIRECT_DIFFERENT_CLAUSES_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() when query contains only 'such that' clauses, and uses 'and' validly.", "[query_parser]") {
  WHEN("The query uses 'and' to directly connect two clauses.") {
    string DIRECT_AND_SUCH_THAT_STRING = "stmt s1, s2, s3; Select s1 such that Parent(s1, s2) and Follows(s2, s3)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(DIRECT_AND_SUCH_THAT_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s3"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query uses 'and' to directly connect two clauses, and BOOLEAN is selected.") {
    string DIRECT_AND_SUCH_THAT_STRING = "stmt s1, s2, s3; Select BOOLEAN such that Parent(s1, s2) and Follows(s2, s3)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(DIRECT_AND_SUCH_THAT_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s3"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query uses 'and' to indirectly connect multiple clauses.") {
    string INDIRECT_AND_SUCH_THAT_STRING = "stmt s1, s2, s3; assign a1, a2; procedure p1, p2; prog_line n; while w1; variable v1, BOOLEAN; Select w1 such that Parent(s1, a1) and Affects*(s1, 3) such that Next(5, n) and Calls*(p1, p2) and Follows*(s2, a2) and Modifies(p2, v1) such that Uses(w1, BOOLEAN)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INDIRECT_AND_SUCH_THAT_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w1")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1"))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("The query uses 'and' to indirectly connect multiple clauses, and excessive valid whitespace is used.") {
    string INDIRECT_AND_SUCH_THAT_STRING = "stmt s1, s2, s3; assign a1, a2; procedure p1, p2; prog_line n; while w1; variable v1, BOOLEAN; Select		\n w1	\t\r\nsuch that Parent  (s1, a1) such that Affects*(s1, 3) such that Next(5, n)and  \n\rCalls*		(p1, p2) and Follows*(s2, a2)\r\nand	 Modifies(p2, v1)		such that\t Uses(w1, BOOLEAN)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INDIRECT_AND_SUCH_THAT_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w1")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1"))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN"))));

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

SCENARIO("Testing GenerateQuery() when query contains only 'pattern' clauses, and uses 'and' validly.", "[query_parser]") {
  WHEN("The query uses 'and' to directly connect two clauses.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "stmt s; assign a, a1; variable v; Select a pattern a(\"string\", _\"a+b\"_) and a1(v, \"b+c/d\")";
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

  WHEN("The query uses 'and' to directly connect two clauses, and BOOLEAN is selected.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "stmt s; assign a, a1; variable v; Select BOOLEAN pattern a(\"string\", _\"a+b\"_) and a1(v, \"b+c/d\")";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_PATTERN_CLAUSE_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam("string"),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a+b"), true))));
    EXPECTED_QUERY.AddClause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("b+c/d"), false))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query uses 'and' to indirectly connect multiple clauses.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "assign a, a1; if ifs, ifs1; while w, BOOLEAN; variable v, v1; Select v pattern BOOLEAN(\"valid\", _) and a1(v1, _\"a*b+22*3\"_) and ifs(v, _, _) and a(_, \"test\") and ifs1(\"fantastic\", _, _) pattern w(_, _)";
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

  WHEN("The query uses 'and' to indirectly connect multiple clauses, and excessive valid whitespace exists.") {
    string MULTIPLE_PATTERN_CLAUSE_STRING = "assign a, a1; if ifs, ifs1; while w, BOOLEAN; variable v, v1; Select v		\n\npattern BOOLEAN		(\"valid\", _) \r\n and\n\na1(v1, _\"a*b+22*3\"_)   and\r\nifs(v, _, _) and\r\n\ta(_, \"test\")and		 ifs1(\"fantastic\", _, _)pattern	w(_, _)";
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

SCENARIO("Testing GenerateQuery() when query contains only 'with' clauses, and uses 'and' validly.", "[query_parser]") {
  WHEN("The query uses 'and' to directly connect two clauses.") {
    string TWO_WITH_CLAUSES_STRING = "prog_line n, n1; Select n with 1 = 1 and n = n1";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WITH_CLAUSES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n1")), AttributeType::NONE))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query uses 'and' to directly connect two clauses, and BOOLEAN is selected.") {
    string TWO_SYNONYM_BOOLEAN_WITH_STRING = "assign a; read BOOLEAN; Select BOOLEAN with BOOLEAN.varName = \"BOOLEAN\" and 20 = a.stmt#";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYM_BOOLEAN_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "BOOLEAN")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query uses 'and' to indirectly connect multiple clauses.") {
    string MULTIPLE_SYNONYMS_WITH_STRING = "assign a; constant c; stmt s; prog_line n; variable BOOLEAN; Select s with n = 3 and 2 = 1 and BOOLEAN.varName = \"BOOLEAN\" and 20 = a.stmt# with c.value = 2";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_SYNONYMS_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("The query uses 'and' to indirectly connect multiple clauses, and excessive valid whitespace exists.") {
    string MULTIPLE_SYNONYMS_WHITESPACE_WITH_STRING = "assign a; constant c; stmt s; prog_line n; variable BOOLEAN; Select s with n =3		and 2= 1	and	\n\n BOOLEAN.varName	= \"BOOLEAN\" and\n\n20	\t=		a.stmt# with c.value = 2";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_SYNONYMS_WHITESPACE_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));

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

SCENARIO("Testing GenerateQuery() when query contains 'such that', 'pattern', and 'with' clauses, and uses 'and' validly.", "[query_parser]") {
  WHEN("'and' is used only to directly link clauses of the same type.") {
    string MULTIPLE_ALL_CLAUSE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select v with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) and ifs(v, _, _) with 20 = a.stmt# such that Follows*(s2, ifs1) pattern a(_, \"test\") and ifs1(\"fantastic\", _, _) such that Modifies(p2, v1) with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) with n = 3 and 2 = 1 pattern w(_, _) such that Calls*(p1, p2) such that Parent(s1, a1) and Affects*(s1, 3) such that Next(5, n)";
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
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
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

  WHEN("'and' is used to indirectly link clauses of the same type.") {
    string MULTIPLE_ALL_CLAUSE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select v with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) and ifs(v, _, _) and a(_, \"test\") with 20 = a.stmt# and n = 3 and 2 = 1 such that Modifies(p2, v1) and Calls*(p1, p2) such that Parent(s1, a1) such that Affects*(s1, 3) and Next(5, n) such that Follows*(s2, ifs1) pattern ifs1(\"fantastic\", _, _) with r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) pattern w(_, _)";
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
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));

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

SCENARIO("Testing GenerateQuery() given a valid query with multiple 'such that', 'pattern', and 'with' clauses, where a synonym attribute is selected, and 'and' is used validly.", "[query_parser]") {
  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses including duplicates, a synonym attribute is selected, and 'and' is used to indirectly link clauses together.") {
    string MULTIPLE_ALL_CLAUSE_ATTRIBUTE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; Select BOOLEAN.stmt# with c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) and ifs(v, _, _) and a(_, \"test\") with 20 = a.stmt# and n = 3 and 2 = 1 such that Follows*(s2, ifs1) and Modifies(p2, v1) and Calls*(p1, p2) such that Parent(s1, a1) and Affects*(s1, 3) and Next(5, n) such that Follows*(s2, ifs1) pattern ifs1(\"fantastic\", _, _) with r.varName = \"BOOLEAN\" and r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) pattern w(_, _)";
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
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::WHILE, "w"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));

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

SCENARIO("Testing GenerateQuery() given a valid query with multiple 'such that', 'pattern', and 'with' clauses, where a tuple is selected, and 'and' is used validly.", "[query_parser]") {
  WHEN("There are multiple 'such that', 'pattern', and 'with' clauses including duplicates, and a tuple with multiple elements of synonyms and synonym attributes, with duplicates and excessive valid whitespace, is selected, and 'and' is used to indirectly link clauses together.") {
    string MULTIPLE_ALL_CLAUSE_TUPLE_DUPLICATE_WHITESPACE_STRING = "stmt s1, s2, s3; assign a, a1; constant c; if ifs, ifs1; print pn; prog_line n; read r; while w, w1, BOOLEAN; procedure p1, p2; variable v, v1; \n\n	Select		<\n\ns1.stmt#	,	s1\n,c.value ,c	,\npn.varName, c   , pn.stmt#	\n> \nwith c.value = 2 such that Uses(w1, v) pattern BOOLEAN(\"valid\", _) and ifs(v, _, _) and a(_, \"test\") with 20 = a.stmt# and n = 3 and 2 = 1 such that Follows*(s2, ifs1) and Modifies(p2, v1) and Calls*(p1, p2) such that Parent(s1, a1) and Affects*(s1, 3) and Next(5, n) such that Follows*(s2, ifs1) pattern ifs1(\"fantastic\", _, _) with r.varName = \"BOOLEAN\" and r.varName = \"BOOLEAN\" pattern a1(v1, _\"a*b+22*3\"_) pattern w(_, _)";
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
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("test"), false)))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::MODIFIES,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::CALLS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::PARENT,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                   ClauseParam(3))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::NEXT,
                                                   ClauseParam(5),
                                                   ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")))));
    EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                   ClauseParam(DesignEntity(DesignEntityType::IF, "ifs1")))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::IF, "ifs1"),
        ClauseParam("fantastic"),
        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(PatternClause(
        DesignEntity(DesignEntityType::ASSIGN, "a1"),
        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
        ClauseParam(PatternExpression(parser_utils::ExpressionParser::ParseExpression("a*b+22*3"), true)))));
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
