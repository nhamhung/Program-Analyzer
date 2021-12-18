#include "catch.hpp"
#include "query_processor/commons/query/clause/WithClause.h"

using namespace std;
using namespace query_processor;

SCENARIO("Test WithClause constructor") {
  WHEN("Valid with clause with valid int, int") {
    pair<ClauseParam, AttributeType> valid_lhs = make_pair(ClauseParam(1), AttributeType::INTEGER);
    pair<ClauseParam, AttributeType> valid_rhs = make_pair(ClauseParam(2), AttributeType::INTEGER);
    WithClause test_with_clause = WithClause(valid_lhs, valid_rhs);
    THEN("Resultant clause has AttributeType INTEGER and corresponding clause params") {
      REQUIRE(test_with_clause.GetLHSParam() == ClauseParam(1));
      REQUIRE(test_with_clause.GetRHSParam() == ClauseParam(2));
      REQUIRE(test_with_clause.GetLHSAttributeType() == AttributeType::INTEGER);
      REQUIRE(test_with_clause.GetRHSAttributeType() == AttributeType::INTEGER);
    }
    AND_THEN("Clause should pass validation") {
      REQUIRE(test_with_clause.IsValidClause());
    }
  }

  WHEN("Valid with clause with valid DE and attributes") {
    pair<ClauseParam, AttributeType> valid_lhs = make_pair(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")), AttributeType::VAR_NAME);
    pair<ClauseParam, AttributeType> valid_rhs = make_pair(ClauseParam("cenX"), AttributeType::NAME);
    WithClause test_with_clause = WithClause(valid_lhs, valid_rhs);
    THEN("Resultant clause has AttributeType NAME and corresponding clause params") {
      REQUIRE(test_with_clause.GetLHSParam() == ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      REQUIRE(test_with_clause.GetRHSParam() == ClauseParam("cenX"));
      REQUIRE(test_with_clause.GetLHSAttributeType() == AttributeType::VAR_NAME);
      REQUIRE(test_with_clause.GetRHSAttributeType() == AttributeType::NAME);
    }
    AND_THEN("Clause should pass validation") {
      REQUIRE(test_with_clause.IsValidClause());
    }
  }

  WHEN("Valid with clause has attribute types not matched to DE type") {
    pair<ClauseParam, AttributeType> valid_lhs = make_pair(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")), AttributeType::INTEGER);
    pair<ClauseParam, AttributeType> valid_rhs = make_pair(ClauseParam(DesignEntity(DesignEntityType::STMT, "s")), AttributeType::STMT_NO);
    WithClause test_with_clause = WithClause(valid_lhs, valid_rhs);
    THEN("Resultant clause has AttributeType Integer and Stmt No and corresponding clause params") {
      REQUIRE(test_with_clause.GetLHSParam() == ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      REQUIRE(test_with_clause.GetRHSParam() == ClauseParam(DesignEntity(DesignEntityType::STMT, "s")));
      REQUIRE(test_with_clause.GetLHSAttributeType() == AttributeType::INTEGER);
      REQUIRE(test_with_clause.GetRHSAttributeType() == AttributeType::STMT_NO);
    }

    AND_THEN("Clause should fail validation due to incorrect attribute type for variable") {
      REQUIRE_THROWS(test_with_clause.IsValidClause());
    }
  }

  WHEN("Valid with clause has synonyms without attribute type and is not prog_line") {
    pair<ClauseParam, AttributeType> valid_lhs = make_pair(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")), AttributeType::VAR_NAME);
    pair<ClauseParam, AttributeType> valid_rhs = make_pair(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::NONE);
    WithClause test_with_clause = WithClause(valid_lhs, valid_rhs);
    THEN("Resultant clause has corresponding attribute types and clause params") {
      REQUIRE(test_with_clause.GetLHSParam() == ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      REQUIRE(test_with_clause.GetRHSParam() == ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      REQUIRE(test_with_clause.GetLHSAttributeType() == AttributeType::VAR_NAME);
      REQUIRE(test_with_clause.GetRHSAttributeType() == AttributeType::NONE);
    }

    AND_THEN("Clause should fail validation due to incorrect attribute type for variable") {
      REQUIRE_THROWS(test_with_clause.IsValidClause());
    }
  }
}