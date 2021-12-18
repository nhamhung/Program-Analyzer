#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/NextTable.h"

SCENARIO("Construct a new next_table.") {
  GIVEN("A next_table has just been constructed.") {
    NextTable next_table;

    THEN("All underlying tables exist with size 0.") {
      REQUIRE(next_table.GetNextTable().IsEmpty());
      REQUIRE(next_table.GetInverseNextTable().IsEmpty());
      REQUIRE(next_table.GetNextTTable().IsEmpty());
      REQUIRE(next_table.GetInverseNextTTable().IsEmpty());
    }
  }
}

SCENARIO("Underlying next and nextT tables created.") {
  NextTable next_table;

  next_table.InsertNext(1, 2);
  next_table.InsertNext(2, 3);
  next_table.InsertNext(4, 5);
  next_table.InsertNext(6, 7);
  next_table.InsertNext(7, 8);
  next_table.InsertNext(8, 9);
  next_table.InsertNext(10, 11);
  next_table.InsertNext(12, 13);
  next_table.InsertNext(13, 14);
  next_table.InsertNext(14, 15);
  next_table.InsertNext(15, 16);
  next_table.InsertNext(16, 17);
  next_table.InsertNext(17, 18);
  next_table.InsertNext(18, 14);
  next_table.InsertNext(19, 20);
  next_table.InsertNext(20, 21);
  next_table.InsertNext(21, 22);
  next_table.InsertNext(20, 23);
  next_table.InsertNext(22, 23);

  GIVEN("Test next_table.") {
    WHEN("Insert a Next(prog_line1, prog_line2) relationship that is valid.") {
      THEN("Insertion returns True. Both next_table and inverse_next_table are filled.") {
        REQUIRE(next_table.GetNextTable().Size() == 18);
        REQUIRE(next_table.GetInverseNextTable().Size() == 17);
      }
    }

    WHEN("Insert a Next(prog_line1, prog_line2) such that prog_line1 == prog_line2 or prog_line1 <= 0 or prog_line2 <= 0") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(next_table.InsertNext(24, 24));
        REQUIRE_FALSE(next_table.InsertNext(24, 0));
        REQUIRE_FALSE(next_table.InsertNext(0, 24));
      }
    }

    WHEN("Insert a Next(prog_line1, prog_line2) such that both already exist.") {
      THEN("Insertion returns False. prog_line2 already exists in statement set of prog_line1.") {
        REQUIRE_FALSE(next_table.InsertNext(1, 2));
        REQUIRE_FALSE(next_table.InsertNext(22, 23));
      }
    }

    WHEN("IsNext(prog_line1, prog_line2) called with existing prog_line1 and prog_line2.") {
      THEN("Returns true.") {
        REQUIRE(next_table.IsNext(1, 2));
        REQUIRE(next_table.IsNext(2, 3));
        REQUIRE(next_table.IsNext(4, 5));
      }
    }

    WHEN("IsNext(prog_line1, prog_line2) called with invalid or non-existing or false relationship.") {
      THEN("Returns false.") {
        REQUIRE_FALSE(next_table.IsNext(2, 1));
        REQUIRE_FALSE(next_table.IsNext(2, 2));
        REQUIRE_FALSE(next_table.IsNext(0, 1));
        REQUIRE_FALSE(next_table.IsNext(1, 0));
        REQUIRE_FALSE(next_table.IsNext(24, 25));
      }
    }

    WHEN("GetNextStatements(prog_line1) called with valid and existing prog_line1.") {
      THEN("Returns all statements that are Next to prog_line1.") {
        std::unordered_set<int> all_next_statements_of_20 = next_table.GetNextStatements(20);
        REQUIRE(ContainsExactly(all_next_statements_of_20, {21, 23}));
      }
    }

    WHEN("GetNextStatements(prog_line1) called with invalid or non-existing prog_line1.") {
      THEN("Returns default empty set.") {
        REQUIRE(next_table.GetNextStatements(0) == std::unordered_set<int>());
        REQUIRE(next_table.GetNextStatements(24) == std::unordered_set<int>());
      }
    }

    WHEN("GetPreviousStatements(prog_line2) called with valid and existing prog_line2.") {
      THEN("Returns all statements that are Previous to prog_line2.") {
        std::unordered_set<int> all_previous_statements_of_14 = next_table.GetPreviousStatements(14);
        REQUIRE(ContainsExactly(all_previous_statements_of_14, {13, 18}));
      }
    }

    WHEN("GetPreviousStatements(prog_line2) called with invalid or non-existing prog_line2.") {
      THEN("Returns default empty set.") {
        REQUIRE(next_table.GetPreviousStatements(0) == std::unordered_set<int>());
        REQUIRE(next_table.GetPreviousStatements(24) == std::unordered_set<int>());
      }
    }

    WHEN("GetAllNextStatements() called.") {
      THEN("Returns all prog_lines that are Next to one or more prog_line.") {
        std::unordered_set<int> all_next_statements = next_table.GetAllNextStatements();
        REQUIRE(ContainsExactly(all_next_statements, {2, 3, 5, 7, 8, 9, 11, 13, 14, 15, 16, 17, 18, 20, 21, 22, 23}));
      }
    }

    WHEN("GetAllPreviousStatements() called.") {
      THEN("Returns all prog_lines that are Previous to one or more prog_line.") {
        std::unordered_set<int> all_previous_statements = next_table.GetAllPreviousStatements();
        REQUIRE(ContainsExactly(all_previous_statements, {1, 2, 4, 6, 7, 8, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22}));
      }
    }
  }

  REQUIRE(next_table.InsertNextT(1, 2));
  REQUIRE(next_table.InsertNextT(1, 3));
  REQUIRE(next_table.InsertNextT(2, 3));
  REQUIRE(next_table.InsertNextT(4, 5));
  REQUIRE(next_table.InsertNextT(6, 7));
  REQUIRE(next_table.InsertNextT(6, 8));
  REQUIRE(next_table.InsertNextT(6, 9));
  REQUIRE(next_table.InsertNextT(7, 8));
  REQUIRE(next_table.InsertNextT(7, 9));
  REQUIRE(next_table.InsertNextT(8, 9));
  REQUIRE(next_table.InsertNextT(14, 14));

  GIVEN("Test next_T_table.") {
    WHEN("Insert a NextT(prog_line1, prog_line2) relationship that is valid.") {
      THEN("Insertion returns True. Both next_table and inverse_next_table are filled.") {
        REQUIRE(next_table.GetNextTTable().Size() == 7);
        REQUIRE(next_table.GetInverseNextTTable().Size() == 7);
      }
    }

    WHEN("Insert a NextT(prog_line1, prog_line2) such that prog_line1 <= 0 or prog_line2 <= 0") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(next_table.InsertNextT(24, 0));
        REQUIRE_FALSE(next_table.InsertNextT(0, 24));
      }
    }

    WHEN("Insert a NextT(prog_line1, prog_line2) such that both already exist.") {
      THEN("Insertion returns False. prog_line2 already exists in NextT statement set of prog_line1.") {
        REQUIRE_FALSE(next_table.InsertNextT(1, 2));
        REQUIRE_FALSE(next_table.InsertNext(8, 9));
      }
    }

    WHEN("IsNextT(prog_line1, prog_line2) called with existing prog_line1 and prog_line2.") {
      THEN("Returns true.") {
        REQUIRE(next_table.IsNextT(1, 2));
        REQUIRE(next_table.IsNextT(1, 3));
        REQUIRE(next_table.IsNextT(2, 3));
        REQUIRE(next_table.IsNextT(4, 5));
        REQUIRE(next_table.IsNextT(6, 7));
        REQUIRE(next_table.IsNextT(6, 8));
        REQUIRE(next_table.IsNextT(6, 9));
        REQUIRE(next_table.IsNextT(7, 8));
        REQUIRE(next_table.IsNextT(7, 9));
        REQUIRE(next_table.IsNextT(8, 9));
        REQUIRE(next_table.IsNextT(14, 14));
      }
    }

    WHEN("IsNextT(prog_line1, prog_line2) called with invalid or non-existing or false relationship.") {
      THEN("Returns false.") {
        REQUIRE_FALSE(next_table.IsNext(2, 1));
        REQUIRE_FALSE(next_table.IsNext(0, 1));
        REQUIRE_FALSE(next_table.IsNext(1, 0));
        REQUIRE_FALSE(next_table.IsNext(24, 25));
      }
    }

    WHEN("GetNextTStatements(prog_line1) called with valid and existing prog_line1.") {
      THEN("Returns all statements that are Next to prog_line1.") {
        std::unordered_set<int> all_nextT_statements_of_6 = next_table.GetNextTStatements(6);
        REQUIRE(ContainsExactly(all_nextT_statements_of_6, {7, 8, 9}));
      }
    }

    WHEN("GetNextTStatements(prog_line1) called with invalid or non-existing prog_line1.") {
      THEN("Returns default empty set.") {
        REQUIRE(next_table.GetNextTStatements(0) == std::unordered_set<int>());
        REQUIRE(next_table.GetNextTStatements(24) == std::unordered_set<int>());
      }
    }

    WHEN("GetPreviousTStatements(prog_line2) called with valid and existing prog_line2.") {
      THEN("Returns all statements that are PreviousT to prog_line2.") {
        std::unordered_set<int> all_previousT_statements_of_9 = next_table.GetPreviousTStatements(9);
        REQUIRE(ContainsExactly(all_previousT_statements_of_9, {6, 7, 8}));
      }
    }

    WHEN("GetPreviousTStatements(prog_line2) called with invalid or non-existing prog_line2.") {
      THEN("Returns default empty set.") {
        REQUIRE(next_table.GetPreviousTStatements(0) == std::unordered_set<int>());
        REQUIRE(next_table.GetPreviousTStatements(24) == std::unordered_set<int>());
      }
    }

    WHEN("GetAllNextTStatements() called.") {
      THEN("Returns all prog_lines that are Next to one or more prog_line.") {
        std::unordered_set<int> all_nextT_statements = next_table.GetAllNextTStatements();
        REQUIRE(ContainsExactly(all_nextT_statements, {2, 3, 5, 7, 8, 9, 14}));
      }
    }

    WHEN("GetAllPreviousStatements() called.") {
      THEN("Returns all prog_lines that are Previous to one or more prog_line.") {
        std::unordered_set<int> all_previousT_statements = next_table.GetAllPreviousTStatements();
        REQUIRE(ContainsExactly(all_previousT_statements, {1, 2, 4, 6, 7, 8, 14}));
      }
    }
  }

  GIVEN("ClearNextTable() called.") {
    THEN("Clear content of next table. Its size becomes 0.") {
      next_table.ClearNextTable();
      REQUIRE(next_table.GetNextTable().IsEmpty());
      REQUIRE(next_table.GetInverseNextTable().IsEmpty());
      REQUIRE(next_table.GetNextTTable().IsEmpty());
      REQUIRE(next_table.GetInverseNextTTable().IsEmpty());
    }
  }
}