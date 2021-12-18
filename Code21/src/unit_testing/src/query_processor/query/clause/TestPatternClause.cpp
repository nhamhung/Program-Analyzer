#include "catch.hpp"
#include "query_processor/commons/query/clause/PatternClause.h"
#include "query_processor/commons/query/entities/DesignEntity.h"

using namespace std;
using namespace query_processor;
using namespace source_processor;

SCENARIO("Test Pattern Clause constructor and validation") {
  WHEN("Valid assign design_entity, valid string and valid TokenList") {
    DesignEntity valid_design_entity = DesignEntity(DesignEntityType::ASSIGN, "a");
    PatternClause test_valid_pattern_clause = PatternClause(valid_design_entity,
                                                            ClauseParam("x"),
                                                            ClauseParam(PatternExpression(TokenList())));
    THEN("Pattern Clause should return the corresponding ClauseParams and DesignEntity") {
      REQUIRE(test_valid_pattern_clause.GetDesignEntity() == valid_design_entity);
      REQUIRE(test_valid_pattern_clause.GetRHSParam() == ClauseParam(PatternExpression(TokenList())));
      REQUIRE(test_valid_pattern_clause.GetLHSParam() == ClauseParam("x"));
    }

    AND_THEN("Pattern Clause should pass validation") {
      REQUIRE(test_valid_pattern_clause.IsValidClause() == true);
    }
  }

  WHEN("Valid while design_entity, valid string and invalid TokenList") {
    DesignEntity valid_design_entity = DesignEntity(DesignEntityType::WHILE, "w");
    PatternClause test_invalid_pattern_clause = PatternClause(valid_design_entity,
                                                              ClauseParam("x"),
                                                              ClauseParam(PatternExpression(TokenList())));
    THEN("Pattern Clause should return the corresponding ClauseParams and DesignEntity") {
      REQUIRE(test_invalid_pattern_clause.GetDesignEntity() == valid_design_entity);
      REQUIRE(test_invalid_pattern_clause.GetRHSParam() == ClauseParam(PatternExpression(TokenList())));
      REQUIRE(test_invalid_pattern_clause.GetLHSParam() == ClauseParam("x"));
    }

    AND_THEN("Pattern Clause should fail validation due to non-wildcard RHS") {
      REQUIRE_THROWS(test_invalid_pattern_clause.IsValidClause());
    }
  }

  WHEN("Valid design entity, invalid int and valid TokenList") {
    DesignEntity valid_design_entity = DesignEntity(DesignEntityType::ASSIGN, "a");
    PatternClause test_valid_pattern_clause = PatternClause(valid_design_entity,
                                                            ClauseParam(1),
                                                            ClauseParam(PatternExpression(TokenList())));
    THEN("Pattern Clause should return the corresponding ClauseParams and DesignEntity") {
      REQUIRE(test_valid_pattern_clause.GetDesignEntity() == valid_design_entity);
      REQUIRE(test_valid_pattern_clause.GetRHSParam() == ClauseParam(PatternExpression(TokenList())));
      REQUIRE(test_valid_pattern_clause.GetLHSParam() == ClauseParam(1));
    }

    AND_THEN("Pattern Clause should fail validation due to int on LHS") {
      REQUIRE_THROWS(test_valid_pattern_clause.IsValidClause());
    }
  }

  WHEN("Valid design entity, valid DesignEntity and invalid string") {
    DesignEntity valid_design_entity = DesignEntity(DesignEntityType::ASSIGN, "a");
    DesignEntity param_design_entity = DesignEntity(DesignEntityType::VARIABLE, "v");
    PatternClause test_valid_pattern_clause = PatternClause(valid_design_entity,
                                                            ClauseParam(param_design_entity),
                                                            ClauseParam("x"));

    THEN("Pattern Clause should return the corresponding ClauseParams and DesignEntity") {
      REQUIRE(test_valid_pattern_clause.GetDesignEntity() == valid_design_entity);
      REQUIRE(test_valid_pattern_clause.GetRHSParam() == ClauseParam("x"));
      REQUIRE(test_valid_pattern_clause.GetLHSParam() == ClauseParam(param_design_entity));
    }

    AND_THEN("Pattern Clause should fail validation due to invalid string on RHS") {
      REQUIRE_THROWS(test_valid_pattern_clause.IsValidClause());
    }
  }

  WHEN("Valid design entity, invalid DesignEntity with wrong type and valid TokenList") {
    DesignEntity valid_design_entity = DesignEntity(DesignEntityType::ASSIGN, "a");
    DesignEntity param_design_entity = DesignEntity(DesignEntityType::PROCEDURE, "p");
    PatternClause test_valid_pattern_clause = PatternClause(valid_design_entity,
                                                            ClauseParam(param_design_entity),
                                                            ClauseParam(PatternExpression(TokenList())));

    THEN("Pattern Clause should return the corresponding ClauseParams and DesignEntity") {
      REQUIRE(test_valid_pattern_clause.GetDesignEntity() == valid_design_entity);
      REQUIRE(test_valid_pattern_clause.GetRHSParam() == ClauseParam(PatternExpression(TokenList())));
      REQUIRE(test_valid_pattern_clause.GetLHSParam() == ClauseParam(param_design_entity));
    }

    AND_THEN("Pattern Clause should fail validation due to invalid design entity type on LHS") {
      REQUIRE_THROWS(test_valid_pattern_clause.IsValidClause());
    }
  }

  WHEN("Invalid design entity, valid DesignEntity and valid TokenList") {
    DesignEntity invalid_design_entity = DesignEntity(DesignEntityType::STMT, "s");
    DesignEntity valid_design_entity = DesignEntity(DesignEntityType::VARIABLE, "v");
    PatternClause test_valid_pattern_clause = PatternClause(invalid_design_entity,
                                                            ClauseParam(valid_design_entity),
                                                            ClauseParam(PatternExpression(TokenList())));

    THEN("Pattern Clause should return the corresponding ClauseParams and DesignEntity") {
      REQUIRE(test_valid_pattern_clause.GetDesignEntity() == invalid_design_entity);
      REQUIRE(test_valid_pattern_clause.GetRHSParam() == ClauseParam(PatternExpression(TokenList())));
      REQUIRE(test_valid_pattern_clause.GetLHSParam() == ClauseParam(valid_design_entity));
    }

    AND_THEN("Pattern Clause should fail validation due to invalid design entity type on clause") {
      REQUIRE_THROWS(test_valid_pattern_clause.IsValidClause());
    }
  }
}

SCENARIO("Test PatternClause Equality") {
  ClauseParam cp1 = ClauseParam(DesignEntity(DesignEntityType::STMT, "s"));
  ClauseParam cp2 = ClauseParam(DesignEntity(DesignEntityType::STMT, "s"));
  WHEN("Only two clauses are the same") {
    PatternClause c1 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"), cp1, cp2);
    PatternClause c2 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"), cp1, cp2);
    PatternClause c3 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "v"), cp1, cp2);
    PatternClause c4 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"), ClauseParam(2), cp2);
    PatternClause c5 = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"), DesignEntity(DesignEntityType::ASSIGN, "s"), cp2);
    THEN("Only c1 and c2 are equal") {
      REQUIRE(c1 == c2);
      REQUIRE_FALSE(c1 == c3);
      REQUIRE_FALSE(c1 == c4);
      REQUIRE_FALSE(c1 == c5);
    }
  }
}