#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/templates/TableMultiple.h"

SCENARIO("Construct an empty table.") {
  GIVEN("A template class TableMultiple construction.") {
    TableMultiple<int, std::string> table;
    THEN("TableMultiple exists with size 0.") {
      REQUIRE(table.TableExists());
      REQUIRE(table.IsEmpty());
    }
  }
}

SCENARIO("TableMultiple has been constructed.") {
  TableMultiple<int, std::string> table;

  GIVEN("TableMultiple is currently empty.") {
    WHEN("Insert a statement index and a variable.") {
      THEN("Insertion returns True. TableMultiple is of size 1.") {
        REQUIRE(table.Insert(1, "x"));
        REQUIRE(table.Size() == 1);
      }
    }
  }

  GIVEN("TableMultiple is not empty.") {
    REQUIRE(table.Insert(1, "x"));
    REQUIRE(table.Insert(2, "y"));

    WHEN("Insert the same statement index multiple times with different variables.") {
      THEN("Insertion returns True. List of variables are stored at statement index.") {
        REQUIRE(table.Insert(1, "y"));
        std::unordered_set<std::string> values_at_stmt_1 = table.Get(1);
        REQUIRE(table.Size() == 2);
        REQUIRE(Contains(values_at_stmt_1, "x"));
        REQUIRE(Contains(values_at_stmt_1, "y"));
        REQUIRE(table.Get(1).size() == 2);
        REQUIRE(table.Contains(1));
      }
    }

    WHEN("Insert the same statement index multiple times with the same variable.") {
      THEN("Insertion returns False. List of variables at statement index remain unchanged.") {
        REQUIRE(table.Insert(1, "x") == false);
        REQUIRE(table.Get(1).size() == 1);
        REQUIRE(table.GetAllKeys().size() == 2);
      }
    }

    WHEN("Multiple insertions to the same set of statement indexes.") {
      REQUIRE(table.Insert(1, "z"));
      REQUIRE_FALSE(table.Insert(1, "x"));
      REQUIRE(table.Insert(2, "x"));
      REQUIRE_FALSE(table.Insert(2, "y"));
      THEN("table.GetAllStmts() returns a set of unique statement indexes.") {
        std::unordered_set<int> all_stmt_indexes = table.GetAllKeys();
        REQUIRE(all_stmt_indexes.size() == 2);
        REQUIRE(all_stmt_indexes.find(1) != all_stmt_indexes.end());
        REQUIRE(all_stmt_indexes.find(2) != all_stmt_indexes.end());
      }
    }
  }
}