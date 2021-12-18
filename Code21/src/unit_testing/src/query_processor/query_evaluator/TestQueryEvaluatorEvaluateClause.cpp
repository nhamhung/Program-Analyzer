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

//typedef std::unordered_map<std::string, QueryResult> ResultTable;

/*
 * The tests in this evaluator are largely written based on Code 3 in Basic SPA Requirements
 */

SCENARIO("Test EvaluateSuchThatClause with Follows Clause") {
  GIVEN("PKB Stub") {
    PKBStub pkb_stub = PKBStub();
    QueryEvaluator::SetPKB(&pkb_stub);
    Database empty_database;
    WHEN("Invalid Follows SuchThatClause is evaluated") {
      SuchThatClause invalid_follows_clause_with_variables = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                            ClauseParam(1),
                                                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "V")));
      THEN("Throws error on invalid DesignEntity") {
        REQUIRE_THROWS(QueryEvaluator::EvaluateSuchThatClause(invalid_follows_clause_with_variables, empty_database));
      }

      SuchThatClause invalid_follows_clause_with_names = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                        ClauseParam(1),
                                                                        ClauseParam("test"));
      THEN("Throws error on invalid SuchThatClause parameter") {
        REQUIRE_THROWS(QueryEvaluator::EvaluateSuchThatClause(invalid_follows_clause_with_names, empty_database));
      }
    }

    WHEN("Valid Follows(INT, INT) SuchThatClause is evaluated") {
      SuchThatClause valid_follows_clause_returns_true = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(1), ClauseParam(2));
      bool test_bool = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_returns_true,
                                                              empty_database);
      THEN("Clause evaluates to true when Follows(1, 2)") {
        REQUIRE(test_bool == true);
      }

      SuchThatClause valid_follows_clause_returns_false = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(1), ClauseParam(3));
      bool test_bool_false = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_returns_false,
                                                                    empty_database);
      THEN("Clause evaluates to false when Follows(1, 3") {
        REQUIRE(test_bool_false == false);
      }

      SuchThatClause valid_follows_clause_inverted = SuchThatClause(DesignAbstraction::FOLLOWS, ClauseParam(2), ClauseParam(1));
      bool test_bool_inverted = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_inverted,
                                                                       empty_database);
      THEN("Clause evaluates to false when Follows(2, 1)") {
        REQUIRE(test_bool_inverted == false);
      }
    }

    WHEN("Valid Follows(INT, DE) is evaluated") {
      Database database;
      // synonym map includes space {1, 2} where Follows (1,1) returns false but Follows (1,2) returns true
      ResultTable result_table = CreateSingleColumnResultTable("s", unordered_set<int>{1, 2});
      database.push_back(result_table);
      SuchThatClause valid_follows_clause_returns_true = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                        ClauseParam(1),
                                                                        ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));
      bool test_bool = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_returns_true, database);
      THEN("Clause evaluates to true when Follows(1, s) where s = {1, 2}") {
        REQUIRE(test_bool == true);
      }

      ResultTable synonym_mappings_without_follows = CreateSingleColumnResultTable("a", unordered_set<int>{4, 5, 6});
      database.push_back(synonym_mappings_without_follows);
      SuchThatClause valid_follows_clause_returns_false = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                         ClauseParam(2),
                                                                         ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      bool test_bool_false = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_returns_false, database);
      THEN("Clause evaluates to false when Follows (2, a) where a = {4, 5, 6}") {
        REQUIRE(test_bool_false == false);
      }

      Database new_database;
      ResultTable statement_result_set = CreateSingleColumnResultTable("s", unordered_set<int>{1, 2, 3, 4, 5, 6, 7});
      new_database.push_back(statement_result_set);
      SuchThatClause valid_follows_separated_returns_true = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                           ClauseParam(3),
                                                                           ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));
      bool test_separated_bool = QueryEvaluator::EvaluateSuchThatClause(valid_follows_separated_returns_true, new_database);
      THEN("Clause evaluates to true when Follows (3, s) where the only statement after is 7") {
        REQUIRE(test_separated_bool == true);
      }
    }

    WHEN("Valid Follows(INT, _) is evaluated") {
      SuchThatClause valid_follows_wildcard_clause_returns_true = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                                 ClauseParam(4),
                                                                                 ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      bool test_bool = QueryEvaluator::EvaluateSuchThatClause(valid_follows_wildcard_clause_returns_true, empty_database);
      THEN("SuchThatClause evaluates to true when Follows (4, _)") {
        REQUIRE(test_bool == true);
      }

      SuchThatClause valid_follows_wildcard_clause_returns_false = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                                  ClauseParam(6),
                                                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      bool test_bool_false = QueryEvaluator::EvaluateSuchThatClause(valid_follows_wildcard_clause_returns_false, empty_database);
      THEN("SuchThatClause evaluates to false with Follows (6, _)") {
        REQUIRE(test_bool_false == false);
      }
    }

    WHEN("Valid Follows(_, _) is evaluated") {
      SuchThatClause valid_follows_wildcard_clause_returns_true = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                                 ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                                                 ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      bool test_bool = QueryEvaluator::EvaluateSuchThatClause(valid_follows_wildcard_clause_returns_true, empty_database);
      THEN("SuchThatClause evaluates to true when Follows (_, _)") {
        REQUIRE(test_bool == true);
      }
    }

    WHEN("Valid Follows(DE, DE) is evaluated, with both parameters being the same synonym") {
      Database database;
      unordered_set<int> all_statements = {1, 2, 3, 4, 5, 6, 7};
      ResultTable synonym_mappings = CreateSingleColumnResultTable("s", all_statements);
      database.push_back(synonym_mappings);
      SuchThatClause valid_follows_clause_stmt_stmt = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                     ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                                     ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));
      bool test_bool_false = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_stmt_stmt, database);
      THEN("SuchThatClause evaluates to false.") {
        REQUIRE(test_bool_false == false);
      }
    }

    WHEN("Valid Follows(DE, DE) is evaluated, with both parameters being the same type") {
      Database database;
      unordered_set<int> assign_statements = {2, 4, 5, 6};
      ResultTable a1_table = CreateSingleColumnResultTable("a1", assign_statements);
      ResultTable a2_table = CreateSingleColumnResultTable("a2", assign_statements);
      database.push_back(a1_table);
      database.push_back(a2_table);
      SuchThatClause valid_follows_clause_assign1_assign2 = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                           ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")),
                                                                           ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a2")));
      bool test_bool = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_assign1_assign2, database);
      THEN("SuchThatClause evaluates to true.") {
        REQUIRE(test_bool == true);
      }
    }

    WHEN("Valid Follows(DE, DE) is evaluated, with both parameters being the same type, with no matches") {
      ResultTable r1_table = CreateSingleColumnResultTable("r1", unordered_set<int>{1});
      ResultTable r2_table = CreateSingleColumnResultTable("r2", unordered_set<int>{1});
      Database database;
      database.push_back(r1_table);
      database.push_back(r2_table);
      SuchThatClause valid_follows_clause_read1_read2 = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                       ClauseParam(DesignEntity(DesignEntityType::READ, "r1")),
                                                                       ClauseParam(DesignEntity(DesignEntityType::READ, "r2")));
      bool test_bool_false = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_read1_read2, database);
      THEN("SuchThatClause evaluates to false.") {
        REQUIRE(test_bool_false == false);
      }
    }

    WHEN("Valid Follows(DE, DE) is evaluated, with both parameters being different types, with a match") {
      ResultTable assign_table = CreateSingleColumnResultTable("a", unordered_set<int>{2, 4, 5, 6});
      ResultTable while_table = CreateSingleColumnResultTable("w", unordered_set<int>{3});
      Database database;
      database.push_back(assign_table);
      database.push_back(while_table);
      SuchThatClause valid_follows_clause_assign_while = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                        ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                                        ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      bool test_bool = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_assign_while, database);
      THEN("SuchThatClause evaluates to true.") {
        REQUIRE(test_bool == true);
      }
    }

    WHEN("Valid Follows(DE, DE) is evaluated, with both parameters being different types, with no matches") {
      ResultTable assign_table = CreateSingleColumnResultTable("a", unordered_set<int>{2, 4, 5, 6});
      ResultTable while_table = CreateSingleColumnResultTable("w", unordered_set<int>{3});
      Database database;
      database.push_back(assign_table);
      database.push_back(while_table);
      SuchThatClause valid_follows_clause_while_assign = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                        ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                                        ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      bool test_bool_false = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_while_assign, database);
      THEN("SuchThatClause evaluates to false.") {
        REQUIRE(test_bool_false == false);
      }
    }
  }
}

