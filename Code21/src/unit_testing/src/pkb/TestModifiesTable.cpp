#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/ModifiesTable.h"

SCENARIO("Construct an empty ModifiesTable.") {
  ModifiesTable modifies_table;

  GIVEN("Initial modifies_table is empty.") {
    THEN("modifies_table exists with size 0.") {
      REQUIRE(modifies_table.GetModifiesStmtTable().Size() == 0);
      REQUIRE(modifies_table.GetInverseModifiesStmtTable().Size() == 0);
    }
  }
}

SCENARIO("ModifiesTable has been constructed.") {
  ModifiesTable modifies_table;

  GIVEN("Insert a Modifies(stmt_index, variable) relationship into modifies_table.") {
    REQUIRE(modifies_table.InsertStmtModifies(1, "flag"));
    REQUIRE(modifies_table.InsertStmtModifies(4, "x"));
    REQUIRE(modifies_table.InsertStmtModifies(5, "y"));
    REQUIRE(modifies_table.InsertStmtModifies(10, "count"));
    REQUIRE(modifies_table.InsertStmtModifies(11, "cenX"));
    REQUIRE(modifies_table.InsertStmtModifies(12, "cenY"));
    REQUIRE(modifies_table.InsertStmtModifies(14, "count"));
    REQUIRE(modifies_table.InsertStmtModifies(14, "cenX"));
    REQUIRE(modifies_table.InsertStmtModifies(14, "cenY"));
    REQUIRE(modifies_table.InsertStmtModifies(15, "count"));
    REQUIRE(modifies_table.InsertStmtModifies(16, "cenX"));
    REQUIRE(modifies_table.InsertStmtModifies(17, "cenY"));
    REQUIRE(modifies_table.InsertStmtModifies(19, "flag"));
    REQUIRE(modifies_table.InsertStmtModifies(19, "cenX"));
    REQUIRE(modifies_table.InsertStmtModifies(19, "cenY"));
    REQUIRE(modifies_table.InsertStmtModifies(20, "flag"));
    REQUIRE(modifies_table.InsertStmtModifies(21, "cenX"));
    REQUIRE(modifies_table.InsertStmtModifies(22, "cenY"));
    REQUIRE(modifies_table.InsertStmtModifies(23, "normSq"));

    WHEN("Valid stmt_index and variable.") {
      THEN("Insertion returns True. Relationship is stored in pkb.") {
        REQUIRE(modifies_table.GetModifiesStmtTable().Size() == 15);
        REQUIRE(modifies_table.GetAllModifiesStatements().size() == 15);
      }
    }

    WHEN("Invalid stmt_index <= 0.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(modifies_table.InsertStmtModifies(0, "x"));
        REQUIRE_FALSE(modifies_table.InsertStmtModifies(-1, "y"));
      }
    }

    WHEN("Invalid variable such as empty string ''") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(modifies_table.InsertStmtModifies(1, ""));
      }
    }

    WHEN("Both stmt_index and variable already stored in modifies_table.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(modifies_table.InsertStmtModifies(1, "flag"));
        REQUIRE_FALSE(modifies_table.InsertStmtModifies(4, "x"));
      }
    }

    WHEN("IsModifies(stmt_index, variable) is called.") {
      THEN("Returns True is relationship exists in modifies_table. False otherwise.") {
        REQUIRE(modifies_table.IsStmtModifies(1, "flag"));
        REQUIRE_FALSE(modifies_table.IsStmtModifies(1, "x"));
        REQUIRE(modifies_table.IsStmtModifies(14, "count"));
        REQUIRE(modifies_table.IsStmtModifies(14, "cenX"));
        REQUIRE(modifies_table.IsStmtModifies(14, "cenY"));
      }
    }

    WHEN("GetModifiesStatements(variable) called.") {
      THEN("Returns a set of statements that modifies a variable. If no such statement then return empty vector.") {
        std::unordered_set<int> statements_that_modifies_variable_count = modifies_table.GetModifiesStatements("count");
        REQUIRE(statements_that_modifies_variable_count.size() == 3);
        REQUIRE(ContainsExactly(statements_that_modifies_variable_count, {10, 14, 15}));
        REQUIRE(modifies_table.GetModifiesStatements("unknown_variable").empty());
      }
    }

    WHEN("GetModifiedVariables(stmt_index) called.") {
      THEN("Returns a set of variables that a statement modifies. If a statement does not modify any then return empty vector.") {
        std::unordered_set<std::string> modified_variables_statement_14 = modifies_table.GetModifiedStmtVariables(14);
        REQUIRE(modified_variables_statement_14.size() == 3);
        REQUIRE(ContainsExactly(modified_variables_statement_14, {"count", "cenX", "cenY"}));
        REQUIRE(modifies_table.GetModifiedStmtVariables(2).empty());
      }
    }

    WHEN("GetAllModifiesStatements() called.") {
      THEN("Returns a set of statement indexes that Modifies(stmt_index, variable) holds.") {
        std::unordered_set<int> all_modifies_statements = modifies_table.GetAllModifiesStatements();
        REQUIRE(all_modifies_statements.size() == 15);
      }
    }
  }
}