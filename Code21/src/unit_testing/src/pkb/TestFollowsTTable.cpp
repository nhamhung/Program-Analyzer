#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/FollowsTTable.h"

SCENARIO("Construct an empty FollowsTTable.") {
  FollowsTTable follows_T_table;

  GIVEN("Initial FollowsTTable is empty.") {
    THEN("follows_T_table exists with size 0.") {
      REQUIRE(follows_T_table.GetFollowsTTable().Size() == 0);
      REQUIRE(follows_T_table.GetInverseFollowsTTable().Size() == 0);
    }
  }
}

SCENARIO("FollowsTTable has been constructed.") {
  FollowsTTable follows_T_table;

  GIVEN("FollowsTTable is currently empty.") {
    WHEN("Insert a FollowsT(stmt1, stmt2) relationship.") {
      THEN("Insertion returns True. Both follows_T and inverse_follows_T are filled.") {
        REQUIRE(follows_T_table.InsertFollowsT(1, 2));
        REQUIRE(follows_T_table.InsertFollowsT(1, 3));
        REQUIRE(follows_T_table.InsertFollowsT(1, 4));
        REQUIRE(follows_T_table.InsertFollowsT(1, 10));
        TableMultiple<int, int> follows_T = follows_T_table.GetFollowsTTable();
        TableMultiple<int, int> inverse_follows_T = follows_T_table.GetInverseFollowsTTable();
        REQUIRE(follows_T.Size() == 1);
        REQUIRE(follows_T.Get(1).size() == 4);
        REQUIRE(inverse_follows_T.Size() == 4);
        REQUIRE(Contains(inverse_follows_T.Get(10), 1));
        REQUIRE(inverse_follows_T.Get(10).size() == 1);
        REQUIRE(Contains(inverse_follows_T.Get(4), 1));
        REQUIRE(inverse_follows_T.Get(9).empty());
      }
    }

    WHEN("Insert a FollowsT(stmt1, stmt2) such that stmt1 >= stmt2 or stmt1 <= 0.") {
      THEN("Insertion returns False. stmt1 must be strictly less than stmt2 for FollowsT(stmt1, stmt2).") {
        REQUIRE(follows_T_table.InsertFollowsT(2, 2) == false);
        REQUIRE(follows_T_table.InsertFollowsT(2, 1) == false);
        REQUIRE(follows_T_table.InsertFollowsT(0, 1) == false);
      }
    }
  }

  GIVEN("FollowsTTable already contains entries.") {
    REQUIRE(follows_T_table.InsertFollowsT(3, 10));
    REQUIRE(follows_T_table.InsertFollowsT(3, 4));
    REQUIRE(follows_T_table.InsertFollowsT(3, 5));
    REQUIRE(follows_T_table.InsertFollowsT(4, 10));
    REQUIRE(follows_T_table.InsertFollowsT(1, 14));
    REQUIRE(follows_T_table.InsertFollowsT(1, 3));

    WHEN("Check if IsFollowsT(stmt1, stmt2) holds.") {
      THEN("Returns True if FollowsT(stmt1, stmt2) holds. Otherwise returns False.") {
        REQUIRE(follows_T_table.IsFollowsT(1, 2) == false);
        REQUIRE(follows_T_table.IsFollowsT(3, 4));
        REQUIRE(follows_T_table.InsertFollowsT(3, 5) == false);
        REQUIRE(follows_T_table.InsertFollowsT(3, 10) == false);
        REQUIRE(follows_T_table.InsertFollowsT(1, 14) == false);
        REQUIRE(follows_T_table.InsertFollowsT(1, 3) == false);
        REQUIRE(follows_T_table.InsertFollowsT(10, 3) == false);
      }
    }

    WHEN("Check if inverse_follows_T_table stores the inverse relationship.") {
      THEN("inverse_follows_T_table should be filled up the same moment InsertFollowsT(stmt1, stmt2).") {
        REQUIRE(follows_T_table.GetInverseFollowsTTable().Size() == 5);
        REQUIRE(follows_T_table.GetInverseFollowsTTable().Get(10).size() == 2);
        REQUIRE(Contains(follows_T_table.GetInverseFollowsTTable().Get(10), 3));
        REQUIRE(Contains(follows_T_table.GetInverseFollowsTTable().Get(10), 4));
        REQUIRE(follows_T_table.GetInverseFollowsTTable().Get(4).size() == 1);
        REQUIRE(Contains(follows_T_table.GetInverseFollowsTTable().Get(4), 3));
      }
    }

    WHEN("GetStmtsFollowedTBy(stmt2) called.") {
      THEN("Returns a vector of statements s_i that FollowsT(s_i, stmt2) holds.") {
        REQUIRE(follows_T_table.GetStmtsFollowedTBy(10).size() == 2);
        REQUIRE(Contains(follows_T_table.GetStmtsFollowedTBy(10), 3));
        REQUIRE(Contains(follows_T_table.GetStmtsFollowedTBy(10), 4));
        REQUIRE(Contains(follows_T_table.GetStmtsFollowedTBy(14), 1));
      }
    }

    WHEN("GetStmtsFollowsT(stmt1) called.") {
      THEN("Returns a vector of statements s_i that FollowsT(stmt1, s_i) holds.") {
        REQUIRE(follows_T_table.GetStmtsFollowsT(3).size() == 3);
        REQUIRE(ContainsExactly(follows_T_table.GetStmtsFollowsT(3), {4, 5, 10}));
        REQUIRE(follows_T_table.GetStmtsFollowsT(1).size() == 2);
        REQUIRE(ContainsExactly(follows_T_table.GetStmtsFollowsT(1), {14, 3}));
      }
    }

    WHEN("GetAllFollowedTStmts() called.") {
      THEN("Returns a set of all statements that are followedT.") {
        REQUIRE(follows_T_table.GetAllFollowedTStmts().size() == 3);
        REQUIRE(ContainsExactly(follows_T_table.GetAllFollowedTStmts(), {1, 3, 4}));
      }
    }

    WHEN("GetAllFollowsTStmts() called.") {
      THEN("Returns a set of all statements that are followsT.") {
        REQUIRE(follows_T_table.GetAllFollowsTStmts().size() == 5);
        REQUIRE(ContainsExactly(follows_T_table.GetAllFollowsTStmts(), {3, 4, 5, 10, 14}));
      }
    }
  }
}