SCENARIO("Test EvaluateSuchThatClause with Parent Clause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;

    WHEN("Invalid Parent Clause is evaluated") {
      SuchThatClause invalid_clause_with_string = SuchThatClause(DesignAbstraction::PARENT,
                                                                 ClauseParam(1),
                                                                 ClauseParam("s"));
      THEN("Clause throws error when Parent(int, string)") {
        REQUIRE_THROWS(QueryEvaluator::EvaluateSuchThatClause(invalid_clause_with_string, empty_database));
      }

      SuchThatClause invalid_clause_with_procedure = SuchThatClause(DesignAbstraction::PARENT,
                                                                    ClauseParam(1),
                                                                    ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      THEN("Clause throws error when Parent(int, PROCEDURE DE)") {
        REQUIRE_THROWS(QueryEvaluator::EvaluateSuchThatClause(invalid_clause_with_procedure, empty_database));
      }
    }

    WHEN("Valid Parent (INT, INT) Clause is evaluated") {
      SuchThatClause valid_clause_return_true = SuchThatClause(DesignAbstraction::PARENT,
                                                               ClauseParam(3),
                                                               ClauseParam(6));
      THEN("Clause evaluates to true when Parent(3, 6)") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_return_true, empty_database));
      }

      SuchThatClause valid_inverted_clause_return_false = SuchThatClause(DesignAbstraction::PARENT,
                                                                         ClauseParam(6),
                                                                         ClauseParam(3));
      THEN("Clause evaluates to false when Parent(6, 3)") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_inverted_clause_return_false, empty_database));
      }

      SuchThatClause valid_clause_nested_return_true = SuchThatClause(DesignAbstraction::PARENT,
                                                                      ClauseParam(9),
                                                                      ClauseParam(10));
      THEN("Clause evaluates to true when Parent (9, 10)") {
        //Statement 9 is a container nested within another container
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_nested_return_true, empty_database));
      }
    }

    WHEN("Valid Parent (INT, DE) Clause is evaluated") {
      SuchThatClause valid_clause_return_true = SuchThatClause(DesignAbstraction::PARENT,
                                                               ClauseParam(3),
                                                               ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));

      THEN("Clause evaluates to true when Parent(3, a)") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_return_true, empty_database));
      }

      SuchThatClause valid_clause_return_false = SuchThatClause(DesignAbstraction::PARENT,
                                                                ClauseParam(3),
                                                                ClauseParam(DesignEntity(DesignEntityType::READ, "r")));
      THEN("Clause evaluates to false when Parent(3, r)") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_return_false, empty_database));
      }

      SuchThatClause valid_clause_nested_return_false = SuchThatClause(DesignAbstraction::PARENT,
                                                                       ClauseParam(8),
                                                                       DesignEntity(DesignEntityType::PRINT, "pn"));

      THEN("Clause evaluates to false when Parent(8, pn)") {
        // This should evaluate to True when Parent* but false when Parent
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_nested_return_false, empty_database));
      }
    }

    WHEN("Valid Parent(DE, DE) Clause is evaluated") {
      SuchThatClause valid_clause_return_true = SuchThatClause(DesignAbstraction::PARENT,
                                                               ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                               ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      THEN("Clause evaluates to true when Parent(w, a)") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_return_true, empty_database));
      }

      SuchThatClause valid_clause_nested_return_false = SuchThatClause(DesignAbstraction::PARENT,
                                                                       ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                                       ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")));
      bool result = QueryEvaluator::EvaluateSuchThatClause(valid_clause_nested_return_false, empty_database);
      THEN("Clause evaluates to true when Parent(w,pn)") {
        REQUIRE(result);
      }

      SuchThatClause valid_clause_repeated_return_false = SuchThatClause(DesignAbstraction::PARENT,
                                                                         ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                                         ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      THEN("Clause evaluates to false when Parent(w,w)") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_repeated_return_false, empty_database));
      }
    }

    WHEN("Valid Parent(DE, _) or Parent(_, DE) is evaluated") {
      ResultTable filled_result_table;
      SuchThatClause valid_clause_return_true = SuchThatClause(DesignAbstraction::PARENT,
                                                               ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                               ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("Clause evaluates to true when Parent(w, _)") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_return_true, empty_database));
      }
      SuchThatClause valid_clause_return_false = SuchThatClause(DesignAbstraction::PARENT,
                                                                ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                                ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));

      THEN("Clause evaluates to false when Parent(a, _)") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_return_false, empty_database));
      }

      SuchThatClause valid_clause_inverted_return_true = SuchThatClause(DesignAbstraction::PARENT,
                                                                        ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                                        ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      THEN("Clause evaluates to true when Parent(_,a)") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_inverted_return_true, empty_database));
      }
    }

    WHEN("Valid Parent(_, _) is evaluated") {
      SuchThatClause valid_clause_return_true = SuchThatClause(DesignAbstraction::PARENT,
                                                               ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                               ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("Clause evaluates to true when Parent(_, _)") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_return_true, empty_database));
      }
    }
  }
}

