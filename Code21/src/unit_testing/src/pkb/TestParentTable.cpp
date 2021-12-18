#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/ParentTable.h"

SCENARIO("Construct an empty ParentTable.") {
  ParentTable parent_table;

  GIVEN("Initial parent_table is empty.") {
    THEN("parent_table exists with size 0.") {
      REQUIRE(parent_table.GetParentTable().IsEmpty());
      REQUIRE(parent_table.GetInverseParentTable().IsEmpty());
    }
  }
}

SCENARIO("ParentTable has been constructed.") {
  ParentTable parent_table;

  GIVEN("ParentTable is currently empty.") {
    WHEN("Insert a valid Parent(stmt1, stmt2) relationship.") {
      THEN("Insertion returns True. Both parent_table and inverse_parent_table are filled.") {
        REQUIRE(parent_table.InsertParent(5, 7));
        REQUIRE(parent_table.InsertParent(5, 8));
        REQUIRE(parent_table.InsertParent(10, 11));
        REQUIRE(parent_table.InsertParent(10, 13));
        TableMultiple<int, int> parent = parent_table.GetParentTable();
        TableSingle<int, int> inverse_parent = parent_table.GetInverseParentTable();
        REQUIRE(parent.Size() == 2);
        REQUIRE(parent.Get(5).size() == 2);
        REQUIRE(Contains(parent.Get(5), 7));
        REQUIRE(Contains(parent.Get(5), 8));
        REQUIRE(parent.Get(10).size() == 2);
        REQUIRE(Contains(parent.Get(10), 11));
        REQUIRE(Contains(parent.Get(10), 13));
        REQUIRE(inverse_parent.Size() == 4);
      }
    }

    WHEN("Insert a Parent(stmt1, stmt2) such that stmt1 >= stmt2 or stmt1 <= 0.") {
      THEN("Insertion returns False. stmt1 must be strictly less than stmt2 for Parent(stmt1, stmt2).") {
        REQUIRE(parent_table.InsertParent(2, 2) == false);
        REQUIRE(parent_table.InsertParent(2, 1) == false);
        REQUIRE(parent_table.InsertParent(0, 1) == false);
      }
    }

    WHEN("Insert a Parent(stmt1, stmt2) such that stmt2 already exists in inverse_parent_table.") {
      THEN("Insertion returns False. stmt2 only has 1 unique parent.") {
        REQUIRE(parent_table.InsertParent(5, 7));
        REQUIRE(parent_table.InsertParent(3, 7) == false);
        REQUIRE(parent_table.InsertParent(5, 7) == false);
        REQUIRE(parent_table.InsertParent(5, 6));
      }
    }
  }

  GIVEN("ParentTable already contains entries.") {
    REQUIRE(parent_table.InsertParent(5, 7));
    REQUIRE(parent_table.InsertParent(5, 8));
    REQUIRE(parent_table.InsertParent(10, 11));
    REQUIRE(parent_table.InsertParent(10, 13));

    WHEN("Check if IsParent(stmt1, stmt2) holds. Otherwise returns False.") {
      THEN("Returns True if Parent(stmt1, stmt2) holds. Otherwise returns False.") {
        REQUIRE(parent_table.IsParent(5, 7));
        REQUIRE(parent_table.IsParent(5, 8));
        REQUIRE(parent_table.IsParent(10, 11));
        REQUIRE(parent_table.IsParent(10, 13));
        REQUIRE(parent_table.IsParent(7, 5) == false);
      }
    }

    WHEN("Check if inverse_parent_table stores the inverse relationship.") {
      THEN("inverse_parent_table should be filled up the same moment InsertParent(stmt1, stmt2).") {
        REQUIRE(parent_table.GetInverseParentTable().Size() == 4);
        TableSingle<int, int> inverse_parent = parent_table.GetInverseParentTable();
        REQUIRE(inverse_parent.Contains(7));
        REQUIRE(inverse_parent.Contains(8));
        REQUIRE(inverse_parent.Contains(11));
        REQUIRE(inverse_parent.Contains(13));
        REQUIRE(inverse_parent.Get(7) == 5);
        REQUIRE(inverse_parent.Get(8) == 5);
        REQUIRE(inverse_parent.Get(11) == 10);
        REQUIRE(inverse_parent.Get(13) == 10);
      }
    }

    WHEN("GetChildrenStatements(stmt1) is called.") {
      THEN("Return a set of children statements of stmt1. Return empty vector if does not exists.") {
        REQUIRE(parent_table.GetChildrenStatements(5).size() == 2);
        REQUIRE(parent_table.GetChildrenStatements(10).size() == 2);
        REQUIRE(parent_table.GetChildrenStatements(1).empty());
      }
    }

    WHEN("GetParentStatement(stmt2) is called.") {
      THEN("Return the parent statement of stmt2. Return -1 if there is no parent statement.") {
        REQUIRE(parent_table.GetParentStatement(7) == 5);
        REQUIRE(parent_table.GetParentStatement(8) == 5);
        REQUIRE(parent_table.GetParentStatement(11) == 10);
        REQUIRE(parent_table.GetParentStatement(13) == 10);
        REQUIRE(parent_table.GetParentStatement(9) == -1);
      }
    }

    WHEN("GetAllParentStmts() is called.") {
      THEN("Return a set of all parent statements currently stored.") {
        REQUIRE(parent_table.GetAllParentStmts().size() == 2);
        REQUIRE(ContainsExactly(parent_table.GetAllParentStmts(), {5, 10}));
      }
    }

    WHEN("GetAllChildrenStmts() is called.") {
      THEN("Return a set of all children statements currently stored.") {
        REQUIRE(parent_table.GetAllChildrenStmts().size() == 4);
        REQUIRE(ContainsExactly(parent_table.GetAllChildrenStmts(), {7, 8, 11, 13}));
      }
    }
  }
}
