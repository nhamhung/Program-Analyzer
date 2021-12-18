#include "BuildPKBUtils.h"
#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "source_processor/token/TokenList.h"

using namespace source_processor;

SCENARIO("Construct a new PKB pkb.") {
  GIVEN("A pkb has just been constructed.") {
    PKB pkb;

    THEN("pkb exists with default tables.") {
      REQUIRE(pkb.GetAllVariables().empty());
      REQUIRE(pkb.GetAllStmts().empty());
      REQUIRE(pkb.GetAllConstants().empty());
      REQUIRE(pkb.GetAllIfStmts().empty());
      REQUIRE(pkb.GetAllWhileStmts().empty());
      REQUIRE(pkb.GetAllReadStmts().empty());
      REQUIRE(pkb.GetAllPrintStmts().empty());
      REQUIRE(pkb.GetAllEntities().empty());
      REQUIRE(pkb.GetAllCallsStmts().empty());
      REQUIRE(pkb.GetAllAssignStmts().empty());
      REQUIRE(pkb.GetAllProcedures().empty());
      REQUIRE(pkb.GetAllFollowsStmts().empty());
      REQUIRE(pkb.GetAllFollowedStmts().empty());
      REQUIRE(pkb.GetAllFollowsTStmts().empty());
      REQUIRE(pkb.GetAllFollowedTStmts().empty());
      REQUIRE(pkb.GetAllParentStmts().empty());
      REQUIRE(pkb.GetAllParentTStmts().empty());
      REQUIRE(pkb.GetAllChildrenStmts().empty());
      REQUIRE(pkb.GetAllChildrenTStmts().empty());
      REQUIRE(pkb.GetAllModifiesStatements().empty());
      REQUIRE(pkb.GetAllUsesStatements().empty());
    }
  }
}

