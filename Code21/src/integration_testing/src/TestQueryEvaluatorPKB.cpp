#include <iostream>

#include "BuildPKBUtils.h"
#include "TestUtils.h"
#include "catch.hpp"
#include "query_processor/commons/query/Query.h"
#include "query_processor/commons/query/clause/SuchThatClause.h"
#include "query_processor/commons/query/entities/DesignEntity.h"
#include "query_processor/query_evaluator/QueryEvaluator.h"
#include "source_processor/token/TokenList.h"

using namespace std;
using namespace query_processor;
using namespace source_processor;

SCENARIO("Test Follows queries", "[query, follows]") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    PKB pkb_nested = BuildNestedProgram();

    WHEN("Follows(INT, INT) evaluates to true, second INT separated from first INT by containers") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(1), ClauseParam(9));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select a such that Follows(1, 9) returns all assignment statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("Follows(INT, INT) evaluates to false") {
      Query valid_query = Query(DesignEntity(DesignEntityType::STMT, "s"));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(13), ClauseParam(15));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select s such that Follows(13, 15) returns empty result") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Follows(INT, INT) evaluates to false, ints separated by procedure") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(9), ClauseParam(10));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select s such that Follows(9, 10) results empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }
    WHEN("Follows(DE, DE) evaluates to true, selected DE different from evaluated DEs") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::CALL, "cl")));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select w such that Follows(a, cl) returns all while statements") {
        //pkb code 4 not nested pkb
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
      }
    }

    WHEN("Follows(INT, DE) evaluates to true, evaluated DE is selected") {
      DesignEntity print_entity = DesignEntity(DesignEntityType::PRINT, "pn");
      Query valid_query = Query(print_entity);
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(7), ClauseParam(print_entity));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select pn such that Follows(7, pn) returns statement 8") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
      }
    }

    WHEN("Follows(DE, DE) evaluates to true, selected DE is same type but different synonym from evaluated DE") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::CALL, "c1")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::CALL, "c2")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select c1 such that Follows(c2, w) returns all call statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 13));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 18));
      }
    }

    WHEN("Follows(DE, DE) evaluates to false, DE separated by if-else container") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn1")),
                                                     ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn2")));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Follows(pn1, pn2) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Follows(DE, _) evaluates to true, evaluated DE is selected") {
      DesignEntity assign_entity = DesignEntity(DesignEntityType::ASSIGN, "a");
      DesignEntity wildcard_entity = DesignEntity(DesignEntityType::WILDCARD, "_");
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(assign_entity),
                                                     ClauseParam(wildcard_entity));
      Query valid_query = Query(assign_entity);
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a such that Follows(a, _) returns all assignment statements that are not the last statement in a statement list") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 8);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 1));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 10));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 11));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 12));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 15));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 17));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
      }
    }

    WHEN("Follows(_, DE) evaluates to false, all preceding statements are parent statements") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Follows(_, a) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }
  }
}

SCENARIO("Test Follows* queries", "[query]") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("FollowsT(INT, DE) evaluates to true and evaluated DE is selected") {
      DesignEntity stmt_entity = DesignEntity(DesignEntityType::STMT, "s");
      Query valid_query = Query(stmt_entity);
      SuchThatClause followsT_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T, ClauseParam(12), ClauseParam(stmt_entity));
      valid_query.AddClause(Clause(followsT_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select s that that FollowsT(12, s) returns statement 13, 14 19, and 23") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 13));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 19));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 23));
      }
    }

    WHEN("FollowsT(DE, DE) evaluates to true, both DEs are of the same type but of different synonyms") {
      DesignEntity call_entity_1 = DesignEntity(DesignEntityType::CALL, "c1");
      DesignEntity call_entity_2 = DesignEntity(DesignEntityType::CALL, "c2");
      SuchThatClause followsT_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T, ClauseParam(call_entity_1),
                                                      ClauseParam(call_entity_2));
      Query valid_query = Query(call_entity_1);
      valid_query.AddClause(Clause(followsT_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select c1 such that Follows(c1, c2) returns call statement that comes before other call statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
      }
    }

    WHEN("FollowsT(DE, INT) evaluates to true, but evaluated DE is not selected") {
      DesignEntity while_entity = DesignEntity(DesignEntityType::WHILE, "w");
      DesignEntity read_entity = DesignEntity(DesignEntityType::READ, "r");
      SuchThatClause followsT_clause_returns_read = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                                   ClauseParam(while_entity), 23);
      Query valid_query_returns_read = Query(read_entity);
      valid_query_returns_read.AddClause(Clause(followsT_clause_returns_read));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query_returns_read, &pkb_code_4, false);
      THEN("Select r such that Follows(w, 23) returns all read statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("FollowsT(DE, INT) evaluates to false, INT is the same DE type as evaluated DE") {
      DesignEntity while_entity = DesignEntity(DesignEntityType::WHILE, "w");
      SuchThatClause followsT_clause_returns_none = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                                   ClauseParam(while_entity), 14);
      Query valid_query_returns_none = Query(while_entity);
      valid_query_returns_none.AddClause(Clause(followsT_clause_returns_none));
      QueryResult test_result_reads = QueryEvaluator::EvaluateQuery(valid_query_returns_none, &pkb_code_4, false);
      THEN("Select w such that Follows(w, 14) returns empty result") {
        REQUIRE(test_result_reads.statement_indexes_or_constants.empty());
      }
    }
  }
}