SCENARIO("Test EvaluateSuchThatClause with ParentT Clause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;

    WHEN("Valid ParentT (INT, INT) Clause is evaluated") {
      SuchThatClause valid_clause_return_true = SuchThatClause(DesignAbstraction::PARENT_T,
                                                               ClauseParam(8),
                                                               ClauseParam(10));
      THEN("Clause evaluates to true when ParentT(8, 10)") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_return_true, empty_database));
      }

      SuchThatClause valid_inverted_clause_return_false = SuchThatClause(DesignAbstraction::PARENT_T,
                                                                         ClauseParam(10),
                                                                         ClauseParam(8));
      THEN("Clause evaluates to false when ParentT(10, 8)") {
        REQUIRE_FALSE(
            QueryEvaluator::EvaluateSuchThatClause(valid_inverted_clause_return_false, empty_database));
      }
    }

    WHEN("Valid ParentT (INT, DE) Clause is evaluated") {
      SuchThatClause valid_clause_nested_return_true = SuchThatClause(DesignAbstraction::PARENT_T,
                                                                      ClauseParam(8),
                                                                      ClauseParam(DesignEntity(DesignEntityType::PRINT,
                                                                                               "pn")));

      THEN("Clause evaluates to true when ParentT(8, pn)") {
        bool result = QueryEvaluator::EvaluateSuchThatClause(valid_clause_nested_return_true, empty_database);
        REQUIRE(result == true);
      }
    }

    WHEN("Valid ParentT (DE, DE) Clause is evaluated") {
      SuchThatClause valid_clause_nested_return_true = SuchThatClause(DesignAbstraction::PARENT_T,
                                                                      ClauseParam(DesignEntity(
                                                                          DesignEntityType::WHILE, "w")),
                                                                      ClauseParam(DesignEntity(
                                                                          DesignEntityType::PRINT, "pn")));
      THEN("Clause evaluates to true when ParentT(w,pn)") {
        //This should evaluate to True when Parent* but false when Parent
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_nested_return_true, empty_database));
      }

      SuchThatClause valid_clause_repeated_return_false = SuchThatClause(DesignAbstraction::PARENT_T,
                                                                         ClauseParam(DesignEntity(
                                                                             DesignEntityType::WHILE, "w")),
                                                                         ClauseParam(DesignEntity(
                                                                             DesignEntityType::WHILE, "w")));
      THEN("Clause evaluates to false when ParentT(w,w)") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_clause_repeated_return_false,
                                                             empty_database));
      }
    }
  }
}

