#include "catch.hpp"
#include "query_processor/commons/query/clause/DesignAbstraction.h"
#include "query_processor/commons/query/clause/SuchThatClause.h"
#include "query_processor/commons/query/entities/DesignEntity.h"

using namespace std;
using namespace query_processor;

SCENARIO("Test SuchThatClause constructor") {
  WHEN("Valid design abstraction, string and int") {
    DesignAbstraction valid_design_abstraction = DesignAbstraction::USES;
    ClauseParam valid_param_string = ClauseParam("random_procedure");
    ClauseParam valid_param_int = ClauseParam(10);
    SuchThatClause test_valid_clause = SuchThatClause(valid_design_abstraction, valid_param_string, valid_param_int);
    THEN("SuchThatClause should return the same design abstraction, string and int") {
      REQUIRE(test_valid_clause.GetDesignAbstraction() == DesignAbstraction::USES);
      REQUIRE(test_valid_clause.GetLHSParam().var_proc_name == "random_procedure");
      REQUIRE(test_valid_clause.GetLHSParam().param_type == ClauseParamType::NAME);
      REQUIRE(test_valid_clause.GetRHSParam().statement_index == 10);
      REQUIRE(test_valid_clause.GetRHSParam().param_type == ClauseParamType::INDEX);
    }
  }

  WHEN("Valid design abstraction, design entity and int") {
    DesignAbstraction valid_design_abstraction = DesignAbstraction::FOLLOWS;
    DesignEntityType valid_entity_type = DesignEntityType::STMT;
    DesignEntity valid_design_entity = DesignEntity(valid_entity_type, "s1");
    ClauseParam valid_param_design_entity = ClauseParam(valid_design_entity);
    ClauseParam valid_param_int = ClauseParam(10);
    SuchThatClause test_valid_clause = SuchThatClause(valid_design_abstraction, valid_param_design_entity, valid_param_int);
    THEN("SuchThatClause should return the same design abstraction, design entity and int") {
      REQUIRE(test_valid_clause.GetDesignAbstraction() == DesignAbstraction::FOLLOWS);

      DesignEntity lhs_param = test_valid_clause.GetLHSParam().design_entity;
      REQUIRE(lhs_param.GetDesignEntityType() == DesignEntityType::STMT);
      REQUIRE(test_valid_clause.GetLHSParam().param_type == ClauseParamType::DESIGN_ENTITY);
      REQUIRE(test_valid_clause.GetRHSParam().statement_index == 10);
      REQUIRE(test_valid_clause.GetRHSParam().param_type == ClauseParamType::INDEX);
    }
  }
}