SCENARIO("Test Parent queries", "[query][parent]") {
  GIVEN("Nested PKB") {
    PKB pkb_nested = BuildNestedProgram();

    WHEN("Parent(INT, INT) evaluates to true and selected DE exists") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(1),
                                                    ClauseParam(2));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent(1, 2) returns all print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Parent(INT, INT) evaluates to true and selected DE does not exist in the program") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::READ, "r")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(1),
                                                    ClauseParam(2));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select r such that Parent(1, 2) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Parent(INT, DE) evaluates to true, selected DE same as evaluated DE") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(6),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent(6, pn) returns print results nested in statement 6") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
      }
    }

    WHEN("Parent(INT, DE) evaluates to false and DE is deeply nested in first INT, selected DE exists") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(1),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent(1, pn) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Parent(INT, DE) evaluates to false and INT and DE are in the same level, nested in another container") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(4),
                                                    ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Parent(4, ifs) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Parent(DE, INT) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(2));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select a such that Parent(w, 2) returns all assignment statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("Parent(DE, DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Parent(w, a) returns all statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 9);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 1));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Parent(DE, DE) evaluates to false, second DE is deeply nested in first DE, selected DE exists") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Parent(w, pn) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }
    WHEN("Parent(DE, DE) evaluates to true, same DE type but different synonym") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w2")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w2")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select w2 such that Parent(w1, w2) returns all nested whiles") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 4));
      }
    }

    WHEN("Parent(DE, DE) evaluates to false, both synonyms are same as each other") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Parent(w, w) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Parent(INT, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(6),
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent(6, _) returns all print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Parent (DE, _) evaluates to false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Parent(a, _) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Parent(_, DE) evaluates to true, only nested print statements are returned") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent(_, pn) returns nested print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
      }
    }

    WHEN("Parent(_, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent(_,_) returns all print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }
  }
}

SCENARIO("Test Parent* queries", "[query][parent]") {
  GIVEN("Nested PKB") {
    PKB pkb_nested = BuildNestedProgram();
    WHEN("Parent*(INT, INT) evaluates to true and second INT is deeply nested in first INT") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(1),
                                                    ClauseParam(8));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent*(1, 8) returns all print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Parent*(INT, DE) evaluates to true, selected DE same as evaluated DE") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(1),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent*(2, pn) returns print results deeply nested in statement 2") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
      }
    }

    WHEN("Parent*(INT, DE) evaluates to false and DE nested in another statement in the same statement list") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(4),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent*(4, pn) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Parent*(DE, INT) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(5));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select a such that Parent*(w, 5) returns all assignment statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("Parent*(DE, DE) evaluates to true, second DE is deeply nested in first DE") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select a such that Parent*(w, pn) returns all assignment statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("Parent*(DE, DE) evaluates to false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Parent(ifs, a) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Parent*(INT, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(6),
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent*(6, _) returns all print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Parent*(DE, _) evaluates to false, DE is not in the deepest level but it is not a parent") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select s such that Parent(a, _) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Parent*(_, DE) evaluates to true, only nested pn statements are returned") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent*(_, pn) returns nested print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
      }
    }

    WHEN("Parent*(_, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_nested, false);
      THEN("Select pn such that Parent*(_,_) returns all print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }
  }
}

SCENARIO("Test Uses queries", "[query][uses") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("Uses(INT, NAME) evaluates to true, variable is part of expression") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::CALL, "c")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(23),
                                                  ClauseParam("cenY"));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select c such that Uses(23, \"cenY\") returns all call statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 13));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 18));
      }
    }
    WHEN("Uses(INT, DE) returns to false, statement is assignment statement assigned to constant") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(10),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a such that Uses(10, v) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Uses(NAME, DE) evaluates to true, procedure calls another procedure that uses the variable") {
      THEN("Select pn such that Uses(\"main\", \"y\")") {
        //TODO: To add once UsesP is fully implemented
      }
    }

    WHEN("Uses(DE, NAME) evaluates to true, variable is used in condition of container") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                  ClauseParam("x"));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select w such that Uses(w, \"x\") returns {14}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
      }
    }

    WHEN("Uses(DE, DE) evaluates to true, container DE uses some variable return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select v such that Uses(ifs, v) returns {count, cenX, cenY}") {
        REQUIRE(test_result.var_proc_names.size() == 3);
        REQUIRE(Contains(test_result.var_proc_names, "count"));
        REQUIRE(Contains(test_result.var_proc_names, "cenX"));
        REQUIRE(Contains(test_result.var_proc_names, "cenY"));
      }
    }

    WHEN("Uses(INT, _) evaluates to false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(20),
                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select w such that Uses(20, _) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Uses(DE, _) evaluates true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a such that Uses(a, _) returns {15, 16, 17, 21, 22, 23}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 6);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 15));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 17));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 22));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 23));
      }
    }
  }
}

