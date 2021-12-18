#include "catch.hpp"
#include "query_processor/commons/query/clause/Clause.h"

using namespace std;
using namespace query_processor;

SCENARIO("Test Clause comparator") {
  WHEN("Two clauses of the same type and different synonyms are compared") {
    SuchThatClause such_that_clause_1 = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                       ClauseParam(1),
                                                       ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    SuchThatClause such_that_clause_2 = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                       ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                       ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    Clause clause_1 = Clause(such_that_clause_1);
    Clause clause_2 = Clause(such_that_clause_2);
    THEN("First clause should be lesser than second clause") {
      REQUIRE(clause_1 < clause_2);
      REQUIRE_FALSE(clause_2 < clause_1);
    }
  }

  WHEN("Two clauses of different types and different synonyms are compared") {
    PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                 ClauseParam(1),
                                                 ClauseParam(PatternExpression()));
    SuchThatClause such_that_clause = SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                     ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    Clause clause_1 = Clause(pattern_clause);
    Clause clause_2 = Clause(such_that_clause);
    THEN("First clause should be lesser than second clause") {
      REQUIRE(clause_1 < clause_2);
      REQUIRE_FALSE(clause_2 < clause_1);
    }
  }

  WHEN("Two clauses of different types but same number of synonyms are compared") {
    PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                 ClauseParam(1),
                                                 ClauseParam(PatternExpression()));
    WithClause with_clause = WithClause(make_pair(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO),
                                        make_pair(ClauseParam(1), AttributeType::INTEGER));
    Clause clause_1 = Clause(pattern_clause);
    Clause clause_2 = Clause(with_clause);
    THEN("The with clause is smaller than the pattern clause") {
      REQUIRE_FALSE(clause_1 < clause_2);
      REQUIRE(clause_2 < clause_1);
    }
  }
}