#include "PKBStub.h"
#include "TestUtils.h"
#include "catch.hpp"
#include "query_processor/commons/query/Query.h"
#include "query_processor/commons/query/clause/SuchThatClause.h"
#include "query_processor/commons/query/entities/DesignEntity.h"
#include "query_processor/commons/query_result/QueryResult.h"
#include "query_processor/query_evaluator/QueryEvaluator.h"
#include "source_processor/token/TokenList.h"

using namespace std;
using namespace query_processor;
using namespace source_processor;

SCENARIO("Test EvaluateQuery Select BOOLEAN") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    WHEN("Query evaluates to true") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause true_follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                          ClauseParam(1),
                                                          ClauseParam(7));
      PatternClause true_pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                        ClauseParam(
                                                            DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                        ClauseParam(
                                                            DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(true_follows_clause));
      valid_query.AddClause(Clause(true_pattern_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select BOOLEAN such that Follows*(1, 7) pattern a(_,_) return true QueryResult") {
        REQUIRE(final_result.result_type == QueryResultType::BOOLEAN);
        REQUIRE(final_result.boolean);
      }
    }

    WHEN("Query evaluates to false") {
      Query valid_query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
      SuchThatClause true_parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                         ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                         ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause false_follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                           ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                           ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));

      valid_query.AddClause(Clause(true_parent_clause));
      valid_query.AddClause(Clause(false_follows_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select BOOLEAN such that Parent(w, s) and Follows(s, pn) return false QueryResult") {
        REQUIRE(final_result.result_type == QueryResultType::BOOLEAN);
        REQUIRE_FALSE(final_result.boolean);
      }
    }
  }
}
SCENARIO("Test EvaluateQuery with one Such That and one Pattern clause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    WHEN("Such that clause and pattern clause evaluates to true separately, selects a different synonym") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::READ, "r")));
      SuchThatClause true_follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                          ClauseParam(1),
                                                          ClauseParam(2));
      PatternClause true_pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                        ClauseParam(
                                                            DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                        ClauseParam(
                                                            DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(true_follows_clause));
      valid_query.AddClause(Clause(true_pattern_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);

      THEN("Select r such that Follows(1, 2) pattern a(_,_) returns all read statements") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 1));
      }

      Query valid_query_2 = Query(SelectedEntity(DesignEntity(DesignEntityType::READ, "r")));
      SuchThatClause true_parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                         ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                         ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")));

      Token sum("sum", TokenType::VariableName);
      Token digit("digit", TokenType::VariableName);
      Token plus("+", TokenType::ExpressionOp);
      TokenList rhs_expr;
      rhs_expr.Push(sum).Push(plus).Push(digit);

      PatternClause true_pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a2"),
                                                          ClauseParam(
                                                              DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                          ClauseParam(
                                                              PatternExpression(rhs_expr)));
      valid_query.AddClause(Clause(true_parent_clause));
      valid_query.AddClause(Clause(true_pattern_clause_2));
      QueryResult final_result_2 = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select r such that Parent(w, a1) pattern a2(_, \"sum + digit\") returns all read statements") {
        REQUIRE(final_result_2.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result_2.statement_indexes_or_constants, 1));
      }
    }

    WHEN("One of such that clause and pattern clause evaluates to false independently, selection returns none") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause false_follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                           ClauseParam(1),
                                                           ClauseParam(3));
      PatternClause true_pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                        ClauseParam(
                                                            DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                        ClauseParam(
                                                            DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(false_follows_clause));
      valid_query.AddClause(Clause(true_pattern_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a such that Follows(1, 3) pattern a(_,_) returns empty result") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Such that clause and pattern clause evaluates the same synonym that is selected") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause true_parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                         ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                         ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause true_pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                        ClauseParam(
                                                            "sum"),
                                                        ClauseParam(
                                                            DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(true_parent_clause));
      valid_query.AddClause(Clause(true_pattern_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a such that Parent(w, a) pattern a(\"sum\", _) returns only matching assignment statement in while loop") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("Such that clause and pattern clause evaluates to true independently but intersected synonym set is empty") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a2")));
      SuchThatClause true_follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                          ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")),
                                                          ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a2")));

      Token number("number", TokenType::VariableName);
      Token div("/", TokenType::ExpressionOp);
      Token ten("10", TokenType::ConstantValue);
      TokenList rhs_expr;
      rhs_expr.Push(number).Push(div).Push(ten);

      PatternClause true_pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a1"),
                                                        ClauseParam(
                                                            DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                        ClauseParam(
                                                            PatternExpression(rhs_expr)));
      valid_query.AddClause(Clause(true_follows_clause));
      valid_query.AddClause(Clause(true_pattern_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a2 such that Follows(a1, a2) pattern a1(_, \"number/10\") returns empty result") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }
    WHEN("Such that clause and pattern clause have similar pairs of synonyms") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(
                                                       DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                   ClauseParam(
                                                       DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select v such that Uses(a, v) pattern a(v, _) returns {sum, number, x}") {
        REQUIRE(final_result.var_proc_names.size() == 3);
        REQUIRE(Contains(final_result.var_proc_names, "sum"));
        REQUIRE(Contains(final_result.var_proc_names, "number"));
        REQUIRE(Contains(final_result.var_proc_names, "x"));
      }
    }
    WHEN("Such that clause and pattern clause have overlapping synonyms") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(
                                                       DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                   ClauseParam(
                                                       DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(pattern_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select v such that Uses(s, v) pattern a(v, _) returns {sum, number, digit, x}") {
        REQUIRE(final_result.var_proc_names.size() == 4);
        REQUIRE(Contains(final_result.var_proc_names, "sum"));
        REQUIRE(Contains(final_result.var_proc_names, "number"));
        REQUIRE(Contains(final_result.var_proc_names, "digit"));
        REQUIRE(Contains(final_result.var_proc_names, "x"));
      }
    }
  }
}

SCENARIO("Test EvaluateQuery with multiple SuchThatClauses") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    WHEN("Two SuchThatClauses evaluate independently and selected design entity is independent returns true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                  ClauseParam("number"));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select v such that Follows(a, _) and Uses(w, \"number\") returns all variables") {
        REQUIRE(final_result.var_proc_names.size() == 5);
        REQUIRE(Contains(final_result.var_proc_names, "sum"));
        REQUIRE(Contains(final_result.var_proc_names, "number"));
        REQUIRE(Contains(final_result.var_proc_names, "digit"));
        REQUIRE(Contains(final_result.var_proc_names, "x"));
        REQUIRE(Contains(final_result.var_proc_names, "y"));
      }
    }

    WHEN("Two SuchThatClauses evaluate independently and selected design entity is independent returns false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(3),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(2),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select pn such that Parent(3, a) and Uses(2, v) returns empty result") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Two SuchThatClause evaluates dependently and selected design entity is independent returns true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                   ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                  ClauseParam("x"));
      valid_query.AddClause(Clause(calls_clause));
      valid_query.AddClause(Clause(uses_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select pn such that Calls(p, _) and Uses(p, \"x\") returns all print statements") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 10));
      }
    }

    WHEN("Two SuchThatClauses evaluate identical pair of synonyms and the selected design entity is dependent return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(modifies_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a such that Uses(a, v) and Modifies (a, v) returns {5, 6, 11}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 5));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 6));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 11));
      }
    }

    WHEN("Two SuchThatClauses evaluate identical pair of synonyms and the selected design entity is dependent return false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a such that Follows*(a, w) and Parent*(w, a) returns empty result") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Two SuchThatClauses evaluate one overlapping synonym and the selected design entity is dependent return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                    ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));
      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select s such that Modifies(s, v) and Parent*(_, s) returns {4, 5, 6, 11, 12}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 5);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 5));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 6));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 11));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 12));
      }
    }

    WHEN("Two SuchThatClauses evaluate one overlapping synonym and previously evaluated synonym is modified return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause uses_clauses = SuchThatClause(DesignAbstraction::USES,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(1),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));

      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(uses_clauses));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select s such that Uses(s, v) and Modifies(1, v) returns {3, 4, 6, 12}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 4);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 6));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 12));
      }
    }

    WHEN("Two SuchThatClauses evaluate one overlapping synonym and the selected design entity is dependent return false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                     ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));

      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(follows_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select pn such that Parent(w, s) and Follows*(s, pn) returns none") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Three or more SuchThatClauses evaluate independently and selected design entity is dependent") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(3),
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(3),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                  ClauseParam("number"));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(uses_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select w such that Follows*(a, pn) and Parent(3, _) and Modifies(3, v) and Uses(s, \"number\") returns all while statements") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Three or more SuchThatClauses evaluate overlapping pairs of synonyms and the selected design entity is dependent return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(uses_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select s such that Follows*(s, a) and Parent*(_, a) and Uses (s, v) and Modifies(s, v) returns {5}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 5));
      }
    }

    WHEN("Three or more SuchThatClauses evaluate overlapping pairs of synonyms and the selected design entity is dependent return true return false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a1")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(uses_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a1 such that Follows*(a1, w) and Uses(a1, v) and Modifies(w, v) returns none") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }
  }
}

