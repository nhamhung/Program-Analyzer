#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/UsesTable.h"

SCENARIO("Construct an empty UsesTable.") {
  UsesTable uses_table;

  GIVEN("Initial uses_table is empty.") {
    THEN("uses_table exists with size 0.") {
      REQUIRE(uses_table.GetUsesStmtTable().Size() == 0);
      REQUIRE(uses_table.GetInverseUsesStmtTable().Size() == 0);
    }
  }
}

SCENARIO("UsesTable has been constructed.") {
  UsesTable uses_table;

  GIVEN("Insert a Uses(stmt_index, variable) relationship into uses_table.") {
    REQUIRE(uses_table.InsertStmtUses(6, "flag"));
    REQUIRE(uses_table.InsertStmtUses(7, "cenX"));
    REQUIRE(uses_table.InsertStmtUses(8, "cenY"));
    REQUIRE(uses_table.InsertStmtUses(9, "normSq"));
    REQUIRE(uses_table.InsertStmtUses(14, "x"));
    REQUIRE(uses_table.InsertStmtUses(14, "y"));
    REQUIRE(uses_table.InsertStmtUses(15, "count"));
    REQUIRE(uses_table.InsertStmtUses(16, "cenX"));
    REQUIRE(uses_table.InsertStmtUses(16, "x"));
    REQUIRE(uses_table.InsertStmtUses(17, "cenY"));
    REQUIRE(uses_table.InsertStmtUses(17, "y"));
    REQUIRE(uses_table.InsertStmtUses(19, "count"));
    REQUIRE(uses_table.InsertStmtUses(21, "cenX"));
    REQUIRE(uses_table.InsertStmtUses(21, "count"));
    REQUIRE(uses_table.InsertStmtUses(22, "cenY"));
    REQUIRE(uses_table.InsertStmtUses(22, "count"));
    REQUIRE(uses_table.InsertStmtUses(23, "cenX"));
    REQUIRE(uses_table.InsertStmtUses(23, "cenY"));

    WHEN("Valid stmt_index and variable.") {
      THEN("Insertion returns True. Relationship is stored in pkb.") {
        REQUIRE(uses_table.GetUsesStmtTable().Size() == 12);
        REQUIRE(uses_table.GetAllUsesStatements().size() == 12);
      }
    }

    WHEN("Invalid stmt_index <= 0.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(uses_table.InsertStmtUses(0, "x"));
        REQUIRE_FALSE(uses_table.InsertStmtUses(-1, "y"));
      }
    }

    WHEN("Invalid variable such as empty string ''") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(uses_table.InsertStmtUses(1, ""));
      }
    }

    WHEN("Both stmt_index and variable already stored in uses_table.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(uses_table.InsertStmtUses(6, "flag"));
        REQUIRE_FALSE(uses_table.InsertStmtUses(7, "cenX"));
      }
    }

    WHEN("IsStmtUses(stmt_index, variable) is called.") {
      THEN("Returns True is relationship exists in uses_table. False otherwise.") {
        REQUIRE(uses_table.IsStmtUses(6, "flag"));
        REQUIRE_FALSE(uses_table.IsStmtUses(6, "cenX"));
        REQUIRE(uses_table.IsStmtUses(14, "x"));
        REQUIRE(uses_table.IsStmtUses(14, "y"));
      }
    }

    WHEN("GetUsesStatements(variable) called.") {
      THEN("Returns a set of statements that uses a variable. If no such statement then return empty vector.") {
        std::unordered_set<int> statements_that_uses_variable_count = uses_table.GetUsesStatements("count");
        REQUIRE(statements_that_uses_variable_count.size() == 4);
        REQUIRE(ContainsExactly(statements_that_uses_variable_count, {15, 19, 21, 22}));
        REQUIRE(uses_table.GetUsesStatements("unknown_variable").empty());
      }
    }

    WHEN("GetUsedStmtVariables(stmt_index) called.") {
      THEN("Returns a set of variables that a statement uses. If a statement does not use any then return empty vector.") {
        std::unordered_set<std::string> used_variables_statement_14 = uses_table.GetUsedStmtVariables(14);
        REQUIRE(used_variables_statement_14.size() == 2);
        REQUIRE(ContainsExactly(used_variables_statement_14, {"x", "y"}));
        REQUIRE(uses_table.GetUsedStmtVariables(2).empty());
      }
    }

    WHEN("GetAllUsesStatements() called.") {
      THEN("Returns a set of statement indexes that Modifies(stmt_index, variable) holds.") {
        std::unordered_set<int> all_uses_statements = uses_table.GetAllUsesStatements();
        REQUIRE(all_uses_statements.size() == 12);
      }
    }
  }
}