SCENARIO("Test Modifies queries", "[query][modifies]") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("Modifies(INT, NAME) evaluates to true, variable is modified by assignment statement") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(11),
                                                      ClauseParam("cenX"));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select pn such that Modifies(11, \"cenX\") returns all print statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 6));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Modifies (INT, NAME) evaluates to true, variable is modified by call statement") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(2),
                                                      ClauseParam("normSq"));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select w such that Modifies(2, \"normSq\") returns all assignment statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
      }
    }

    WHEN("Modifies (INT, DE) evaluates to true, index is a container statement") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(19),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select v such that Modifies(19, v) returns {flag, cenX, cenY)") {
        REQUIRE(test_result.var_proc_names.size() == 3);
        REQUIRE(Contains(test_result.var_proc_names, "flag"));
        REQUIRE(Contains(test_result.var_proc_names, "cenX"));
        REQUIRE(Contains(test_result.var_proc_names, "cenY"));
      }
    }

    WHEN("Modifies (INT, DE) evaluates to false, index is a call statement that uses but not modify variables") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(3),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select s such that Modifies(3, v) returns empty result") {
        REQUIRE(test_result.var_proc_names.empty());
      }
    }

    WHEN("Modifies (DE, NAME) evaluates to true, variable modified in nested container DE") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                      ClauseParam("count"));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select w such that Modifies(w, \"count\") returns 14") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
      }
    }

    WHEN("Modifies (DE, NAME) evaluates to false, modified variable only modified by read and not assign") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                      ClauseParam("x"));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);

      THEN("Select a such that Modifies(a, \"x\") returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Modifies (INT, _) evaluates to false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(9),
                                                      ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select s such that Modifies(9, _) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Modifies (DE, _) evaluates to true") {
      THEN("Select p such that Modifies(p, _) returns {main, redPoint, computeCentroid}") {
        //TODO: To add once InsertModifiesP is in the PKB.
      }
    }
  }
}

SCENARIO("Test Call/* queries") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("Calls(NAME, NAME) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::READ, "r")));
      SuchThatClause call_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                  ClauseParam("main"),
                                                  ClauseParam("computeCentroid"));
      valid_query.AddClause(Clause(call_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select r such that Calls(main, computeCentroid) returns all read statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("Calls(NAME, NAME) evaluates to false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause call_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                  ClauseParam("main"),
                                                  ClauseParam("readPoint"));
      valid_query.AddClause(Clause(call_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select s such that Calls(main, readPoint) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Calls(NAME, DE) evaluates to false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      SuchThatClause call_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                  ClauseParam("readPoint"),
                                                  ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      valid_query.AddClause(Clause(call_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select p such that Calls(readPoint, p) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Calls(DE, DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      SuchThatClause call_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                  ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                  ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")));
      valid_query.AddClause(Clause(call_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select p such that Calls(p, p1) returns {main, computeCentroid}") {
        REQUIRE(test_result.var_proc_names.size() == 2);
        REQUIRE(Contains(test_result.var_proc_names, "main"));
        REQUIRE(Contains(test_result.var_proc_names, "computeCentroid"));
      }
    }

    WHEN("Calls(_,DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      SuchThatClause call_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                  ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      valid_query.AddClause(Clause(call_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select p such that Calls(_, p) returns {computeCentroid, printResults, readPoint}") {
        REQUIRE(test_result.var_proc_names.size() == 3);
        REQUIRE(Contains(test_result.var_proc_names, "printResults"));
        REQUIRE(Contains(test_result.var_proc_names, "readPoint"));
        REQUIRE(Contains(test_result.var_proc_names, "computeCentroid"));
      }
    }

    WHEN("Calls*(NAME, NAME) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::READ, "r")));
      SuchThatClause call_clause = SuchThatClause(DesignAbstraction::CALLS_T,
                                                  ClauseParam("main"),
                                                  ClauseParam("readPoint"));
      valid_query.AddClause(Clause(call_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);

      THEN("Select r such that Calls(main, readPoint) returns all read statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }
  }
}

SCENARIO("Test Next/*  and NextBip/* queries") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("Next(INT, INT) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::CALL, "cl")));
      SuchThatClause next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                  ClauseParam(14),
                                                  ClauseParam(15));
      valid_query.AddClause(next_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select cl such that Next(14, 15) returns all call statements") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 13));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 18));
      }
    }

    WHEN("Next(INT, DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      SuchThatClause next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                  ClauseParam(19),
                                                  ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      WithClause with_clause = WithClause(make_pair(DesignEntity(DesignEntityType::PROG_LINE, "n"), AttributeType::NONE),
                                          make_pair(DesignEntity(DesignEntityType::ASSIGN, "a"), AttributeType::STMT_NO));
      valid_query.AddClause(next_clause);
      valid_query.AddClause(uses_clause);
      valid_query.AddClause(with_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select n such that Next(19, n) and Uses(a, v) with n = a.stmt# returns 21 only") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
      }
    }

    WHEN("Next(DE, DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n1")));
      SuchThatClause next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                  ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")),
                                                  ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n1")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));

      valid_query.AddClause(next_clause);
      valid_query.AddClause(parent_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select n1 such that Next(n, n1) such that Parent(w, n) returns {16, 17, 18, 14} ") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 17));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 18));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
      }
    }

    WHEN("Next(DE, DE) evaluates to false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      SuchThatClause next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                  ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")),
                                                  ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));

      valid_query.AddClause(next_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select n such that Next(n, n) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Next*(INT, INT) evaluates to false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                  ClauseParam(19),
                                                  ClauseParam(19));
      valid_query.AddClause(next_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a such that Next*(19, 19) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Next*(DE, DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause next_clause = SuchThatClause(DesignAbstraction::NEXT_T,
                                                  ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                  ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));

      valid_query.AddClause(next_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select w such that Next*(w, w) returns {14}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
      }
    }

    WHEN("Next*(DE, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause next_clause = SuchThatClause(DesignAbstraction::NEXT_T,
                                                  ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")),
                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));

      valid_query.AddClause(next_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select pn such that Next*(pn, _) returns {6, 7, 8}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 6));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
      }
    }

    WHEN("NextBip(INT, INT) evaluates to false") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause nextbip_clause = SuchThatClause(DesignAbstraction::NEXTBIP,
                                                     ClauseParam(5),
                                                     ClauseParam(19));
      valid_query.AddClause(nextbip_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that NextBip(5, 19) returns FALSE") {
        REQUIRE_FALSE(test_result.boolean);
      }
    }

    WHEN("NextBip(_, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause nextbip_clause = SuchThatClause(DesignAbstraction::NEXTBIP,
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(nextbip_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that NextBip(_,  _) returns TRUE") {
        REQUIRE(test_result.boolean);
      }
    }

    WHEN("NextBip*(INT, INT) evaluates to true") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause nextbip_clause = SuchThatClause(DesignAbstraction::NEXTBIP_T,
                                                     ClauseParam(5),
                                                     ClauseParam(19));
      valid_query.AddClause(nextbip_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that NextBip(5, 19) returns TRUE") {
        REQUIRE(test_result.boolean);
      }
    }

    WHEN("NextBip*(INT, _) evaluates to false") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause nextbip_clause = SuchThatClause(DesignAbstraction::NEXTBIP_T,
                                                     ClauseParam(9),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(nextbip_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that NextBip(9, _) returns FALSE") {
        REQUIRE_FALSE(test_result.boolean);
      }
    }

    WHEN("NextBip*(DE, DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::READ, "r")));
      SuchThatClause nextbip_clause = SuchThatClause(DesignAbstraction::NEXTBIP_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::READ, "r")),
                                                     ClauseParam(DesignEntity(DesignEntityType::READ, "r")));
      valid_query.AddClause(nextbip_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select r such that NextBip*(r, r) returns {4, 5}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }
  }
}