SCENARIO("Test EvaluateQuery with multiple PatternClauses") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Token number("number", TokenType::VariableName);
    Token sum("sum", TokenType::VariableName);
    Token digit("digit", TokenType::VariableName);
    WHEN("Two PatternClauses evaluate independently return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      PatternClause pattern_clause_1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      TokenList token_list;
      token_list.Push(number);

      PatternClause pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a1"),
                                                     ClauseParam("digit"),
                                                     ClauseParam(PatternExpression(token_list, true)));
      valid_query.AddClause(Clause(pattern_clause_1));
      valid_query.AddClause(Clause(pattern_clause_2));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select w pattern a(v, _) pattern a1(\"digit\", _\"number\"_) returns all while statements") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 3);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 3));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 8));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Two PatternClauses evaluate independently return false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      PatternClause pattern_clause_1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      TokenList token_list;
      token_list.Push(digit);

      PatternClause pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a1"),
                                                     ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                     ClauseParam(PatternExpression(token_list)));
      valid_query.AddClause(Clause(pattern_clause_1));
      valid_query.AddClause(Clause(pattern_clause_2));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a such that pattern a(_, _) pattern a1(_, \"digit\") returns empty result") {
        REQUIRE(final_result.var_proc_names.empty());
      }
    }

    WHEN("Two PatternClauses evaluate overlapping synonyms return true") {
      TokenList token_list;
      token_list.Push(number);
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                   ClauseParam(PatternExpression(token_list, true)));
      PatternClause while_pattern_clause = PatternClause(DesignEntity(DesignEntityType::WHILE, "w"),
                                                         ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                         ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      valid_query.AddClause(Clause(pattern_clause));
      valid_query.AddClause(Clause(while_pattern_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select w pattern a(v, _\"number\"_) pattern w(v,_) returns {3}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 3));
      }
    }
    WHEN("Two PatternClauses evaluate identical synonym pairs return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));

      TokenList token_list;
      token_list.Push(sum);
      PatternClause pattern_clause_1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(PatternExpression(token_list, true)));
      TokenList token_list2;
      token_list2.Push(digit);

      PatternClause pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(PatternExpression(token_list2, true)));
      valid_query.AddClause(Clause(pattern_clause_1));
      valid_query.AddClause(Clause(pattern_clause_2));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select v such that pattern a(v, _\"sum\"_) pattern a(v, _\"digit\"_) returns {sum}") {
        REQUIRE(final_result.var_proc_names.size() == 1);
        REQUIRE(Contains(final_result.var_proc_names, "sum"));
      }
    }

    WHEN("Two PatternClauses evaluate identical synonym pairs return false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));

      TokenList token_list;
      token_list.Push(number);
      PatternClause pattern_clause_1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(PatternExpression(token_list, true)));
      TokenList token_list2;
      token_list2.Push(digit);

      PatternClause pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                     ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                     ClauseParam(PatternExpression(token_list2, false)));
      valid_query.AddClause(Clause(pattern_clause_1));
      valid_query.AddClause(Clause(pattern_clause_2));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select v such that pattern a(v, _\"number\") pattern a(v, \"digit\") returns empty result") {
        REQUIRE(final_result.var_proc_names.empty());
      }
    }
  }
}

