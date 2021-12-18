#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/entity_tables/ProcTable.h"

SCENARIO("Construct a new proc_table.") {
  GIVEN("A proc_table has just been constructed.") {
    ProcTable proc_table;

    THEN("proc_table exists with size 0.") {
      REQUIRE(proc_table.GetProcTable().IsEmpty());
    }
  }

  GIVEN("A proc_table has been constructed to insert entries.") {
    ProcTable proc_table;

    WHEN("Inserts to proc_table with different proc names and non-overlapping index ranges.") {
      THEN("Insertion returns true. Insertions are successful.") {
        REQUIRE(proc_table.InsertProc("Tokenize", std::pair<int, int>(1, 5)));
        REQUIRE(proc_table.InsertProc("Parse", std::pair<int, int>(6, 10)));
      }
    }

    WHEN("Inserts to proc_table with the same proc name and non-overlapping index range.") {
      THEN("Insertion returns false. Only the first insertion is executed.") {
        REQUIRE(proc_table.InsertProc("Tokenize", std::pair<int, int>(1, 5)));
        REQUIRE_FALSE(proc_table.InsertProc("Tokenize", std::pair<int, int>(6, 10)));
      }
    }

    WHEN("Inserts to proc_table with different proc names and overlapping index range.") {
      THEN("Insertion returns false for second insertion due to overlap.") {
        REQUIRE(proc_table.InsertProc("Tokenize", std::pair<int, int>(1, 5)));
        REQUIRE_FALSE(proc_table.InsertProc("Parse", std::pair<int, int>(5, 10)));
        REQUIRE(proc_table.InsertProc("Test", std::pair<int, int>(6, 10)));
      }
    }

    WHEN("Gets all procedures from proc_table.") {
      THEN("proc_table returns a unique set of proc_name.") {
        REQUIRE(proc_table.InsertProc("Tokenize", std::pair<int, int>(1, 5)));
        REQUIRE_FALSE(proc_table.InsertProc("Parse", std::pair<int, int>(5, 10)));
        REQUIRE(proc_table.InsertProc("Test", std::pair<int, int>(6, 10)));
        std::unordered_set<std::string> all_procedures = proc_table.GetAllProcedures();
        REQUIRE(all_procedures.size() == 2);
        REQUIRE(ContainsExactly(all_procedures, {"Tokenize", "Test"}));
        REQUIRE_FALSE(Contains(all_procedures, "Parse"));
      }
    }
  }
}