SCENARIO("Populate a pkb.") {
  PKB pkb;

  GIVEN("pkb.InsertVariable(variable) called.") {
    WHEN("Inserts non-existing variables into var_table.") {
      THEN("Insertion returns true. Stores the variable in var_table.") {
        REQUIRE(pkb.InsertVariable("x"));
        REQUIRE(pkb.InsertVariable("y"));
        REQUIRE(pkb.GetAllVariables().size() == 2);
        REQUIRE(Contains(pkb.GetAllVariables(), "x"));
        REQUIRE(Contains(pkb.GetAllVariables(), "y"));
      }
    }

    WHEN("Inserts existing variables into var_table.") {
      THEN("Insertion returns False.") {
        REQUIRE(pkb.InsertVariable("x"));
        REQUIRE(pkb.InsertVariable("y"));
        REQUIRE_FALSE(pkb.InsertVariable("x"));
        REQUIRE(pkb.GetAllVariables().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllVariables(), {"x", "y"}));
      }
    }
  }

  GIVEN("pkb.InsertStatement(stmt_index) called.") {
    REQUIRE(pkb.InsertStatement(1));
    REQUIRE(pkb.InsertStatement(2));

    WHEN("Inserts existing statement numbers into stmt_table.") {
      THEN("Insertion returns False.") {
        REQUIRE(pkb.InsertStatement(1) == false);
        REQUIRE(pkb.GetAllStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllStmts(), {1, 2}));
      }
    }

    WHEN("Inserts non-existing statement numbers into stmt_table.") {
      THEN("Insertion returns True. Statement index is stored in stmt_table.") {
        REQUIRE(pkb.InsertStatement(3));
        REQUIRE(pkb.GetAllStmts().size() == 3);
        REQUIRE(Contains(pkb.GetAllStmts(), 3));
      }
    }

    WHEN("Gets all statements from stmt_table.") {
      THEN("Returns the unique set of all statement indexes stored in stmt_table.") {
        REQUIRE(pkb.GetAllStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllStmts(), {1, 2}));
      }
    }
  }

  GIVEN("pkb.InsertAssignment(stmt_index, left_var, token_list) called.") {
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

    REQUIRE(pkb.InsertAssignment(1, {"flag"}, tokens1));
    REQUIRE(pkb.InsertAssignment(10, {"count"}, tokens10));
    REQUIRE(pkb.InsertAssignment(11, {"cenX"}, tokens11));
    REQUIRE(pkb.InsertAssignment(12, {"cenY"}, tokens12));
    REQUIRE(pkb.InsertAssignment(15, {"count"}, tokens15));
    REQUIRE(pkb.InsertAssignment(16, {"cenX"}, tokens16));
    REQUIRE(pkb.InsertAssignment(17, {"cenY"}, tokens17));
    REQUIRE(pkb.InsertAssignment(20, {"flag"}, tokens20));
    REQUIRE(pkb.InsertAssignment(21, {"cenX"}, tokens21));
    REQUIRE(pkb.InsertAssignment(22, {"cenY"}, tokens22));
    REQUIRE(pkb.InsertAssignment(23, {"normSq"}, tokens23));

    WHEN("Inserts assignment into assign_table at different statement indexes.") {
      THEN("Stores the statement index, modified variable and token list into assign_table.") {
        REQUIRE(pkb.GetAllAssignStmts().size() == 11);
        REQUIRE(ContainsExactly(pkb.GetAllAssignStmts(), {1, 10, 11, 12, 15, 16, 17, 20, 21, 22, 23}));
      }
    }

    WHEN("Inserts assignment into assign_table at same statement index.") {
      THEN("Insertion returns false.") {
        REQUIRE_FALSE(pkb.InsertAssignment(20, "flag", tokens1));
      }
    }

    WHEN("Inserts assignment into assign_table at invalid statement index 0.") {
      THEN("Insertion returns false.") {
        REQUIRE_FALSE(pkb.InsertAssignment(0, "flag", tokens1));
      }
    }

    WHEN("Get the assigned variable at an assignment statement.") {
      THEN("Returns the variable assigned.") {
        REQUIRE(pkb.GetAssignedVariable(16) == "cenX");
        REQUIRE(pkb.GetAssignedVariable(21) == "cenX");
        REQUIRE(pkb.GetAssignedVariable(22) == "cenY");
      }
    }

    WHEN("GetAllAssignStmtsThatMatches(rhs_expression).") {
      TokenList rhs_cenX_div_count_expression;
      TokenList rhs_0_expression;
      rhs_cenX_div_count_expression.Push(cenX).Push(div).Push(count);
      rhs_0_expression.Push(zero);
      std::unordered_set<int> all_assign_stmts_cenX_div_count = pkb.GetAllAssignStmtsThatMatches(rhs_cenX_div_count_expression);
      std::unordered_set<int> all_assign_stmts_0 = pkb.GetAllAssignStmtsThatMatches(rhs_0_expression);

      THEN("Returns a set of all assign statements that matches exactly with the rhs_expression.") {
        REQUIRE(all_assign_stmts_cenX_div_count.size() == 1);
        REQUIRE(Contains(all_assign_stmts_cenX_div_count, 21));
        REQUIRE(all_assign_stmts_0.size() == 4);
        REQUIRE(ContainsExactly(all_assign_stmts_0, {1, 10, 11, 12}));
      }
    }

    WHEN("GetAllAssignStmtsThatContains(rhs_sub_expression).") {
      TokenList rhs_variable_x_sub_expression;
      TokenList rhs_variable_cenX_sub_expression;
      TokenList rhs_constant_0_sub_expression;

      Token cenX("cenX", TokenType::VariableName);
      Token x("x", TokenType::VariableName);
      Token zero("0", TokenType::ConstantValue);

      rhs_constant_0_sub_expression.Push(zero);
      rhs_variable_x_sub_expression.Push(x);
      rhs_variable_cenX_sub_expression.Push(cenX);

      THEN("Returns a set of all assign statements that matches exactly with the rhs_expression.") {
        std::unordered_set<int> all_assign_stmts_contains_0 = pkb.GetAllAssignStmtsThatContains(rhs_constant_0_sub_expression);
        std::unordered_set<int> all_assign_stmts_contains_x = pkb.GetAllAssignStmtsThatContains(rhs_variable_x_sub_expression);
        std::unordered_set<int> all_assign_stmts_contains_cenX = pkb.GetAllAssignStmtsThatContains(rhs_variable_cenX_sub_expression);
        REQUIRE(all_assign_stmts_contains_0.size() == 4);
        REQUIRE(ContainsExactly(all_assign_stmts_contains_0, {1, 10, 11, 12}));
        REQUIRE(all_assign_stmts_contains_x.size() == 1);
        REQUIRE(Contains(all_assign_stmts_contains_x, 16));
        REQUIRE(all_assign_stmts_contains_cenX.size() == 3);
        REQUIRE(ContainsExactly(all_assign_stmts_contains_cenX, {16, 21, 23}));
      }
    }
  }

  GIVEN("pkb.InsertPrint(stmt_index, variable) called.") {
    REQUIRE(pkb.InsertPrint(6, "flag"));
    REQUIRE(pkb.InsertPrint(7, "cenX"));
    REQUIRE(pkb.InsertPrint(8, "cenY"));
    REQUIRE(pkb.InsertPrint(9, "normSq"));

    WHEN("pkb.GetPrintVarName(stmt_index) called.") {
      THEN("Returns the variable used in a given print statement.") {
        REQUIRE(pkb.GetPrintVarName(6) == "flag");
        REQUIRE(pkb.GetPrintVarName(7) == "cenX");
        REQUIRE(pkb.GetPrintVarName(8) == "cenY");
        REQUIRE(pkb.GetPrintVarName(9) == "normSq");
        REQUIRE(pkb.GetPrintVarName(4) == "");
      }
    }
  }

  GIVEN("pkb.InsertRead(proc_name, variable) called.") {
    REQUIRE(pkb.InsertRead(4, "x"));
    REQUIRE(pkb.InsertRead(5, "y"));

    WHEN("pkb.GetReadVarName(stmt_index) called.") {
      THEN("Returns the variable used in a given print statement.") {
        REQUIRE(pkb.GetReadVarName(4) == "x");
        REQUIRE(pkb.GetReadVarName(5) == "y");
        REQUIRE(pkb.GetReadVarName(8) == "");
      }
    }
  }

  GIVEN("pkb.InsertProcedure(proc_name, start_index, end_index) called.") {
    REQUIRE(pkb.InsertProcedure("main", 1, 3));
    REQUIRE(pkb.InsertProcedure("readPoint", 4, 5));
    REQUIRE(pkb.InsertProcedure("printResults", 6, 9));
    REQUIRE(pkb.InsertProcedure("computeCentroid", 10, 23));

    WHEN("Insert non-existing procedure names, valid procedure start and end indexes into proc_table.") {
      THEN("Insertion returns True. Inserts procedure name and its start and end indexes into assign_table.") {
        REQUIRE(pkb.InsertProcedure("subtract", 24, 25));
      }
    }

    WHEN("Insert existing procedure names, valid procedure start and end indexes into proc_table.") {
      THEN("Insertion returns False.") {
        REQUIRE(pkb.InsertProcedure("computeCentroid", 11, 15) == false);
      }
    }

    WHEN("Insert non-existing procedure names, invalid procedure start and end indexes into proc_table.") {
      THEN("Insertion returns False. Inserts procedure name and its start and end indexes into assign_table.") {
        REQUIRE_FALSE(pkb.InsertProcedure("newProc", 1, 2));
        REQUIRE_FALSE(pkb.InsertProcedure("newProc", 2, 5));
        REQUIRE_FALSE(pkb.InsertProcedure("newProc", 2, 7));
        REQUIRE_FALSE(pkb.InsertProcedure("newProc", 7, 12));
        REQUIRE_FALSE(pkb.InsertProcedure("newProc", 1, 1));
        REQUIRE_FALSE(pkb.InsertProcedure("newProc", 5, 6));
      }
    }

    WHEN("Get all procedure names.") {
      THEN("Returns a set of unique procedure names populated so far.") {
        std::unordered_set<std::string> all_procedures = pkb.GetAllProcedures();
        REQUIRE(all_procedures.size() == 4);
        REQUIRE(ContainsExactly(all_procedures, {"computeCentroid", "main", "printResults", "readPoint"}));
      }
    }

    WHEN("pkb.GetProcRange(proc_name) called.") {
      THEN("Returns the pair of start and end index of a given procedure name.") {
        std::pair<int, int> proc_range = pkb.GetProcRange("computeCentroid");
        REQUIRE(proc_range.first == 10);
        REQUIRE(proc_range.second == 23);
      }
    }
  }

  GIVEN("pkb.InsertConstant(constant) called.") {
    REQUIRE(pkb.InsertConstant(0));
    REQUIRE(pkb.InsertConstant(1));

    WHEN("Insert non-existing constant.") {
      THEN("Returns True. Constant inserted to pkb.") {
        REQUIRE(pkb.InsertConstant(2));
      }
    }

    WHEN("Insert existing constant.") {
      THEN("Returns False. Constant not inserted to pkb.") {
        REQUIRE(pkb.InsertConstant(1) == false);
      }
    }

    WHEN("Get all constants.") {
      THEN("Returns all constants inserted so far.") {
        REQUIRE(pkb.GetAllConstants().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllConstants(), {0, 1}));
      }
    }
  }

  GIVEN("pkb.InsertFollows(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertFollows(1, 2));
    REQUIRE(pkb.InsertFollows(2, 3));

    WHEN("Insert non-existing stmt1 with valid statement indexes stmt1 < stmt2.") {
      THEN("Returns True. Inserts the relationship into PKB.") {
        REQUIRE(pkb.InsertFollows(3, 4));
      }
    }

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 < stmt2.") {
      THEN("Returns False.") {
        REQUIRE(pkb.InsertFollows(2, 4) == false);
        REQUIRE(pkb.InsertFollows(1, 4) == false);
      }
    }

    WHEN("Insert non-existing stmt1 with valid statement indexes stmt1 < stmt2 but existing stmt2.") {
      REQUIRE(pkb.InsertFollows(3, 10));

      THEN("Returns False.") {
        REQUIRE(pkb.InsertFollows(4, 10) == false);
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 >= stmt2.") {
      THEN("Returns False.") {
        REQUIRE(pkb.InsertFollows(3, 3) == false);
        REQUIRE(pkb.InsertFollows(5, 2) == false);
        REQUIRE(pkb.InsertFollows(0, 1) == false);
      }
    }

    WHEN("Check if IsFollows(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsFollows(1, 2));
        REQUIRE(pkb.IsFollows(2, 3));
        REQUIRE(pkb.IsFollows(1, 3) == false);
      }
    }

    WHEN("Check GetStmtFollowedBy(stmt2).") {
      THEN("Return a statement that is followed by stmt2.") {
        REQUIRE(pkb.GetStmtFollowedBy(3) == 2);
      }
    }

    WHEN("Check GetStmtFollows(stmt1).") {
      THEN("Return a statement that Follows stmt1.") {
        REQUIRE(pkb.GetStmtFollows(1) == 2);
        REQUIRE(pkb.GetStmtFollows(2) == 3);
      }
    }

    WHEN("Check GetAllFollowsStmts().") {
      THEN("Return a set of statements that Follows another statement.") {
        REQUIRE(pkb.GetAllFollowsStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllFollowsStmts(), {2, 3}));
      }
    }

    WHEN("Check GetAllFollowedStmts().") {
      THEN("Return a set of statements that are Followed by another statement.") {
        REQUIRE(pkb.GetAllFollowedStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllFollowedStmts(), {1, 2}));
      }
    }
  }

  GIVEN("pkb.InsertFollowsT(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertFollowsT(1, 2));
    REQUIRE(pkb.InsertFollowsT(1, 3));
    REQUIRE(pkb.InsertFollowsT(2, 3));

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 < stmt2.") {
      REQUIRE(pkb.InsertFollowsT(1, 3) == false);
      REQUIRE(pkb.InsertFollowsT(2, 5));

      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
        REQUIRE(pkb.GetAllFollowedTStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllFollowedTStmts(), {1, 2}));
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 >= stmt2.") {
      THEN("Returns False.") {
        REQUIRE(pkb.InsertFollowsT(3, 3) == false);
        REQUIRE(pkb.InsertFollowsT(5, 2) == false);
        REQUIRE(pkb.InsertFollowsT(0, 1) == false);
      }
    }

    WHEN("Check if IsFollowsT(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsFollowsT(1, 2));
        REQUIRE(pkb.IsFollowsT(2, 3));
      }
    }

    WHEN("Check GetStmtsFollowedTBy(stmt2).") {
      THEN("Return a list of statements that are FollowsT by stmt2.") {
        REQUIRE(pkb.GetStmtsFollowedTBy(3).size() == 2);
        REQUIRE(ContainsExactly(pkb.GetStmtsFollowedTBy(3), {1, 2}));
      }
    }

    WHEN("Check GetStmtsFollowsT(stmt1).") {
      THEN("Return a list of statements that FollowsT stmt1.") {
        REQUIRE(pkb.GetStmtsFollowsT(1).size() == 2);
        REQUIRE(ContainsExactly(pkb.GetStmtsFollowsT(1), {2, 3}));
      }
    }

    WHEN("Check GetAllFollowsTStmts().") {
      THEN("Return a set of statements that FollowsT one or more other statements.") {
        REQUIRE(pkb.GetAllFollowsTStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllFollowsTStmts(), {2, 3}));
      }
    }

    WHEN("Check GetAllFollowedTStmts().") {
      THEN("Return a set of statements that are FollowedT by one or more other statements.") {
        REQUIRE(pkb.GetAllFollowedTStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllFollowedTStmts(), {1, 2}));
      }
    }
  }

  GIVEN("pkb.InsertParent(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertParent(5, 6));
    REQUIRE(pkb.InsertParent(5, 7));
    REQUIRE(pkb.InsertParent(5, 8));
    REQUIRE(pkb.InsertParent(10, 11));
    REQUIRE(pkb.InsertParent(10, 12));

    WHEN("Insert valid stmt1 and stmt2 indexes such that stmt1 >= 0, stmt1 < stmt2 and Parent(stmt1, stmt2) has not been captured.") {
      THEN("Insertion returns True. stmt2 is added onto the children list of stmt1.") {
        REQUIRE(pkb.InsertParent(5, 9));
        REQUIRE(pkb.InsertParent(10, 13));
        REQUIRE(ContainsExactly(pkb.GetChildrenStatements(5), {6, 7, 8, 9}));
        REQUIRE(ContainsExactly(pkb.GetChildrenStatements(10), {11, 12, 13}));
      }
    }

    WHEN("Insert invalid stmt1 and stmt2 indexes or stmt2 already inside children list of some statement.") {
      THEN("Insertion returns False.") {
        REQUIRE(pkb.InsertParent(1, 6) == false);
        REQUIRE(pkb.InsertParent(1, 7) == false);
        REQUIRE(pkb.InsertParent(1, 8) == false);
        REQUIRE(pkb.InsertParent(1, 11) == false);
        REQUIRE(pkb.InsertParent(1, 12) == false);
        REQUIRE(pkb.InsertParent(0, 1) == false);
        REQUIRE(pkb.InsertParent(8, 7) == false);
        REQUIRE(pkb.InsertParent(8, 8) == false);
      }
    }

    WHEN("Check if IsParent(stmt1, stmt2) holds.") {
      THEN("Return True if it holds. False otherwise.") {
        REQUIRE(pkb.IsParent(5, 6));
        REQUIRE(pkb.IsParent(5, 7));
        REQUIRE(pkb.IsParent(5, 8));
        REQUIRE(pkb.IsParent(10, 11));
        REQUIRE(pkb.IsParent(10, 12));
      }
    }

    WHEN("Check GetChildrenStatements(stmt1).") {
      THEN("Returns a vector of children statements of stmt1.") {
        REQUIRE(pkb.GetChildrenStatements(5).size() == 3);
        REQUIRE(ContainsExactly(pkb.GetChildrenStatements(5), {6, 7, 8}));
        REQUIRE(pkb.GetChildrenStatements(10).size() == 2);
        REQUIRE(ContainsExactly(pkb.GetChildrenStatements(10), {11, 12}));
      }
    }

    WHEN("Check GetParentStatement(stmt2).") {
      THEN("Returns the statement index of parent of stmt2.") {
        REQUIRE(pkb.GetParentStatement(6) == 5);
        REQUIRE(pkb.GetParentStatement(7) == 5);
        REQUIRE(pkb.GetParentStatement(8) == 5);
        REQUIRE(pkb.GetParentStatement(11) == 10);
        REQUIRE(pkb.GetParentStatement(12) == 10);
      }
    }

    WHEN("Check GetAllParentStmts().") {
      THEN("Returns a set of statements that are parent to other children statements.") {
        REQUIRE(pkb.GetAllParentStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllParentStmts(), {5, 10}));
      }
    }

    WHEN("Check GetAllChildrenStmts().") {
      THEN("Returns a set of statements that are children to other parent statements.") {
        REQUIRE(pkb.GetAllChildrenStmts().size() == 5);
        REQUIRE(ContainsExactly(pkb.GetAllChildrenStmts(), {6, 7, 8, 11, 12}));
      }
    }
  }

  GIVEN("pkb.InsertParentT(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertParentT(5, 6));
    REQUIRE(pkb.InsertParentT(5, 7));
    REQUIRE(pkb.InsertParentT(5, 8));
    REQUIRE(pkb.InsertParentT(5, 11));
    REQUIRE(pkb.InsertParentT(5, 12));
    REQUIRE(pkb.InsertParentT(5, 10));
    REQUIRE(pkb.InsertParentT(10, 11));
    REQUIRE(pkb.InsertParentT(10, 12));

    WHEN("Insert valid stmt1 and stmt2 indexes such that stmt1 >= 0, stmt1 < stmt2 and ParentT(stmt1, stmt2) has not been captured.") {
      THEN("Insertion returns True. stmt2 is added onto the children list of stmt1.") {
        REQUIRE(pkb.GetChildrenTStatements(5).size() == 6);
        REQUIRE(pkb.GetChildrenTStatements(10).size() == 2);
      }
    }

    WHEN("Insert invalid stmt1 and stmt2 indexes or stmt2 already inside children list of stmt1.") {
      THEN("Insertion returns False.") {
        REQUIRE(pkb.InsertParentT(5, 6) == false);
        REQUIRE(pkb.InsertParentT(5, 10) == false);
        REQUIRE(pkb.InsertParentT(10, 11) == false);
        REQUIRE(pkb.InsertParentT(0, 1) == false);
        REQUIRE(pkb.InsertParentT(8, 7) == false);
        REQUIRE(pkb.InsertParentT(8, 8) == false);
      }
    }

    WHEN("Check if IsParentT(stmt1, stmt2) holds.") {
      THEN("Return True if it holds. False otherwise.") {
        REQUIRE(pkb.IsParentT(5, 6));
        REQUIRE(pkb.IsParentT(5, 7));
        REQUIRE(pkb.IsParentT(5, 8));
        REQUIRE(pkb.IsParentT(5, 10));
        REQUIRE(pkb.IsParentT(5, 11));
        REQUIRE(pkb.IsParentT(5, 12));
        REQUIRE(pkb.IsParentT(10, 11));
        REQUIRE(pkb.IsParentT(10, 12));
      }
    }

    WHEN("Check GetChildrenTStatements(stmt1).") {
      THEN("Returns a vector of children statements of stmt1.") {
        REQUIRE(pkb.GetChildrenTStatements(5).size() == 6);
        REQUIRE(pkb.GetChildrenTStatements(10).size() == 2);
      }
    }

    WHEN("Check GetParentTStatement(stmt2).") {
      THEN("Returns the list of statements which are parentT of stmt2.") {
        REQUIRE(pkb.GetParentTStatements(11).size() == 2);
        REQUIRE(Contains(pkb.GetParentTStatements(6), 5));
        REQUIRE(Contains(pkb.GetParentTStatements(7), 5));
        REQUIRE(Contains(pkb.GetParentTStatements(8), 5));
        REQUIRE(Contains(pkb.GetParentTStatements(10), 5));
        REQUIRE(Contains(pkb.GetParentTStatements(11), 5));
        REQUIRE(Contains(pkb.GetParentTStatements(11), 10));
        REQUIRE(Contains(pkb.GetParentTStatements(12), 5));
        REQUIRE(Contains(pkb.GetParentTStatements(12), 10));
        REQUIRE(pkb.GetParentTStatements(12).size() == 2);
      }
    }

    WHEN("Check GetAllParentTStmts().") {
      THEN("Returns a set of statements that are parentT to other children statements.") {
        REQUIRE(pkb.GetAllParentTStmts().size() == 2);
        REQUIRE(Contains(pkb.GetAllParentTStmts(), 5));
        REQUIRE(Contains(pkb.GetAllParentTStmts(), 10));
      }
    }

    WHEN("Check GetAllChildrenTStmts().") {
      THEN("Returns a set of statements that are childrenT to other parent statements.") {
        REQUIRE(pkb.GetAllChildrenTStmts().size() == 6);
        REQUIRE(ContainsExactly(pkb.GetAllChildrenTStmts(), {6, 7, 8, 10, 11, 12}));
      }
    }
  }

  GIVEN("pkb.InsertIf(stmt_index, variable) called.") {
    REQUIRE(pkb.InsertIf(19, {"count"}));
    REQUIRE(pkb.InsertIf(20, {}));

    WHEN("Statement index and variable are valid.") {
      THEN("Returns True if insertion is successful. False otherwise.") {
      }
    }

    WHEN("Statement index or variable is invalid.") {
      THEN("Returns True if insertion is successful. False otherwise.") {
        REQUIRE_FALSE(pkb.InsertIf(-1, {"count"}));
      }
    }

    WHEN("pkb.GetAllIfStmts() called.") {
      THEN("Returns all statement indexes of stored If statements.") {
        REQUIRE(pkb.GetAllIfStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllIfStmts(), {19, 20}));
      }
    }

    WHEN("pkb.GetVariablesUsedByIfStmt(stmt_index) called.") {
      THEN("Returns a set of variables used by the given If statement.") {
        REQUIRE(Contains(pkb.GetVariablesUsedByIfStmt(19), "count"));
        REQUIRE(Contains(pkb.GetVariablesUsedByIfStmt(20), ""));
      }
    }
  }

  GIVEN("pkb.InsertWhile(stmt_index, variable) called.") {
    REQUIRE(pkb.InsertWhile(14, {"x", "y"}));
    REQUIRE(pkb.InsertWhile(20, {}));

    WHEN("Statement index and variable are valid.") {
      THEN("Returns True if insertion is successful. False otherwise.") {
      }
    }

    WHEN("Statement index or variable is invalid.") {
      THEN("Returns True if insertion is successful. False otherwise.") {
        REQUIRE_FALSE(pkb.InsertWhile(-1, {"count"}));
      }
    }

    WHEN("pkb.GetAllWhileStmts() called.") {
      THEN("Returns all statement indexes of stored While statements.") {
        REQUIRE(pkb.GetAllWhileStmts().size() == 2);
        REQUIRE(ContainsExactly(pkb.GetAllWhileStmts(), {14, 20}));
      }
    }

    WHEN("pkb.GetVariablesUsedByWhileStmt(stmt_index) called.") {
      THEN("Returns a set of variables used by the given If statement.") {
        REQUIRE(Contains(pkb.GetVariablesUsedByWhileStmt(14), "x"));
        REQUIRE(Contains(pkb.GetVariablesUsedByWhileStmt(14), "y"));
        REQUIRE(Contains(pkb.GetVariablesUsedByWhileStmt(20), ""));
      }
    }
  }

  GIVEN("pkb.InsertEntity(stmt_index, entity) called.") {
    REQUIRE(pkb.InsertEntity(1, "assign"));
    REQUIRE(pkb.InsertEntity(4, "read"));
    REQUIRE(pkb.InsertEntity(5, "read"));
    REQUIRE(pkb.InsertEntity(6, "print"));
    REQUIRE(pkb.InsertEntity(7, "print"));
    REQUIRE(pkb.InsertEntity(8, "print"));
    REQUIRE(pkb.InsertEntity(9, "print"));
    REQUIRE(pkb.InsertEntity(10, "assign"));
    REQUIRE(pkb.InsertEntity(11, "assign"));
    REQUIRE(pkb.InsertEntity(12, "assign"));
    REQUIRE(pkb.InsertEntity(14, "while"));
    REQUIRE(pkb.InsertEntity(15, "assign"));
    REQUIRE(pkb.InsertEntity(16, "assign"));
    REQUIRE(pkb.InsertEntity(17, "assign"));
    REQUIRE(pkb.InsertEntity(19, "if"));
    REQUIRE(pkb.InsertEntity(20, "assign"));
    REQUIRE(pkb.InsertEntity(21, "assign"));
    REQUIRE(pkb.InsertEntity(22, "assign"));
    REQUIRE(pkb.InsertEntity(23, "assign"));

    WHEN("Statement indexes and entity type are valid.") {
      THEN("Insertion returns True. Statement - entity mapping stored in pkb.") {
      }
    }

    WHEN("Inserts to pkb with the same index.") {
      THEN("Insertion returns false. Statement has already been inserted.") {
        REQUIRE_FALSE(pkb.InsertEntity(1, "read"));
        REQUIRE_FALSE(pkb.InsertEntity(4, "assign"));
      }
    }

    WHEN("Inserts to entity_table with invalid index <= 0.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(pkb.InsertEntity(0, "read"));
        REQUIRE_FALSE(pkb.InsertEntity(-1, "read"));
      }
    }

    WHEN("pkb.GetStatementType(stmt_index) called.") {
      THEN("Returns the DesignEntityType of the statement.") {
        REQUIRE(pkb.GetStatementType(1) == "assign");
        REQUIRE(pkb.GetStatementType(4) == "read");
        REQUIRE(pkb.GetStatementType(5) == "read");
      }
    }

    WHEN("pkb.GetAllEntities() called.") {
      THEN("Returns a set of all entities stored in the pkb.") {
        std::unordered_set<std::string> all_entities_present = pkb.GetAllEntities();
        REQUIRE(all_entities_present.size() == 5);
        REQUIRE(ContainsExactly(all_entities_present, {"assign", "if", "while", "read", "print"}));
      }
    }
  }

  GIVEN("pkb.InsertModifies(stmt_index, variable) called.") {
    REQUIRE(pkb.InsertModifies(1, "flag"));
    REQUIRE(pkb.InsertModifies(4, "x"));
    REQUIRE(pkb.InsertModifies(5, "y"));
    REQUIRE(pkb.InsertModifies(10, "count"));
    REQUIRE(pkb.InsertModifies(11, "cenX"));
    REQUIRE(pkb.InsertModifies(12, "cenY"));
    REQUIRE(pkb.InsertModifies(14, "count"));
    REQUIRE(pkb.InsertModifies(14, "cenX"));
    REQUIRE(pkb.InsertModifies(14, "cenY"));
    REQUIRE(pkb.InsertModifies(15, "count"));
    REQUIRE(pkb.InsertModifies(16, "cenX"));
    REQUIRE(pkb.InsertModifies(17, "cenY"));
    REQUIRE(pkb.InsertModifies(19, "flag"));
    REQUIRE(pkb.InsertModifies(19, "cenX"));
    REQUIRE(pkb.InsertModifies(19, "cenY"));
    REQUIRE(pkb.InsertModifies(20, "flag"));
    REQUIRE(pkb.InsertModifies(21, "cenX"));
    REQUIRE(pkb.InsertModifies(22, "cenY"));
    REQUIRE(pkb.InsertModifies(23, "normSq"));
    REQUIRE(pkb.InsertModifies("computeCentroid", "count"));
    REQUIRE(pkb.InsertModifies("computeCentroid", "cenX"));
    REQUIRE(pkb.InsertModifies("computeCentroid", "cenY"));
    REQUIRE(pkb.InsertModifies("computeCentroid", "flag"));
    REQUIRE(pkb.InsertModifies("computeCentroid", "normSq"));
    REQUIRE(pkb.InsertModifies("computeCentroid", "x"));
    REQUIRE(pkb.InsertModifies("computeCentroid", "y"));
    REQUIRE(pkb.InsertModifies("main", "count"));
    REQUIRE(pkb.InsertModifies("main", "cenX"));
    REQUIRE(pkb.InsertModifies("main", "cenY"));
    REQUIRE(pkb.InsertModifies("main", "flag"));
    REQUIRE(pkb.InsertModifies("main", "normSq"));
    REQUIRE(pkb.InsertModifies("main", "x"));
    REQUIRE(pkb.InsertModifies("main", "y"));
    REQUIRE(pkb.InsertModifies("readPoint", "x"));
    REQUIRE(pkb.InsertModifies("readPoint", "y"));

    WHEN("Valid stmt_index and variable.") {
      THEN("Insertion returns True. Relationship is stored in pkb.") {
        REQUIRE(pkb.GetAllModifiesStatements().size() == 15);
        REQUIRE(pkb.GetAllModifiesProcedures().size() == 3);
      }
    }

    WHEN("Invalid stmt_index <= 0.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(pkb.InsertModifies(0, "x"));
        REQUIRE_FALSE(pkb.InsertModifies(-1, "y"));
      }
    }

    WHEN("Invalid variable or proc_name such as empty string ''") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(pkb.InsertModifies(1, ""));
        REQUIRE_FALSE(pkb.InsertModifies("", "x"));
        REQUIRE_FALSE(pkb.InsertModifies("computeCentroid", ""));
      }
    }

    WHEN("Both stmt_index and variable already stored in modifies_table.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(pkb.InsertModifies(1, "flag"));
        REQUIRE_FALSE(pkb.InsertModifies(4, "x"));
      }
    }

    WHEN("Both proc_name and variable already stored in modifies_table.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(pkb.InsertModifies("computeCentroid", "flag"));
        REQUIRE_FALSE(pkb.InsertModifies("main", "x"));
      }
    }

    WHEN("IsModifies(stmt_index, variable) is called.") {
      THEN("Returns True is relationship exists in modifies_table. False otherwise.") {
        REQUIRE(pkb.IsModifies(1, "flag"));
        REQUIRE_FALSE(pkb.IsModifies(1, "x"));
        REQUIRE(pkb.IsModifies(14, "count"));
        REQUIRE(pkb.IsModifies(14, "cenX"));
        REQUIRE(pkb.IsModifies(14, "cenY"));
      }
    }

    WHEN("IsModifies(proc_name, variable) is called.") {
      THEN("Returns True is relationship exists in modifies_table. False otherwise.") {
        REQUIRE(pkb.IsModifies("computeCentroid", "flag"));
        REQUIRE_FALSE(pkb.IsModifies("printResults", "x"));
        REQUIRE(pkb.IsModifies("main", "count"));
        REQUIRE(pkb.IsModifies("readPoint", "x"));
        REQUIRE(pkb.IsModifies("readPoint", "y"));
      }
    }

    WHEN("GetModifiesStatements(variable) called.") {
      THEN("Returns a set of statements that modifies a variable. If no such statement then return empty set.") {
        std::unordered_set<int> statements_that_modifies_variable_count = pkb.GetModifiesStatements("count");
        REQUIRE(statements_that_modifies_variable_count.size() == 3);
        REQUIRE(ContainsExactly(statements_that_modifies_variable_count, {10, 14, 15}));
        REQUIRE(pkb.GetModifiesStatements("unknown_variable").empty());
      }
    }

    WHEN("GetModifiesProcedures(variable) called.") {
      THEN("Returns a set of procedures that modifies a variable. If no such statement then return empty set.") {
        std::unordered_set<std::string> procedures_that_modifies_variable_x = pkb.GetModifiesProcedures("x");
        REQUIRE(procedures_that_modifies_variable_x.size() == 3);
        REQUIRE(ContainsExactly(procedures_that_modifies_variable_x, {"main", "readPoint", "computeCentroid"}));
        REQUIRE(pkb.GetModifiesProcedures("unknown_variable").empty());
      }
    }

    WHEN("GetModifiedVariables(stmt_index) called.") {
      THEN("Returns a set of variables that a statement modifies. If a statement does not modify any then return empty vector.") {
        std::unordered_set<std::string> modified_variables_statement_14 = pkb.GetModifiedVariables(14);
        REQUIRE(modified_variables_statement_14.size() == 3);
        REQUIRE(ContainsExactly(modified_variables_statement_14, {"count", "cenX", "cenY"}));
        REQUIRE(pkb.GetModifiedVariables(2).empty());
      }
    }

    WHEN("GetModifiedVariables(proc_name) called.") {
      THEN("Returns a set of variables that a procedure modifies. If a statement does not modify any then return empty set.") {
        std::unordered_set<std::string> modified_variables_procedure_main = pkb.GetModifiedVariables("main");
        REQUIRE(modified_variables_procedure_main.size() == 7);
        REQUIRE(ContainsExactly(modified_variables_procedure_main, {"count", "cenX", "cenY", "flag", "normSq", "x", "y"}));
        REQUIRE(pkb.GetModifiesProcedures("printResults").empty());
      }
    }

    WHEN("GetAllModifiesStatements() called.") {
      THEN("Returns a set of statement indexes that Modifies(stmt_index, variable) holds.") {
        std::unordered_set<int> all_modifies_statements = pkb.GetAllModifiesStatements();
        REQUIRE(all_modifies_statements.size() == 15);
      }
    }

    WHEN("GetAllModifiesProcedures() called.") {
      THEN("Returns a set of procedures that Modifies(proc_name, variable) holds.") {
        std::unordered_set<std::string> all_modifies_procedures = pkb.GetAllModifiesProcedures();
        REQUIRE(all_modifies_procedures.size() == 3);
      }
    }
  }

  GIVEN("pkb.InsertUses() called.") {
    REQUIRE(pkb.InsertUses(6, "flag"));
    REQUIRE(pkb.InsertUses(7, "cenX"));
    REQUIRE(pkb.InsertUses(8, "cenY"));
    REQUIRE(pkb.InsertUses(9, "normSq"));
    REQUIRE(pkb.InsertUses(14, "x"));
    REQUIRE(pkb.InsertUses(14, "y"));
    REQUIRE(pkb.InsertUses(15, "count"));
    REQUIRE(pkb.InsertUses(16, "cenX"));
    REQUIRE(pkb.InsertUses(16, "x"));
    REQUIRE(pkb.InsertUses(17, "cenY"));
    REQUIRE(pkb.InsertUses(17, "y"));
    REQUIRE(pkb.InsertUses(19, "count"));
    REQUIRE(pkb.InsertUses(21, "cenX"));
    REQUIRE(pkb.InsertUses(21, "count"));
    REQUIRE(pkb.InsertUses(22, "cenY"));
    REQUIRE(pkb.InsertUses(22, "count"));
    REQUIRE(pkb.InsertUses(23, "cenX"));
    REQUIRE(pkb.InsertUses(23, "cenY"));

    WHEN("Valid stmt_index and variable.") {
      THEN("Insertion returns True. Relationship is stored in pkb.") {
        REQUIRE(pkb.GetAllUsesStatements().size() == 12);
      }
    }

    WHEN("Invalid stmt_index <= 0.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(pkb.InsertUses(0, "x"));
        REQUIRE_FALSE(pkb.InsertUses(-1, "y"));
      }
    }

    WHEN("Invalid variable such as empty string ''") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(pkb.InsertUses(1, ""));
      }
    }

    WHEN("Both stmt_index and variable already stored in pkb.") {
      THEN("Insertion returns False.") {
        REQUIRE_FALSE(pkb.InsertUses(6, "flag"));
        REQUIRE_FALSE(pkb.InsertUses(7, "cenX"));
      }
    }

    WHEN("pkb.IsStmtUses(stmt_index, variable) is called.") {
      THEN("Returns True is relationship exists in pkb. False otherwise.") {
        REQUIRE(pkb.IsUses(6, "flag"));
        REQUIRE_FALSE(pkb.IsUses(6, "cenX"));
        REQUIRE(pkb.IsUses(14, "x"));
        REQUIRE(pkb.IsUses(14, "y"));
      }
    }

    WHEN("pkb.GetUsesStatements(variable) called.") {
      THEN("Returns a set of statements that uses a variable. If no such statement then return empty set.") {
        std::unordered_set<int> statements_that_uses_variable_count = pkb.GetUsesStatements("count");
        REQUIRE(statements_that_uses_variable_count.size() == 4);
        REQUIRE(ContainsExactly(statements_that_uses_variable_count, {15, 19, 21, 22}));
        REQUIRE(pkb.GetUsesStatements("unknown_variable").empty());
      }
    }

    WHEN("pkb.GetUsedStmtVariables(stmt_index) called.") {
      THEN("Returns a set of variables that a statement uses. If a statement does not use any then returns empty set.") {
        std::unordered_set<std::string> used_variables_statement_14 = pkb.GetUsedVariables(14);
        REQUIRE(used_variables_statement_14.size() == 2);
        REQUIRE(ContainsExactly(used_variables_statement_14, {"x", "y"}));
        REQUIRE(pkb.GetUsedVariables(2).empty());
      }
    }

    WHEN("pkb.GetAllUsesStatements() called.") {
      THEN("Returns a set of statement indexes that Uses(stmt_index, variable) holds.") {
        std::unordered_set<int> all_uses_statements = pkb.GetAllUsesStatements();
        REQUIRE(all_uses_statements.size() == 12);
      }
    }
  }

  GIVEN("pkb.InsertCalls(stmt_index, proc_name) called.") {
    REQUIRE(pkb.InsertCalls(2, "computeCentroid"));
    REQUIRE(pkb.InsertCalls(3, "printResults"));
    REQUIRE(pkb.InsertCalls(13, "readPoint"));
    REQUIRE(pkb.InsertCalls(18, "readPoint"));

    WHEN("Insertion is valid. Returns True and stores a mapping of which statement index calls which procedure in CallTable.") {
    }

    WHEN("pkb.GetAllCallsStmts() called.") {
      THEN("Returns a set of statement indexes of call statements.") {
        std::unordered_set<int> all_call_statements = pkb.GetAllCallsStmts();
        REQUIRE(all_call_statements.size() == 4);
        REQUIRE(ContainsExactly(all_call_statements, {2, 3, 13, 18}));
      }
    }

    WHEN("pkb.GetCallsProcName(stmt_index) called.") {
      THEN("Returns the procedure called at the statement index.") {
        REQUIRE(pkb.GetCallsProcName(2) == "computeCentroid");
        REQUIRE(pkb.GetCallsProcName(3) == "printResults");
        REQUIRE(pkb.GetCallsProcName(13) == "readPoint");
        REQUIRE(pkb.GetCallsProcName(18) == "readPoint");
      }
    }

    WHEN("pkb.GetStmtIndexesThatCalls(proc_name) called.") {
      THEN("Returns the statement indexes of call statement given the procedure name.") {
        std::unordered_set<int> statement_indexes_that_calls_readPoint = pkb.GetStmtIndexesThatCalls(
            "readPoint");
        REQUIRE(statement_indexes_that_calls_readPoint.size() == 2);
        REQUIRE(ContainsExactly(statement_indexes_that_calls_readPoint, {13, 18}));
      }
    }

    WHEN("pkb.GetCallsProcName(stmt_index) called.") {
      THEN("Returns the procedure called at that statement index.") {
        REQUIRE(pkb.GetCallsProcName(2) == "computeCentroid");
        REQUIRE(pkb.GetCallsProcName(3) == "printResults");
        REQUIRE(pkb.GetCallsProcName(13) == "readPoint");
        REQUIRE(pkb.GetCallsProcName(18) == "readPoint");
      }
    }
  }

  GIVEN("pkb.InsertCalls(caller, callee) called.") {
    pkb.InsertCalls("main", "computeCentroid");
    pkb.InsertCalls("main", "printResults");
    pkb.InsertCalls("computeCentroid", "readPoint");

    WHEN("Both caller and callee are valid.") {
      THEN("Insertion returns True. Calls(caller, callee) relationship is stored in the CallTable.") {
      }
    }

    WHEN("pkb.IsCalls(caller, callee) called.") {
      THEN("Returns True if Calls(caller, callee) relationship holds.") {
        REQUIRE(pkb.IsCalls("main", "computeCentroid"));
        REQUIRE(pkb.IsCalls("main", "printResults"));
        REQUIRE(pkb.IsCalls("computeCentroid", "readPoint"));
        REQUIRE_FALSE(pkb.IsCalls("main", "readPoint"));
      }
    }

    WHEN("pkb.GetProceduresThatCalls(proc_name) is called.") {
      THEN("Returns all procedures which Calls a given proc_name.") {
        std::unordered_set<std::string> all_procedures_that_calls_computeCentroid = pkb.GetProceduresThatCalls("computeCentroid");
        REQUIRE(all_procedures_that_calls_computeCentroid.size() == 1);
        REQUIRE(Contains(all_procedures_that_calls_computeCentroid, "main"));
      }
    }

    WHEN("pkb.GetProceduresCalled(proc_name) is called.") {
      THEN("Returns all procedures which are Called by a given proc_name.") {
        std::unordered_set<std::string> all_procedures_called_by_main = pkb.GetProceduresCalled("main");
        REQUIRE(all_procedures_called_by_main.size() == 2);
        REQUIRE(ContainsExactly(all_procedures_called_by_main, {"computeCentroid", "printResults"}));
      }
    }

    WHEN("pkb.GetAllProceduresThatCalls() called.") {
      THEN("Returns all procedures which Calls by some other procedure.") {
        std::unordered_set<std::string> all_calls_procedures = pkb.GetAllProceduresThatCalls();
        REQUIRE(all_calls_procedures.size() == 2);
        REQUIRE(ContainsExactly(all_calls_procedures, {"main", "computeCentroid"}));
      }
    }

    WHEN("pkb.GetAllProceduresCalled() called.") {
      THEN("Returns all procedures which are Called by some other procedure.") {
        std::unordered_set<std::string> all_procedures_called = pkb.GetAllProceduresCalled();
        REQUIRE(all_procedures_called.size() == 3);
        REQUIRE(ContainsExactly(all_procedures_called, {"computeCentroid", "printResults", "readPoint"}));
      }
    }
  }

  GIVEN("pkb.InsertCallsT(caller, callee) called.") {
    pkb.InsertCallsT("main", "computeCentroid");
    pkb.InsertCallsT("main", "printResults");
    pkb.InsertCallsT("main", "readPoint");
    pkb.InsertCallsT("computeCentroid", "readPoint");

    GIVEN("Both caller and callee are valid.") {
      THEN("Insertion returns True. Calls(caller, callee) relationship is stored in the CallTable.") {
      }
    }

    WHEN("pkb.IsCallsT(caller, callee) called.") {
      THEN("Returns True if CallsT(caller, callee) relationship holds.") {
        REQUIRE(pkb.IsCallsT("main", "computeCentroid"));
        REQUIRE(pkb.IsCallsT("main", "printResults"));
        REQUIRE(pkb.IsCallsT("main", "readPoint"));
        REQUIRE(pkb.IsCallsT("computeCentroid", "readPoint"));
        REQUIRE_FALSE(pkb.IsCallsT("computeCentroid", "main"));
      }
    }

    WHEN("pkb.GetProceduresThatCallsT(proc_name) is called.") {
      THEN("Returns all procedures which CallsT a given proc_name.") {
        std::unordered_set<std::string> all_procedures_that_callsT_readPoint = pkb.GetProceduresThatCallsT("readPoint");
        REQUIRE(all_procedures_that_callsT_readPoint.size() == 2);
        REQUIRE(ContainsExactly(all_procedures_that_callsT_readPoint, {"main", "computeCentroid"}));
      }
    }

    WHEN("pkb.GetProceduresCalledT(proc_name) is called.") {
      THEN("Returns all procedures which are Called by a given proc_name.") {
        std::unordered_set<std::string> all_procedures_calledT_by_main = pkb.GetProceduresCalledT("main");
        REQUIRE(all_procedures_calledT_by_main.size() == 3);
        REQUIRE(ContainsExactly(all_procedures_calledT_by_main, {"computeCentroid", "printResults", "readPoint"}));
      }
    }

    WHEN("pkb.GetAllProceduresThatCallsT() called.") {
      THEN("Returns all procedures which Calls by some other procedure.") {
        std::unordered_set<std::string> all_callsT_procedures = pkb.GetAllProceduresThatCallsT();
        REQUIRE(all_callsT_procedures.size() == 2);
        REQUIRE(ContainsExactly(all_callsT_procedures, {"main", "computeCentroid"}));
      }
    }

    WHEN("pkb.GetAllProceduresCalledT() called.") {
      THEN("Returns all procedures which are CalledT by some other procedure.") {
        std::unordered_set<std::string> all_procedures_calledT = pkb.GetAllProceduresCalledT();
        REQUIRE(all_procedures_calledT.size() == 3);
        REQUIRE(ContainsExactly(all_procedures_calledT, {"computeCentroid", "printResults", "readPoint"}));
      }
    }
  }

  GIVEN("pkb.InsertNext(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertNext(2, 3));
    REQUIRE(pkb.InsertNext(3, 4));
    REQUIRE(pkb.InsertNext(3, 7));
    REQUIRE(pkb.InsertNext(5, 6));
    REQUIRE(pkb.InsertNext(6, 3));
    REQUIRE(pkb.InsertNext(7, 9));
    REQUIRE(pkb.InsertNext(8, 10));

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 != stmt2 and stmt1 and stmt2 != 0.") {
      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 == stmt2 or stmt1 <= 0 or stmt2 <= 0.") {
      THEN("Returns False.") {
        REQUIRE_FALSE(pkb.InsertNext(3, 3));
        REQUIRE_FALSE(pkb.InsertNext(0, 2));
        REQUIRE_FALSE(pkb.InsertNext(3, 0));
      }
    }

    WHEN("Check if IsNext(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsNext(2, 3));
        REQUIRE(pkb.IsNext(3, 4));
        REQUIRE(pkb.IsNext(3, 7));
        REQUIRE(pkb.IsNext(5, 6));
        REQUIRE(pkb.IsNext(6, 3));
        REQUIRE(pkb.IsNext(7, 9));
        REQUIRE(pkb.IsNext(8, 10));
      }
    }

    WHEN("Check GetNextStatements(stmt1).") {
      THEN("Return a set of statements that are Next to stmt1.") {
        std::unordered_set<int> next_statements_set_of_3 = pkb.GetNextStatements(3);
        REQUIRE(next_statements_set_of_3.size() == 2);
        REQUIRE(ContainsExactly(next_statements_set_of_3, {4, 7}));
      }
    }

    WHEN("Check GetPreviousStatements(stmt2).") {
      THEN("Return a set of statements that are Previous to a stmt2.") {
        std::unordered_set<int> prev_statements_set_of_3 = pkb.GetPreviousStatements(3);
        REQUIRE(prev_statements_set_of_3.size() == 2);
        REQUIRE(ContainsExactly(prev_statements_set_of_3, {2, 6}));
      }
    }

    WHEN("Check GetAllNextStatements().") {
      THEN("Return a set of statements that Next to one or more other statements.") {
        std::unordered_set<int> all_next_statements_set = pkb.GetAllNextStatements();
        REQUIRE(all_next_statements_set.size() == 6);
        REQUIRE(ContainsExactly(all_next_statements_set, {3, 4, 6, 7, 9, 10}));
      }
    }

    WHEN("Check GetAllPreviousStatements().") {
      THEN("Return a set of statements that are Previous to one or more other statements.") {
        std::unordered_set<int> all_prev_statements_set = pkb.GetAllPreviousStatements();
        REQUIRE(all_prev_statements_set.size() == 6);
        REQUIRE(ContainsExactly(all_prev_statements_set, {2, 3, 5, 6, 7, 8}));
      }
    }
  }

  GIVEN("pkb.InsertNextT(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertNextT(1, 2));
    REQUIRE(pkb.InsertNextT(1, 3));
    REQUIRE(pkb.InsertNextT(2, 5));
    REQUIRE(pkb.InsertNextT(4, 3));
    REQUIRE(pkb.InsertNextT(5, 5));
    REQUIRE(pkb.InsertNextT(5, 8));
    REQUIRE(pkb.InsertNextT(5, 12));

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 != stmt2 and stmt1 and stmt2 != 0.") {
      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 <= 0 or stmt2 <= 0.") {
      THEN("Returns False.") {
        REQUIRE_FALSE(pkb.InsertNextT(0, 2));
        REQUIRE_FALSE(pkb.InsertNextT(3, 0));
      }
    }

    WHEN("Check if IsNextT(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsNextT(1, 2));
        REQUIRE(pkb.IsNextT(1, 3));
        REQUIRE(pkb.IsNextT(2, 5));
        REQUIRE(pkb.IsNextT(4, 3));
        REQUIRE(pkb.IsNextT(5, 5));
        REQUIRE(pkb.IsNextT(5, 8));
        REQUIRE(pkb.IsNextT(5, 12));
      }
    }

    WHEN("Check GetNextTStatements(stmt1).") {
      THEN("Return a set of statements that are NextT to stmt1.") {
        std::unordered_set<int> next_T_statements_set_of_5 = pkb.GetNextTStatements(5);
        REQUIRE(next_T_statements_set_of_5.size() == 3);
        REQUIRE(ContainsExactly(next_T_statements_set_of_5, {5, 8, 12}));
      }
    }

    WHEN("Check GetPreviousTStatements(stmt2).") {
      THEN("Return a set of statements that are PreviousT to a stmt2.") {
        std::unordered_set<int> prev_T_statements_set_of_3 = pkb.GetPreviousTStatements(3);
        REQUIRE(prev_T_statements_set_of_3.size() == 2);
        REQUIRE(ContainsExactly(prev_T_statements_set_of_3, {1, 4}));
      }
    }

    WHEN("Check GetAllNextTStatements().") {
      THEN("Return a set of statements that are NextT to one or more other statements.") {
        std::unordered_set<int> all_next_T_statements_set = pkb.GetAllNextTStatements();
        REQUIRE(all_next_T_statements_set.size() == 5);
        REQUIRE(ContainsExactly(all_next_T_statements_set, {2, 3, 5, 8, 12}));
      }
    }

    WHEN("Check GetAllPreviousTStatements().") {
      THEN("Return a set of statements that are PreviousT to one or more other statements.") {
        std::unordered_set<int> all_prev_T_statements_set = pkb.GetAllPreviousTStatements();
        REQUIRE(all_prev_T_statements_set.size() == 4);
        REQUIRE(ContainsExactly(all_prev_T_statements_set, {1, 2, 4, 5}));
      }
    }
  }

  GIVEN("pkb.InsertAffects(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertAffects(2, 6));
    REQUIRE(pkb.InsertAffects(4, 8));
    REQUIRE(pkb.InsertAffects(4, 10));
    REQUIRE(pkb.InsertAffects(6, 6));
    REQUIRE(pkb.InsertAffects(1, 4));
    REQUIRE(pkb.InsertAffects(1, 8));
    REQUIRE(pkb.InsertAffects(1, 10));
    REQUIRE(pkb.InsertAffects(1, 12));
    REQUIRE(pkb.InsertAffects(2, 10));
    REQUIRE(pkb.InsertAffects(9, 10));

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 != stmt2 and stmt1 and stmt2 != 0.") {
      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 <= 0 or stmt2 <= 0.") {
      THEN("Returns False.") {
        REQUIRE_FALSE(pkb.InsertAffects(0, 2));
        REQUIRE_FALSE(pkb.InsertAffects(3, 0));
      }
    }

    WHEN("Check if IsAffects(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsAffects(2, 6));
        REQUIRE(pkb.IsAffects(4, 8));
        REQUIRE(pkb.IsAffects(4, 10));
        REQUIRE(pkb.IsAffects(6, 6));
        REQUIRE(pkb.IsAffects(1, 4));
        REQUIRE(pkb.IsAffects(1, 8));
        REQUIRE(pkb.IsAffects(1, 10));
        REQUIRE(pkb.IsAffects(1, 12));
        REQUIRE(pkb.IsAffects(2, 10));
        REQUIRE(pkb.IsAffects(9, 10));
      }
    }

    WHEN("Check GetStatementsThatAffects(stmt2).") {
      THEN("Return a set of statements that Affects stmt1.") {
        std::unordered_set<int> stmts_that_affects_10 = pkb.GetStatementsThatAffects(10);
        REQUIRE(stmts_that_affects_10.size() == 4);
        REQUIRE(ContainsExactly(stmts_that_affects_10, {1, 2, 4, 9}));
      }
    }

    WHEN("Check GetAffectedStatements(stmt2).") {
      THEN("Return a set of statements that are Affected by stmt2.") {
        std::unordered_set<int> affected_statements_by_4 = pkb.GetAffectedStatements(4);
        REQUIRE(affected_statements_by_4.size() == 2);
        REQUIRE(ContainsExactly(affected_statements_by_4, {8, 10}));
      }
    }

    WHEN("Check GetAllStatementsThatAffects().") {
      THEN("Return a set of statements that Next to one or more other statements.") {
        std::unordered_set<int> all_affects_statements_set = pkb.GetAllStatementsThatAffects();
        REQUIRE(all_affects_statements_set.size() == 5);
        REQUIRE(ContainsExactly(all_affects_statements_set, {1, 2, 4, 6, 9}));
      }
    }

    WHEN("Check GetAllAffectedStatements().") {
      THEN("Return a set of statements that are Previous to one or more other statements.") {
        std::unordered_set<int> all_affected_statements_set = pkb.GetAllAffectedStatements();
        REQUIRE(all_affected_statements_set.size() == 5);
        REQUIRE(ContainsExactly(all_affected_statements_set, {4, 6, 8, 10, 12}));
      }
    }
  }

  GIVEN("pkb.InsertAffectsT(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertAffectsT(1, 4));
    REQUIRE(pkb.InsertAffectsT(1, 10));
    REQUIRE(pkb.InsertAffectsT(1, 11));
    REQUIRE(pkb.InsertAffectsT(1, 12));
    REQUIRE(pkb.InsertAffectsT(2, 10));
    REQUIRE(pkb.InsertAffectsT(2, 6));

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 != stmt2 and stmt1 and stmt2 != 0.") {
      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 <= 0 or stmt2 <= 0.") {
      THEN("Returns False.") {
        REQUIRE_FALSE(pkb.InsertAffectsT(0, 2));
        REQUIRE_FALSE(pkb.InsertAffectsT(3, 0));
      }
    }

    WHEN("Check if IsAffectsT(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsAffectsT(1, 4));
        REQUIRE(pkb.IsAffectsT(1, 10));
        REQUIRE(pkb.IsAffectsT(1, 11));
        REQUIRE(pkb.IsAffectsT(1, 12));
        REQUIRE(pkb.IsAffectsT(2, 10));
        REQUIRE(pkb.IsAffectsT(2, 6));
      }
    }

    WHEN("Check GetStatementsThatAffectsT(stmt2).") {
      THEN("Return a set of statements that AffectsT stmt2.") {
        std::unordered_set<int> stmts_that_affectsT_10 = pkb.GetStatementsThatAffectsT(10);
        REQUIRE(stmts_that_affectsT_10.size() == 2);
        REQUIRE(ContainsExactly(stmts_that_affectsT_10, {1, 2}));
      }
    }

    WHEN("Check GetAffectedTStatements(stmt1).") {
      THEN("Return a set of statements that are AffectedT by stmt1.") {
        std::unordered_set<int> affectedT_statements_by_1 = pkb.GetAffectedTStatements(1);
        REQUIRE(affectedT_statements_by_1.size() == 4);
        REQUIRE(ContainsExactly(affectedT_statements_by_1, {4, 10, 11, 12}));
      }
    }

    WHEN("Check GetAllStatementsThatAffectsT().") {
      THEN("Return a set of statements that Next to one or more other statements.") {
        std::unordered_set<int> all_affectsT_statements_set = pkb.GetAllStatementsThatAffectsT();
        REQUIRE(all_affectsT_statements_set.size() == 2);
        REQUIRE(ContainsExactly(all_affectsT_statements_set, {1, 2}));
      }
    }

    WHEN("Check GetAllAffectedTStatements().") {
      THEN("Return a set of statements that are Previous to one or more other statements.") {
        std::unordered_set<int> all_affectedT_statements_set = pkb.GetAllAffectedTStatements();
        REQUIRE(all_affectedT_statements_set.size() == 5);
        REQUIRE(ContainsExactly(all_affectedT_statements_set, {4, 6, 10, 11, 12}));
      }
    }
  }

  /* Sample Program in Appendix B: Extension Definition
     * https://github.com/nus-cs3203/project-wiki/wiki/Project-Iterations-2-and-3
     */
  //    procedure Bill {
  //            01      x = 5;
  //            02      call Mary;
  //            03      y = x + 6;
  //            04      x = 5;
  //            05      z = x \* y + 2; }
  //
  //    procedure Mary {
  //            06      y = x \* 3;
  //            07      call John;
  //            08      z = x + y; }
  //
  //    procedure John {
  //            09      if (i > 0) then {
  //                10              x = x + z; } }
  //    else {
  //        11              y = x \* y; } }

  GIVEN("pkb.InsertNextBip(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertNextBip(1, 2));
    REQUIRE(pkb.InsertNextBip(2, 6));
    REQUIRE(pkb.InsertNextBip(6, 7));
    REQUIRE(pkb.InsertNextBip(7, 9));
    REQUIRE(pkb.InsertNextBip(9, 10));
    REQUIRE(pkb.InsertNextBip(9, 11));
    REQUIRE(pkb.InsertNextBip(10, 8));
    REQUIRE(pkb.InsertNextBip(11, 8));
    REQUIRE(pkb.InsertNextBip(8, 3));
    REQUIRE(pkb.InsertNextBip(3, 4));
    REQUIRE(pkb.InsertNextBip(4, 5));

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 != stmt2 and stmt1 and stmt2 != 0.") {
      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 == stmt2 or stmt1 <= 0 or stmt2 <= 0.") {
      THEN("Returns False.") {
        REQUIRE_FALSE(pkb.InsertNextBip(3, 3));
        REQUIRE_FALSE(pkb.InsertNextBip(0, 2));
        REQUIRE_FALSE(pkb.InsertNextBip(3, 0));
      }
    }

    WHEN("Check if IsNextBip(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsNextBip(1, 2));
        REQUIRE(pkb.IsNextBip(2, 6));
        REQUIRE(pkb.IsNextBip(6, 7));
        REQUIRE(pkb.IsNextBip(7, 9));
        REQUIRE(pkb.IsNextBip(9, 10));
        REQUIRE(pkb.IsNextBip(9, 11));
        REQUIRE(pkb.IsNextBip(10, 8));
        REQUIRE(pkb.IsNextBip(11, 8));
        REQUIRE(pkb.IsNextBip(8, 3));
        REQUIRE(pkb.IsNextBip(3, 4));
        REQUIRE(pkb.IsNextBip(4, 5));
        REQUIRE_FALSE(pkb.IsNextBip(1, 3));
      }
    }

    WHEN("Check GetNextBipStatements(stmt1).") {
      THEN("Return a set of statements that are Next to stmt1.") {
        std::unordered_set<int> nextbip_statements_set_of_9 = pkb.GetNextBipStatements(9);
        REQUIRE(nextbip_statements_set_of_9.size() == 2);
        REQUIRE(ContainsExactly(nextbip_statements_set_of_9, {10, 11}));
      }
    }

    WHEN("Check GetPreviousBipStatements(stmt2).") {
      THEN("Return a set of statements that are PreviousBip to a stmt2.") {
        std::unordered_set<int> previousbip_statements_set_of_8 = pkb.GetPreviousBipStatements(8);
        REQUIRE(previousbip_statements_set_of_8.size() == 2);
        REQUIRE(ContainsExactly(previousbip_statements_set_of_8, {10, 11}));
      }
    }

    WHEN("Check GetAllNextStatements().") {
      THEN("Return a set of statements that Next to one or more other statements.") {
        std::unordered_set<int> all_nextbip_statements_set = pkb.GetAllNextBipStatements();
        REQUIRE(all_nextbip_statements_set.size() == 10);
        REQUIRE(ContainsExactly(all_nextbip_statements_set, {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}));
      }
    }

    WHEN("Check GetAllPreviousStatements().") {
      THEN("Return a set of statements that are Previous to one or more other statements.") {
        std::unordered_set<int> all_prevbip_statements_set = pkb.GetAllPreviousBipStatements();
        REQUIRE(all_prevbip_statements_set.size() == 10);
        REQUIRE(ContainsExactly(all_prevbip_statements_set, {1, 2, 3, 4, 6, 7, 8, 9, 10, 11}));
      }
    }
  }

  GIVEN("pkb.InsertNextBipT(stmt1, stmt2) called.") {
    std::vector<int> nextbip_values_of_1 = {2, 6, 7, 9, 10, 11, 8, 3, 4, 5};
    std::vector<int> nextbip_values_of_2 = {6, 7, 9, 10, 11, 8, 3, 4, 5};
    std::vector<int> nextbip_values_of_6 = {7, 9, 10, 11, 8, 3, 4, 5};
    std::vector<int> nextbip_values_of_7 = {9, 10, 11, 8, 3, 4, 5};
    std::vector<int> nextbip_values_of_9 = {10, 11, 8, 3, 4, 5};
    std::vector<int> nextbip_values_of_10 = {8, 3, 4, 5};
    std::vector<int> nextbip_values_of_11 = {8, 3, 4, 5};
    std::vector<int> nextbip_values_of_8 = {3, 4, 5};
    std::vector<int> nextbip_values_of_3 = {4, 5};
    std::vector<int> nextbip_values_of_4 = {5};

    for (const auto& value : nextbip_values_of_1) {
      REQUIRE(pkb.InsertNextBipT(1, value));
    }
    for (const auto& value : nextbip_values_of_2) {
      REQUIRE(pkb.InsertNextBipT(2, value));
    }
    for (const auto& value : nextbip_values_of_6) {
      REQUIRE(pkb.InsertNextBipT(6, value));
    }
    for (const auto& value : nextbip_values_of_7) {
      REQUIRE(pkb.InsertNextBipT(7, value));
    }
    for (const auto& value : nextbip_values_of_9) {
      REQUIRE(pkb.InsertNextBipT(9, value));
    }
    for (const auto& value : nextbip_values_of_10) {
      REQUIRE(pkb.InsertNextBipT(10, value));
    }
    for (const auto& value : nextbip_values_of_11) {
      REQUIRE(pkb.InsertNextBipT(11, value));
    }
    for (const auto& value : nextbip_values_of_8) {
      REQUIRE(pkb.InsertNextBipT(8, value));
    }
    for (const auto& value : nextbip_values_of_3) {
      REQUIRE(pkb.InsertNextBipT(3, value));
    }
    for (const auto& value : nextbip_values_of_4) {
      REQUIRE(pkb.InsertNextBipT(4, value));
    }

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 != stmt2 and stmt1 and stmt2 != 0.") {
      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 <= 0 or stmt2 <= 0.") {
      THEN("Returns False.") {
        REQUIRE_FALSE(pkb.InsertNextBipT(0, 2));
        REQUIRE_FALSE(pkb.InsertNextBipT(3, 0));
      }
    }

    WHEN("Check if IsNextT(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsNextBipT(1, 2));
        REQUIRE(pkb.IsNextBipT(1, 6));
        REQUIRE(pkb.IsNextBipT(1, 7));
        REQUIRE(pkb.IsNextBipT(1, 9));
        REQUIRE(pkb.IsNextBipT(1, 10));
        REQUIRE(pkb.IsNextBipT(1, 11));
        REQUIRE(pkb.IsNextBipT(1, 8));
        REQUIRE(pkb.IsNextBipT(1, 3));
        REQUIRE(pkb.IsNextBipT(1, 4));
        REQUIRE(pkb.IsNextBipT(1, 5));
        REQUIRE(pkb.IsNextBipT(2, 6));
        REQUIRE(pkb.IsNextBipT(2, 7));
        REQUIRE(pkb.IsNextBipT(2, 9));
        REQUIRE(pkb.IsNextBipT(2, 10));
        REQUIRE(pkb.IsNextBipT(2, 11));
        REQUIRE(pkb.IsNextBipT(2, 8));
        REQUIRE(pkb.IsNextBipT(2, 3));
        REQUIRE(pkb.IsNextBipT(2, 4));
        REQUIRE(pkb.IsNextBipT(2, 5));
        REQUIRE(pkb.IsNextBipT(6, 7));
        REQUIRE(pkb.IsNextBipT(6, 9));
        REQUIRE(pkb.IsNextBipT(6, 10));
        REQUIRE(pkb.IsNextBipT(6, 11));
        REQUIRE(pkb.IsNextBipT(6, 8));
        REQUIRE(pkb.IsNextBipT(6, 3));
        REQUIRE(pkb.IsNextBipT(6, 4));
        REQUIRE(pkb.IsNextBipT(6, 5));
        REQUIRE(pkb.IsNextBipT(7, 9));
        REQUIRE(pkb.IsNextBipT(7, 10));
        REQUIRE(pkb.IsNextBipT(7, 11));
        REQUIRE(pkb.IsNextBipT(7, 8));
        REQUIRE(pkb.IsNextBipT(7, 3));
        REQUIRE(pkb.IsNextBipT(7, 4));
        REQUIRE(pkb.IsNextBipT(7, 5));
        REQUIRE(pkb.IsNextBipT(9, 10));
        REQUIRE(pkb.IsNextBipT(9, 11));
        REQUIRE(pkb.IsNextBipT(9, 8));
        REQUIRE(pkb.IsNextBipT(9, 3));
        REQUIRE(pkb.IsNextBipT(9, 4));
        REQUIRE(pkb.IsNextBipT(9, 5));
        REQUIRE(pkb.IsNextBipT(10, 8));
        REQUIRE(pkb.IsNextBipT(10, 3));
        REQUIRE(pkb.IsNextBipT(10, 4));
        REQUIRE(pkb.IsNextBipT(10, 5));
        REQUIRE(pkb.IsNextBipT(11, 8));
        REQUIRE(pkb.IsNextBipT(11, 3));
        REQUIRE(pkb.IsNextBipT(11, 4));
        REQUIRE(pkb.IsNextBipT(11, 5));
        REQUIRE(pkb.IsNextBipT(8, 3));
        REQUIRE(pkb.IsNextBipT(8, 4));
        REQUIRE(pkb.IsNextBipT(8, 5));
        REQUIRE(pkb.IsNextBipT(3, 4));
        REQUIRE(pkb.IsNextBipT(3, 5));
        REQUIRE(pkb.IsNextBipT(4, 5));
      }
    }

    WHEN("Check GetNextTStatements(stmt1).") {
      THEN("Return a set of statements that are NextT to stmt1.") {
        std::unordered_set<int> nextbip_T_statements_set_of_1 = pkb.GetNextBipTStatements(1);
        REQUIRE(nextbip_T_statements_set_of_1.size() == 10);
        REQUIRE(ContainsExactly<int>(nextbip_T_statements_set_of_1, {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}));
      }
    }

    WHEN("Check GetPreviousTStatements(stmt2).") {
      THEN("Return a set of statements that are PreviousT to a stmt2.") {
        std::unordered_set<int> prevbip_T_statements_set_of_5 = pkb.GetPreviousBipTStatements(5);
        REQUIRE(prevbip_T_statements_set_of_5.size() == 10);
        REQUIRE(ContainsExactly<int>(prevbip_T_statements_set_of_5, {1, 2, 3, 4, 6, 7, 8, 9, 10, 11}));
      }
    }

    WHEN("Check GetAllNextTStatements().") {
      THEN("Return a set of statements that are NextT to one or more other statements.") {
        std::unordered_set<int> all_nextbip_T_statements_set = pkb.GetAllNextBipTStatements();
        REQUIRE(all_nextbip_T_statements_set.size() == 10);
        REQUIRE(ContainsExactly<int>(all_nextbip_T_statements_set, {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}));
      }
    }

    WHEN("Check GetAllPreviousTStatements().") {
      THEN("Return a set of statements that are PreviousT to one or more other statements.") {
        std::unordered_set<int> all_prevbip_T_statements_set = pkb.GetAllPreviousBipTStatements();
        REQUIRE(all_prevbip_T_statements_set.size() == 10);
        REQUIRE(ContainsExactly<int>(all_prevbip_T_statements_set, {1, 2, 3, 4, 6, 7, 8, 9, 10, 11}));
      }
    }
  }

  GIVEN("pkb.InsertAffectsBip(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertAffectsBip(1, 6));
    REQUIRE(pkb.InsertAffectsBip(1, 10));
    REQUIRE(pkb.InsertAffectsBip(1, 11));
    REQUIRE(pkb.InsertAffectsBip(1, 8));
    REQUIRE(pkb.InsertAffectsBip(1, 3));
    REQUIRE(pkb.InsertAffectsBip(3, 5));
    REQUIRE(pkb.InsertAffectsBip(4, 5));
    REQUIRE(pkb.InsertAffectsBip(6, 11));
    REQUIRE(pkb.InsertAffectsBip(6, 8));

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 != stmt2 and stmt1 and stmt2 != 0.") {
      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 <= 0 or stmt2 <= 0.") {
      THEN("Returns False.") {
        REQUIRE_FALSE(pkb.InsertAffects(0, 2));
        REQUIRE_FALSE(pkb.InsertAffects(3, 0));
      }
    }

    WHEN("Check if IsAffectsBip(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsAffectsBip(1, 6));
        REQUIRE(pkb.IsAffectsBip(1, 10));
        REQUIRE(pkb.IsAffectsBip(1, 11));
        REQUIRE(pkb.IsAffectsBip(1, 8));
        REQUIRE(pkb.IsAffectsBip(1, 3));
        REQUIRE(pkb.IsAffectsBip(3, 5));
        REQUIRE(pkb.IsAffectsBip(4, 5));
        REQUIRE(pkb.IsAffectsBip(6, 11));
        REQUIRE(pkb.IsAffectsBip(6, 8));
      }
    }

    WHEN("Check GetStatementsThatAffectsBip(stmt2).") {
      THEN("Return a set of statements that AffectsBip stmt2.") {
        std::unordered_set<int> stmts_that_affects_bip_11 = pkb.GetStatementsThatAffectsBip(11);
        REQUIRE(stmts_that_affects_bip_11.size() == 2);
        REQUIRE(ContainsExactly(stmts_that_affects_bip_11, {1, 6}));
      }
    }

    WHEN("Check GetAffectedBipStatements(stmt1).") {
      THEN("Return a set of statements that are Affected by stmt1.") {
        std::unordered_set<int> affected_bip_statements_by_1 = pkb.GetAffectedBipStatements(1);
        REQUIRE(affected_bip_statements_by_1.size() == 5);
        REQUIRE(ContainsExactly(affected_bip_statements_by_1, {3, 6, 8, 10, 11}));
      }
    }

    WHEN("Check GetAllStatementsThatAffectsBip().") {
      THEN("Return a set of statements that AffectsBip one or more other statements.") {
        std::unordered_set<int> all_affects_bip_statements_set = pkb.GetAllStatementsThatAffectsBip();
        REQUIRE(all_affects_bip_statements_set.size() == 4);
        REQUIRE(ContainsExactly(all_affects_bip_statements_set, {1, 3, 4, 6}));
      }
    }

    WHEN("Check GetAllAffectedBipStatements().") {
      THEN("Return a set of statements that are Previous to one or more other statements.") {
        std::unordered_set<int> all_affected_bip_statements_set = pkb.GetAllAffectedBipStatements();
        REQUIRE(all_affected_bip_statements_set.size() == 6);
        REQUIRE(ContainsExactly(all_affected_bip_statements_set, {3, 5, 6, 8, 10, 11}));
      }
    }
  }

  GIVEN("pkb.InsertAffectsBipT(stmt1, stmt2) called.") {
    REQUIRE(pkb.InsertAffectsBipT(1, 6));
    REQUIRE(pkb.InsertAffectsBipT(1, 10));
    REQUIRE(pkb.InsertAffectsBipT(1, 11));
    REQUIRE(pkb.InsertAffectsBipT(1, 8));
    REQUIRE(pkb.InsertAffectsBipT(1, 3));
    REQUIRE(pkb.InsertAffectsBipT(3, 5));
    REQUIRE(pkb.InsertAffectsBipT(4, 5));
    REQUIRE(pkb.InsertAffectsBipT(6, 11));
    REQUIRE(pkb.InsertAffectsBipT(6, 8));

    WHEN("Insert existing stmt1 with valid statement indexes stmt1 != stmt2 and stmt1 and stmt2 != 0.") {
      THEN("Returns True. Existing stmt_index maps to a list of statements that Follows it.") {
      }
    }

    WHEN("Insert with invalid statement indexes stmt1 <= 0 or stmt2 <= 0.") {
      THEN("Returns False.") {
        REQUIRE_FALSE(pkb.InsertAffectsBipT(0, 2));
        REQUIRE_FALSE(pkb.InsertAffectsBipT(3, 0));
      }
    }

    WHEN("Check if IsAffectsBipT(stmt1, stmt2) holds.") {
      THEN("Returns True if it holds. False otherwise.") {
        REQUIRE(pkb.IsAffectsBipT(1, 6));
        REQUIRE(pkb.IsAffectsBipT(1, 10));
        REQUIRE(pkb.IsAffectsBipT(1, 11));
        REQUIRE(pkb.IsAffectsBipT(1, 8));
        REQUIRE(pkb.IsAffectsBipT(1, 3));
        REQUIRE(pkb.IsAffectsBipT(3, 5));
        REQUIRE(pkb.IsAffectsBipT(4, 5));
        REQUIRE(pkb.IsAffectsBipT(6, 11));
        REQUIRE(pkb.IsAffectsBipT(6, 8));
      }
    }

    WHEN("Check GetStatementsThatAffectsBipT(stmt2).") {
      THEN("Return a set of statements that AffectsBipT stmt2.") {
        std::unordered_set<int> stmts_that_affectsBipT_11 = pkb.GetStatementsThatAffectsBipT(11);
        REQUIRE(stmts_that_affectsBipT_11.size() == 2);
        REQUIRE(ContainsExactly(stmts_that_affectsBipT_11, {1, 6}));
      }
    }

    WHEN("Check GetAffectedBipTStatements(stmt1).") {
      THEN("Return a set of statements that are AffectedT by stmt1.") {
        std::unordered_set<int> affected_bip_T_statements_by_1 = pkb.GetAffectedBipTStatements(1);
        REQUIRE(affected_bip_T_statements_by_1.size() == 5);
        REQUIRE(ContainsExactly(affected_bip_T_statements_by_1, {6, 10, 11, 8, 3}));
      }
    }

    WHEN("Check GetAllStatementsThatAffectsBipT().") {
      THEN("Return a set of statements that Next to one or more other statements.") {
        std::unordered_set<int> all_affects_bip_T_statements_set = pkb.GetAllStatementsThatAffectsBipT();
        REQUIRE(all_affects_bip_T_statements_set.size() == 4);
        REQUIRE(ContainsExactly(all_affects_bip_T_statements_set, {1, 3, 4, 6}));
      }
    }

    WHEN("Check GetAllAffectedBipTStatements().") {
      THEN("Return a set of statements that are Previous to one or more other statements.") {
        std::unordered_set<int> all_affected_bip_T_statements_set = pkb.GetAllAffectedBipTStatements();
        REQUIRE(all_affected_bip_T_statements_set.size() == 6);
        REQUIRE(ContainsExactly(all_affected_bip_T_statements_set, {3, 5, 6, 8, 10, 11}));
      }
    }
  }
}

