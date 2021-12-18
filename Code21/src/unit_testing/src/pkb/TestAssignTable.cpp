#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/entity_tables/AssignTable.h"

using namespace source_processor;

SCENARIO("Construct a new AssignTable.") {
  AssignTable assign_table;

  GIVEN("An assign_table has just been constructed.") {
    THEN("assign_table exists with size 0.") {
      REQUIRE(assign_table.GetAssignedTable().IsEmpty());
      REQUIRE(assign_table.GetAssignTable().IsEmpty());
      REQUIRE(assign_table.GetInverseAssignedTable().IsEmpty());
    }
  }
}

SCENARIO("AssignTable has been constructed.") {
  AssignTable assign_table;
  Token plus("+", TokenType::ExpressionOp);
  Token div("/", TokenType::ExpressionOp);
  Token mult("*", TokenType::ExpressionOp);
  Token count("count", TokenType::VariableName);
  Token cenX("cenX", TokenType::VariableName);
  Token cenY("cenY", TokenType::VariableName);
  Token x("x", TokenType::VariableName);
  Token y("y", TokenType::VariableName);
  Token one("1", TokenType::ConstantValue);
  Token zero("0", TokenType::ConstantValue);

  TokenList tokens1;
  TokenList tokens10;
  TokenList tokens11;
  TokenList tokens12;
  TokenList tokens15;
  TokenList tokens16;
  TokenList tokens17;
  TokenList tokens20;
  TokenList tokens21;
  TokenList tokens22;
  TokenList tokens23;

  tokens1.Push(zero);
  tokens10.Push(zero);
  tokens11.Push(zero);
  tokens12.Push(zero);
  tokens15.Push(count).Push(plus).Push(one);
  tokens16.Push(cenX).Push(plus).Push(x);
  tokens17.Push(cenY).Push(plus).Push(y);
  tokens20.Push(one);
  tokens21.Push(cenX).Push(div).Push(count);
  tokens22.Push(cenY).Push(div).Push(count);
  tokens23.Push(cenX).Push(mult).Push(cenX).Push(plus).Push(cenY).Push(mult).Push(cenY);

  GIVEN("AssignTable is currently empty.") {
    WHEN("Inserts to assign_table. Index is 0 or less.") {
      THEN("Insertion returns False.") {
        REQUIRE(assign_table.InsertAssign(0, "cenX", tokens21) == false);
      }
    }

    WHEN("Inserts to assign_table. Index is greater than 0.") {
      THEN("Insertion returns True. assign_table is populated") {
        REQUIRE(assign_table.InsertAssign(21, "cenX", tokens21));
        REQUIRE(assign_table.InsertAssign(22, "cenY", tokens22));
        REQUIRE(assign_table.GetAssignTable().Size() == 2);
        REQUIRE(assign_table.GetAssignTable().Get(21) == tokens21);
        REQUIRE(assign_table.GetAssignTable().Get(22) == tokens22);
        REQUIRE(assign_table.GetAssignedTable().Size() == 2);
        REQUIRE(assign_table.GetAssignedTable().Get(21) == "cenX");
        REQUIRE(assign_table.GetAssignedTable().Get(22) == "cenY");
        REQUIRE(Contains(assign_table.GetInverseAssignedTable().Get("cenX"), 21));
        REQUIRE(Contains(assign_table.GetInverseAssignedTable().Get("cenY"), 22));
      }
    }
  }

  GIVEN("AssignTable is not empty.") {
    REQUIRE(assign_table.InsertAssign(21, "cenX", tokens21));
    REQUIRE(assign_table.InsertAssign(22, "cenY", tokens22));
    REQUIRE(assign_table.InsertAssign(23, "normSq", tokens23));

    WHEN("InsertAssign() inserts invalid indexes or statement indexes that are already stored.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(assign_table.InsertAssign(0, "c", tokens1));
        REQUIRE_FALSE(assign_table.InsertAssign(21, "c", tokens1));
        REQUIRE_FALSE(assign_table.InsertAssign(22, "c", tokens1));
        REQUIRE(assign_table.GetAssignedTable().Size() == 3);
        REQUIRE(assign_table.GetAssignTable().Size() == 3);
        REQUIRE(assign_table.GetInverseAssignedTable().Size() == 3);
      }
    }

    WHEN("GetAllAssignStmts from assign_table.") {
      THEN("Returns a set of all assign statement indexes.") {
        std::unordered_set<int> all_assign_stmts = assign_table.GetAllAssignStmts();
        REQUIRE(all_assign_stmts.size() == 3);
        REQUIRE(ContainsExactly(all_assign_stmts, {21, 22, 23}));
      }
    }

    WHEN("Get the modified variable given a statement index.") {
      THEN("Returns the modified variable in a statement") {
        REQUIRE(assign_table.GetAssignedVariable(21) == "cenX");
        REQUIRE(assign_table.GetAssignedVariable(22) == "cenY");
        REQUIRE(assign_table.GetAssignedVariable(23) == "normSq");
        REQUIRE(assign_table.GetAssignedVariable(1) == "");
      }
    }

    WHEN("GetAllAssignedVariables() called.") {
      THEN("Returns a set of all assigned variables.") {
        std::unordered_set<std::string> all_assigned_variables = assign_table.GetAllAssignedVariables();
        REQUIRE(all_assigned_variables.size() == 3);
        REQUIRE(Contains(all_assigned_variables, "cenX"));
        REQUIRE(Contains(all_assigned_variables, "cenY"));
        REQUIRE(Contains(all_assigned_variables, "normSq"));
      }
    }

    WHEN("GetAllAssignStmtsThatModifies(variable) called.") {
      REQUIRE(assign_table.InsertAssign(16, "cenX", tokens16));

      THEN("Returns a set of all assign statements that modifies the variable.") {
        std::unordered_set<int> all_assign_statements_that_modifies_cenX = assign_table.GetAllAssignStmtsThatModifies("cenX");
        std::unordered_set<int> all_assign_statements_that_modifies_cenY = assign_table.GetAllAssignStmtsThatModifies("cenY");
        REQUIRE(all_assign_statements_that_modifies_cenX.size() == 2);
        REQUIRE(all_assign_statements_that_modifies_cenY.size() == 1);
        REQUIRE(ContainsExactly(all_assign_statements_that_modifies_cenX, {16, 21}));
        REQUIRE(Contains(all_assign_statements_that_modifies_cenY, 22));
      }
    }

    WHEN("GetAllAssignStmtsThatMatches(token_list).") {
      THEN("Returns a set of all assign statements that have RHS exactly matches an expression.") {
        TokenList token_list_matches_21;
        token_list_matches_21.Push(cenX).Push(div).Push(count);
        std::unordered_set<int> all_assign_stmts_that_matches = assign_table.GetAllAssignStmtsThatMatches(token_list_matches_21);
        REQUIRE(all_assign_stmts_that_matches.size() == 1);
        REQUIRE(Contains(all_assign_stmts_that_matches, 21));
      }
    }

    WHEN("GetAllAssignStmtsThatContains(token_list) and GetAllAssignStmtsThatMatches(token_list).") {
      REQUIRE(assign_table.InsertAssign(1, {"flag"}, tokens1));
      REQUIRE(assign_table.InsertAssign(10, {"count"}, tokens10));
      REQUIRE(assign_table.InsertAssign(11, {"cenX"}, tokens11));
      REQUIRE(assign_table.InsertAssign(12, {"cenY"}, tokens12));
      REQUIRE(assign_table.InsertAssign(15, {"count"}, tokens15));
      REQUIRE(assign_table.InsertAssign(16, {"cenX"}, tokens16));
      REQUIRE(assign_table.InsertAssign(17, {"cenY"}, tokens17));
      REQUIRE(assign_table.InsertAssign(20, {"flag"}, tokens20));

      TokenList rhs_variable_x_sub_expression;
      TokenList rhs_variable_cenX_sub_expression;
      TokenList rhs_constant_0_sub_expression;

      Token cenX("cenX", TokenType::VariableName);
      Token x("x", TokenType::VariableName);
      Token zero("0", TokenType::ConstantValue);

      rhs_constant_0_sub_expression.Push(zero);
      rhs_variable_x_sub_expression.Push(x);
      rhs_variable_cenX_sub_expression.Push(cenX);

      THEN("Returns a set of all assign statements that have RHS contains a sub-expression.") {
        std::unordered_set<int> all_assign_stmts_contains_0 = assign_table.GetAllAssignStmtsThatContains(rhs_constant_0_sub_expression);
        std::unordered_set<int> all_assign_stmts_contains_x = assign_table.GetAllAssignStmtsThatContains(rhs_variable_x_sub_expression);
        std::unordered_set<int> all_assign_stmts_contains_cenX = assign_table.GetAllAssignStmtsThatContains(rhs_variable_cenX_sub_expression);
        REQUIRE(all_assign_stmts_contains_0.size() == 4);
        REQUIRE(ContainsExactly(all_assign_stmts_contains_0, {1, 10, 11, 12}));
        REQUIRE(all_assign_stmts_contains_x.size() == 1);
        REQUIRE(Contains(all_assign_stmts_contains_x, 16));
        REQUIRE(all_assign_stmts_contains_cenX.size() == 3);
        REQUIRE(ContainsExactly(all_assign_stmts_contains_cenX, {16, 21, 23}));

        std::unordered_set<int> all_assign_stmts_matches_0 = assign_table.GetAllAssignStmtsThatMatches(rhs_constant_0_sub_expression);
        std::unordered_set<int> all_assign_stmts_matches_cenX_div_count = assign_table.GetAllAssignStmtsThatMatches(tokens21);
        std::unordered_set<int> all_assign_stmts_matches_cenY_div_count = assign_table.GetAllAssignStmtsThatMatches(tokens22);
        REQUIRE(ContainsExactly(all_assign_stmts_matches_0, {1, 10, 11, 12}));
        REQUIRE(ContainsExactly(all_assign_stmts_matches_cenX_div_count, {21}));
        REQUIRE(ContainsExactly(all_assign_stmts_matches_cenY_div_count, {22}));
      }
    }
  }
}