SCENARIO("Test EvaluateSuchThatClause with Uses Clause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;

    WHEN("Valid Uses(INT, NAME) is evaluated") {
      THEN("Uses(4, \"number\") is an assignment statement that returns true") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(4),
                                                          ClauseParam("number"));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }
      THEN("Uses(7, \"sum\") is a print statement that returns true") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(7),
                                                          ClauseParam("sum"));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }
      THEN("Uses(4, \"digit\" is an assignment statement that modifies digit, returns false") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(4),
                                                          ClauseParam("digit"));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }
    }

    WHEN("Valid Uses(INT, DE) is evaluated") {
      THEN("Uses(5, v) is an assignment statement that returns true") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(5),
                                                          ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }

      ResultTable v1_table = CreateSingleColumnResultTable("v1", std::unordered_set<std::string>{"number"});
      Database filled_database{v1_table};
      THEN("Uses(5, v1) with variable space reduced to only contain {number} will return false") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(5),
                                                          ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, filled_database));
      }

      THEN("Uses(2, v) is an assignment statement that only uses constants, returns false") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(2),
                                                          ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }
    }

    WHEN("Valid Uses(INT, _) is evaluated") {
      THEN("Uses(7, _) returns true") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(7),
                                                          ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }

      THEN("Uses(2, _) returns false") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(2),
                                                          ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }
    }

    WHEN("Valid Uses(DE, NAME) is evaluated") {
      THEN("Uses(w, \"digit\") returns true, container statements") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                          ClauseParam("digit"));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }

      THEN("Uses(a, \"digit\") returns true") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                          ClauseParam("digit"));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }

      THEN("Uses(p, \"x\") returns true") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                          ClauseParam("x"));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }
    }

    WHEN("Valid Uses(DE, DE) is evaluated") {
      THEN("Uses(w, v) returns true") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                          ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }

      THEN("Uses(r, v) returns false") {
        SuchThatClause valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                          ClauseParam(DesignEntity(DesignEntityType::READ, "r")),
                                                          ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_uses_clause, empty_database));
      }
    }
  }
}

