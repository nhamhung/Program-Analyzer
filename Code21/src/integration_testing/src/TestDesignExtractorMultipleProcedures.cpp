#include <functional>
#include <iostream>

#include "TestUtils.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "source_processor/ast/TNode.h"

using namespace std;

/*
EXAMPLE USAGE:

vector<int> ans = {14, 19}
REQUIRE(ContainsExactly<int>(pkb.GetAllParentStmts(), ans)) 

equivalent to:

auto tmp = pkb.GetAllParentStmts();
REQUIRE(tmp.size() == ans.size())
REQUIRE(Contains(tmp, 14))
REQUIRE(Contains(tmp, 19))
*/
template <typename T>
bool ContainsExactly(unordered_set<T> set, vector<T> expected_vals) {
  bool ok = true;

  if (set.size() != expected_vals.size()) {
    ok = false;
  }
  for (auto& v : expected_vals) {
    if (!Contains(set, v)) {
      ok = false;
    }
  }

  if (!ok) {
    cout << "Result size: " << set.size() << ". Expected: " << expected_vals.size() << endl;
    cout << "Result : {";
    auto it = set.begin();
    while (it != set.end()) {
      cout << *it;
      ++it;
      if (it != set.end()) {
        cout << ", ";
      }
    }
    cout << "}, Expected : {";
    auto it2 = expected_vals.begin();
    while (it2 != expected_vals.end()) {
      cout << *it2;
      ++it2;
      if (it2 != expected_vals.end()) {
        cout << ", ";
      }
    }
    cout << "}";
    return false;
  }
  return true;
}

/*
EXAMPLE USAGE:

vector<int> ins = {1, 2}
vector<int> outs = {2, 3}
REQUIRE(ContainsPairwise<int, int>(&PKB::GetStmtFollows, &pkb, ins, outs))

equivalent to:

REQUIRE(ins.size() == outs.size())
REQUIRE(pkb.GetStmtFollows(1) == 2);
REQUIRE(pkb.GetStmtFollows(2) == 3);
*/
template <typename I, typename O>
bool ContainsPairwise(O (PKB::*query_method)(I), PKB* pkbptr, vector<I> inputs, vector<O> expected_outputs) {
  if (inputs.size() != expected_outputs.size()) {
    cout << "container size difference. check if called correctly" << endl;
    return false;
  }

  auto callback = bind(query_method, pkbptr, placeholders::_1);
  for (int i = 0; i < inputs.size(); ++i) {
    if (callback(inputs[i]) != expected_outputs[i]) {
      cout << "i: " << i << "expected: " << expected_outputs[i] << " got: " << callback(inputs[i]) << endl;
      return false;
    }
  }
  return true;
}

