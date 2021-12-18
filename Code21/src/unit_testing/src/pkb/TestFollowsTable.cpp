#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/FollowsTable.h"

SCENARIO("Construct an empty Follows table.") {
  FollowsTable follows_table;

  GIVEN("Initial Follows table is empty.") {
    THEN("TableSingle exists with size 0.") {
      REQUIRE(follows_table.GetFollowsTable().IsEmpty());
      REQUIRE(follows_table.GetInverseFollowsTable().IsEmpty());
    }
  }
}

SCENARIO("FollowsTable has been constructed.") {
  FollowsTable follows_table;

  GIVEN("FollowsTable is currently empty.") {
    WHEN("Insert a Follows(stmt1, stmt2) relationship.") {
      THEN("Insertion returns True. Both follows and inverse_follows are filled.") {
        REQUIRE(follows_table.InsertFollows(1, 2));
        REQUIRE(follows_table.InsertFollows(4, 5));
        REQUIRE(follows_table.InsertFollows(5, 10));
        TableSingle<int, int> follows = follows_table.GetFollowsTable();
        TableSingle<int, int> inverse_follows = follows_table.GetInverseFollowsTable();
        REQUIRE(follows.Size() == 3);
        REQUIRE(inverse_follows.Size() == 3);
        REQUIRE(follows.Get(1) == 2);
        REQUIRE(inverse_follows.Get(2) == 1);
        REQUIRE(follows.Get(4) == 5);
        REQUIRE(inverse_follows.Get(5) == 4);
        REQUIRE(follows.Get(5) == 10);
        REQUIRE(inverse_follows.Get(10) == 5);
      }
    }
  }

  GIVEN("FollowsTable is not empty.") {
    REQUIRE(follows_table.InsertFollows(1, 2));
    REQUIRE(follows_table.InsertFollows(4, 5));
    REQUIRE(follows_table.InsertFollows(5, 10));

    WHEN("Insert an existing stmt1 index.") {
      THEN("Insertion returns False. Each stmt1 can only have one stmt2 such that Follows(stmt1, stmt2).") {
        REQUIRE_FALSE(follows_table.InsertFollows(1, 3));
        REQUIRE_FALSE(follows_table.InsertFollows(4, 6));
        REQUIRE_FALSE(follows_table.InsertFollows(5, 7));
      }
    }

    WHEN("Insert an existing stmt2 index") {
      THEN("Insertion returns False. Each stmt2 can only have one stmt1 such that Follows(stmt1, stmt2)") {
        REQUIRE(follows_table.InsertFollows(3, 5) == false);
        REQUIRE(follows_table.InsertFollows(6, 10) == false);
      }
    }

    WHEN("Check if Follows(stmt1, stmt2) holds.") {
      THEN("Returns True if Follows(stmt1, stmt2) holds. Otherwise returns False.") {
        REQUIRE(follows_table.IsFollows(1, 2));
        REQUIRE(follows_table.IsFollows(4, 5));
        REQUIRE(follows_table.IsFollows(5, 10));
        REQUIRE(follows_table.IsFollows(2, 1) == false);
      }
    }

    WHEN("Get statement followed by stmt2.") {
      THEN("Returns stmt1 if Follows(stmt1, stmt2) holds. Otherwise, returns -1.") {
        REQUIRE(follows_table.GetStmtFollowedBy(2) == 1);
        REQUIRE(follows_table.GetStmtFollowedBy(5) == 4);
        REQUIRE(follows_table.GetStmtFollowedBy(10) == 5);
        REQUIRE(follows_table.GetStmtFollowedBy(1) == -1);
      }
    }

    WHEN("Get statement that follows stmt1.") {
      THEN("Returns stmt2 if Follows(stmt1, stmt2) holds. Returns -1 if no statement follows stmt1.") {
        REQUIRE(follows_table.GetStmtFollows(1) == 2);
        REQUIRE(follows_table.GetStmtFollows(4) == 5);
        REQUIRE(follows_table.GetStmtFollows(5) == 10);
        REQUIRE(follows_table.GetStmtFollows(10) == -1);
      }
    }

    WHEN("GetAllFollowedStmts() called.") {
      THEN("Get all statements that are followed.") {
        REQUIRE(follows_table.GetAllFollowedStmts().size() == 3);
        REQUIRE(ContainsExactly(follows_table.GetAllFollowedStmts(), {1, 4, 5}));
      }
    }

    WHEN("GetAllFollowedStmts() called.") {
      THEN("Get all statements that are followed.") {
        REQUIRE(follows_table.GetAllFollowsStmts().size() == 3);
        REQUIRE(ContainsExactly(follows_table.GetAllFollowsStmts(), {2, 5, 10}));
      }
    }
  }
}