SCENARIO("Test Affects/* queries") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("Affects(INT, INT) evaluates to true") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                     ClauseParam(15),
                                                     ClauseParam(15));
      valid_query.AddClause(affects_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that Affects(15, 15) returns TRUE") {
        REQUIRE(test_result.boolean == true);
      }
    }

    WHEN("Affects(INT, DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                     ClauseParam(11),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      valid_query.AddClause(affects_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a such that Affects(11, a) returns assignment statements affected by 11") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 23));
      }
    }

    WHEN("Affects(DE, DE) evaluates to true") {
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");

      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(a), SelectedEntity(a)});
      SuchThatClause affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      valid_query.AddClause(affects_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select <a, a> such that Affects(a, a) returns assignment statements that affect themselves") {
        REQUIRE(test_result.tuple_result.size() == 3);
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"15", "15"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"16", "16"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"17", "17"}));
      }
    }

    WHEN("Affects(DE, DE) evaluates to false") {
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");

      Query valid_query = Query(SelectedEntity(a));
      SuchThatClause affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")));
      valid_query.AddClause(affects_clause);
      valid_query.AddClause(follows_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a such that Affects(a, a) and Follows*(a, ifs) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Affects*(DE, INT) evaluates to false") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(1));
      valid_query.AddClause(affects_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that Affects(a, 1) returns false") {
        REQUIRE(test_result.boolean == false);
      }
    }

    WHEN("Affects*(_, a) evaluates to true") {
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");
      Query valid_query = Query(SelectedEntity(a));
      SuchThatClause affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));

      valid_query.AddClause(affects_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a such that Affects(_, a) returns all assignment statements that are affected") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 6);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 15));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 17));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 22));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 23));
      }
    }
  }
}