SCENARIO("Test population of PKB with Call relations") {
  GIVEN("A valid SIMPLE source program with only linear call hierarchy") {
    /* NUMBERED PROGRAM STRING
    procedure A {                    \
      call B;                  #1    \
    }                                \
    procedure B {                    \
      call C;                  #2    \
    }                                \
    procedure C {                    \
      call D;                  #3    \
    }                                \
    procedure D {                    \
      call E;                  #4    \
    }                                \
    procedure E {                    \
      print endofcallchain; #5    \
    }                                \
    */
    const std::string test_program =
        "procedure A {                 \
        call B;                     \
      }                             \
      procedure B {                 \
        call C;                     \
      }                             \
      procedure C {                 \
        call D;                     \
      }                             \
      procedure D {                 \
        call E;                     \
      }                             \
      procedure E {                 \
        print endofcallchain;    \
      }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      std::unordered_set<int> call_statements = pkb.GetAllCallsStmts();

      std::unordered_set<std::string> A_calls = pkb.GetProceduresCalled("A");
      std::unordered_set<std::string> B_calls = pkb.GetProceduresCalled("B");
      std::unordered_set<std::string> C_calls = pkb.GetProceduresCalled("C");
      std::unordered_set<std::string> D_calls = pkb.GetProceduresCalled("D");
      std::unordered_set<std::string> E_calls = pkb.GetProceduresCalled("E");

      std::unordered_set<std::string> A_callsT = pkb.GetProceduresCalledT("A");
      std::unordered_set<std::string> B_callsT = pkb.GetProceduresCalledT("B");
      std::unordered_set<std::string> C_callsT = pkb.GetProceduresCalledT("C");
      std::unordered_set<std::string> D_callsT = pkb.GetProceduresCalledT("D");
      std::unordered_set<std::string> E_callsT = pkb.GetProceduresCalledT("E");

      THEN("PKB gets populated with calls statement information") {
        REQUIRE(ContainsExactly<int>(call_statements, {1, 2, 3, 4}));
      }

      THEN("PKB gets populated with Calls relation information") {
        REQUIRE(ContainsExactly<std::string>(A_calls, {"B"}));
        REQUIRE(ContainsExactly<std::string>(B_calls, {"C"}));
        REQUIRE(ContainsExactly<std::string>(C_calls, {"D"}));
        REQUIRE(ContainsExactly<std::string>(D_calls, {"E"}));
        REQUIRE(ContainsExactly<std::string>(E_calls, {}));
      }

      THEN("PKB gets populated with CallsT relation information") {
        REQUIRE(ContainsExactly<std::string>(A_callsT, {"B", "C", "D", "E"}));
        REQUIRE(ContainsExactly<std::string>(B_callsT, {"C", "D", "E"}));
        REQUIRE(ContainsExactly<std::string>(C_callsT, {"D", "E"}));
        REQUIRE(ContainsExactly<std::string>(D_callsT, {"E"}));
        REQUIRE(ContainsExactly<std::string>(E_callsT, {}));
      }
    }
  }

  GIVEN("A valid SIMPLE source program with branching call hierarchy") {
    const std::string test_program =
        "procedure A {       \
      call B;           \
      call C;           \
      call D;           \
    }                   \
    procedure B {       \
      call E;           \
      call F;           \
      call G;           \
    }                   \
    procedure C {       \
      call H;           \
      call I;           \
      call J;           \
    }                   \
    procedure D {       \
      call K;           \
      call L;           \
      call M;           \
    }                   \
    procedure E {       \
      print end;        \
    }                   \
    procedure F {       \
      print end;        \
    }                   \
    procedure G {       \
      print end;        \
    }                   \
    procedure H {       \
      print end;        \
    }                   \
    procedure I {       \
      print end;        \
    }                   \
    procedure J {       \
      print end;        \
    }                   \
    procedure K {       \
      print end;        \
    }                   \
    procedure L {       \
      print end;        \
    }                   \
    procedure M {       \
      print end;        \
    }";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      std::unordered_set<int> call_statements = pkb.GetAllCallsStmts();

      std::unordered_set<std::string> A_calls = pkb.GetProceduresCalled("A");
      std::unordered_set<std::string> B_calls = pkb.GetProceduresCalled("B");
      std::unordered_set<std::string> C_calls = pkb.GetProceduresCalled("C");
      std::unordered_set<std::string> D_calls = pkb.GetProceduresCalled("D");
      std::unordered_set<std::string> E_calls = pkb.GetProceduresCalled("E");
      std::unordered_set<std::string> F_calls = pkb.GetProceduresCalled("F");
      std::unordered_set<std::string> G_calls = pkb.GetProceduresCalled("G");
      std::unordered_set<std::string> H_calls = pkb.GetProceduresCalled("H");
      std::unordered_set<std::string> I_calls = pkb.GetProceduresCalled("I");
      std::unordered_set<std::string> J_calls = pkb.GetProceduresCalled("J");
      std::unordered_set<std::string> K_calls = pkb.GetProceduresCalled("K");
      std::unordered_set<std::string> L_calls = pkb.GetProceduresCalled("L");
      std::unordered_set<std::string> M_calls = pkb.GetProceduresCalled("M");

      std::unordered_set<std::string> A_callsT = pkb.GetProceduresCalledT("A");
      std::unordered_set<std::string> B_callsT = pkb.GetProceduresCalledT("B");
      std::unordered_set<std::string> C_callsT = pkb.GetProceduresCalledT("C");
      std::unordered_set<std::string> D_callsT = pkb.GetProceduresCalledT("D");
      std::unordered_set<std::string> E_callsT = pkb.GetProceduresCalledT("E");
      std::unordered_set<std::string> F_callsT = pkb.GetProceduresCalledT("F");
      std::unordered_set<std::string> G_callsT = pkb.GetProceduresCalledT("G");
      std::unordered_set<std::string> H_callsT = pkb.GetProceduresCalledT("H");
      std::unordered_set<std::string> I_callsT = pkb.GetProceduresCalledT("I");
      std::unordered_set<std::string> J_callsT = pkb.GetProceduresCalledT("J");
      std::unordered_set<std::string> K_callsT = pkb.GetProceduresCalledT("K");
      std::unordered_set<std::string> L_callsT = pkb.GetProceduresCalledT("L");
      std::unordered_set<std::string> M_callsT = pkb.GetProceduresCalledT("M");

      THEN("PKB gets populated with calls statement information") {
        REQUIRE(ContainsExactly<int>(call_statements, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}));
      }

      THEN("PKB gets populated with Calls relation information") {
        REQUIRE(ContainsExactly<std::string>(A_calls, {"B", "C", "D"}));
        REQUIRE(ContainsExactly<std::string>(B_calls, {"E", "F", "G"}));
        REQUIRE(ContainsExactly<std::string>(C_calls, {"H", "I", "J"}));
        REQUIRE(ContainsExactly<std::string>(D_calls, {"K", "L", "M"}));
        REQUIRE(ContainsExactly<std::string>(E_calls, {}));
        REQUIRE(ContainsExactly<std::string>(F_calls, {}));
        REQUIRE(ContainsExactly<std::string>(G_calls, {}));
        REQUIRE(ContainsExactly<std::string>(H_calls, {}));
        REQUIRE(ContainsExactly<std::string>(I_calls, {}));
        REQUIRE(ContainsExactly<std::string>(J_calls, {}));
        REQUIRE(ContainsExactly<std::string>(K_calls, {}));
        REQUIRE(ContainsExactly<std::string>(L_calls, {}));
        REQUIRE(ContainsExactly<std::string>(M_calls, {}));
      }

      THEN("PKB gets populated with CallsT relation information") {
        REQUIRE(ContainsExactly<std::string>(A_callsT, {"B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M"}));
        REQUIRE(ContainsExactly<std::string>(B_callsT, {"E", "F", "G"}));
        REQUIRE(ContainsExactly<std::string>(C_callsT, {"H", "I", "J"}));
        REQUIRE(ContainsExactly<std::string>(D_callsT, {"K", "L", "M"}));
        REQUIRE(ContainsExactly<std::string>(E_callsT, {}));
        REQUIRE(ContainsExactly<std::string>(F_callsT, {}));
        REQUIRE(ContainsExactly<std::string>(G_callsT, {}));
        REQUIRE(ContainsExactly<std::string>(H_callsT, {}));
        REQUIRE(ContainsExactly<std::string>(I_callsT, {}));
        REQUIRE(ContainsExactly<std::string>(J_callsT, {}));
        REQUIRE(ContainsExactly<std::string>(K_callsT, {}));
        REQUIRE(ContainsExactly<std::string>(L_callsT, {}));
        REQUIRE(ContainsExactly<std::string>(M_callsT, {}));
      }
    }
  }
}

