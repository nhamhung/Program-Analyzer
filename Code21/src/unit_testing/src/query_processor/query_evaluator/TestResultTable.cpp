#include "TestUtils.h"
#include "catch.hpp"
#include "query_processor/query_evaluator/ResultTable.h"
#include "query_processor/query_evaluator/utils/QueryEvaluatorUtils.h"

using namespace std;
using namespace query_processor;

SCENARIO("Test CrossTable") {
  WHEN("Two tables with single columns are crossed") {
    ResultTable table1 = CreateSingleColumnResultTable("a", unordered_set<int>{1, 2, 3});
    ResultTable table2 = CreateSingleColumnResultTable("w", unordered_set<int>{4, 5, 6});
    ResultTable table1_2 = table1.CrossTable(table2);
    THEN("Resultant table should be a cross product of the first two tables") {
      REQUIRE(table1_2.GetSize() == 2);
      REQUIRE(table1_2.GetHeight() == 9);
      Column col1 = table1_2.GetColumn("a");
      Column col2 = table1_2.GetColumn("w");
      REQUIRE(Contains(col1, 1));
      REQUIRE(Contains(col1, 2));
      REQUIRE(Contains(col1, 3));
      REQUIRE(Contains(col2, 4));
      REQUIRE(Contains(col2, 5));
      REQUIRE(Contains(col2, 6));
    }
  }
  WHEN("Two tables with multiple columns are crossed correctly") {
    ResultTable table1 = CreateSingleColumnResultTable("a", unordered_set<int>{1, 2, 3});
    Column s_column;
    s_column.push_back(TableElement(2));
    s_column.push_back(TableElement(3));
    s_column.push_back(TableElement(4));
    table1.AddColumn("s", s_column);
    ResultTable table2 = CreateSingleColumnResultTable("w", unordered_set<int>{4, 5, 6});
    ResultTable table1_2 = table1.CrossTable(table2);
    THEN("Resultant table should be a cross product of the first two tables") {
      REQUIRE(table1_2.GetSize() == 3);
      REQUIRE(table1_2.GetHeight() == 9);
      Column col1 = table1_2.GetColumn("a");
      Column col2 = table1_2.GetColumn("w");
      Column col3 = table1_2.GetColumn("s");
      REQUIRE(Contains(col1, 1));
      REQUIRE(Contains(col1, 2));
      REQUIRE(Contains(col1, 3));
      REQUIRE(Contains(col2, 4));
      REQUIRE(Contains(col2, 5));
      REQUIRE(Contains(col2, 6));
      REQUIRE(Contains(col3, 2));
      REQUIRE(Contains(col3, 3));
      REQUIRE(Contains(col3, 4));
    }
  }
}

