#include "TestUtils.h"
#include "catch.hpp"
#include "query_processor/query_evaluator/utils/QueryEvaluatorUtils.h"

using namespace std;
using namespace query_processor;

SCENARIO("Test Conversion to Query Result") {
  WHEN("Query Result requires a tuple") {
    Column a_col{TableElement(1), TableElement(2), TableElement(3)};
    Column a1_col{TableElement(4), TableElement(5), TableElement(6)};
    Column v_col{TableElement("x"), TableElement("y"), TableElement("z")};
    ResultTable table;
    table.AddColumn("a", a_col);
    table.AddColumn("a1", a1_col);
    table.AddColumn("v", v_col);
    std::vector<SelectedEntity> selected_tuple{SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")), SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "v"))};
    QueryResult test_result = QueryEvaluatorUtils::ConvertResultTableToTupleResult(table, selected_tuple);
    THEN("Selected tuple <a, v> returns correct values") {
      REQUIRE(Contains(test_result.tuple_result, vector<string>{"1", "x"}));
      REQUIRE(Contains(test_result.tuple_result, vector<string>{"2", "y"}));
      REQUIRE(Contains(test_result.tuple_result, vector<string>{"3", "z"}));
    }
  }
}

SCENARIO("Test RemoveDuplicateElements") {
  WHEN("Column has duplicates") {
    Column a_col{TableElement(1), TableElement(2), TableElement(3),
                 TableElement(1), TableElement(2), TableElement(3),
                 (1), TableElement(2), TableElement(3)};
    Column result_col = QueryEvaluatorUtils::RemoveDuplicateTableElements(a_col);
    THEN("Resultant column only has 3 elements") {
      REQUIRE(result_col.size() == 3);
      REQUIRE(Contains(result_col, 1));
      REQUIRE(Contains(result_col, 2));
      REQUIRE(Contains(result_col, 3));
    }
  }

  WHEN("Column has no duplicates") {
    Column a_col{TableElement(1), TableElement(2), TableElement(3)};
    Column result_col = QueryEvaluatorUtils::RemoveDuplicateTableElements(a_col);
    THEN("Resultant column only has 3 elements") {
      REQUIRE(result_col.size() == 3);
      REQUIRE(Contains(result_col, 1));
      REQUIRE(Contains(result_col, 2));
      REQUIRE(Contains(result_col, 3));
    }
  }

  WHEN("Column is empty") {
    Column empty_col;
    Column result_col = QueryEvaluatorUtils::RemoveDuplicateTableElements(empty_col);
    THEN("Resultant column is empty") {
      REQUIRE(result_col.empty());
    }
  }

  WHEN("Invalid column has mixed attributes") {
    Column invalid_col{TableElement(1), TableElement("x")};
    THEN("Error will be thrown") {
      REQUIRE_THROWS(QueryEvaluatorUtils::RemoveDuplicateTableElements(invalid_col));
    }
  }
}