SCENARIO("Test EvaluateQuery with multiple SuchThatClauses and PatternClauses") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Token number("number", TokenType::VariableName);
    Token sum("sum", TokenType::VariableName);
    Token digit("digit", TokenType::VariableName);
    WHEN("Three or more SuchThatClauses and PatternClauses evaluate independently return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(1),
                                                     ClauseParam(2));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam("number"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));

      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(pattern_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select pn such that Follows*(1, 2) such that Parent(w, _) pattern a(\"number\", _) returns all print statements") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 7));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 10));
      }
    }
    WHEN("Three or more SuchThatClauses and PatternClauses evaluate the overlapping synonym pairs return true") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause modifies_clause1 = SuchThatClause(DesignAbstraction::MODIFIES,
                                                       ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                       ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")));
      SuchThatClause modifies_clause2 = SuchThatClause(DesignAbstraction::MODIFIES,
                                                       ClauseParam(DesignEntity(DesignEntityType::READ, "r")),
                                                       ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")));
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));

      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(modifies_clause1));
      valid_query.AddClause(Clause(modifies_clause2));
      valid_query.AddClause(Clause(pattern_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a such that Uses(a, v) and Follows*(a, s) and Modifies(s, v1) and Modifies(r, v1) pattern a(v, _) returns {5}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 5));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 11));
      }
    }
    WHEN("Three or more SuchThatClauses and PatternClauses evaluate overlapping synonym pairs return false") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                     ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")),
                                                     ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));

      TokenList token_list;
      token_list.Push(number);

      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                   ClauseParam(PatternExpression(token_list, true)));

      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(pattern_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select s such that Parent(s, a) and Follows*(pn, s) pattern a(v, _\"number\") returns none") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }
  }
}