SCENARIO("Test AffectsBip/* queries") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("AffectsBip(INT, INT) evaluates to true") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause affectsbip_clause = SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                                        ClauseParam(17),
                                                        ClauseParam(17));
      valid_query.AddClause(affectsbip_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that AffectsBip(17, 17) returns TRUE") {
        REQUIRE(test_result.boolean == true);
      }
    }

    WHEN("AffectsBip(INT, DE) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause affectsbip_clause = SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                                        ClauseParam(22),
                                                        ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      valid_query.AddClause(affectsbip_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a such that Affects(22, a) returns assignment statements affected by 22") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 23));
      }
    }

    WHEN("AffectsBip(DE, DE) evaluates to true") {
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");

      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(a), SelectedEntity(a)});
      SuchThatClause affectsbip_clause = SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                                        ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                        ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      valid_query.AddClause(affectsbip_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select <a, a> such that AffectsBip(a, a) returns assignment statements that affect themselves") {
        REQUIRE(test_result.tuple_result.size() == 3);
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"15", "15"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"16", "16"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"17", "17"}));
      }
    }

    WHEN("AffectsBip*(DE, INT) evaluates to false") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause affectsbip_t_clause = SuchThatClause(DesignAbstraction::AFFECTSBIP_T,
                                                          ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                          ClauseParam(13));
      valid_query.AddClause(affectsbip_t_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that AffectsBip*(a, 13) returns false") {
        REQUIRE(test_result.boolean == false);
      }
    }

    WHEN("AffectsBip*(_, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause affects_clause = SuchThatClause(DesignAbstraction::AFFECTSBIP_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));

      valid_query.AddClause(affects_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select BOOLEAN such that AffectsBip*(_, _) returns true") {
        REQUIRE(test_result.boolean);
      }
    }
  }
}

SCENARIO("Test pattern queries") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("pattern a(_, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select w pattern a(_, _) returns {14}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
      }
    }
    WHEN("pattern a(_, complete match) evaluates to true") {
      Token zero("0", TokenType::ConstantValue);
      TokenList expression;
      expression.Push(zero);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                   ClauseParam(PatternExpression(expression)));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a pattern a(_, \"0\") returns {1, 10, 11, 12}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 1));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 10));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 11));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 12));
      }
    }

    WHEN("pattern a(_, complete match) evaluates to false") {
      Token cenX("cenX", TokenType::VariableName);
      TokenList expression;
      expression.Push(cenX);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                   ClauseParam(PatternExpression(expression)));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a pattern a(_, \"cenX\") returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("pattern a(_, partial match) evaluates to true") {
      Token count("count", TokenType::VariableName);
      TokenList expression;
      expression.Push(count);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                   ClauseParam(PatternExpression(expression, true)));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a pattern a(_, _\"count\"_) returns {15, 21, 22}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 15));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 22));
      }
    }

    WHEN("pattern a(_, partial match) evaluates to false") {
      Token normSq("normSq", TokenType::VariableName);
      TokenList expression;
      expression.Push(normSq);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                   ClauseParam(PatternExpression(expression, true)));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a pattern a(_, _\"normSq\") returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("pattern a(NAME, _) evaluates to true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam("cenX"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a pattern a(cenX, _) returns {11, 16, 21}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 11));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
      }
    }

    WHEN("pattern a(NAME, complete match) evaluates to true") {
      Token count("count", TokenType::VariableName);
      Token plus("+", TokenType::ExpressionOp);
      Token one("1", TokenType::ConstantValue);
      TokenList expression;
      expression.Push(count).Push(plus).Push(one);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam("count"),
                                                   ClauseParam(PatternExpression(expression)));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a pattern a(\"count\", \"count + 1\") returns {15}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 15));
      }
    }

    WHEN("pattern a(NAME, partial match) evaluates to false") {
      Token y("y", TokenType::VariableName);
      TokenList expression;
      expression.Push(y);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam("cenX"),
                                                   ClauseParam(PatternExpression(expression, true)));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a pattern a(\"cenX\", _\"y\"_) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("pattern w(NAME, _) evaluates to false") {
      Query valid_query = Query(DesignEntity(DesignEntityType::WHILE, "w"));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::WHILE, "w"),
                                                   ClauseParam("cenX"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select w pattern w(\"cenX\", _) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }
  }
}