SCENARIO("Test Equality") {
  ClauseParam cp1 = ClauseParam(DesignEntity(DesignEntityType::STMT, "s"));
  ClauseParam cp2 = ClauseParam(DesignEntity(DesignEntityType::STMT, "s"));
  WHEN("Test SuchThatClause Param Equality") {
    THEN("ClauseParams must be equal") {
      REQUIRE(cp1 == cp2);
    }
  }

  WHEN("Test SuchThatClause Equality") {
    SuchThatClause c1 = SuchThatClause(DesignAbstraction::FOLLOWS_T, cp1, cp2);
    SuchThatClause c2 = SuchThatClause(DesignAbstraction::FOLLOWS_T, cp1, cp2);
    SuchThatClause c3 = SuchThatClause(DesignAbstraction::FOLLOWS, cp1, cp2);
    SuchThatClause c4 = SuchThatClause(DesignAbstraction::FOLLOWS_T, cp1, ClauseParam(2));
    THEN("Only c1 and c2 are equal") {
      REQUIRE(c1 == c2);
      REQUIRE_FALSE(c1 == c3);
      REQUIRE_FALSE(c1 == c4);
    }
  }
}
SCENARIO("Test SuchThatClause Validation") {
  GIVEN("Follows or FollowsT Clause") {
    DesignAbstraction valid_follow_abstraction = DesignAbstraction::FOLLOWS;
    WHEN("Valid Follows(stmt design entity, index)") {
      DesignEntityType valid_entity_type = DesignEntityType::STMT;
      DesignEntity valid_design_entity = DesignEntity(valid_entity_type, "s1");
      ClauseParam valid_param_design_entity = ClauseParam(valid_design_entity);
      ClauseParam valid_param_int = ClauseParam(10);
      SuchThatClause test_valid_follow_clause = SuchThatClause(valid_follow_abstraction, valid_param_design_entity, valid_param_int);
      THEN("Follows(s1, 10) should return true") {
        REQUIRE(test_valid_follow_clause.IsValidClause() == true);
      }
    }

    WHEN("Invalid Follows(index, name)") {
      ClauseParam valid_param_int = ClauseParam(10);
      ClauseParam invalid_param_variable = ClauseParam("x");
      SuchThatClause test_invalid_follow_clause = SuchThatClause(valid_follow_abstraction, valid_param_int, invalid_param_variable);
      THEN("Follows(10, \"x\") throws error") {
        REQUIRE_THROWS(test_invalid_follow_clause.IsValidClause());
      }
    }

    WHEN("Invalid Follows(var design entity, stmt design entity)") {
      DesignEntityType invalid_entity_type = DesignEntityType::VARIABLE;
      DesignEntity invalid_design_entity = DesignEntity(invalid_entity_type, "v");
      ClauseParam invalid_param_design_entity = ClauseParam(invalid_design_entity);
      DesignEntityType valid_entity_type = DesignEntityType::STMT;
      DesignEntity valid_design_entity = DesignEntity(valid_entity_type, "s1");
      ClauseParam valid_param_design_entity = ClauseParam(valid_design_entity);
      SuchThatClause test_invalid_follow_clause = SuchThatClause(valid_follow_abstraction, invalid_param_design_entity, valid_param_design_entity);
      THEN("Follows(v, s1) throws error") {
        REQUIRE_THROWS(test_invalid_follow_clause.IsValidClause());
      }
    }
  }

  GIVEN("Uses Clause") {
    WHEN("Valid Uses(index, variable)") {
      SuchThatClause test_valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                             ClauseParam(1),
                                                             ClauseParam("varX"));
      THEN("Uses(1, \"varX\") should return true") {
        REQUIRE(test_valid_uses_clause.IsValidClause());
      }
    }

    WHEN("Valid Uses(stmt design entity, variable)") {
      SuchThatClause test_valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                             ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                             ClauseParam("varX"));
      THEN("Uses(w, \"varX\") should return true") {
        REQUIRE(test_valid_uses_clause.IsValidClause());
      }
    }

    WHEN("Valid Uses(index, variable entity") {
      SuchThatClause test_valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                             ClauseParam(1),
                                                             ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      THEN("Uses(w, \"varX\") should return true") {
        REQUIRE(test_valid_uses_clause.IsValidClause());
      }
    }

    WHEN("Valid Uses(stmt design entity, wildcard") {
      SuchThatClause test_valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                             ClauseParam(DesignEntity(DesignEntityType::PRINT, "pn")),
                                                             ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("Uses(pn, _) should return true") {
        REQUIRE(test_valid_uses_clause.IsValidClause());
      }
    }

    WHEN("Valid Uses(procedure, variable") {
      SuchThatClause test_valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                             ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                             ClauseParam("x"));
      THEN("Uses(p, \"x\") should return true") {
        REQUIRE(test_valid_uses_clause.IsValidClause());
      }
    }

    WHEN("Valid Uses(name, variable") {
      SuchThatClause test_valid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                             ClauseParam("test"),
                                                             ClauseParam("hi"));
      THEN("Uses(\"test\", \"hi\") should return true") {
        REQUIRE(test_valid_uses_clause.IsValidClause());
      }
    }

    WHEN("Invalid Uses(variable design entity, variable") {
      SuchThatClause test_invalid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                               ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                               ClauseParam("hi"));
      THEN("Uses(v, \"hi\") should throw error") {
        REQUIRE_THROWS(test_invalid_uses_clause.IsValidClause());
      }
    }

    WHEN("Invalid Uses(index, index)") {
      SuchThatClause test_invalid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                               ClauseParam(1),
                                                               ClauseParam(2));
      THEN("Uses(1, 2) should throw error") {
        REQUIRE_THROWS(test_invalid_uses_clause.IsValidClause());
      }
    }

    WHEN("Invalid Uses(_, variable)") {
      SuchThatClause test_invalid_uses_clause = SuchThatClause(DesignAbstraction::USES,
                                                               ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                               ClauseParam("var"));
      THEN("Uses(_, \"var\") should throw error") {
        REQUIRE_THROWS(test_invalid_uses_clause.IsValidClause());
      }
    }
  }

  GIVEN("Modifies Clause") {
    WHEN("Invalid Modifies(_, variable)") {
      SuchThatClause test_invalid_modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                                   ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                                                   ClauseParam("var"));
      THEN("Modifies(_, \"var\") should throw error") {
        REQUIRE_THROWS(test_invalid_modifies_clause.IsValidClause());
      }
    }
  }

  GIVEN("Calls Clause") {
    WHEN("Valid Calls(name, name)") {
      SuchThatClause test_valid_calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                              ClauseParam("main"),
                                                              ClauseParam("square"));
      THEN("Calls(\"main\", \"square\") returns true") {
        REQUIRE(test_valid_calls_clause.IsValidClause());
      }
    }

    WHEN("Valid Calls(name, procedure)") {
      SuchThatClause test_valid_calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                              ClauseParam("main"),
                                                              ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")));
      THEN("Calls(\"main\", p) returns true") {
        REQUIRE(test_valid_calls_clause.IsValidClause());
      }
    }

    WHEN("Valid Calls(procedure, wildcard)") {
      SuchThatClause test_valid_calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                              ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                                              ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("Calls(p, _) returns true") {
        REQUIRE(test_valid_calls_clause.IsValidClause());
      }
    }

    WHEN("Invalid Calls(index, name)") {
      SuchThatClause test_invalid_calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                                ClauseParam(1),
                                                                ClauseParam("square"));
      THEN("Calls(1, \"square\") throws error") {
        REQUIRE_THROWS(test_invalid_calls_clause.IsValidClause());
      }
    }

    WHEN("Invalid Calls(call stmt, name)") {
      SuchThatClause test_invalid_calls_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                                ClauseParam(DesignEntity(DesignEntityType::CALL, "c")),
                                                                ClauseParam("square"));
      THEN("Calls(c, \"square\") throws error") {
        REQUIRE_THROWS(test_invalid_calls_clause.IsValidClause());
      }
    }
  }

  GIVEN("Next clause") {
    WHEN("Valid Next(int ,int) returns true") {
      SuchThatClause test_valid_next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                             ClauseParam(1),
                                                             ClauseParam(2));
      THEN("Next(1, 2) returns true") {
        REQUIRE(test_valid_next_clause.IsValidClause());
      }
    }
    WHEN("Valid Next(int, progline) returns true") {
      SuchThatClause test_valid_next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                             ClauseParam(1),
                                                             ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      THEN("Next(1, n) returns true") {
        REQUIRE(test_valid_next_clause.IsValidClause());
      }
    }

    WHEN("Valid Next(int , _) returns true") {
      SuchThatClause test_valid_next_clause = SuchThatClause(DesignAbstraction::NEXT,
                                                             ClauseParam(1),
                                                             ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("Next(1, n) returns true") {
        REQUIRE(test_valid_next_clause.IsValidClause());
      }
    }

    WHEN("Invalid Next(int, name)") {
      SuchThatClause test_invalid_next_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                               ClauseParam(1),
                                                               ClauseParam("square"));
      THEN("Next(1, \"square\") throws error") {
        REQUIRE_THROWS(test_invalid_next_clause.IsValidClause());
      }
    }

    WHEN("Invalid Next(int, variable)") {
      SuchThatClause test_invalid_next_clause = SuchThatClause(DesignAbstraction::CALLS,
                                                               ClauseParam(1),
                                                               ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
      THEN("Next(1, v) throws error") {
        REQUIRE_THROWS(test_invalid_next_clause.IsValidClause());
      }
    }
  }

  GIVEN("Affects clause") {
    WHEN("Valid Affects(int, int) returns true") {
      SuchThatClause test_valid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                ClauseParam(1),
                                                                ClauseParam(2));
      THEN("Affects(1, 2) returns true") {
        REQUIRE(test_valid_affects_clause.IsValidClause());
      }
    }

    WHEN("Valid Affects(int ,assign) returns true") {
      SuchThatClause test_valid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                ClauseParam(1),
                                                                ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
      THEN("Affects(1, a) returns true") {
        REQUIRE(test_valid_affects_clause.IsValidClause());
      }
    }

    WHEN("Valid Affects(stmt, int) returns true") {
      SuchThatClause test_valid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                                ClauseParam(2));
      THEN("Affects(s, 2) returns true") {
        REQUIRE(test_valid_affects_clause.IsValidClause());
      }
    }

    WHEN("Valid Affects(stmt, prog_line) returns true") {
      SuchThatClause test_valid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                                ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")));
      THEN("Affects(s, n) returns true") {
        REQUIRE(test_valid_affects_clause.IsValidClause());
      }
    }

    WHEN("Valid Affects(assign, _) returns true") {
      SuchThatClause test_valid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                                                ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")));
      THEN("Affects(a, _) returns true") {
        REQUIRE(test_valid_affects_clause.IsValidClause());
      }
    }

    WHEN("Invalid Affects(int, name) throws error") {
      SuchThatClause test_invalid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                  ClauseParam(1),
                                                                  ClauseParam("X"));
      THEN("Affects(1, X) throws error") {
        REQUIRE_THROWS(test_invalid_affects_clause.IsValidClause());
      }
    }

    WHEN("Invalid Affects(int, while) throws error") {
      SuchThatClause test_invalid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                  ClauseParam(1),
                                                                  ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")));
      THEN("Affects(1, w) throws error") {
        REQUIRE_THROWS(test_invalid_affects_clause.IsValidClause());
      }
    }

    WHEN("Invalid Affects(var, int) throws error") {
      SuchThatClause test_invalid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                  ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                                  ClauseParam(1));
      THEN("Affects(v, 1) throws error") {
        REQUIRE_THROWS(test_invalid_affects_clause.IsValidClause());
      }
    }

    WHEN("Invalid Affects(int, constant) throws error") {
      SuchThatClause test_invalid_affects_clause = SuchThatClause(DesignAbstraction::AFFECTS,
                                                                  ClauseParam(1),
                                                                  ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")));
      THEN("Affects(1, c) throws error") {
        REQUIRE_THROWS(test_invalid_affects_clause.IsValidClause());
      }
    }
  }
}