SCENARIO("Test With queries") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    WHEN("Multiple such that clauses result shrunk by with clause") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      SuchThatClause uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));

      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")), AttributeType::VAR_NAME),
                                          make_pair(ClauseParam("number"), AttributeType::NAME));

      valid_query.AddClause(Clause(uses_clause));
      valid_query.AddClause(Clause(parent_clause));
      valid_query.AddClause(Clause(with_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a such that Parent(w, a) and Uses(a, v) with v.varName = \"number\" returns {4, 6}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 2);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 4));
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 6));
      }
    }

    WHEN("With clause that involves conversion compared to variable") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")));

      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));

      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")), AttributeType::VAR_NAME),
                                          make_pair(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME));

      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(with_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select a such that Modifies(a, v) with v.varName = r.varName returns {6}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 6));
      }
    }

    WHEN("With clause evaluates to false on reduced space") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")));
      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")), AttributeType::VAR_NAME),
                                          make_pair(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")), AttributeType::VAR_NAME));

      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(with_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select v such that Modifies(a, v) and Follows(a, a1) with pn.varName = v.varName returns empty result") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Column converted by with clause is being selected") {
      Query valid_query = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")));
      SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                      ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                      ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                     ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")), AttributeType::VAR_NAME),
                                          make_pair(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")), AttributeType::VAR_NAME));

      valid_query.AddClause(Clause(modifies_clause));
      valid_query.AddClause(Clause(follows_clause));
      valid_query.AddClause(Clause(with_clause));

      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select pn such that Follows(a, w) and Modifies(w, v) with pn.varName = v.varName returns {7}") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 7));
      }
    }
  }
}