SCENARIO("Test multiple SuchThatClauses") {
  GIVEN("Two assignment statements that sandwich a single while statement.") {
    PKB pkb_intersection = PKB();
    source_processor::TokenList tokens;
    source_processor::Token t1(std::string("t1"), source_processor::TokenType::VariableName);
    tokens.Push(t1);
    pkb_intersection.InsertStatement(1);
    pkb_intersection.InsertStatement(2);
    pkb_intersection.InsertStatement(3);
    pkb_intersection.InsertAssignment(1, {"test"}, tokens);
    pkb_intersection.InsertWhile(2, {"test"});
    pkb_intersection.InsertAssignment(3, {"test"}, tokens);
    pkb_intersection.InsertFollows(1, 2);
    pkb_intersection.InsertFollowsT(1, 2);
    pkb_intersection.InsertFollows(2, 3);
    pkb_intersection.InsertFollowsT(2, 3);
    pkb_intersection.InsertFollowsT(1, 3);

    WHEN("Query consists of two follow clauses that both evaluate to true individually but intersection is empty") {
      DesignEntity assign_entity = DesignEntity(DesignEntityType::ASSIGN, "a");
      DesignEntity while_entity = DesignEntity(DesignEntityType::WHILE, "w");
      DesignEntity statement_entity = DesignEntity(DesignEntityType::STMT, "s");
      SuchThatClause first_clause = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(assign_entity),
                                                   ClauseParam(while_entity));
      SuchThatClause second_clause = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(while_entity),
                                                    ClauseParam(assign_entity));

      THEN("QueryResult should return all statements when only first clause") {
        Query valid_query = Query(statement_entity);
        valid_query.AddClause(Clause(first_clause));
        QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_intersection, false);
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 1));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
      }

      THEN("QueryResult should return all statements when only second clause") {
        Query valid_query = Query(statement_entity);
        valid_query.AddClause(Clause(second_clause));
        QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_intersection, false);
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 1));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
      }

      THEN("QueryResult should return none when both clauses") {
        Query valid_query = Query(statement_entity);
        valid_query.AddClause(Clause(first_clause));
        valid_query.AddClause(Clause(second_clause));
        QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_intersection, false);
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }
  }

  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("Clauses evaluate overlapping synonyms return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause calls_clause_1 = SuchThatClause(DesignAbstraction::CALLS,
                                                     ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      SuchThatClause calls_clause_2 = SuchThatClause(DesignAbstraction::CALLS,
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                     ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));

      valid_query.AddClause(Clause(calls_clause_1));
      valid_query.AddClause(Clause(calls_clause_2));
      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);

      THEN("Select v such that Calls(p, _) and Calls(_, p) and Uses(p, v) and Modifies(ifs, v) returns {cenX, cenY}") {
        REQUIRE(test_result.var_proc_names.size() == 2);
        REQUIRE(Contains(test_result.var_proc_names, "cenX"));
        REQUIRE(Contains(test_result.var_proc_names, "cenY"));
      }
    }

    WHEN("Clauses evaluate true independently but false together") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));

      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause followst_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                      ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                      ClauseParam(DesignEntity(DesignEntityType::CALL, "c")));
      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(followst_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);

      THEN("Select a such that Uses(a, v) Modifies(a, v) Parent(ifs, a) Follows*(a, c) returns empty result") {
        REQUIRE(test_result.statement_indexes_or_constants.empty());
      }
    }
  }
}

SCENARIO("Test multiple pattern clauses") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("Distinct pattern clauses evaluate to true") {
      Token count("count", TokenType::VariableName);
      Token plus("+", TokenType::ExpressionOp);
      Token one("1", TokenType::ConstantValue);
      TokenList expr1;
      Token cenX("cenX", TokenType::VariableName);
      Token mult("*", TokenType::ExpressionOp);
      TokenList expr2;
      expr1.Push(count).Push(plus).Push(one);
      expr2.Push(cenX).Push(mult).Push(cenX);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause_1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(PatternExpression(expr1)));
      PatternClause pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a1"),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                     ClauseParam(PatternExpression(expr2, true)));
      valid_query.AddClause(Clause(pattern_clause_1));
      valid_query.AddClause(Clause(pattern_clause_2));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select a pattern a(v, \"count + 1\") pattern a1(_, _\"cenX * cenX\"_) returns {15}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 15));
      }
    }

    WHEN("Overlapping pattern clauses return false") {
      Token x("x", TokenType::VariableName);
      Token cenX("cenX", TokenType::VariableName);
      TokenList expr1;
      expr1.Push(x);
      TokenList expr2;
      expr2.Push(cenX);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v1")));
      PatternClause pattern_clause_1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(PatternExpression(expr1, true)));
      PatternClause pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
                                                     ClauseParam(PatternExpression(expr2, true)));
      valid_query.AddClause(Clause(pattern_clause_1));
      valid_query.AddClause(Clause(pattern_clause_2));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select v1 pattern a(v, _\"x\"_) pattern a(v1, _\"cenX\"_) returns {cenX}") {
        REQUIRE(test_result.var_proc_names.size() == 1);
        REQUIRE(Contains(test_result.var_proc_names, "cenX"));
      }
    }

    WHEN("Pattern a and Pattern ifs evaluates overlapping variables return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      PatternClause pattern_clause_1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      PatternClause pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::IF, "ifs"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));

      valid_query.AddClause(Clause(pattern_clause_1));
      valid_query.AddClause(Clause(pattern_clause_2));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select v pattern a(v,_) pattern ifs(v,_) returns {count}") {
        REQUIRE(test_result.var_proc_names.size() == 1);
        REQUIRE(Contains(test_result.var_proc_names, "count"));
      }
    }

    WHEN("Pattern a and pattern w evaluates overlapping variables return false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      PatternClause pattern_clause_1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      PatternClause pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::WHILE, "w"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));

      valid_query.AddClause(Clause(pattern_clause_1));
      valid_query.AddClause(Clause(pattern_clause_2));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select v pattern a(v, _) pattern w(, _) returns empty result") {
        REQUIRE(test_result.var_proc_names.empty());
      }
    }
  }
}

