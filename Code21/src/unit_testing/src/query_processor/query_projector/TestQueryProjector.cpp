#include "TestUtils.h"
#include "catch.hpp"
#include "query_processor/query_projector/QueryProjector.h"

using namespace std;
using namespace query_processor;

SCENARIO("QueryResult contains valid raw data") {
  WHEN("Raw data comprises of statement indexes") {
    unordered_set<int> valid_indexes = {1, 2, 3};
    QueryResult valid_statements = QueryResult(valid_indexes);
    list<string> result_list = QueryProjector::FormatResult(valid_statements);
    THEN("QueryResult list corresponds to indexes") {
      REQUIRE(Contains(result_list, "1"));
      REQUIRE(Contains(result_list, "2"));
      REQUIRE(Contains(result_list, "3"));
    }
  }
  WHEN("Raw data comprises of variable or procedure names") {
    unordered_set<string> valid_names = {"computeCentroid", "main", "funX"};
    QueryResult valid_vars = QueryResult(valid_names);
    list<string> result_list = QueryProjector::FormatResult(valid_vars);

    THEN("QueryResult string corresponds to variable names") {
      REQUIRE(Contains(result_list, "computeCentroid"));
      REQUIRE(Contains(result_list, "main"));
      REQUIRE(Contains(result_list, "funX"));
    }
  }
  WHEN("Raw data comprises of constants") {
    unordered_set<int> valid_constants = {0, 1, 1000};
    QueryResult valid_const = QueryResult(valid_constants);
    list<string> result_list = QueryProjector::FormatResult(valid_const);

    THEN("QueryResult string corresponds to constants' values") {
      REQUIRE(Contains(result_list, "0"));
      REQUIRE(Contains(result_list, "1"));
      REQUIRE(Contains(result_list, "1000"));
    }
  }

  WHEN("Raw data is a boolean value") {
    bool boolean_result = true;
    QueryResult valid_bool = QueryResult(boolean_result);
    list<string> result_list = QueryProjector::FormatResult(valid_bool);

    THEN("QueryResult list contains a single boolean value") {
      REQUIRE(result_list.front() == "TRUE");
    }
  }

  WHEN("Raw data comprises of sets of tuples") {
    vector<vector<string>> tuple_result{vector<string>{"1", "x"}, vector<string>{"2", "y"}, vector<string>{"3", "z"}};
    QueryResult valid_tuple = QueryResult(tuple_result);
    list<string> result_list = QueryProjector::FormatResult(valid_tuple);
    THEN("QueryResult list contains a list of tuple strings") {
      REQUIRE(Contains(result_list, "1 x"));
      REQUIRE(Contains(result_list, "2 y"));
      REQUIRE(Contains(result_list, "3 z"));
    }
  }
}