SCENARIO("Test Merge Table") {
  WHEN("Merge Table on two empty tables") {
    ResultTable table1;
    ResultTable table2;
    ResultTable result = table1.MergeTable(table2);
    THEN("Resultant table should be empty") {
      REQUIRE(result.IsEmpty());
      REQUIRE(result.GetHeight() == 0);
    }
  }

  WHEN("Merge Table on one empty table and one table") {
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(2)};
    Column table1_a{TableElement(2), TableElement(3), TableElement(3)};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("a", table1_a);

    ResultTable table2;
    ResultTable result = table1.MergeTable(table2);

    THEN("Resultant table should be table 1") {
      REQUIRE(result.GetHeight() == 3);
      REQUIRE(result.GetSize() == 2);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), table1_w));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), table1_a));
    }
  }

  WHEN("Merge Table with no overlapping columns") {
    /*
        * The inner join of the following result table on "a" should return:
        * w s    a  v      -->     w s a v
        * 1 2    2  x              1 2 2 x
        * 1 3    3  y              1 2 3 y
        *                          1 3 2 x
         *                         1 3 3 y
        */
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1)};
    Column table1_s{TableElement(2), TableElement(3)};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("s", table1_s);

    ResultTable table2;
    Column table2_a{TableElement(2), TableElement(3)};
    Column table2_v{TableElement("x"), TableElement("y")};
    table2.AddColumn("a", table2_a);
    table2.AddColumn("v", table2_v);

    ResultTable result = table1.MergeTable(table2);
    THEN("Result table should be cross product of the two tables should occur") {
      REQUIRE(result.GetHeight() == 4);
      REQUIRE(result.GetSize() == 4);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), Column{TableElement(1), TableElement(1), TableElement(1), TableElement(1)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), Column{TableElement(2), TableElement(3), TableElement(2), TableElement(3)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("s"), Column{TableElement(2), TableElement(2), TableElement(3), TableElement(3)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("v"), Column{TableElement("x"), TableElement("y"), TableElement("x"), TableElement("y")}));
    }
  }
  WHEN("Merge Table on two tables with only one overlapping int column") {
    /*
         * The inner join of the following result table on "a" should return:
         * w a    a  v      -->     w a v
         * 1 2    2  x              1 2 x
         * 1 3    3  y              1 3 y
         * 2 3                      2 3 y
         */
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(2)};
    Column table1_a{TableElement(2), TableElement(3), TableElement(3)};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("a", table1_a);

    ResultTable table2;
    Column table2_a{TableElement(2), TableElement(3)};
    Column table2_v{TableElement("x"), TableElement("y")};
    table2.AddColumn("a", table2_a);
    table2.AddColumn("v", table2_v);

    ResultTable result = table1.MergeTable(table2);
    THEN("Resultant table should be similar to merged table as per comments above") {
      REQUIRE(result.GetHeight() == 3);
      REQUIRE(result.GetSize() == 3);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), table1_w));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), table1_a));
      REQUIRE(IsSimilarColumn(result.GetColumn("v"), Column{TableElement("x"), TableElement("y"), TableElement("y")}));
    }
  }

  WHEN("Merge Table on two tables with only one overlapping string column") {
    /*
         * The inner join of the following result table on "v" should return:
         * w v    v  a      -->     w v a
         * 1 x    x  2              1 x 2
         * 1 y    y  3              2 x 2
         * 2 x                      1 y 3
         */
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(2)};
    Column table1_v{TableElement("x"), TableElement("y"), TableElement("x")};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("v", table1_v);

    ResultTable table2;
    Column table2_v{TableElement("x"), TableElement("y")};
    Column table2_a{TableElement(2), TableElement(3)};
    table2.AddColumn("v", table2_v);
    table2.AddColumn("a", table2_a);

    ResultTable result = table1.MergeTable(table2);
    THEN("Resultant table should be similar to merged table as per comments above") {
      REQUIRE(result.GetHeight() == 3);
      REQUIRE(result.GetSize() == 3);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), Column{TableElement(1), TableElement(2), TableElement(1)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("v"), Column{TableElement("x"), TableElement("x"), TableElement("y")}));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), Column{{TableElement(2), TableElement(2), TableElement(3)}}));
    }
  }

  WHEN("Merge Table on two tables with only two overlapping columns") {
    /*
         * The inner join of the following result table on "v" should return:
         * w a    w  a      -->     w a
         * 1 2    1  2              1 2
         * 1 3    2  3              2 4
         * 2 4    2  4
         */
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(2)};
    Column table1_a{TableElement(2), TableElement(3), TableElement(4)};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("a", table1_a);

    ResultTable table2;
    Column table2_w{TableElement(1), TableElement(2), TableElement(2)};
    Column table2_a{TableElement(2), TableElement(3), TableElement(4)};
    table2.AddColumn("w", table2_w);
    table2.AddColumn("a", table2_a);

    ResultTable result = table1.MergeTable(table2);
    THEN("Resultant table should be similar to merged table as per comments above") {
      REQUIRE(result.GetHeight() == 2);
      REQUIRE(result.GetSize() == 2);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), Column{TableElement(1), TableElement(2)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), Column{TableElement(2), TableElement(4)}));
    }
  }

  WHEN("Merge Table on two tables with two overlapping columns and different columns") {
    /*
         * The inner join of the following result table on "v" should return:
         * w a v    w  a  s    -->     w a v s
         * 1 2 x    1  2  7            1 2 x 7
         * 1 3 y    2  3  8            2 4 y 9
         * 2 4 y    2  4  9
         */
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(2)};
    Column table1_a{TableElement(2), TableElement(3), TableElement(4)};
    Column table1_v{TableElement("x"), TableElement("y"), TableElement("y")};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("a", table1_a);
    table1.AddColumn("v", table1_v);

    ResultTable table2;
    Column table2_w{TableElement(1), TableElement(2), TableElement(2)};
    Column table2_a{TableElement(2), TableElement(3), TableElement(4)};
    Column table2_s{TableElement(7), TableElement(8), TableElement(9)};
    table2.AddColumn("w", table2_w);
    table2.AddColumn("a", table2_a);
    table2.AddColumn("s", table2_s);

    ResultTable result = table1.MergeTable(table2);
    THEN("Resultant table should be similar to merged table as per comments above") {
      REQUIRE(result.GetHeight() == 2);
      REQUIRE(result.GetSize() == 4);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), Column{TableElement(1), TableElement(2)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), Column{TableElement(2), TableElement(4)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("v"), Column{TableElement("x"), TableElement("y")}));
      REQUIRE(IsSimilarColumn(result.GetColumn("s"), Column{TableElement(7), TableElement(9)}));
    }
  }
  WHEN("Merge Table on two tables with two overlapping columns and different columns") {
    /*
         * The inner join of the following result table on "v" should return:
         * w a v    w  a  s    -->     w a v s
         * 1 2 x    1  2  7            1 2 x 7
         * 1 3 y    2  3  8            2 4 y 9
         * 2 4 y    2  4  9
         */
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(2)};
    Column table1_a{TableElement(2), TableElement(3), TableElement(4)};
    Column table1_v{TableElement("x"), TableElement("y"), TableElement("y")};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("a", table1_a);
    table1.AddColumn("v", table1_v);

    ResultTable table2;
    Column table2_w{TableElement(1), TableElement(2), TableElement(2)};
    Column table2_a{TableElement(2), TableElement(3), TableElement(4)};
    Column table2_s{TableElement(7), TableElement(8), TableElement(9)};
    table2.AddColumn("w", table2_w);
    table2.AddColumn("a", table2_a);
    table2.AddColumn("s", table2_s);

    ResultTable result = table1.MergeTable(table2);
    THEN("Resultant table should be similar to merged table as per comments above") {
      REQUIRE(result.GetHeight() == 2);
      REQUIRE(result.GetSize() == 4);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), Column{TableElement(1), TableElement(2)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), Column{TableElement(2), TableElement(4)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("v"), Column{TableElement("x"), TableElement("y")}));
      REQUIRE(IsSimilarColumn(result.GetColumn("s"), Column{TableElement(7), TableElement(9)}));
    }
  }
  WHEN("Merge Table on two tables with two overlapping columns and different columns") {
    /*
         * The inner join of the following result table on "v" should return:
         * w a v    w  a  s    -->     w a v s
         * 1 2 x    1  2  7            1 2 x 7
         * 1 3 y    2  3  8            2 4 y 9
         * 2 4 y    2  4  9
         */
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(2)};
    Column table1_a{TableElement(2), TableElement(3), TableElement(4)};
    Column table1_v{TableElement("x"), TableElement("y"), TableElement("y")};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("a", table1_a);
    table1.AddColumn("v", table1_v);

    ResultTable table2;
    Column table2_w{TableElement(1), TableElement(2), TableElement(2)};
    Column table2_a{TableElement(2), TableElement(3), TableElement(4)};
    Column table2_s{TableElement(7), TableElement(8), TableElement(9)};
    table2.AddColumn("w", table2_w);
    table2.AddColumn("a", table2_a);
    table2.AddColumn("s", table2_s);

    ResultTable result = table1.MergeTable(table2);
    THEN("Resultant table should be similar to merged table as per comments above") {
      REQUIRE(result.GetHeight() == 2);
      REQUIRE(result.GetSize() == 4);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), Column{TableElement(1), TableElement(2)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), Column{TableElement(2), TableElement(4)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("v"), Column{TableElement("x"), TableElement("y")}));
      REQUIRE(IsSimilarColumn(result.GetColumn("s"), Column{TableElement(7), TableElement(9)}));
    }
  }
  WHEN("Merge three tables with overlapping columns") {
    /*
         * The inner join of the following result table on "v" should return:
         * w  a    w  v   -->  w  a  v     w  a  s    -->     w  a  v  s
         * 1  2    2  x        2  4  x     1  2  7            2  4  x  9
         * 1  3    2  y        2  4  y     2  3  8            2  4  y  9
         * 2  4                            2  4  9
         */
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(2)};
    Column table1_a{TableElement(2), TableElement(3), TableElement(4)};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("a", table1_a);

    ResultTable table2;
    Column table2_w{TableElement(2), TableElement(2)};
    Column table2_v{TableElement("x"), TableElement("y")};
    table2.AddColumn("w", table2_w);
    table2.AddColumn("v", table2_v);

    ResultTable table3;
    Column table3_w{TableElement(1), TableElement(2), TableElement(2)};
    Column table3_a{TableElement(2), TableElement(3), TableElement(4)};
    Column table3_s{TableElement(7), TableElement(8), TableElement(9)};
    table3.AddColumn("w", table3_w);
    table3.AddColumn("a", table3_a);
    table3.AddColumn("s", table3_s);

    ResultTable result = table1.MergeTable(table2);
    result = result.MergeTable(table3);
    THEN("Resultant table should be similar to merged table as per comments above") {
      REQUIRE(result.GetHeight() == 2);
      REQUIRE(result.GetSize() == 4);
      REQUIRE(IsSimilarColumn(result.GetColumn("w"), Column{TableElement(2), TableElement(2)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("a"), Column{TableElement(4), TableElement(4)}));
      REQUIRE(IsSimilarColumn(result.GetColumn("v"), Column{TableElement("x"), TableElement("y")}));
      REQUIRE(IsSimilarColumn(result.GetColumn("s"), Column{TableElement(9), TableElement(9)}));
    }
  }
}