//Modifies is generally the same as Uses for Iteration 1 so test cases are a little more sparse.
//To add on in future iterations where necessary.
SCENARIO("Test EvaluateSuchThatClause with Modifies Clause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;

    WHEN("Valid Modifies(INT, NAME) is evaluated") {
      THEN("Modifies(2, \"sum\") returns true") {
        SuchThatClause valid_modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                              ClauseParam(2),
                                                              ClauseParam("sum"));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_modifies_clause, empty_database));
      }

      THEN("Modifies(4, \"number\" returns false") {
        SuchThatClause valid_modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                              ClauseParam(4),
                                                              ClauseParam("number"));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_modifies_clause, empty_database));
      }
    }

    WHEN("Valid Modifies(INT, DE) is evaluated") {
      THEN("Modifies(1, v) returns true") {
        SuchThatClause valid_modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                              ClauseParam(1),
                                                              ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_modifies_clause, empty_database));
      }

      ResultTable v1_table = CreateSingleColumnResultTable("v1", std::unordered_set<std::string>{"digit"});
      Database filled_database{v1_table};
      THEN("Modifies(1, v1) returns false when v1 = {digit}") {
        SuchThatClause valid_modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                              ClauseParam(1),
                                                              ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1")));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_modifies_clause, filled_database));
      }
    }

    WHEN("Valid Modifies(DE, DE) is evaluated") {
      THEN("Modifies(s, v) returns true") {
        SuchThatClause valid_modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                              ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                              ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_modifies_clause, empty_database));
      }

      ResultTable s1_table = CreateSingleColumnResultTable("s1", std::unordered_set<int>{7});
      Database filled_database{s1_table};
      THEN("Modifies(s1, v) returns false when s1 = {7}") {
        SuchThatClause valid_modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                              ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                                              ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_modifies_clause, filled_database));
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_modifies_clause, empty_database));
      }
    }

    WHEN("Valid Modifies(DE, NAME) is evaluated") {
      THEN("Modifies(p, \"y\") returns false") {
        SuchThatClause valid_modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                              ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                              ClauseParam("y"));
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_modifies_clause, empty_database));
      }
    }
  }
}

