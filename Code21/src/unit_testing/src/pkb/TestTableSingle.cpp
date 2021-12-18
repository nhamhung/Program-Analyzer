#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/templates/TableSingle.h"

using namespace source_processor;

SCENARIO("Construct an empty single table.") {
  GIVEN("Different template class TableSingle construction.") {
    TableSingle<int, std::string> table_int_string;
    TableSingle<std::string, int> table_string_int;
    TableSingle<int, int> table_int_int;
    TableSingle<int, source_processor::TokenList> table_int_tokenlist;
    TableSingle<std::string, std::pair<int, int>> table_string_pair;
    THEN("TableMultiple exists with size 0.") {
      REQUIRE(table_int_string.IsEmpty());
      REQUIRE(table_string_int.IsEmpty());
      REQUIRE(table_int_int.IsEmpty());
      REQUIRE(table_int_tokenlist.IsEmpty());
      REQUIRE(table_string_pair.IsEmpty());
    }
  }
}

SCENARIO("TableSingle has been constructed.") {
  TableSingle<int, std::string> table_int_string;
  TableSingle<std::string, int> table_string_int;
  TableSingle<int, int> table_int_int;
  TableSingle<int, source_processor::TokenList> table_int_tokenlist;
  TableSingle<std::string, std::pair<int, int>> table_string_pair;

  GIVEN("TableMultiple is currently empty.") {
    Token count("count", TokenType::VariableName);
    TokenList sample_token_list;
    sample_token_list.Push(count);

    REQUIRE(table_int_string.Insert(1, "x"));
    REQUIRE(table_string_int.Insert("x", 1));
    REQUIRE(table_int_int.Insert(1, 2));
    REQUIRE(table_int_tokenlist.Insert(1, sample_token_list));
    REQUIRE(table_string_pair.Insert("func", std::pair<int, int>(1, 3)));

    WHEN("Insert value entries into different tables.") {
      THEN("Insertion returns True. TableMultiple is of size 1.") {
        REQUIRE(table_int_string.Size() == 1);
        REQUIRE(table_string_int.Size() == 1);
        REQUIRE(table_int_int.Size() == 1);
        REQUIRE(table_int_tokenlist.Size() == 1);
        REQUIRE(table_string_pair.Size() == 1);
      }
    }

    WHEN("Insert existing statement indexes.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(table_int_string.Insert(1, "x"));
        REQUIRE_FALSE(table_string_int.Insert("x", 1));
        REQUIRE_FALSE(table_int_int.Insert(1, 2));
        REQUIRE_FALSE(table_int_tokenlist.Insert(1, sample_token_list));
        REQUIRE_FALSE(table_string_pair.Insert("func", std::pair<int, int>(1, 3)));
      }
    }

    WHEN("Get(key) called with valid key.") {
      Token count("count", TokenType::VariableName);
      TokenList token_list_stmt_1;
      token_list_stmt_1.Push(count);

      THEN("Returns the corresponding value.") {
        REQUIRE(table_int_string.Get(1) == "x");
        REQUIRE(table_int_string.Get(2) == "");
        REQUIRE(table_string_int.Get("x") == 1);
        REQUIRE(table_string_int.Get("unknown") == 0);
        REQUIRE(table_int_int.Get(1) == 2);
        REQUIRE(table_int_int.Get(2) == 0);
        REQUIRE(table_int_tokenlist.Get(1) == token_list_stmt_1);
        REQUIRE(table_int_tokenlist.Get(2) == TokenList());
        REQUIRE(table_string_pair.Get("func") == std::pair<int, int>(1, 3));
        REQUIRE(table_string_pair.Get("unknown_func") == std::pair<int, int>());
      }
    }

    WHEN("Contains(key) called.") {
      THEN("Returns True if table contains key. False otherwise.") {
        REQUIRE(table_int_string.Contains(1));
        REQUIRE_FALSE(table_int_string.Contains(-1));
        REQUIRE_FALSE(table_int_string.Contains(2));
        REQUIRE(table_string_int.Contains("x"));
        REQUIRE(table_int_int.Contains(1));
        REQUIRE(table_int_tokenlist.Contains(1));
        REQUIRE(table_string_pair.Contains("func"));
      }
    }

    WHEN("Size() called.") {
      THEN("Returns table size.") {
        REQUIRE(table_int_string.Size() == 1);
        REQUIRE(table_string_int.Size() == 1);
        REQUIRE(table_int_int.Size() == 1);
        REQUIRE(table_int_tokenlist.Size() == 1);
        REQUIRE(table_string_pair.Size() == 1);
      }
    }

    WHEN("GetAllKeys() called.") {
      THEN("Returns all keys in the table.") {
        REQUIRE(table_int_string.Insert(2, "y"));
        std::unordered_set<int> all_keys = table_int_string.GetAllKeys();
        REQUIRE(all_keys.size() == 2);
        REQUIRE(Contains(all_keys, 1));
        REQUIRE(Contains(all_keys, 2));
      }
    }

    WHEN("TableExists() called.") {
      THEN("Returns all keys in the table.") {
        REQUIRE(table_int_string.TableExists());
        REQUIRE(table_string_int.TableExists());
        REQUIRE(table_int_int.TableExists());
        REQUIRE(table_int_tokenlist.TableExists());
        REQUIRE(table_string_pair.TableExists());
      }
    }

    WHEN("ClearCallsTable() called.") {
      table_int_string.ClearTable();
      table_string_int.ClearTable();
      table_int_int.ClearTable();
      table_int_tokenlist.ClearTable();
      table_string_pair.ClearTable();

      THEN("Clear table to size 0.") {
        REQUIRE(table_int_string.IsEmpty());
        REQUIRE(table_string_int.IsEmpty());
        REQUIRE(table_int_int.IsEmpty());
        REQUIRE(table_int_tokenlist.IsEmpty());
        REQUIRE(table_string_pair.IsEmpty());
      }
    }
  }
}