SCENARIO("Test selection of tuples") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    WHEN("Selected tuple both in final table") {
      DesignEntity w1 = DesignEntity(DesignEntityType::WHILE, "w1");
      DesignEntity w2 = DesignEntity(DesignEntityType::WHILE, "w2");
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(w1), SelectedEntity(w2)});
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(w1),
                                                    ClauseParam(w2));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select <w1, w2> such that Parent(w1, w2) returns only <8, 9>") {
        REQUIRE(final_result.tuple_result.size() == 1);
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"8", "9"}));
      }
    }
    WHEN("Multiple tuples, some in final table") {
      DesignEntity a = DesignEntity(DesignEntityType::ASSIGN, "a");
      DesignEntity w1 = DesignEntity(DesignEntityType::WHILE, "w1");
      DesignEntity w2 = DesignEntity(DesignEntityType::WHILE, "w2");
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(a), SelectedEntity(w1), SelectedEntity(w2)});
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(w1),
                                                    ClauseParam(w2));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select <a, w1, w2> such that Parent(w1, w2) returns assignment statements crossed with <8, 9>") {
        REQUIRE(final_result.tuple_result.size() == 5);
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"2", "8", "9"}));
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"4", "8", "9"}));
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"5", "8", "9"}));
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"6", "8", "9"}));
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"11", "8", "9"}));
      }
    }

    WHEN("Tuple contains attributes of similar design entity") {
      DesignEntity pn = DesignEntity(DesignEntityType::PRINT, "pn");
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(pn),
                                                       SelectedEntity(make_pair(pn, AttributeType::STMT_NO)),
                                                       SelectedEntity(make_pair(pn, AttributeType::VAR_NAME))});
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);

      THEN("Select <pn, pn.stmt#, pn.varName> returns <7, 7, sum> and <10, 10, x>") {
        REQUIRE(final_result.tuple_result.size() == 2);
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"7", "7", "sum"}));
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"10", "10", "x"}));
      }
    }

    WHEN("Tuple contains some attributes that have been previously evaluated") {
      DesignEntity pn = DesignEntity(DesignEntityType::PRINT, "pn");
      DesignEntity w = DesignEntity(DesignEntityType::WHILE, "w");
      Query valid_query = Query(vector<SelectedEntity>{SelectedEntity(w),
                                                       SelectedEntity(make_pair(pn, AttributeType::VAR_NAME))});
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT_T,
                                                    ClauseParam(w),
                                                    ClauseParam(pn));
      valid_query.AddClause(Clause(parent_clause));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select <w, pn.varName> such that Parent*(w, pn) returns <9, x> and <10, x>") {
        REQUIRE(final_result.tuple_result.size() == 2);
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"8", "x"}));
        REQUIRE(Contains(final_result.tuple_result, vector<string>{"9", "x"}));
      }
    }
  }
}

SCENARIO("Test selection of attributes") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    WHEN("Selected design entity is retrieved from the PKB") {
      Query valid_query = Query(SelectedEntity(
          make_pair(DesignEntity(DesignEntityType::PRINT, "pn"), AttributeType::VAR_NAME)));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select pn.varName returns {sum, x}") {
        REQUIRE(final_result.var_proc_names.size() == 2);
        REQUIRE(Contains(final_result.var_proc_names, "x"));
        REQUIRE(Contains(final_result.var_proc_names, "sum"));
      }
    }

    WHEN("Selected design entity does not exist in the PKB") {
      Query valid_query = Query(SelectedEntity(
          make_pair(DesignEntity(DesignEntityType::IF, "ifs"), AttributeType::STMT_NO)));
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);

      THEN("Select ifs.stmt# returns empty result") {
        REQUIRE(final_result.statement_indexes_or_constants.empty());
      }
    }

    WHEN("Selected design entity is a design entity evaluated in the clause") {
      Query valid_query = Query(SelectedEntity(
          make_pair(DesignEntity(DesignEntityType::WHILE, "w"), AttributeType::STMT_NO)));
      SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      valid_query.AddClause(parent_clause);
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);

      THEN("Select w.stmt# such that Parent(_, w) returns 9 only") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 9));
      }
    }

    WHEN("Selected design entity is evaluated in a with clause") {
      DesignEntity pn = DesignEntity(DesignEntityType::PRINT, "pn");
      Query valid_query = Query(SelectedEntity(
          make_pair(DesignEntity(pn), AttributeType::STMT_NO)));
      WithClause with_clause = WithClause(make_pair(ClauseParam(pn), AttributeType::STMT_NO),
                                          make_pair(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE));
      valid_query.AddClause(with_clause);
      QueryResult final_result = QueryEvaluator::EvaluateQuery(valid_query, &pkb, false);
      THEN("Select pn.stmt# with pn.stmt# = constant.value") {
        REQUIRE(final_result.statement_indexes_or_constants.size() == 1);
        REQUIRE(Contains(final_result.statement_indexes_or_constants, 10));
      }
    }
  }
}