SCENARIO("Test population of PKB with code 4 from CS3203 Wiki") {
  GIVEN("SIMPLE source code 4 from CS3203 Wiki") {
    //source: https://github.com/nus-cs3203/project-wiki/wiki/Full-(Basic-and-Advanced)-SPA-requirements#code-4-program-with-multiple-procedures-in-simple
    const std::string test_program =
        "\
      procedure main {\
        flag = 0;\
        call computeCentroid;\
        call printResults;\
      }\
      procedure readPoint {\
        read x;\
        read y;\
      }\
      procedure printResults {\
        print flag;\
        print cenX;\
        print cenY;\
        print normSq;\
      }\
      procedure computeCentroid {\
        count = 0;\
        cenX = 0;\
        cenY = 0;\
        call readPoint;\
        while ((x != 0) && (y != 0)) {\
          count = count + 1;\
          cenX = cenX + x;\
          cenY = cenY + y;\
          call readPoint;\
        }\
        if (count == 0) then {\
          flag = 1;\
        } else {\
          cenX = cenX / count;\
          cenY = cenY / count;\
        }\
          normSq = cenX * cenX + cenY * cenY;\
      }";
    /* NUMBERED PROGRAM STRING
       procedure main {
  01        flag = 0;
  02        call computeCentroid;
  03        call printResults;
       }
       procedure readPoint {
  04        read x;
  05        read y;
      }
      procedure printResults {
  06        print flag;
  07        print cenX;
  08        print cenY;
  09        print normSq;
      }
      procedure computeCentroid {
  10        count = 0;
  11        cenX = 0;
  12        cenY = 0;
  13        call readPoint;
  14        while ((x != 0) && (y != 0)) {
  15            count = count + 1;
  16            cenX = cenX + x;
  17            cenY = cenY + y;
  18            call readPoint;
            }
  19        if (count == 0) then {
  20            flag = 1;
            } else {
  21            cenX = cenX / count;
  22            cenY = cenY / count;
            }
  23        normSq = cenX * cenX + cenY * cenY;
      }
    */

    source_processor::TNode root = source_processor::Parser::Parse(test_program);
    WHEN("DE extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("Assignment stmts are properly populated") {
        auto assign_stmts = pkb.GetAllAssignStmts();
        REQUIRE(ContainsExactly<int>(assign_stmts, {1, 10, 11, 12, 15, 16, 17, 20, 21, 22, 23}));

        REQUIRE(pkb.GetAssignedVariable(1) == "flag");
        REQUIRE(pkb.GetAssignedVariable(10) == "count");
        REQUIRE(pkb.GetAssignedVariable(11) == "cenX");
        REQUIRE(pkb.GetAssignedVariable(12) == "cenY");
        REQUIRE(pkb.GetAssignedVariable(15) == "count");
        REQUIRE(pkb.GetAssignedVariable(16) == "cenX");
        REQUIRE(pkb.GetAssignedVariable(17) == "cenY");
        REQUIRE(pkb.GetAssignedVariable(20) == "flag");
        REQUIRE(pkb.GetAssignedVariable(21) == "cenX");
        REQUIRE(pkb.GetAssignedVariable(22) == "cenY");
        REQUIRE(pkb.GetAssignedVariable(23) == "normSq");

        //need to find a way to test RHS of assign
      }

      THEN("Program stmt numbers are properly populated") {
        auto all_stmts = pkb.GetAllStmts();
        REQUIRE(ContainsExactly<int>(all_stmts, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
      }

      THEN("Procedure stmt ranges are properly populated") { /*awaiting API*/
      }

      THEN("Constants are properly populated") {
        auto all_constants = pkb.GetAllConstants();
        REQUIRE(ContainsExactly<int>(all_constants, {0, 1}));
      }

      THEN("Entities are properly populated") {
        REQUIRE(pkb.GetStatementType(1) == "assign");
        REQUIRE(pkb.GetStatementType(2) == "call");
        REQUIRE(pkb.GetStatementType(3) == "call");
        REQUIRE(pkb.GetStatementType(4) == "read");
        REQUIRE(pkb.GetStatementType(5) == "read");
        REQUIRE(pkb.GetStatementType(6) == "print");
        REQUIRE(pkb.GetStatementType(7) == "print");
        REQUIRE(pkb.GetStatementType(8) == "print");
        REQUIRE(pkb.GetStatementType(9) == "print");
        REQUIRE(pkb.GetStatementType(10) == "assign");
        REQUIRE(pkb.GetStatementType(11) == "assign");
        REQUIRE(pkb.GetStatementType(12) == "assign");
        REQUIRE(pkb.GetStatementType(13) == "call");
        REQUIRE(pkb.GetStatementType(14) == "while");
        REQUIRE(pkb.GetStatementType(15) == "assign");
        REQUIRE(pkb.GetStatementType(16) == "assign");
        REQUIRE(pkb.GetStatementType(17) == "assign");
        REQUIRE(pkb.GetStatementType(18) == "call");
        REQUIRE(pkb.GetStatementType(19) == "if");
        REQUIRE(pkb.GetStatementType(20) == "assign");
        REQUIRE(pkb.GetStatementType(21) == "assign");
        REQUIRE(pkb.GetStatementType(22) == "assign");
        REQUIRE(pkb.GetStatementType(23) == "assign");
      }

      THEN("Read stmts are properly populated") {
        auto all_read_stmts = pkb.GetAllReadStmts();
        REQUIRE(ContainsExactly<int>(all_read_stmts, {4, 5}));

        //PKB hardcode these to return ""
        //REQUIRE(pkb.GetReadVarName(4) == "x");
        //REQUIRE(pkb.GetReadVarName(5) == "y");
      }

      THEN("Print stmts are properly populated") {
        auto all_print_stmts = pkb.GetAllPrintStmts();
        REQUIRE(ContainsExactly<int>(all_print_stmts, {6, 7, 8, 9}));

        //PKB hardcode these to return ""
        //REQUIRE(pkb.GetPrintVarName(6) == "flag");
        //REQUIRE(pkb.GetPrintVarName(7) == "cenX");
        //REQUIRE(pkb.GetPrintVarName(8) == "cenY");
        //REQUIRE(pkb.GetPrintVarName(9) == "normSq");
      }

      THEN("Variables are properly populated") {
        auto all_vars = pkb.GetAllVariables();
        REQUIRE(ContainsExactly<string>(all_vars, {"normSq", "flag", "cenY", "cenX", "count", "x", "y"}));
      }

      THEN("If stmts are properly populated") {
        auto all_ifs = pkb.GetAllIfStmts();
        REQUIRE(all_ifs.size() == 1);
        REQUIRE(Contains(all_ifs, 19));
      }

      THEN("If vars used are properly populated") {
        auto all_if_vars = pkb.GetVariablesUsedByIfStmt(19);
        REQUIRE(all_if_vars.size() == 1);
        REQUIRE(Contains(all_if_vars, "count"));
      }

      THEN("While stmts are properly populated") {
        auto all_whiles = pkb.GetAllWhileStmts();
        REQUIRE(all_whiles.size() == 1);
        REQUIRE(Contains(all_whiles, 14));
      }

      THEN("While vars used are properly populated") {
        auto all_while_vars = pkb.GetVariablesUsedByWhileStmt(14);
        REQUIRE(all_while_vars.size() == 2);
        REQUIRE(Contains(all_while_vars, "x"));
        REQUIRE(Contains(all_while_vars, "y"));
      }

      THEN("Follows relations are properly populated") {
        //procedure-level stmt lists
        REQUIRE(pkb.GetStmtFollows(1) == 2);
        REQUIRE(pkb.GetStmtFollows(2) == 3);

        REQUIRE(pkb.GetStmtFollows(4) == 5);

        REQUIRE(pkb.GetStmtFollows(6) == 7);
        REQUIRE(pkb.GetStmtFollows(7) == 8);
        REQUIRE(pkb.GetStmtFollows(8) == 9);

        REQUIRE(pkb.GetStmtFollows(10) == 11);
        REQUIRE(pkb.GetStmtFollows(11) == 12);
        REQUIRE(pkb.GetStmtFollows(12) == 13);
        REQUIRE(pkb.GetStmtFollows(13) == 14);
        REQUIRE(pkb.GetStmtFollows(14) == 19);
        REQUIRE(pkb.GetStmtFollows(19) == 23);

        //1 level of nesting
        REQUIRE(pkb.GetStmtFollows(15) == 16);
        REQUIRE(pkb.GetStmtFollows(16) == 17);
        REQUIRE(pkb.GetStmtFollows(17) == 18);

        REQUIRE(pkb.GetStmtFollows(21) == 22);
      }

      THEN("FollowsT relations are properly populated") {
        //proc main
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(1), {2, 3}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(2), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(3), {}));

        //proc readPoint
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(4), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(5), {}));

        //proc printResults
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(6), {7, 8, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(7), {8, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(8), {9}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(9), {}));

        //proc computeCentroid
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(10), {11, 12, 13, 14, 19, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(11), {12, 13, 14, 19, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(12), {13, 14, 19, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(13), {14, 19, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(14), {19, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(19), {23}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(23), {}));

        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(15), {16, 17, 18}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(16), {17, 18}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(17), {18}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(18), {}));

        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(21), {22}));
        REQUIRE(ContainsExactly<int>(pkb.GetStmtsFollowsT(22), {}));
      }

      THEN("Parent relations are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAllParentStmts(), {14, 19}));

        REQUIRE(ContainsExactly<int>(pkb.GetChildrenStatements(14), {15, 16, 17, 18}));
        REQUIRE(ContainsExactly<int>(pkb.GetChildrenStatements(19), {20, 21, 22}));
      }

      THEN("ParentT relations are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAllParentTStmts(), {14, 19}));

        REQUIRE(ContainsExactly<int>(pkb.GetChildrenTStatements(14), {15, 16, 17, 18}));
        REQUIRE(ContainsExactly<int>(pkb.GetChildrenTStatements(19), {20, 21, 22}));
      }

      THEN("Call stmts are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAllCallsStmts(), {2, 3, 13, 18}));

        REQUIRE(ContainsPairwise<int, string>(&PKB::GetCallsProcName, &pkb,
                                              {2, 3, 13, 18}, {"computeCentroid", "printResults", "readPoint", "readPoint"}));
      }

      THEN("Calls relations are properly populated") {
        REQUIRE(ContainsExactly<string>(pkb.GetProceduresCalled("main"), {"computeCentroid", "printResults"}));
        REQUIRE(ContainsExactly<string>(pkb.GetProceduresCalled("computeCentroid"), {"readPoint"}));
        REQUIRE(ContainsExactly<string>(pkb.GetProceduresCalled("readPoint"), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetProceduresCalled("printResults"), {}));
      }

      THEN("CallsT relations are properly populated") {
        REQUIRE(ContainsExactly<string>(pkb.GetProceduresCalledT("main"), {"computeCentroid", "printResults", "readPoint"}));
        REQUIRE(ContainsExactly<string>(pkb.GetProceduresCalledT("computeCentroid"), {"readPoint"}));
        REQUIRE(ContainsExactly<string>(pkb.GetProceduresCalledT("readPoint"), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetProceduresCalledT("printResults"), {}));
      }

      THEN("UsesS relations are properly populated") {
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(1), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(2), {"x", "y", "count", "cenX", "cenY"}));  //call stmt
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(3), {"flag", "cenX", "cenY", "normSq"}));   //call stmt
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(4), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(5), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(6), {"flag"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(7), {"cenX"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(8), {"cenY"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(9), {"normSq"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(10), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(11), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(12), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(13), {}));                                   //call
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(14), {"x", "y", "count", "cenX", "cenY"}));  //contains call
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(15), {"count"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(16), {"cenX", "x"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(17), {"cenY", "y"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(18), {}));  //call
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(19), {"count", "cenX", "cenY"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(20), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(21), {"count", "cenX"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(22), {"count", "cenY"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables(23), {"cenX", "cenY"}));
      }

      THEN("UsesP relations are properly populated") {
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables("main"), {"count", "flag", "cenX", "cenY", "x", "y", "normSq"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables("readPoint"), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables("printResults"), {"flag", "cenX", "cenY", "normSq"}));
        REQUIRE(ContainsExactly<string>(pkb.GetUsedVariables("computeCentroid"), {"count", "cenX", "cenY", "x", "y"}));
      }

      THEN("ModifiesS relations are properly populated") {
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(1), {"flag"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(2), {"count", "cenX", "cenY", "x", "y", "flag", "normSq"}));  //calls computecentroid
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(3), {}));                                                     //calls printResults
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(4), {"x"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(5), {"y"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(6), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(7), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(8), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(9), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(10), {"count"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(11), {"cenX"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(12), {"cenY"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(13), {"x", "y"}));                           //calls readPoint
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(14), {"count", "cenX", "cenY", "x", "y"}));  //contains readPoint
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(15), {"count"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(16), {"cenX"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(17), {"cenY"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(18), {"x", "y"}));  //calls readPoint
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(19), {"flag", "cenX", "cenY"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(20), {"flag"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(21), {"cenX"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(22), {"cenY"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables(23), {"normSq"}));
      }

      THEN("ModifiesP relations are properly populated") {
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables("main"), {"count", "cenX", "cenY", "x", "y", "flag", "normSq"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables("readPoint"), {"x", "y"}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables("printResults"), {}));
        REQUIRE(ContainsExactly<string>(pkb.GetModifiedVariables("computeCentroid"), {"count", "cenX", "cenY", "x", "y", "flag", "normSq"}));
      }

      THEN("Next relations are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(1), {2}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(2), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(4), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(6), {7}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(7), {8}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(8), {9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(10), {11}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(11), {12}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(12), {13}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(13), {14}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(14), {15, 19}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(15), {16}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(16), {17}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(17), {18}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(18), {14}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(19), {20, 21}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(20), {23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(21), {22}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(22), {23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextStatements(23), {}));
      }

      THEN("NextT relations are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(1), {2, 3}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(2), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(4), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(6), {7, 8, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(7), {8, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(8), {9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(10), {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(11), {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(12), {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(13), {14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(14), {14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(15), {14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(16), {14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(17), {14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(18), {14, 15, 16, 17, 18, 19, 20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(19), {20, 21, 22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(20), {23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(21), {22, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(22), {23}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextTStatements(23), {}));
      }
    }
  }
}