SCENARIO("Test SuchThatClause with Calls/* clause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;

    WHEN("Invalid calls clause") {
      SuchThatClause invalid_calls_clause = SuchThatClause(DesignAbstraction::CALLS_T,
                                                           ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                           ClauseParam("test"));
      REQUIRE_THROWS(QueryEvaluator::EvaluateSuchThatClause(invalid_calls_clause, empty_database));
    }

    WHEN("Valid Calls(NAME, NAME) returns true") {
      SuchThatClause valid_calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                         ClauseParam("nestVar"),
                                                         ClauseParam("sumDigits"));
      THEN("Calls(nestVar, sumDigits) return true") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_calls_clause, empty_database));
      }
    }

    WHEN("Valid Calls(DE, NAME) returns false") {
      SuchThatClause valid_calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                         ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                         ClauseParam("nestVar"));
      THEN("Calls(p, nestVar) returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_calls_clause, empty_database));
      }
    }

    WHEN("Valid Calls(DE, DE) returns true") {
      SuchThatClause valid_calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                         ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                         ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")));
      THEN("Calls(p, p2) returns true") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_calls_clause, empty_database));
      }
    }

    WHEN("Valid Calls*(DE, DE) returns false") {
      SuchThatClause valid_calls_clause = SuchThatClause(DesignAbstraction::CALLS_T,
                                                         ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                         ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      THEN("Calls*(p, p) returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_calls_clause, empty_database));
      }
    }

    WHEN("Valid Calls*(_, NAME) returns false") {
      SuchThatClause valid_calls_clause = SuchThatClause(DesignAbstraction::CALLS_T,
                                                         ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                         ClauseParam("nestVar"));
      THEN("Calls*(_, nestVar) returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_calls_clause, empty_database));
      }
    }
  }
}

