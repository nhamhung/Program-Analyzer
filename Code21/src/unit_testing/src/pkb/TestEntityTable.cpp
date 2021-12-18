#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/entity_tables/EntityTable.h"

SCENARIO("Construct a new entity_table.") {
  GIVEN("An entity_table has just been constructed.") {
    EntityTable entity_table;

    THEN("entity_table exists with size 0.") {
      REQUIRE(entity_table.GetEntityTable().IsEmpty());
      REQUIRE(entity_table.GetInverseEntityTable().IsEmpty());
    }
  }

  GIVEN("A entity_table has been constructed to insert entries.") {
    EntityTable entity_table;
    REQUIRE(entity_table.InsertEntity(1, "assign"));
    REQUIRE(entity_table.InsertEntity(4, "read"));
    REQUIRE(entity_table.InsertEntity(5, "read"));
    REQUIRE(entity_table.InsertEntity(6, "print"));
    REQUIRE(entity_table.InsertEntity(7, "print"));
    REQUIRE(entity_table.InsertEntity(8, "print"));
    REQUIRE(entity_table.InsertEntity(9, "print"));
    REQUIRE(entity_table.InsertEntity(10, "assign"));
    REQUIRE(entity_table.InsertEntity(11, "assign"));
    REQUIRE(entity_table.InsertEntity(12, "assign"));
    REQUIRE(entity_table.InsertEntity(14, "while"));
    REQUIRE(entity_table.InsertEntity(15, "assign"));
    REQUIRE(entity_table.InsertEntity(16, "assign"));
    REQUIRE(entity_table.InsertEntity(17, "assign"));
    REQUIRE(entity_table.InsertEntity(19, "if"));
    REQUIRE(entity_table.InsertEntity(20, "assign"));
    REQUIRE(entity_table.InsertEntity(21, "assign"));
    REQUIRE(entity_table.InsertEntity(22, "assign"));
    REQUIRE(entity_table.InsertEntity(23, "assign"));

    WHEN("Inserts to entity_table with valid index and entity.") {
      THEN("Insertion returns true. Insertions are successful.") {
        REQUIRE(entity_table.GetEntityTable().Size() == 19);
      }
    }

    WHEN("Inserts to entity_table with the same index.") {
      THEN("Insertion returns false. Statement has already been inserted.") {
        REQUIRE_FALSE(entity_table.InsertEntity(1, "read"));
        REQUIRE_FALSE(entity_table.InsertEntity(4, "assign"));
      }
    }

    WHEN("Inserts to entity_table with invalid index <= 0.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(entity_table.InsertEntity(0, "read"));
        REQUIRE_FALSE(entity_table.InsertEntity(-1, "read"));
      }
    }

    WHEN("GetStatementType(stmt_index) called.") {
      THEN("Returns the DesignEntityType of the statement.") {
        REQUIRE(entity_table.GetStatementType(1) == "assign");
        REQUIRE(entity_table.GetStatementType(4) == "read");
        REQUIRE(entity_table.GetStatementType(5) == "read");
      }
    }

    WHEN("GetAllEntities() called.") {
      THEN("Returns a set of all entities stored in the pkb.") {
        std::unordered_set<std::string> all_entities_present = entity_table.GetAllEntities();
        REQUIRE(all_entities_present.size() == 5);
        REQUIRE(ContainsExactly(all_entities_present, {"assign", "if", "while", "read", "print"}));
      }
    }

    WHEN("ClearEntityTable() called.") {
      THEN("Clear content of entity table. Its size becomes 0.") {
        entity_table.ClearEntityTable();
        REQUIRE(entity_table.GetEntityTable().IsEmpty());
        REQUIRE(entity_table.GetInverseEntityTable().IsEmpty());
      }
    }
  }
}