SCENARIO("(Scrappy) Test CallHandler::InverseCallGraph") {
  GIVEN("Program with multiple procedures but no call stmts") {
    const std::string test_program =
        "\
      procedure A {\
        print var;\
      }\
      procedure B {\
        print var;\
      }\
      procedure C {\
        print var;\
      }\
      procedure D {\
        print var;\
      }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);
    WHEN("DE extracts all designs") {
      THEN("There should be no assertion error") {
        PKB pkb = PKB();
        pkb.ClearAllTables();
        design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      }
    }
  }
}

SCENARIO("Test population of PKB with Affects/AffectsT relationship") {
  GIVEN("A valid SIMPLE source program") {
    const std::string test_program =
        "\
      procedure p {\
        a = 1;\
        b = a;\
        c = b;\
        d = c;\
        e = d;\
        }";

    /* NUMBERED PROGRAM STRING
      procedure p {
      1.    a = 1;
      2.    b = a;
      3.    c = b;
      4.    d = c;
      5.    e = d;
      }    */
    source_processor::TNode root = source_processor::Parser::Parse(test_program);
    WHEN("DE extract all affects relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("Affects relationships are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(1), {2}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(2), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(3), {4}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(4), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(5), {}));
      }

      THEN("AffectsT relationships are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(1), {2, 3, 4, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(2), {3, 4, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(3), {4, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(4), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(5), {}));
      }
    }
  }
}