SCENARIO("Test SuchThatClause with Next/* clause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;

    WHEN("Valid Next(int, int) returns true") {
      SuchThatClause valid_next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                        ClauseParam(6),
                                                        ClauseParam(3));
      THEN("Next (6, 3) returns true") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_next_clause, empty_database));
      }
    }

    WHEN("Valid Next(int, int) returns false") {
      SuchThatClause valid_next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                        ClauseParam(6),
                                                        ClauseParam(7));
      THEN("Next (6, 7) returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_next_clause, empty_database));
      }
    }

    WHEN("Valid Next(int, progline) returns true") {
      SuchThatClause valid_next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                        ClauseParam(3),
                                                        ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      THEN("Next (3, n) returns true") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_next_clause, empty_database));
      }
    }

    WHEN("Valid Next(int, progline) returns false") {
      SuchThatClause valid_next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                        ClauseParam(7),
                                                        ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      THEN("Next (7, n) returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateSuchThatClause(valid_next_clause, empty_database));
      }
    }

    WHEN("Valid Next*(int, int) returns true") {
      SuchThatClause valid_nextT_clause = SuchThatClause(DesignAbstraction::NEXT_T,
                                                         ClauseParam(3),
                                                         ClauseParam(3));
      THEN("Next*(3, 3) returns true") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_nextT_clause, empty_database));
      }
    }

    WHEN("Valid Next*(progline, progline) returns true") {
      SuchThatClause valid_nextT_clause = SuchThatClause(DesignAbstraction::NEXT_T,
                                                         ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")),
                                                         ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      THEN("Next*(n, n) returns true") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_nextT_clause, empty_database));
      }
    }
  }
}
SCENARIO("Test EvaluatePatternClause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;

    WHEN("Invalid pattern clause") {
      PatternClause invalid_rhs_pattern = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                        ClauseParam("x"),
                                                        ClauseParam("x + 10"));

      THEN("Throws error on invalid non-PatternExpression param on the right side") {
        REQUIRE_THROWS(QueryEvaluator::EvaluatePatternClause(invalid_rhs_pattern, empty_database));
      }

      PatternClause invalid_lhs_pattern = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                        ClauseParam(DesignEntity(DesignEntityType::STMT, "S")),
                                                        ClauseParam(PatternExpression(TokenList())));

      THEN("Throws error on invalid DesignEntity on the LHS") {
        REQUIRE_THROWS(QueryEvaluator::EvaluatePatternClause(invalid_lhs_pattern, empty_database));
      }
    }

    WHEN("Assignment pattern(_, complete match)") {
      TokenList rhs_expr;
      Token number("number", TokenType::VariableName);
      Token mod("%", TokenType::ExpressionOp);
      Token ten("10", TokenType::ConstantValue);
      rhs_expr.Push(number).Push(mod).Push(ten);
      PatternClause valid_pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                         ClauseParam(
                                                             DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                         ClauseParam(PatternExpression(rhs_expr)));
      THEN("pattern a(_, \"number % 10\") returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause, empty_database));
      }

      PatternClause valid_pattern_clause_2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a1"),
                                                           ClauseParam(
                                                               DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                           ClauseParam(PatternExpression(rhs_expr)));

      TokenList partial_rhs;
      partial_rhs.Push(number);
      PatternClause valid_pattern_clause_returns_false = PatternClause(
          DesignEntity(DesignEntityType::ASSIGN, "a"),
          ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
          ClauseParam(PatternExpression(partial_rhs)));
      THEN("pattern a(_, \"number\") returns false") {
        REQUIRE_FALSE(
            QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_returns_false, empty_database));
      }
    }

    WHEN("Assignment pattern(var, complete match") {
      TokenList rhs_constant;
      Token zero("0", TokenType::ConstantValue);
      rhs_constant.Push(zero);
      PatternClause valid_pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                         ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                         ClauseParam(PatternExpression(rhs_constant)));
      THEN("pattern a(v, \"0\") returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause, empty_database));
      }

      TokenList rhs_expr;
      Token sum("sum", TokenType::VariableName);
      Token digit("digit", TokenType::VariableName);
      Token plus("+", TokenType::ExpressionOp);
      rhs_expr.Push(sum).Push(plus).Push(digit);
      PatternClause valid_pattern_clause_lhs_sum = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                                 ClauseParam("sum"),
                                                                 ClauseParam(PatternExpression(rhs_expr)));
      THEN("pattern a(\"sum\", \"sum + digit\") returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_lhs_sum, empty_database));
      }

      PatternClause valid_pattern_clause_lhs_digit = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                                   ClauseParam("digit"),
                                                                   ClauseParam(PatternExpression(rhs_expr)));

      THEN("pattern a(\"digit\", \"sum + digit\") returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_lhs_digit, empty_database));
      }
    }

    WHEN("Assignment pattern(_, partial match)") {
      Token sum("sum", TokenType::VariableName);
      Token digit("digit", TokenType::VariableName);
      Token plus("+", TokenType::ExpressionOp);

      TokenList partial_list;
      partial_list.Push(sum);
      PatternClause valid_pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                         ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                         ClauseParam(PatternExpression(partial_list, true)));
      THEN("pattern a(_, _\"sum\"_) returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause, empty_database));
      }

      TokenList full_list;
      full_list.Push(sum).Push(plus).Push(digit);
      PatternClause valid_pattern_clause_full = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                              ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                              ClauseParam(PatternExpression(full_list, true)));

      THEN("pattern a(_, _\"sum + digit\"_) returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_full, empty_database));
      }

      Token nonexistent("nonexistent", TokenType::VariableName);
      TokenList bad_list;
      bad_list.Push(nonexistent);
      PatternClause valid_pattern_clause_return_false = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                                      ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                                      ClauseParam(PatternExpression(bad_list, true)));
      THEN("pattern a(_, _\"nonexistent\"_) returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_return_false, empty_database));
      }
    }

    WHEN("Assignment pattern(var, partial match)") {
      Token sum("sum", TokenType::VariableName);
      Token digit("digit", TokenType::VariableName);
      Token plus("+", TokenType::ExpressionOp);
      Token ten("10", TokenType::ConstantValue);

      TokenList rhs_constant;
      rhs_constant.Push(ten);
      PatternClause valid_pattern_clause_var = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                             ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                             ClauseParam(PatternExpression(rhs_constant, true)));
      THEN("pattern a(v, _\"10\"_) returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_var, empty_database));
      }

      TokenList rhs_expr;
      rhs_expr.Push(sum).Push(plus).Push(digit);
      PatternClause valid_pattern_clause_sum = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                             ClauseParam("sum"),
                                                             ClauseParam(PatternExpression(rhs_expr, true)));
      THEN("pattern a(\"sum\", _\"sum + digit\"_) returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_sum, empty_database));
      }

      TokenList rhs_digit;
      rhs_digit.Push(digit);
      PatternClause valid_pattern_clause_return_false = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                                      ClauseParam("number"),
                                                                      ClauseParam(PatternExpression(rhs_digit, true)));
      THEN("pattern a(\"number\", _\"digit\"_) returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_return_false, empty_database));
      }
    }
    WHEN("Assignment pattern(_,_)") {
      PatternClause valid_pattern_clause_wildcard = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                                  ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("pattern a(_,_) returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(valid_pattern_clause_wildcard, empty_database));
      }
    }
  }
}