SCENARIO("Test multiple such that clauses and pattern clauses") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();

    WHEN("Clauses with identical synonyms") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      Token cenX("cenX", TokenType::VariableName);
      TokenList expr1;
      expr1.Push(cenX);
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                   ClauseParam(PatternExpression(expr1, true)));
      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);

      THEN("Select a such that Uses(a, v) and Modifies(a, v) and pattern a(v, _\"cenX\"_) returns {16, 21}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
      }
    }

    WHEN("Numerous clauses") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s4")));
      SuchThatClause follows_clause_1 = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                       ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                       ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")));
      SuchThatClause follows_clause_2 = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                       ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                                       ClauseParam(DesignEntity(DesignEntityType::STMT, "s3")));
      SuchThatClause follows_clause_3 = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                       ClauseParam(DesignEntity(DesignEntityType::STMT, "s3")),
                                                       ClauseParam(DesignEntity(DesignEntityType::STMT, "s4")));
      SuchThatClause modifies_clause_1 = SuchThatClause(DesignAbstraction::MODIFIES,
                                                        ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause_2 = SuchThatClause(DesignAbstraction::MODIFIES,
                                                        ClauseParam(DesignEntity(DesignEntityType::STMT, "s4")),
                                                        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(follows_clause_1));
      valid_query.AddClause(Clause(follows_clause_2));
      valid_query.AddClause(Clause(follows_clause_3));
      valid_query.AddClause(Clause(modifies_clause_1));
      valid_query.AddClause(Clause(modifies_clause_2));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select s4 such that Follows(s1, s2) and Follows(s2, s3) and Follows(s3, s4) and Modifies(s1, v) and Modifies(s4, v) pattern a(v,_) returns {14, 19}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 19));
      }
    }

    WHEN("Multiple such that clauses with different pattern clauses") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v2")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")));
      PatternClause pattern_w_clause = PatternClause(DesignEntity(DesignEntityType::WHILE, "w"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      PatternClause pattern_a_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      PatternClause pattern_if_clause = PatternClause(DesignEntity(DesignEntityType::IF, "ifs"),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v2")),
                                                      ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(pattern_w_clause));
      valid_query.AddClause(Clause(pattern_a_clause));
      valid_query.AddClause(Clause(pattern_if_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select v2 such that Follows*(a, a1) and Uses(a, v) and Modifies(ifs, v1) pattern w(v, _) pattern a(v1, _) pattern ifs(v2, _, _) returns {count}") {
        REQUIRE(test_result.var_proc_names.size() == 1);
        REQUIRE(Contains(test_result.var_proc_names, "count"));
      }
    }
  }
}

SCENARIO("Test selection of tuples with multiple clauses") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();

    WHEN("Select tuple where synonyms are from the same clause") {
      DesignEntity w = DesignEntity(DesignEntityType::WHILE, "w");
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");

      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(w), SelectedEntity(a)});
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(w),
                                                    ClauseParam(a));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select <w, a> such that Parent(w, a) returns {<14, 15>, <14, 16>, <14, 17>") {
        REQUIRE(test_result.tuple_result.size() == 3);
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"14", "15"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"14", "16"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"14", "17"}));
      }
    }

    WHEN("Select tuple where synonyms are from different clauses") {
      DesignEntity ifs = DesignEntity(DesignEntityType::IF, "ifs");
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");
      DesignEntity v = DesignEntity(DesignEntityType::VARIABLE, "v");

      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(ifs), SelectedEntity(v)});
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(ifs),
                                                    ClauseParam(a));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(a),
                                                      ClauseParam(v));
      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select <ifs, v> such that Parent(ifs, a) and Modifies(a, v) returns {<19, flag>, <19, cenX>, <19, cenY>}") {
        REQUIRE(test_result.tuple_result.size() == 3);
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"19", "flag"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"19", "cenX"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"19", "cenY"}));
      }
    }

    WHEN("Select tuple where one synonym has no results") {
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");
      DesignEntity s = DesignEntity(DesignEntityType::STMT, "s");

      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(s), SelectedEntity(a)});
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(s),
                                                     ClauseParam(s));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(a),
                                                      ClauseParam("count"));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select <s, a> such that Follows(s, s) and Modifies(a, \"count\") return empty result") {
        REQUIRE(test_result.tuple_result.empty());
      }
    }

    WHEN("Select tuple where one synonym is not evaluated") {
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");
      DesignEntity r = DesignEntity(DesignEntityType::READ, "r");
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(r), SelectedEntity(a)});
      SuchThatClause modifies_clause_1 = SuchThatClause(DesignAbstraction::MODIFIES,
                                                        ClauseParam(1),
                                                        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause_2 = SuchThatClause(DesignAbstraction::MODIFIES,
                                                        ClauseParam(a),
                                                        ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(modifies_clause_1));
      valid_query.AddClause(Clause(modifies_clause_2));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);

      THEN("Select <r, a> such that Modifies(1, v) and Modifies(a, v) returns {<4, 1>, <4, 20>, <5, 1>, <5, 20>}") {
        REQUIRE(test_result.tuple_result.size() == 4);
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"4", "1"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"4", "20"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"5", "1"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"5", "20"}));
      }
    }
  }

  GIVEN("Nested source code") {
    PKB nested_pkb = BuildNestedProgram();
    WHEN("Select tuple where design entity is not present in the code") {
      DesignEntity pn = DesignEntity(DesignEntityType::PRINT, "pn");
      DesignEntity r = DesignEntity(DesignEntityType::READ, "r");
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(r), SelectedEntity(pn)});
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                    ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &nested_pkb, false);
      THEN("Select <r, pn> such that Parent(s, pn) returns empty result") {
        REQUIRE(test_result.tuple_result.empty());
      }
    }
  }
}

