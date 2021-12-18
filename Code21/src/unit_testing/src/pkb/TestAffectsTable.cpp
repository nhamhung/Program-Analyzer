#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/AffectsTable.h"

SCENARIO("Construct a new affects_table.") {
  GIVEN("A affects_table has just been constructed.") {
    AffectsTable affects_table;

    THEN("All underlying tables exist with size 0.") {
      REQUIRE(affects_table.GetAffectsTable().IsEmpty());
      REQUIRE(affects_table.GetAffectedTable().IsEmpty());
      REQUIRE(affects_table.GetAffectsTTable().IsEmpty());
      REQUIRE(affects_table.GetAffectedTTable().IsEmpty());
    }
  }
}

SCENARIO("Underlying affects and affectsT tables created.") {
  AffectsTable affects_table;

  REQUIRE(affects_table.InsertAffects(10, 15));
  REQUIRE(affects_table.InsertAffects(10, 21));
  REQUIRE(affects_table.InsertAffects(10, 22));
  REQUIRE(affects_table.InsertAffects(11, 16));
  REQUIRE(affects_table.InsertAffects(11, 21));
  REQUIRE(affects_table.InsertAffects(11, 23));
  REQUIRE(affects_table.InsertAffects(12, 17));
  REQUIRE(affects_table.InsertAffects(12, 22));
  REQUIRE(affects_table.InsertAffects(12, 23));
  REQUIRE(affects_table.InsertAffects(15, 15));
  REQUIRE(affects_table.InsertAffects(16, 16));
  REQUIRE(affects_table.InsertAffects(17, 17));
  REQUIRE(affects_table.InsertAffects(15, 21));
  REQUIRE(affects_table.InsertAffects(15, 22));
  REQUIRE(affects_table.InsertAffects(16, 21));
  REQUIRE(affects_table.InsertAffects(17, 22));
  REQUIRE(affects_table.InsertAffects(16, 23));
  REQUIRE(affects_table.InsertAffects(17, 23));
  REQUIRE(affects_table.InsertAffects(21, 23));
  REQUIRE(affects_table.InsertAffects(22, 23));

  GIVEN("Test affects_table.") {
    WHEN("Insert a Affects(stmt1, stmt2) relationship that is valid.") {
      THEN("Insertion returns True. Both affects_table and inverse_affects_table are filled.") {
        REQUIRE(affects_table.GetAffectsTable().Size() == 8);
        REQUIRE(affects_table.GetAffectedTable().Size() == 6);
      }
    }

    WHEN("Insert a Affects(stmt1, stmt2) such that stmt1 <= 0 or stmt2 <= 0") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(affects_table.InsertAffects(24, 0));
        REQUIRE_FALSE(affects_table.InsertAffects(0, 24));
      }
    }

    WHEN("Insert a Affects(stmt1, stmt2) such that relationship already exists.") {
      THEN("Insertion returns False. stmt2 already exists in statement set of stmt1.") {
        REQUIRE_FALSE(affects_table.InsertAffects(10, 15));
        REQUIRE_FALSE(affects_table.InsertAffects(22, 23));
      }
    }

    WHEN("IsAffects(stmt1, stmt2) called with existing stmt1 and stmt2.") {
      THEN("Returns true.") {
        REQUIRE(affects_table.IsAffects(10, 15));
        REQUIRE(affects_table.IsAffects(10, 21));
        REQUIRE(affects_table.IsAffects(10, 22));
        REQUIRE(affects_table.IsAffects(11, 16));
        REQUIRE(affects_table.IsAffects(11, 21));
        REQUIRE(affects_table.IsAffects(11, 23));
        REQUIRE(affects_table.IsAffects(12, 17));
        REQUIRE(affects_table.IsAffects(12, 22));
      }
    }

    WHEN("IsAffects(stmt1, stmt2) called with invalid or non-existing or false relationship.") {
      THEN("Returns false.") {
        REQUIRE_FALSE(affects_table.IsAffects(2, 1));
        REQUIRE_FALSE(affects_table.IsAffects(0, 1));
        REQUIRE_FALSE(affects_table.IsAffects(1, 0));
        REQUIRE_FALSE(affects_table.IsAffects(24, 25));
      }
    }

    WHEN("GetStatementsThatAffects(stmt2) called with valid and existing stmt2.") {
      THEN("Returns all statements that are Affects stmt2.") {
        std::unordered_set<int> all_statements_that_affects_22 = affects_table.GetStatementsThatAffects(22);
        REQUIRE(ContainsExactly(all_statements_that_affects_22, {10, 12, 15, 17}));
      }
    }

    WHEN("GetStatementsThatAffects(stmt2) called with invalid or non-existing stmt2.") {
      THEN("Returns default empty set.") {
        REQUIRE(affects_table.GetStatementsThatAffects(0) == std::unordered_set<int>());
        REQUIRE(affects_table.GetStatementsThatAffects(24) == std::unordered_set<int>());
      }
    }

    WHEN("GetAffectedStatements(stmt1) called with valid and existing stmt1.") {
      THEN("Returns all statements that are affected by stmt1.") {
        std::unordered_set<int> all_statements_affected_by_10 = affects_table.GetAffectedStatements(10);
        REQUIRE(ContainsExactly(all_statements_affected_by_10, {15, 21, 22}));
      }
    }

    WHEN("GetAffectedStatements(stmt1) called with invalid or non-existing stmt1.") {
      THEN("Returns default empty set.") {
        REQUIRE(affects_table.GetAffectedStatements(0) == std::unordered_set<int>());
        REQUIRE(affects_table.GetAffectedStatements(24) == std::unordered_set<int>());
      }
    }

    WHEN("GetAllStatementsThatAffects() called.") {
      THEN("Returns all statements that Affects one or more other statements.") {
        std::unordered_set<int> all_statements_that_affects = affects_table.GetAllStatementsThatAffects();
        REQUIRE(ContainsExactly(all_statements_that_affects, {10, 11, 12, 15, 16, 17, 21, 22}));
      }
    }

    WHEN("GetAllAffectedStatements() called.") {
      THEN("Returns all statements that are Affected by one or more other statements.") {
        std::unordered_set<int> all_affected_statements = affects_table.GetAllAffectedStatements();
        REQUIRE(ContainsExactly(all_affected_statements, {15, 16, 17, 21, 22, 23}));
      }
    }
  }

  REQUIRE(affects_table.InsertAffectsT(10, 15));
  REQUIRE(affects_table.InsertAffectsT(10, 21));
  REQUIRE(affects_table.InsertAffectsT(10, 22));
  REQUIRE(affects_table.InsertAffectsT(10, 23));
  REQUIRE(affects_table.InsertAffectsT(11, 16));
  REQUIRE(affects_table.InsertAffectsT(11, 21));
  REQUIRE(affects_table.InsertAffectsT(11, 23));
  REQUIRE(affects_table.InsertAffectsT(12, 17));
  REQUIRE(affects_table.InsertAffectsT(12, 22));
  REQUIRE(affects_table.InsertAffectsT(12, 23));
  REQUIRE(affects_table.InsertAffectsT(15, 15));
  REQUIRE(affects_table.InsertAffectsT(16, 16));
  REQUIRE(affects_table.InsertAffectsT(17, 17));
  REQUIRE(affects_table.InsertAffectsT(15, 21));
  REQUIRE(affects_table.InsertAffectsT(15, 22));
  REQUIRE(affects_table.InsertAffectsT(15, 23));
  REQUIRE(affects_table.InsertAffectsT(16, 21));
  REQUIRE(affects_table.InsertAffectsT(17, 22));
  REQUIRE(affects_table.InsertAffectsT(16, 23));
  REQUIRE(affects_table.InsertAffectsT(17, 23));
  REQUIRE(affects_table.InsertAffectsT(21, 23));
  REQUIRE(affects_table.InsertAffectsT(22, 23));

  GIVEN("Test affectsT_table.") {
    WHEN("Insert a AffectsT(stmt1, stmt2) relationship that is valid.") {
      THEN("Insertion returns True. Both affectsT_table and inverse_affectsT_table are filled.") {
        REQUIRE(affects_table.GetAffectsTTable().Size() == 8);
        REQUIRE(affects_table.GetAffectedTTable().Size() == 6);
      }
    }

    WHEN("Insert a AffectsT(stmt1, stmt2) such that stmt1 <= 0 or stmt2 <= 0") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(affects_table.InsertAffectsT(24, 0));
        REQUIRE_FALSE(affects_table.InsertAffectsT(0, 24));
      }
    }

    WHEN("Insert a AffectsT(stmt1, stmt2) such that relationship already exists.") {
      THEN("Insertion returns False. stmt2 already exists in statement set of stmt1.") {
        REQUIRE_FALSE(affects_table.InsertAffectsT(10, 15));
        REQUIRE_FALSE(affects_table.InsertAffectsT(22, 23));
      }
    }

    WHEN("IsAffectsT(stmt1, stmt2) called with existing stmt1 and stmt2.") {
      THEN("Returns true.") {
        REQUIRE(affects_table.IsAffectsT(10, 15));
        REQUIRE(affects_table.IsAffectsT(10, 21));
        REQUIRE(affects_table.IsAffectsT(10, 22));
        REQUIRE(affects_table.IsAffectsT(10, 23));
        REQUIRE(affects_table.IsAffectsT(11, 16));
        REQUIRE(affects_table.IsAffectsT(11, 21));
        REQUIRE(affects_table.IsAffectsT(11, 23));
        REQUIRE(affects_table.IsAffectsT(12, 17));
      }
    }

    WHEN("IsAffectsT(stmt1, stmt2) called with invalid or non-existing or false relationship.") {
      THEN("Returns false.") {
        REQUIRE_FALSE(affects_table.IsAffectsT(15, 10));
        REQUIRE_FALSE(affects_table.IsAffectsT(10, 16));
        REQUIRE_FALSE(affects_table.IsAffectsT(0, 1));
        REQUIRE_FALSE(affects_table.IsAffectsT(1, 0));
        REQUIRE_FALSE(affects_table.IsAffectsT(24, 25));
      }
    }

    WHEN("GetStatementsThatAffectsT(stmt2) called with valid and existing stmt2.") {
      THEN("Returns all statements that are AffectsT stmt2.") {
        std::unordered_set<int> all_statements_that_affectsT_22 = affects_table.GetStatementsThatAffectsT(22);
        REQUIRE(ContainsExactly(all_statements_that_affectsT_22, {10, 12, 15, 17}));
      }
    }

    WHEN("GetStatementsThatAffectsT(stmt2) called with invalid or non-existing stmt2.") {
      THEN("Returns default empty set.") {
        REQUIRE(affects_table.GetStatementsThatAffectsT(0) == std::unordered_set<int>());
        REQUIRE(affects_table.GetStatementsThatAffectsT(24) == std::unordered_set<int>());
      }
    }

    WHEN("GetAffectedTStatements(stmt1) called with valid and existing stmt1.") {
      THEN("Returns all statements that are affectedT by stmt1.") {
        std::unordered_set<int> all_statements_affectedT_by_10 = affects_table.GetAffectedTStatements(10);
        REQUIRE(ContainsExactly(all_statements_affectedT_by_10, {15, 21, 22, 23}));
      }
    }

    WHEN("GetAffectedTStatements(stmt1) called with invalid or non-existing stmt1.") {
      THEN("Returns default empty set.") {
        REQUIRE(affects_table.GetAffectedTStatements(0) == std::unordered_set<int>());
        REQUIRE(affects_table.GetAffectedTStatements(24) == std::unordered_set<int>());
      }
    }

    WHEN("GetAllStatementsThatAffectsT() called.") {
      THEN("Returns all statements that AffectsT one or more other statements.") {
        std::unordered_set<int> all_statements_that_affectsT = affects_table.GetAllStatementsThatAffectsT();
        REQUIRE(ContainsExactly(all_statements_that_affectsT, {10, 11, 12, 15, 16, 17, 21, 22}));
      }
    }

    WHEN("GetAllAffectedTStatements() called.") {
      THEN("Returns all statements that are AffectedT by one or more other statements.") {
        std::unordered_set<int> all_affectedT_statements = affects_table.GetAllAffectedTStatements();
        REQUIRE(ContainsExactly(all_affectedT_statements, {15, 16, 17, 21, 22, 23}));
      }
    }
  }

  GIVEN("ClearAffectsTable() called.") {
    THEN("Clear content of affects table. Its size becomes 0.") {
      affects_table.ClearAffectsTable();
      REQUIRE(affects_table.GetAffectsTable().IsEmpty());
      REQUIRE(affects_table.GetAffectedTable().IsEmpty());
      REQUIRE(affects_table.GetAffectsTTable().IsEmpty());
      REQUIRE(affects_table.GetAffectedTTable().IsEmpty());
    }
  }
}