SCENARIO("Test evaluate conditional pattern clauses") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;
    WHEN("While pattern(NAME, _) returns true") {
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::WHILE, "w"),
                                                   ClauseParam("x"),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("pattern w(\"x\",_) returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(pattern_clause, empty_database));
      }
    }

    WHEN("While pattern(VAR,_) returns true") {
      PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::WHILE, "w"),
                                                   ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("pattern w(v, _) returns true") {
        REQUIRE(QueryEvaluator::EvaluatePatternClause(pattern_clause, empty_database));
      }
    }
  }
}

SCENARIO("Test EvaluateWithClause") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;
    WHEN("with INT == INT returns true") {
      WithClause with_clause = WithClause(make_pair(ClauseParam(1), AttributeType::INTEGER), make_pair(ClauseParam(1), AttributeType::INTEGER));
      THEN("with 1==1 returns true") {
        REQUIRE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }
    WHEN("with NAME == NAME returns false") {
      WithClause with_clause = WithClause(make_pair(ClauseParam("x"), AttributeType::NAME), make_pair(ClauseParam("y"), AttributeType::NAME));
      THEN("with x==y returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }

    WHEN("with INT == NAME returns false") {
      WithClause with_clause = WithClause(make_pair(ClauseParam("x"), AttributeType::NAME), make_pair(ClauseParam(1), AttributeType::INTEGER));
      THEN("with x==1 returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }
    WHEN("with DE == INT returns true") {
      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")), AttributeType::STMT_NO),
                                          make_pair(ClauseParam(9), AttributeType::INTEGER));

      THEN("with w.stmt# == 9 returns true") {
        REQUIRE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }

    WHEN("with DE == INT returns false") {
      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO),
                                          make_pair(ClauseParam(3), AttributeType::INTEGER));

      THEN("with a.stmt# == 3 returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }
    WHEN("with DE == NAME returns true") {
      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::CALL, "c")), AttributeType::PROC_NAME),
                                          make_pair(ClauseParam("sumDigits"), AttributeType::NAME));

      THEN("with c.procName == sumDigits returns true") {
        REQUIRE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }

    WHEN("with DE == NAME returns false") {
      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")), AttributeType::VAR_NAME),
                                          make_pair(ClauseParam("y"), AttributeType::NAME));

      THEN("with pn.varName == y returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }
    WHEN("with DE == DE returns true") {
      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")), AttributeType::VAR_NAME),
                                          make_pair(ClauseParam(DesignEntity(DesignEntityType::READ, "r")), AttributeType::VAR_NAME));

      THEN("with v.varName == r.varName returns true") {
        REQUIRE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }
    WHEN("with DE == DE returns false") {
      WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO),
                                          make_pair(ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")), AttributeType::STMT_NO));

      THEN("with a.stmt# == w.stmt# returns false") {
        REQUIRE_FALSE(QueryEvaluator::EvaluateWithClause(with_clause, empty_database));
      }
    }
  }
}

SCENARIO("Test special cases") {
  GIVEN("PKB Stub") {
    PKBStub pkb = PKBStub();
    QueryEvaluator::SetPKB(&pkb);
    Database empty_database;
    WHEN("Synonyms names are the same as left key and right key") {
      SuchThatClause valid_follows_clause_returns_true = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                        ClauseParam(DesignEntity(
                                                                            DesignEntityType::STMT, "LEFT")),
                                                                        ClauseParam(DesignEntity(
                                                                            DesignEntityType::STMT,
                                                                            "RIGHT")));

      THEN("stmt LEFT, RIGHT; Clause evaluates to true when Follows(LEFT, RIGHT)") {
        REQUIRE(QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_returns_true, empty_database));
      }
    }

    WHEN("Synonym names are same as left key and right key evaluates false") {
      SuchThatClause valid_follows_clause_returns_false = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                                         ClauseParam(DesignEntity(
                                                                             DesignEntityType::IF, "LEFT")),
                                                                         ClauseParam(DesignEntity(
                                                                             DesignEntityType::STMT,
                                                                             "RIGHT")));
      bool evaluation = QueryEvaluator::EvaluateSuchThatClause(valid_follows_clause_returns_false,
                                                               empty_database);
      THEN("if LEFT; stmt RIGHT; Clause evaluates to false when Follows(LEFT, RIGHT)") {
        REQUIRE_FALSE(evaluation);
      }
    }
  }
}