SCENARIO("Test use of program line") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();
    WHEN("Program line used in such that clauses and selected") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")),
                                                    ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      valid_query.AddClause(follows_clause);
      valid_query.AddClause(parent_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select n such that Follows(a, n) and Parent(ifs, n) returns {22}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 22));
      }
    }

    WHEN("Program line used in with clauses") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")),
                                                      ClauseParam("cenX"));
      WithClause with_clause = WithClause(make_pair(DesignEntity(DesignEntityType::PROG_LINE, "n"), AttributeType::NONE),
                                          make_pair(DesignEntity(DesignEntityType::ASSIGN, "a"), AttributeType::STMT_NO));
      valid_query.AddClause(modifies_clause);
      valid_query.AddClause(with_clause);
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Select n such that Modifies(n, \"cenX\") with n = a.stmt# returns {11, 16, 21}") {
        REQUIRE(test_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 11));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
      }
    }
  }
}
SCENARIO("Test select queries without clauses", "[query]") {
  GIVEN("PKB built from Sample Program 4 given in SPA requirements") {
    PKB pkb_code_4 = BuildPKBSampleProgram();

    WHEN("Select statements") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Return all 23 statements in Sample Code 4") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 23);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 1));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 13));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 23));
      }
    }

    WHEN("Select prog line") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Return all 23 statements in Sample Code 4") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 23);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 22));
      }
    }
    WHEN("Select variables") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("7 variables from Sample Code 4") {
        REQUIRE(test_result.var_proc_names.size() == 7);
        REQUIRE(test_result.statement_indexes_or_constants.empty());
        REQUIRE(Contains(test_result.var_proc_names, "flag"));
        REQUIRE(Contains(test_result.var_proc_names, "x"));
        REQUIRE(Contains(test_result.var_proc_names, "y"));
        REQUIRE(Contains(test_result.var_proc_names, "cenX"));
        REQUIRE(Contains(test_result.var_proc_names, "cenY"));
        REQUIRE(Contains(test_result.var_proc_names, "normSq"));
        REQUIRE(Contains(test_result.var_proc_names, "count"));
      }
    }

    WHEN("Select constants") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "c")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns two constants 0 and 1") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 0));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 1));
      }
    }

    WHEN("Select while") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns while statement on line 14") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 14));
      }
    }

    WHEN("Select if") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::IF, "ifs")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns if statement on line 19") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 19));
      }
    }

    WHEN("Select print") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns 4 print statements") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 6));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Select read") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::READ, "r")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns 2 read statements") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("Select call") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::CALL, "cl")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns 4 call statements") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 2));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 13));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 18));
      }
    }

    WHEN("Select procedure") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("4 procedures defined in Sample Code 4") {
        REQUIRE(test_result.var_proc_names.size() == 4);
        REQUIRE(test_result.statement_indexes_or_constants.empty());
        REQUIRE(Contains(test_result.var_proc_names, "main"));
        REQUIRE(Contains(test_result.var_proc_names, "computeCentroid"));
        REQUIRE(Contains(test_result.var_proc_names, "readPoint"));
        REQUIRE(Contains(test_result.var_proc_names, "printResults"));
      }
    }

    WHEN("Select assign") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns all assignment statements") {
        REQUIRE(test_result.var_proc_names.empty());
        REQUIRE(test_result.statement_indexes_or_constants.size() == 11);
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 1));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 10));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 11));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 12));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 15));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 16));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 17));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 20));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 21));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 22));
        REQUIRE(Contains(test_result.statement_indexes_or_constants, 23));
      }
    }

    WHEN("Select tuple <r, w>") {
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(DesignEntity(DesignEntityType::READ, "r")),
                                                       SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w"))});
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns all cross product of read and while") {
        REQUIRE(test_result.tuple_result.size() == 2);
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"4", "14"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"5", "14"}));
      }
    }

    WHEN("Select tuple <p1, p2>") {
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(DesignEntity(DesignEntityType::PRINT, "p1")),
                                                       SelectedEntity(DesignEntity(DesignEntityType::PRINT, "p2"))});
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns all cross product of print statements") {
        REQUIRE(test_result.tuple_result.size() == 16);
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"6", "7"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"7", "8"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"8", "6"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"7", "9"}));
      }
    }

    WHEN("Select tuple <c, c>") {
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(DesignEntity(DesignEntityType::CALL, "c")),
                                                       SelectedEntity(DesignEntity(DesignEntityType::CALL, "c"))});
      QueryResult test_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb_code_4, false);
      THEN("Returns same pairs of calls") {
        REQUIRE(test_result.tuple_result.size() == 4);
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"2", "2"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"3", "3"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"13", "13"}));
        REQUIRE(Contains(test_result.tuple_result, vector<string>{"18", "18"}));
      }
    }
  }
}
