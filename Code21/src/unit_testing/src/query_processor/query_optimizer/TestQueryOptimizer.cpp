#include "TestUtils.h"
#include "catch.hpp"
#include "query_processor/commons/query/clause/Clause.h"
#include "query_processor/query_optimizer/QueryOptimizer.h"

using namespace std;
using namespace query_processor;

SCENARIO("Test removal of repeated clauses", "[queryoptimizer]") {
  WHEN("Query contains only repeated clauses of the same clause") {
    Query query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    SuchThatClause parent_clause = SuchThatClause(DesignAbstraction::PARENT,
                                                  ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                  ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    query.AddClause(parent_clause);
    query.AddClause(parent_clause);
    query.AddClause(parent_clause);
    query.AddClause(parent_clause);
    query = QueryOptimizer::OptimizeQuery(query, true, false);
    THEN("Clause list for Parent(w, a) and Parent(w, a) and Parent(w, a) and Parent(w, a) should reduce to one clause") {
      REQUIRE(query.GetClauseList().size() == 1);
      REQUIRE(query.GetClauseList().front() == parent_clause);
    }
  }

  WHEN("Query contains some repeated clauses") {
    Query query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    SuchThatClause parent_clause_1 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    SuchThatClause parent_clause_2 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    SuchThatClause parent_clause_3 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")));

    query.AddClause(parent_clause_1);
    query.AddClause(parent_clause_1);
    query.AddClause(parent_clause_1);
    query.AddClause(parent_clause_2);
    query.AddClause(parent_clause_2);
    query.AddClause(parent_clause_3);
    query = QueryOptimizer::OptimizeQuery(query, true, false);
    THEN("Clause list reduces to three clauses") {
      REQUIRE(query.GetClauseList().size() == 3);
      REQUIRE(query.GetClauseList().at(0) == parent_clause_1);
      REQUIRE(query.GetClauseList().at(1) == parent_clause_2);
      REQUIRE(query.GetClauseList().at(2) == parent_clause_3);
    }
  }

  WHEN("Query contains no repeated clauses with clauses of params of similar design entity types but different synonym") {
    Query query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    SuchThatClause parent_clause_1 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    SuchThatClause parent_clause_2 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    SuchThatClause parent_clause_3 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")));
    SuchThatClause parent_clause_4 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")));

    query.AddClause(parent_clause_1);
    query.AddClause(parent_clause_2);
    query.AddClause(parent_clause_3);
    query.AddClause(parent_clause_4);
    query = QueryOptimizer::OptimizeQuery(query, true, false);
    THEN("Clause list for Parent(w, a) and Parent(w1, a) and Parent(w, a1) and Parent(w1, a1) should remain as four clauses") {
      REQUIRE(query.GetClauseList().size() == 4);
      REQUIRE(query.GetClauseList().at(0) == parent_clause_1);
      REQUIRE(query.GetClauseList().at(1) == parent_clause_2);
      REQUIRE(query.GetClauseList().at(2) == parent_clause_3);
      REQUIRE(query.GetClauseList().at(3) == parent_clause_4);
    }
  }
}

SCENARIO("Test sorting of clauses by number of synonyms", "[queryoptimizer]") {
  WHEN("Same type Clauses have different number of synonyms in the wrong order") {
    Query query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    SuchThatClause parent_clause_1 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(1),
                                                    ClauseParam(2));
    SuchThatClause parent_clause_2 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(1),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")));
    SuchThatClause parent_clause_3 = SuchThatClause(DesignAbstraction::PARENT,
                                                    ClauseParam(DesignEntity(DesignEntityType::WHILE, "w")),
                                                    ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1")));

    query.AddClause(parent_clause_3);
    query.AddClause(parent_clause_2);
    query.AddClause(parent_clause_1);
    query = QueryOptimizer::OptimizeQuery(query, false, true);
    THEN("Clause list should start with clause with no synonyms, followed by one, then two synonyms") {
      REQUIRE(query.GetClauseList().size() == 3);
      REQUIRE(query.GetClauseList().at(0) == parent_clause_1);
      REQUIRE(query.GetClauseList().at(1) == parent_clause_2);
      REQUIRE(query.GetClauseList().at(2) == parent_clause_3);
    }
  }

  WHEN("Clauses have same number of synonyms") {
    Query query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    SuchThatClause modifies_clause = SuchThatClause(DesignAbstraction::MODIFIES,
                                                    ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                    ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")));
    SuchThatClause follows_clause = SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                                   ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")));
    PatternClause pattern_clause = PatternClause(DesignEntity(DesignEntityType::ASSIGN, "a"),
                                                 ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v")),
                                                 ClauseParam(PatternExpression()));
    query.AddClause(modifies_clause);
    query.AddClause(follows_clause);
    query.AddClause(pattern_clause);
    query = QueryOptimizer::OptimizeQuery(query, false, true);
    THEN("Clause list should be sorted based on clause type") {
      REQUIRE(query.GetClauseList().size() == 3);
      REQUIRE(query.GetClauseList().at(0) == pattern_clause);
      REQUIRE(query.GetClauseList().at(1) == follows_clause);
      REQUIRE(query.GetClauseList().at(2) == modifies_clause);
    }
  }
}

SCENARIO("Test grouping of clauses using BFS", "[queryoptimizer]") {
  WHEN("Clauses all belong to the same group") {
    ResultTable table1;
    Column table1_w{TableElement(1), TableElement(1), TableElement(1)};
    Column table1_a{TableElement(2), TableElement(3), TableElement(4)};
    table1.AddColumn("w", table1_w);
    table1.AddColumn("a", table1_a);
    ResultTable table2;
    Column table2_a{TableElement(2), TableElement(4)};
    Column table2_s{TableElement(4), TableElement(5)};
    table2.AddColumn("s", table2_s);
    table2.AddColumn("a", table2_a);
    ResultTable table3;
    Column table3_v{TableElement("x"), TableElement("y")};
    Column table3_s{TableElement(4), TableElement(6)};
    table3.AddColumn("s", table3_s);
    table3.AddColumn("v", table3_v);
    vector<ResultTable> database{table1, table2, table3};
    vector<ResultTable> result = QueryOptimizer::OptimizeMerging(database, false, false);
    THEN("Resultant database should have one merged table") {
      REQUIRE(result.size() == 1);
      REQUIRE(result.front().GetHeight() == 1);
      REQUIRE(result.front().GetSize() == 4);
      REQUIRE(IsSimilarColumn(result.front().GetColumn("w"), {TableElement(1)}));
      REQUIRE(IsSimilarColumn(result.front().GetColumn("a"), {TableElement(2)}));
      REQUIRE(IsSimilarColumn(result.front().GetColumn("s"), {TableElement(4)}));
      REQUIRE(IsSimilarColumn(result.front().GetColumn("v"), {TableElement("x")}));
    }
  }

  WHEN("Clauses belong to different groups") {
    ResultTable table1;
    Column table1_s1{TableElement(1), TableElement(2)};
    Column table1_s2{TableElement(2), TableElement(3)};
    table1.AddColumn("s1", table1_s1);
    table1.AddColumn("s2", table1_s2);
    ResultTable table2;
    Column table2_s2{TableElement(2), TableElement(3)};
    Column table2_s3{TableElement(3), TableElement(4)};
    table2.AddColumn("s2", table2_s2);
    table2.AddColumn("s3", table2_s3);
    ResultTable table3;
    Column table3_s4{TableElement(1), TableElement(2)};
    Column table3_s5{TableElement(2), TableElement(3)};
    table3.AddColumn("s4", table3_s4);
    table3.AddColumn("s5", table3_s5);
    ResultTable table4;
    Column table4_s4{TableElement(2), TableElement(3)};
    Column table4_s5{TableElement(3), TableElement(4)};
    table4.AddColumn("s4", table4_s4);
    table4.AddColumn("s5", table4_s5);

    vector<ResultTable> database{table1, table2, table3, table4};
    vector<ResultTable> result = QueryOptimizer::OptimizeMerging(database, false, false);
    THEN("Resultant database should have one merged table") {
      REQUIRE(result.size() == 2);
      REQUIRE(result.front().GetHeight() == 2);
      REQUIRE(result.front().GetSize() == 3);
      REQUIRE(IsSimilarColumn(result.front().GetColumn("s1"), table1_s1));
      REQUIRE(IsSimilarColumn(result.front().GetColumn("s2"), table1_s2));
      REQUIRE(IsSimilarColumn(result.front().GetColumn("s3"), table2_s3));
      REQUIRE(result.at(1).GetHeight() == 1);
      REQUIRE(result.at(1).GetSize() == 2);
      REQUIRE(IsSimilarColumn(result.at(1).GetColumn("s4"), {TableElement(2)}));
      REQUIRE(IsSimilarColumn(result.at(1).GetColumn("s5"), {TableElement(3)}));
    }
  }
}