SCENARIO("Test population of PKB with code 5 from CS3203 Wiki") {
  GIVEN("SIMPLE source code 5 from CS3203 Wiki") {
    //source: https://github.com/nus-cs3203/project-wiki/wiki/Full-(Basic-and-Advanced)-SPA-requirements#code-5-simple-program-used-to-explain-advanced-relationships-and-advanced-pql-queries
    const std::string test_program =
        "\
      procedure First {\
        read x;\
        read z;\
        call Second;\
      }\
      procedure Second {\
        x = 0;\
        i = 5;\
        while (i!=0) {\
          x = x + 2*y;\
          call Third;\
          i = i - 1;\
        }\
        if (x==1) then {\
          x = x+1;\
        } else {\
          z = 1;\
        }\
        z = z + x + i;\
        y = z + 2;\
        x = x * y + z;\
      }\
      procedure Third {\
        z = 5;\
        v = z;\
        print v;\
      } ";

    /* NUMBERED PROGRAM STRING
       procedure First {
  01      read x;
  02      read z;
  03      call Second;
       }
       procedure Second {
  04       x = 0;
  05       i = 5;
  06       while (i!=0) {
  07         x = x + 2 * y; 
  08         call Third;
  09         i = i - 1;
            }
  10       if (x == 1) then {
  11         x = x + 1; 
          } else {
  12         z = 1; 
           }
  13       z = z + x + i;
  14       y = z + 2;
  15       x = x * y + z; 
      }
      procedure Third {
  16       z = 5;
  17       v = z;
  18       print v;
      }
    */
    source_processor::TNode root = source_processor::Parser::Parse(test_program);
    WHEN("DE extract all affects relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("Affects relationships are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(4), {7, 11, 13, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(5), {9, 13}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(6), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(7), {7, 11, 13, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(9), {9, 13}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(10), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(11), {13, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(12), {13}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(13), {14, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(14), {15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(15), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(16), {17}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(17), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(18), {}));
      }

      THEN("AffectsT relationships are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(4), {7, 11, 13, 14, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(5), {9, 13, 14, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(6), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(7), {7, 11, 13, 14, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(9), {9, 13, 14, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(10), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(11), {13, 14, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(12), {13, 14, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(13), {14, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(14), {15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(15), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(16), {17}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(17), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(18), {}));
      }
    }
  }
}

SCENARIO("Test population of PKB with code 8 from CS3203 Wiki") {
  GIVEN("SIMPLE source code 8 from CS3203 Wiki") {
    //source: https://github.com/nus-cs3203/project-wiki/wiki/Full-(Basic-and-Advanced)-SPA-requirements#code-8-sample-code-for-affects-with-procedure-call-and-if-statement
    const std::string test_program =
        "\
      procedure p {\
        x = 1;\
        y = 2;\
        z = y;\
        call q;\
        z = x + y + z;}\
      procedure q {\
        x = 5;\
        t = 4; \
        if ( z > 0 ) then {\
          t = x + 1; }\
        else {\
          y = z + x; }\
        x = t + 1; }";

    /* NUMBERED PROGRAM STRING
      procedure p {
      1.    x = 1;
      2.    y = 2;
      3.    z = y;
      4.    call q;
      5.    z = x + y + z;}
      
      procedure q {
      6.    x = 5;
      7.    t = 4; 
      8.    if ( z > 0 ) then {
      9.        t = x + 1; }
            else {
      10.       y = z + x; }
      11.  x = t + 1; }
    */
    source_processor::TNode root = source_processor::Parser::Parse(test_program);
    WHEN("DE extract all affects relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("Affects relationships are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(2), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(3), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(6), {9, 10}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(7), {11}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(9), {11}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(10), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(11), {}));
      }

      THEN("AffectsT relationships are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(2), {3, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(3), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(6), {9, 10, 11}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(7), {11}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(9), {11}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(10), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(11), {}));
      }
    }
  }
}

SCENARIO("Test population of PKB with code 9 from CS3203 Wiki") {
  GIVEN("SIMPLE source code 8 from CS3203 Wiki") {
    //source: https://github.com/nus-cs3203/project-wiki/wiki/Full-(Basic-and-Advanced)-SPA-requirements#code-9-sample-code-for-affects-with-read-statement
    const std::string test_program =
        "\
      procedure p {\
        x = a;\
        read x;\
        v = x;\
        }";

    /* NUMBERED PROGRAM STRING
      procedure p {
      1.    x = a;
      2.    read x;
      3.    v = x;
      }    */
    source_processor::TNode root = source_processor::Parser::Parse(test_program);
    WHEN("DE extract all affects relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("Affects relationships are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedStatements(3), {}));
      }

      THEN("AffectsT relationships are properly populated") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedTStatements(3), {}));
      }
    }
  }
}