SCENARIO("Clear a populated pkb.") {
  GIVEN("The pkb tables has already been populated.") {
    PKB pkb = BuildPKBSampleProgram();

    WHEN("Clear all entries in the pkb.") {
      pkb.ClearAllTables();

      THEN("pkb will clear all existing tables. The table sizes are now 0.") {
        REQUIRE(pkb.GetAllVariables().empty());
        REQUIRE(pkb.GetAllStmts().empty());
        REQUIRE(pkb.GetAllConstants().empty());
        REQUIRE(pkb.GetAllIfStmts().empty());
        REQUIRE(pkb.GetAllWhileStmts().empty());
        REQUIRE(pkb.GetAllReadStmts().empty());
        REQUIRE(pkb.GetAllPrintStmts().empty());
        REQUIRE(pkb.GetAllEntities().empty());
        REQUIRE(pkb.GetAllCallsStmts().empty());
        REQUIRE(pkb.GetAllAssignStmts().empty());
        REQUIRE(pkb.GetAllProcedures().empty());
        REQUIRE(pkb.GetAllFollowsStmts().empty());
        REQUIRE(pkb.GetAllFollowedStmts().empty());
        REQUIRE(pkb.GetAllFollowsTStmts().empty());
        REQUIRE(pkb.GetAllFollowedTStmts().empty());
        REQUIRE(pkb.GetAllParentStmts().empty());
        REQUIRE(pkb.GetAllParentTStmts().empty());
        REQUIRE(pkb.GetAllChildrenStmts().empty());
        REQUIRE(pkb.GetAllChildrenTStmts().empty());
        REQUIRE(pkb.GetAllModifiesStatements().empty());
        REQUIRE(pkb.GetAllUsesStatements().empty());
        REQUIRE(pkb.GetAllCallsStmts().empty());
        REQUIRE(pkb.GetAllProceduresThatCalls().empty());
        REQUIRE(pkb.GetAllProceduresThatCallsT().empty());
        REQUIRE(pkb.GetAllNextStatements().empty());
        REQUIRE(pkb.GetAllNextTStatements().empty());
        REQUIRE(pkb.GetAllPreviousStatements().empty());
        REQUIRE(pkb.GetAllPreviousTStatements().empty());
        REQUIRE(pkb.GetAllStatementsThatAffects().empty());
        REQUIRE(pkb.GetAllStatementsThatAffectsT().empty());
        REQUIRE(pkb.GetAllAffectedStatements().empty());
        REQUIRE(pkb.GetAllAffectedTStatements().empty());
        REQUIRE(pkb.GetAllNextBipStatements().empty());
        REQUIRE(pkb.GetAllNextBipTStatements().empty());
        REQUIRE(pkb.GetAllPreviousBipStatements().empty());
        REQUIRE(pkb.GetAllPreviousBipTStatements().empty());
        REQUIRE(pkb.GetAllStatementsThatAffectsBip().empty());
        REQUIRE(pkb.GetAllStatementsThatAffectsBipT().empty());
        REQUIRE(pkb.GetAllAffectedBipStatements().empty());
        REQUIRE(pkb.GetAllAffectedBipTStatements().empty());
      }
    }
  }
}