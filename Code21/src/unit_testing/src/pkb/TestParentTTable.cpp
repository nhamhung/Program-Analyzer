#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/ParentTTable.h"

SCENARIO("Construct an empty ParentTTable.") {
  ParentTTable parent_T_table;

  GIVEN("Initial parent_table is empty.") {
    THEN("parent_table exists with size 0.") {
      REQUIRE(parent_T_table.GetParentTTable().Size() == 0);
      REQUIRE(parent_T_table.GetInverseParentTTable().Size() == 0);
    }
  }
}

SCENARIO("ParentTTable has been constructed.") {
  ParentTTable parent_T_table;

  GIVEN("ParentTTable is currently empty.") {
    WHEN("Insert a valid ParentT(stmt1, stmt2) relationship.") {
      THEN("Insertion returns True. Both parent_table and inverse_parent_table are filled.") {
        REQUIRE(parent_T_table.InsertParentT(5, 6));
        REQUIRE(parent_T_table.InsertParentT(5, 7));
        REQUIRE(parent_T_table.InsertParentT(5, 11));
        REQUIRE(parent_T_table.InsertParentT(5, 12));
        REQUIRE(parent_T_table.InsertParentT(10, 11));
        REQUIRE(parent_T_table.InsertParentT(10, 12));
        REQUIRE(parent_T_table.InsertParentT(10, 13));
        TableMultiple<int, int> parent_T = parent_T_table.GetParentTTable();
        TableMultiple<int, int> inverse_parent_T = parent_T_table.GetInverseParentTTable();
        REQUIRE(parent_T.Size() == 2);
        REQUIRE(parent_T.Get(5).size() == 4);
        REQUIRE(ContainsExactly(parent_T.Get(5), {6, 7, 11, 12}));
        REQUIRE(parent_T.Get(10).size() == 3);
        REQUIRE(ContainsExactly(parent_T.Get(10), {11, 12, 13}));
        REQUIRE(inverse_parent_T.Size() == 5);
        REQUIRE(inverse_parent_T.Get(11).size() == 2);
        REQUIRE(ContainsExactly(inverse_parent_T.Get(11), {5, 10}));
      }
    }

    WHEN("Insert an invalid ParentT(stmt1, stmt2) such that stmt1 >= stmt2 or stmt1 <= 0.") {
      THEN("Insertion returns False. stmt1 must be strictly less than stmt2 for ParentT(stmt1, stmt2).") {
        REQUIRE(parent_T_table.InsertParentT(2, 2) == false);
        REQUIRE(parent_T_table.InsertParentT(2, 1) == false);
        REQUIRE(parent_T_table.InsertParentT(0, 1) == false);
      }
    }

    WHEN("Insert an invalid ParentT(stmt1, stmt2) such that ParentT(stmt1, stmt2) already exists.") {
      THEN("Insertion returns False. The pair (stmt1, stmt2) must be unique.") {
        REQUIRE(parent_T_table.InsertParentT(1, 3));
        REQUIRE(parent_T_table.InsertParentT(1, 3) == false);
        REQUIRE(parent_T_table.GetChildrenTStatements(1).size() == 1);
      }
    }
  }

  GIVEN("ParentTTable already contains entries.") {
    REQUIRE(parent_T_table.InsertParentT(5, 6));
    REQUIRE(parent_T_table.InsertParentT(5, 7));
    REQUIRE(parent_T_table.InsertParentT(5, 11));
    REQUIRE(parent_T_table.InsertParentT(5, 12));
    REQUIRE(parent_T_table.InsertParentT(10, 11));
    REQUIRE(parent_T_table.InsertParentT(10, 12));
    REQUIRE(parent_T_table.InsertParentT(10, 13));

    WHEN("Check if IsParentT(stmt1, stmt2) holds. Otherwise returns False.") {
      THEN("Returns True if ParentT(stmt1, stmt2) holds. Otherwise returns False.") {
        REQUIRE(parent_T_table.IsParentT(5, 6));
        REQUIRE(parent_T_table.IsParentT(5, 7));
        REQUIRE(parent_T_table.IsParentT(5, 11));
        REQUIRE(parent_T_table.IsParentT(5, 12));
        REQUIRE(parent_T_table.IsParentT(10, 11));
        REQUIRE(parent_T_table.IsParentT(10, 12));
        REQUIRE(parent_T_table.IsParentT(10, 13));
        REQUIRE(parent_T_table.IsParentT(11, 10) == false);
      }
    }

    WHEN("Check if inverse_parent_T_table stores the inverse relationship.") {
      THEN("inverse_parent_T_table should be filled up the same moment InsertParentT(stmt1, stmt2).") {
        TableMultiple<int, int> inverse_parent_T = parent_T_table.GetInverseParentTTable();
        REQUIRE(inverse_parent_T.Size() == 5);
        REQUIRE(Contains(inverse_parent_T.Get(6), 5));
        REQUIRE(Contains(inverse_parent_T.Get(7), 5));
        REQUIRE(Contains(inverse_parent_T.Get(11), 5));
        REQUIRE(Contains(inverse_parent_T.Get(11), 10));
        REQUIRE(Contains(inverse_parent_T.Get(12), 5));
        REQUIRE(Contains(inverse_parent_T.Get(12), 10));
        REQUIRE(Contains(inverse_parent_T.Get(13), 10));
      }
    }

    WHEN("GetChildrenTStatements(stmt1) is called.") {
      THEN("Return a set of children T statements of stmt1. Return empty vector if does not exist.") {
        REQUIRE(ContainsExactly(parent_T_table.GetChildrenTStatements(5), {6, 7, 11, 12}));
        REQUIRE(ContainsExactly(parent_T_table.GetChildrenTStatements(10), {11, 12, 13}));
        REQUIRE(parent_T_table.GetChildrenTStatements(1).empty());
      }
    }

    WHEN("GetParentTStatements(stmt2) is called.") {
      THEN("Return a set of parentT statements of stmt2. Return empty vector if does not exist.") {
        REQUIRE(parent_T_table.GetParentTStatements(11).size() == 2);
        REQUIRE(ContainsExactly(parent_T_table.GetParentTStatements(11), {5, 10}));
        REQUIRE(parent_T_table.GetParentTStatements(12).size() == 2);
        REQUIRE(ContainsExactly(parent_T_table.GetParentTStatements(12), {5, 10}));
        REQUIRE(parent_T_table.GetParentTStatements(1).empty());
      }
    }

    WHEN("GetAllParentTStmts() is called.") {
      THEN("Return a set of all parentT statements currently stored.") {
        REQUIRE(parent_T_table.GetAllParentTStmts().size() == 2);
        REQUIRE(ContainsExactly(parent_T_table.GetAllParentTStmts(), {5, 10}));
      }
    }

    WHEN("GetAllChildrenTStmts() is called.") {
      THEN("Return a set of all childrenT statements currently stored.") {
        std::unordered_set<int> all_children_T_stmts = parent_T_table.GetAllChildrenTStmts();
        REQUIRE(all_children_T_stmts.size() == 5);
        REQUIRE(ContainsExactly(all_children_T_stmts, {6, 7, 11, 12, 13}));
      }
    }
  }
}
