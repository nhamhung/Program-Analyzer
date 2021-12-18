#include "TestUtils.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "source_processor/ast/TNode.h"

SCENARIO("Test population of PKB with positive cases") {
  GIVEN("A valid SIMPLE source program") {
    /* NUMBERED PROGRAM STRING
    procedure proc {
      print a;        #1
      read b;         #2
      y = 10 + 15;    #3
      print c;        #4
      read d;         #5
    }
    */
    const std::string test_program = "procedure proc {print a; read b; y = 10 + 15; print c; read d;}";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all statement numbers") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> statements_list = pkb.GetAllStmts();

      THEN("PKB gets populated with variable information") {
        REQUIRE(statements_list.size() == 5);
        REQUIRE(Contains(statements_list, 1));
        REQUIRE(Contains(statements_list, 2));
        REQUIRE(Contains(statements_list, 3));
        REQUIRE(Contains(statements_list, 4));
        REQUIRE(Contains(statements_list, 5));
      }
    }

    WHEN("Design extractor extracts all variables") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<std::string> variable_list = pkb.GetAllVariables();

      THEN("PKB gets populated with variable information") {
        REQUIRE(variable_list.size() == 5);
        REQUIRE(Contains(variable_list, "a"));
        REQUIRE(Contains(variable_list, "b"));
        REQUIRE(Contains(variable_list, "y"));
        REQUIRE(Contains(variable_list, "c"));
        REQUIRE(Contains(variable_list, "d"));
      }
    }

    WHEN("Design extractor extracts all procedures") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<std::string> procedure_list = pkb.GetAllProcedures();

      THEN("PKB gets populated with procedure information") {
        REQUIRE(procedure_list.size() == 1);
        REQUIRE(Contains(procedure_list, "proc"));
      }
    }

    WHEN("Design extractor extracts all constants") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> constants_list = pkb.GetAllConstants();

      THEN("PKB gets populated with procedure information") {
        REQUIRE(constants_list.size() == 2);
        REQUIRE(Contains(constants_list, 10));
        REQUIRE(Contains(constants_list, 15));
      }
    }

    WHEN("Design extractor extracts all print statements") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> print_statements = pkb.GetAllPrintStmts();

      THEN("PKB gets populated with print statement information") {
        REQUIRE(print_statements.size() == 2);
        REQUIRE(Contains(print_statements, 1));
        REQUIRE(Contains(print_statements, 4));
      }
    }

    WHEN("Design extractor extracts all read statements") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> read_statements = pkb.GetAllReadStmts();

      THEN("PKB gets populated with read statement information") {
        REQUIRE(read_statements.size() == 2);
        REQUIRE(Contains(read_statements, 2));
        REQUIRE(Contains(read_statements, 5));
      }
    }

    WHEN("Design extractor extracts Follows relation") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with Follows relation information") {
        REQUIRE(pkb.GetStmtFollows(1) == 2);
        REQUIRE(pkb.GetStmtFollows(2) == 3);
        REQUIRE(pkb.GetStmtFollows(3) == 4);
        REQUIRE(pkb.GetStmtFollows(4) == 5);
      }
    }

    WHEN("Design extractor extracts FollowsT relation") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      std::unordered_set<int> followsT_1{2, 3, 4, 5};
      std::unordered_set<int> followsT_2{3, 4, 5};
      std::unordered_set<int> followsT_3{4, 5};
      std::unordered_set<int> followsT_4{5};

      THEN("PKB gets populated with FollowsT relation information") {
        REQUIRE(pkb.GetStmtsFollowsT(1) == followsT_1);
        REQUIRE(pkb.GetStmtsFollowsT(2) == followsT_2);
        REQUIRE(pkb.GetStmtsFollowsT(3) == followsT_3);
        REQUIRE(pkb.GetStmtsFollowsT(4) == followsT_4);
      }
    }

    WHEN("Design extractor extracts Parent relation") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with no Parent relation information") {
        //pkb decides to return -1 if parent doesn't exist
        REQUIRE(pkb.GetParentStatement(1) == -1);
        REQUIRE(pkb.GetParentStatement(2) == -1);
        REQUIRE(pkb.GetParentStatement(3) == -1);
        REQUIRE(pkb.GetParentStatement(4) == -1);
        REQUIRE(pkb.GetParentStatement(5) == -1);
      }
    }

    WHEN("Design extractor extracts ParentT relation") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with no ParentT relation information") {
        REQUIRE(pkb.GetParentTStatements(1).size() == 0);
        REQUIRE(pkb.GetParentTStatements(2).size() == 0);
        REQUIRE(pkb.GetParentTStatements(3).size() == 0);
        REQUIRE(pkb.GetParentTStatements(4).size() == 0);
        REQUIRE(pkb.GetParentTStatements(5).size() == 0);
      }
    }
  }

  GIVEN("A valid SIMPLE source program with 3 lines") {
    /* NUMBERED PROGRAM STRING
    procedure proc {
      print a;           #1
      read b;            #2
      y = 10 + 15;       #3
    }
    */
    const std::string test_program = "procedure proc {print a; read b; y = 10 + 15;}";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all follows statements") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with follows information") {
        REQUIRE(pkb.GetStmtFollows(1) == 2);
        REQUIRE(pkb.GetStmtFollows(2) == 3);
        REQUIRE(pkb.GetAllFollowsStmts().size() == 2);
        REQUIRE(pkb.GetStmtFollowedBy(2) == 1);
        REQUIRE(pkb.GetStmtFollowedBy(3) == 2);
        REQUIRE(pkb.GetAllFollowedStmts().size() == 2);
      }
    }

    WHEN("Design extractor extracts all followsT statements") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> followsT_list = pkb.GetStmtsFollowsT(1);
      std::unordered_set<int> followedT_list = pkb.GetStmtsFollowedTBy(3);

      THEN("PKB gets populated with followsT information") {
        REQUIRE(followsT_list.size() == 2);
        REQUIRE(Contains(followsT_list, 2));
        REQUIRE(Contains(followsT_list, 3));
        REQUIRE(followedT_list.size() == 2);
        REQUIRE(Contains(followedT_list, 1));
        REQUIRE(Contains(followedT_list, 2));
        REQUIRE(pkb.IsFollowsT(1, 2));
        REQUIRE(pkb.IsFollowsT(1, 3));
        REQUIRE(pkb.IsFollowsT(2, 3));
      }
    }
  }
}

SCENARIO("Test population of PKB with while loops and if-clauses") {
  GIVEN("A valid SIMPLE source program with while and if statements that have no conditional variables") {
    const std::string test_program =
        "procedure proc { \
          if (1 == 1) then { \
            b = 2; \
          } else {\
            while (4 > 5) {\
              c = d;\
            }\
          }\
        }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);
    WHEN("Design extractor extracts all container statements") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> if_stmts = pkb.GetAllIfStmts();
      std::unordered_set<std::string> if_vars = pkb.GetVariablesUsedByIfStmt(1);
      std::unordered_set<int> while_stmts = pkb.GetAllWhileStmts();
      std::unordered_set<std::string> while_vars = pkb.GetVariablesUsedByWhileStmt(3);

      THEN("PKB gets populated with container information") {
        REQUIRE(if_stmts.size() == 1);
        REQUIRE(Contains(if_stmts, 1));
        REQUIRE(if_vars.size() == 1);
        REQUIRE(Contains(if_vars, ""));

        REQUIRE(while_stmts.size() == 1);
        REQUIRE(Contains(while_stmts, 3));
        REQUIRE(while_vars.size() == 1);
        REQUIRE(Contains(while_vars, ""));
      }
    }
  }

  GIVEN("A valid SIMPLE source program with a while loop and if clause") {
    /* NUMBERED PROGRAM STRING
    procedure proc {
        print a;                    #1
        while (i != 0) {            #2
          y = 10 + 15;              #3
          read b;                   #4
        }
        if (num1 > num2) then {     #5
          temp = num1;              #6
        } else {
          noSwap = 1;               #7
        }
    }
    */
    const std::string test_program =
        "procedure proc { \
        print a; \
        while(i != 0) {y = 10 + 15; read b;} \
        if (num1 > num2) \
        then { temp = num1;} \
        else {noSwap = 1;}}";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all assignment statements") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> assign_statements = pkb.GetAllAssignStmts();

      THEN("PKB gets populated with assignment information") {
        REQUIRE(assign_statements.size() == 3);
        REQUIRE(Contains(assign_statements, 3));
        REQUIRE(Contains(assign_statements, 6));
        REQUIRE(Contains(assign_statements, 7));
      }
    }

    WHEN("Design extractor extracts all parent relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> children_while = pkb.GetChildrenStatements(2);
      std::unordered_set<int> children_if = pkb.GetChildrenStatements(5);

      THEN("PKB gets populated with parent information") {
        REQUIRE(children_while.size() == 2);
        REQUIRE(Contains(children_while, 3));
        REQUIRE(Contains(children_while, 4));
        REQUIRE(children_if.size() == 2);
        REQUIRE(Contains(children_if, 6));
        REQUIRE(Contains(children_if, 7));
      }
    }

    WHEN("Design extractor extracts all parentT relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> childrenT_while = pkb.GetChildrenTStatements(2);
      std::unordered_set<int> childrenT_if = pkb.GetChildrenTStatements(5);

      THEN("PKB gets populated with parent information") {
        REQUIRE(childrenT_while.size() == 2);
        REQUIRE(Contains(childrenT_while, 3));
        REQUIRE(Contains(childrenT_while, 4));
        REQUIRE(childrenT_if.size() == 2);
        REQUIRE(Contains(childrenT_if, 6));
        REQUIRE(Contains(childrenT_if, 7));
      }
    }

    WHEN("Design extractor extracts Modifies relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      std::unordered_set<int> modifiers_of_y = pkb.GetModifiesStatements("y");
      std::unordered_set<int> modifiers_of_b = pkb.GetModifiesStatements("b");
      std::unordered_set<int> modifiers_of_temp = pkb.GetModifiesStatements("temp");
      std::unordered_set<int> modifiers_of_noSwap = pkb.GetModifiesStatements("noSwap");

      THEN("PKB gets populated with Modifies information") {
        REQUIRE(modifiers_of_y.size() == 2);
        REQUIRE(Contains(modifiers_of_y, 3));
        REQUIRE(Contains(modifiers_of_y, 2));

        REQUIRE(modifiers_of_b.size() == 2);
        REQUIRE(Contains(modifiers_of_b, 4));
        REQUIRE(Contains(modifiers_of_b, 2));

        REQUIRE(modifiers_of_noSwap.size() == 2);
        REQUIRE(Contains(modifiers_of_noSwap, 7));
        REQUIRE(Contains(modifiers_of_noSwap, 5));
      }
    }
  }

  GIVEN("A valid SIMPLE source program with triply nested while loop") {
    /* NUMBERED PROGRAM STRING
    procedure proc {
      while (i < 0) {          #1
        while (j < 0) {        #2
          while (k == 0) {     #3
            num = 1;           #4
          }
        }
      }
    }
    */
    const std::string test_program =
        "procedure proc { \
          while (i < 0) { \
            while (j < 0) {\
              while (k == 0) {\
                num = 1;\
              }\
            }\
          }\
        }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all parent relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> children_while = pkb.GetChildrenStatements(1);
      std::unordered_set<int> children_nested_while = pkb.GetChildrenStatements(2);
      std::unordered_set<int> children_triple_nested_while = pkb.GetChildrenStatements(3);

      THEN("PKB gets populated with parent information") {
        REQUIRE(children_while.size() == 1);
        REQUIRE(Contains(children_while, 2));
        REQUIRE(children_nested_while.size() == 1);
        REQUIRE(Contains(children_nested_while, 3));
        REQUIRE(children_triple_nested_while.size() == 1);
        REQUIRE(Contains(children_triple_nested_while, 4));
      }
    }

    WHEN("Design extractor extracts all parentT relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> childrenT_while = pkb.GetChildrenTStatements(1);
      std::unordered_set<int> childrenT_nested_while = pkb.GetChildrenTStatements(2);
      std::unordered_set<int> childrenT_triple_nested_while = pkb.GetChildrenTStatements(3);

      THEN("PKB gets populated with parent information") {
        REQUIRE(childrenT_while.size() == 3);
        REQUIRE(Contains(childrenT_while, 2));
        REQUIRE(Contains(childrenT_while, 3));
        REQUIRE(Contains(childrenT_while, 4));

        REQUIRE(childrenT_nested_while.size() == 2);
        REQUIRE(Contains(childrenT_nested_while, 3));
        REQUIRE(Contains(childrenT_nested_while, 4));

        REQUIRE(childrenT_triple_nested_while.size() == 1);
        REQUIRE(Contains(childrenT_triple_nested_while, 4));
      }
    }

    WHEN("Design extractor extracts Modifies relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      std::unordered_set<int> modifiers_of_num = pkb.GetModifiesStatements("num");

      THEN("PKB gets populated with Modifies information") {
        REQUIRE(modifiers_of_num.size() == 4);
        REQUIRE(Contains(modifiers_of_num, 4));
        REQUIRE(Contains(modifiers_of_num, 3));
        REQUIRE(Contains(modifiers_of_num, 2));
        REQUIRE(Contains(modifiers_of_num, 1));
      }
    }
  }

  GIVEN("A valid SIMPLE source program with triply nested if clause in if-block") {
    /* NUMBERED PROGRAM STRING
    procedure proc {
      if (i < 0) then {        #1
        if (j < 0) then {      #2
          if (k == 0) then {   #3
            num = 1;           #4
          } else {
            num = 2;           #5
          }
        } else {
          num = 2;             #6
        }
      } else {
        read a;                #7
      }
    }
    */
    const std::string test_program =
        "procedure proc { \
          if (i < 0) then { \
            if (j < 0) then {\
              if (k == 0) then {\
                num = 1;\
              } else {\
                num = 2;\
              }\
            } else { \
              num = 2; \
            } \
          } else { \
            read a; \
          } \
        }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all parent relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> children_if = pkb.GetChildrenStatements(1);
      std::unordered_set<int> children_nested_if = pkb.GetChildrenStatements(2);
      std::unordered_set<int> children_triple_nested_if = pkb.GetChildrenStatements(3);

      THEN("PKB gets populated with parent information") {
        REQUIRE(children_if.size() == 2);
        REQUIRE(Contains(children_if, 2));
        REQUIRE(Contains(children_if, 7));
        REQUIRE(children_nested_if.size() == 2);
        REQUIRE(Contains(children_nested_if, 3));
        REQUIRE(Contains(children_nested_if, 6));
        REQUIRE(children_triple_nested_if.size() == 2);
        REQUIRE(Contains(children_triple_nested_if, 4));
        REQUIRE(Contains(children_triple_nested_if, 5));
      }
    }

    WHEN("Design extractor extracts all parentT relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> childrenT_if = pkb.GetChildrenTStatements(1);
      std::unordered_set<int> childrenT_nested_if = pkb.GetChildrenTStatements(2);
      std::unordered_set<int> childrenT_triple_nested_if = pkb.GetChildrenTStatements(3);

      THEN("PKB gets populated with parent information") {
        REQUIRE(childrenT_if.size() == 6);
        REQUIRE(Contains(childrenT_if, 2));
        REQUIRE(Contains(childrenT_if, 3));
        REQUIRE(Contains(childrenT_if, 4));
        REQUIRE(Contains(childrenT_if, 5));
        REQUIRE(Contains(childrenT_if, 6));
        REQUIRE(Contains(childrenT_if, 7));

        REQUIRE(childrenT_nested_if.size() == 4);
        REQUIRE(Contains(childrenT_nested_if, 3));
        REQUIRE(Contains(childrenT_nested_if, 4));
        REQUIRE(Contains(childrenT_nested_if, 5));
        REQUIRE(Contains(childrenT_nested_if, 6));

        REQUIRE(childrenT_triple_nested_if.size() == 2);
        REQUIRE(Contains(childrenT_triple_nested_if, 4));
        REQUIRE(Contains(childrenT_triple_nested_if, 5));
      }
    }

    WHEN("Design extractor extracts Modifies relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      std::unordered_set<int> modifiers_of_num = pkb.GetModifiesStatements("num");
      std::unordered_set<int> modifiers_of_a = pkb.GetModifiesStatements("a");

      THEN("PKB gets populated with Modifies information") {
        REQUIRE(modifiers_of_num.size() == 6);
        REQUIRE(Contains(modifiers_of_num, 6));
        REQUIRE(Contains(modifiers_of_num, 5));
        REQUIRE(Contains(modifiers_of_num, 4));
        REQUIRE(Contains(modifiers_of_num, 3));
        REQUIRE(Contains(modifiers_of_num, 2));
        REQUIRE(Contains(modifiers_of_num, 1));

        REQUIRE(modifiers_of_a.size() == 2);
        REQUIRE(Contains(modifiers_of_a, 7));
        REQUIRE(Contains(modifiers_of_a, 1));
      }
    }
  }

  GIVEN("A valid SIMPLE source program with triply nested if clause in else-block") {
    /* NUMBERED PROGRAM STRING
    procedure proc {
      if (i < 0) then {          #1
        if (j < 0) then {        #2
          num = 2;               #3
        } else {
          if (k == 0) then {     #4
            num = 1;             #5
          } else {
            num = 2;             #6
          }
        }
      } else {
        read a;                  #7
      }
    }

    */

    const std::string test_program =
        "procedure proc { \
          if (i < 0) then { \
            if (j < 0) then {\
              num = 2; \
            } else { \
              if (k == 0) then {\
                num = 1;\
              } else {\
                num = 2;\
              }\
            } \
          } else { \
            read a; \
          } \
        }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all parent relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> children_if = pkb.GetChildrenStatements(1);
      std::unordered_set<int> children_nested_if = pkb.GetChildrenStatements(2);
      std::unordered_set<int> children_triple_nested_if = pkb.GetChildrenStatements(4);

      THEN("PKB gets populated with parent information") {
        REQUIRE(children_if.size() == 2);
        REQUIRE(Contains(children_if, 2));
        REQUIRE(Contains(children_if, 7));
        REQUIRE(children_nested_if.size() == 2);
        REQUIRE(Contains(children_nested_if, 3));
        REQUIRE(Contains(children_nested_if, 4));
        REQUIRE(children_triple_nested_if.size() == 2);
        REQUIRE(Contains(children_triple_nested_if, 5));
        REQUIRE(Contains(children_triple_nested_if, 6));
      }
    }

    WHEN("Design extractor extracts all parentT relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> childrenT_if = pkb.GetChildrenTStatements(1);
      std::unordered_set<int> childrenT_nested_if = pkb.GetChildrenTStatements(2);
      std::unordered_set<int> childrenT_triple_nested_if = pkb.GetChildrenTStatements(4);

      THEN("PKB gets populated with parent information") {
        REQUIRE(childrenT_if.size() == 6);
        REQUIRE(Contains(childrenT_if, 2));
        REQUIRE(Contains(childrenT_if, 3));
        REQUIRE(Contains(childrenT_if, 4));
        REQUIRE(Contains(childrenT_if, 5));
        REQUIRE(Contains(childrenT_if, 6));
        REQUIRE(Contains(childrenT_if, 7));

        REQUIRE(childrenT_nested_if.size() == 4);
        REQUIRE(Contains(childrenT_nested_if, 3));
        REQUIRE(Contains(childrenT_nested_if, 4));
        REQUIRE(Contains(childrenT_nested_if, 5));
        REQUIRE(Contains(childrenT_nested_if, 6));

        REQUIRE(childrenT_triple_nested_if.size() == 2);
        REQUIRE(Contains(childrenT_triple_nested_if, 5));
        REQUIRE(Contains(childrenT_triple_nested_if, 6));
      }
    }

    WHEN("Design extractor extracts all Modifies relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      /*
      modifies:
        RAW:
        3 num
        2 num
        1 num

        5 num
        4 num
        2 num
        1 num

        6 num
        4 num
        2 num
        1 num

        7 a
        1 a

      COMBINED:
        1 num
        2 num
        3 num
        4 num
        5 num
        6 num

        7 a
        1 a
      */

      std::unordered_set<int> modifiers_of_num = pkb.GetModifiesStatements("num");
      std::unordered_set<int> modifiers_of_a = pkb.GetModifiesStatements("a");

      THEN("PKB gets populated with Modifies information") {
        REQUIRE(modifiers_of_num.size() == 6);
        REQUIRE(Contains(modifiers_of_num, 1));
        REQUIRE(Contains(modifiers_of_num, 2));
        REQUIRE(Contains(modifiers_of_num, 3));
        REQUIRE(Contains(modifiers_of_num, 4));
        REQUIRE(Contains(modifiers_of_num, 5));
        REQUIRE(Contains(modifiers_of_num, 6));

        REQUIRE(modifiers_of_a.size() == 2);
        REQUIRE(Contains(modifiers_of_a, 7));
        REQUIRE(Contains(modifiers_of_a, 1));
      }
    }
  }

  GIVEN("A valid SIMPLE source program with mix of if and while nested") {
    /* NUMBERED PROGRAM STRING
    procedure proc {
      if (i == 1) then {       #1.
        if (i == 2) then {     #2.
          while (i == 3) {     #3
            print a4;          #4
          }
        } else {
          read b5;             #5.
        }
      } else {
        while (i == 6) {       #6
          if (i == 7) then {   #7
            a = 8 + 219;       #8.
          } else {
            print e9;          #9
          }
          read c10;            #10.
        }
        i = 11;                #11.
      }
    }
    */
    const std::string test_program =
        "procedure proc { \
          if (i == 1) then {\
            if (i == 2) then {\
              while (i == 3) {\
                print a4;\
              }\
            } else {\
              read b5;\
            }\
          } else {\
            while (i == 6) {\
              if (i == 7) then {\
                a = 8 + 219;\
              } else {\
                print e9;\
              }\
              read c10;\
            }\
            i = 11;\
          }\
        }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all parent relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> children_if = pkb.GetChildrenStatements(1);
      std::unordered_set<int> children_if_2 = pkb.GetChildrenStatements(2);
      std::unordered_set<int> children_while = pkb.GetChildrenStatements(3);
      std::unordered_set<int> children_while_2 = pkb.GetChildrenStatements(6);
      std::unordered_set<int> children_if_3 = pkb.GetChildrenStatements(7);

      THEN("PKB gets populated with parent information") {
        REQUIRE(children_if.size() == 3);
        REQUIRE(Contains(children_if, 2));
        REQUIRE(Contains(children_if, 6));
        REQUIRE(Contains(children_if, 11));

        REQUIRE(children_if_2.size() == 2);
        REQUIRE(Contains(children_if_2, 3));
        REQUIRE(Contains(children_if_2, 5));

        REQUIRE(children_while.size() == 1);
        REQUIRE(Contains(children_while, 4));

        REQUIRE(children_while_2.size() == 2);
        REQUIRE(Contains(children_while_2, 7));
        REQUIRE(Contains(children_while_2, 10));

        REQUIRE(children_if_3.size() == 2);
        REQUIRE(Contains(children_if_3, 8));
        REQUIRE(Contains(children_if_3, 9));
      }
    }

    WHEN("Design extractor extracts all parentT relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> childrenT_if = pkb.GetChildrenTStatements(1);
      std::unordered_set<int> childrenT_if_2 = pkb.GetChildrenTStatements(2);
      std::unordered_set<int> childrenT_while = pkb.GetChildrenTStatements(3);
      std::unordered_set<int> childrenT_while_2 = pkb.GetChildrenTStatements(6);
      std::unordered_set<int> childrenT_if_3 = pkb.GetChildrenTStatements(7);

      THEN("PKB gets populated with parentT information") {
        REQUIRE(childrenT_if.size() == 10);
        REQUIRE(Contains(childrenT_if, 2));
        REQUIRE(Contains(childrenT_if, 3));
        REQUIRE(Contains(childrenT_if, 4));
        REQUIRE(Contains(childrenT_if, 5));
        REQUIRE(Contains(childrenT_if, 6));
        REQUIRE(Contains(childrenT_if, 7));
        REQUIRE(Contains(childrenT_if, 8));
        REQUIRE(Contains(childrenT_if, 9));
        REQUIRE(Contains(childrenT_if, 10));
        REQUIRE(Contains(childrenT_if, 11));

        REQUIRE(childrenT_if_2.size() == 3);
        REQUIRE(Contains(childrenT_if_2, 3));
        REQUIRE(Contains(childrenT_if_2, 4));
        REQUIRE(Contains(childrenT_if_2, 5));

        REQUIRE(childrenT_while.size() == 1);
        REQUIRE(Contains(childrenT_while, 4));

        REQUIRE(childrenT_while_2.size() == 4);
        REQUIRE(Contains(childrenT_while_2, 7));
        REQUIRE(Contains(childrenT_while_2, 8));
        REQUIRE(Contains(childrenT_while_2, 9));
        REQUIRE(Contains(childrenT_while_2, 10));

        REQUIRE(childrenT_if_3.size() == 2);
        REQUIRE(Contains(childrenT_if_3, 8));
        REQUIRE(Contains(childrenT_if_3, 9));
      }
    }

    WHEN("Design extractor extracts Modifies relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      std::unordered_set<int> modifiers_of_b5 = pkb.GetModifiesStatements("b5");
      std::unordered_set<int> modifiers_of_a = pkb.GetModifiesStatements("a");
      std::unordered_set<int> modifiers_of_c10 = pkb.GetModifiesStatements("c10");
      std::unordered_set<int> modifiers_of_i = pkb.GetModifiesStatements("i");

      THEN("PKB gets populated with Modifies information") {
        /*
          modifies:
          <stmtno, variable_modified>
          5, b5
          2, b5
          1, b5

          8, a
          7, a
          6, a
          1, a

          10, c10
          6, c10
          1, c10

          11, i
          1, i
        */
        REQUIRE(modifiers_of_b5.size() == 3);
        REQUIRE(Contains(modifiers_of_b5, 5));
        REQUIRE(Contains(modifiers_of_b5, 2));
        REQUIRE(Contains(modifiers_of_b5, 1));

        REQUIRE(modifiers_of_a.size() == 4);
        REQUIRE(Contains(modifiers_of_a, 8));
        REQUIRE(Contains(modifiers_of_a, 7));
        REQUIRE(Contains(modifiers_of_a, 6));
        REQUIRE(Contains(modifiers_of_a, 1));

        REQUIRE(modifiers_of_c10.size() == 3);
        REQUIRE(Contains(modifiers_of_c10, 10));
        REQUIRE(Contains(modifiers_of_c10, 6));
        REQUIRE(Contains(modifiers_of_c10, 1));

        REQUIRE(modifiers_of_i.size() == 2);
        REQUIRE(Contains(modifiers_of_i, 11));
        REQUIRE(Contains(modifiers_of_i, 1));
      }
    }
  }

  GIVEN("A valid SIMPLE source program with multiple variables and nested clauses") {
    const std::string test_program =
        "procedure proc { \
          if ((i == 1) && (foo == bar)) then {\
            if (j == 2) then {\
              while (k == 3) {\
                print a4;\
              }\
            } else {\
              read b5;\
            }\
          } else {\
            while (l == 6) {\
              if (m == 7) then {\
                a = 8 + 219;\
              } else {\
                print e9;\
              }\
              read c10;\
            }\
            i = 11;\
          }\
        }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all While statements") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> while_stmts = pkb.GetAllWhileStmts();
      std::unordered_set<std::string> while_vars_3 = pkb.GetVariablesUsedByWhileStmt(3);
      std::unordered_set<std::string> while_vars_6 = pkb.GetVariablesUsedByWhileStmt(6);

      THEN("PKB gets populated with While information") {
        REQUIRE(while_stmts.size() == 2);
        REQUIRE(Contains(while_stmts, 3));
        REQUIRE(Contains(while_stmts, 6));
        REQUIRE(while_vars_3.size() == 1);
        REQUIRE(Contains(while_vars_3, "k"));
        REQUIRE(while_vars_6.size() == 1);
        REQUIRE(Contains(while_vars_6, "l"));
      }
    }

    WHEN("Design extractor extracts all If statements") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<int> if_stmts = pkb.GetAllIfStmts();
      std::unordered_set<std::string> if_vars_1 = pkb.GetVariablesUsedByIfStmt(1);
      std::unordered_set<std::string> if_vars_2 = pkb.GetVariablesUsedByIfStmt(2);
      std::unordered_set<std::string> if_vars_7 = pkb.GetVariablesUsedByIfStmt(7);

      THEN("PKB gets populated with if information") {
        REQUIRE(if_stmts.size() == 3);
        REQUIRE(Contains(if_stmts, 1));
        REQUIRE(Contains(if_stmts, 2));
        REQUIRE(Contains(if_stmts, 7));
        REQUIRE(if_vars_1.size() == 3);
        REQUIRE(Contains(if_vars_1, "i"));
        REQUIRE(Contains(if_vars_1, "foo"));
        REQUIRE(Contains(if_vars_1, "bar"));
        REQUIRE(if_vars_2.size() == 1);
        REQUIRE(Contains(if_vars_2, "j"));
        REQUIRE(if_vars_7.size() == 1);
        REQUIRE(Contains(if_vars_7, "m"));
      }
    }

    WHEN("Design extractor extracts all UsesS relationships") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<std::string> var_used_by_1 = pkb.GetUsedVariables(1);
      std::unordered_set<std::string> var_used_by_2 = pkb.GetUsedVariables(2);
      std::unordered_set<std::string> var_used_by_3 = pkb.GetUsedVariables(3);
      std::unordered_set<std::string> var_used_by_4 = pkb.GetUsedVariables(4);
      std::unordered_set<std::string> var_used_by_5 = pkb.GetUsedVariables(5);
      std::unordered_set<std::string> var_used_by_6 = pkb.GetUsedVariables(6);
      std::unordered_set<std::string> var_used_by_7 = pkb.GetUsedVariables(7);
      std::unordered_set<std::string> var_used_by_8 = pkb.GetUsedVariables(8);
      std::unordered_set<std::string> var_used_by_9 = pkb.GetUsedVariables(9);
      std::unordered_set<std::string> var_used_by_10 = pkb.GetUsedVariables(10);
      std::unordered_set<std::string> var_used_by_11 = pkb.GetUsedVariables(11);

      THEN("PKB gets populated with UsesS information") {
        REQUIRE(var_used_by_1.size() == 9);
        REQUIRE(Contains(var_used_by_1, "i"));
        REQUIRE(Contains(var_used_by_1, "foo"));
        REQUIRE(Contains(var_used_by_1, "bar"));
        REQUIRE(Contains(var_used_by_1, "j"));
        REQUIRE(Contains(var_used_by_1, "k"));
        REQUIRE(Contains(var_used_by_1, "a4"));
        REQUIRE(Contains(var_used_by_1, "l"));
        REQUIRE(Contains(var_used_by_1, "m"));
        REQUIRE(Contains(var_used_by_1, "e9"));

        REQUIRE(var_used_by_2.size() == 3);
        REQUIRE(Contains(var_used_by_2, "j"));
        REQUIRE(Contains(var_used_by_2, "k"));
        REQUIRE(Contains(var_used_by_2, "a4"));

        REQUIRE(var_used_by_3.size() == 2);
        REQUIRE(Contains(var_used_by_3, "k"));
        REQUIRE(Contains(var_used_by_3, "a4"));

        REQUIRE(var_used_by_4.size() == 1);
        REQUIRE(Contains(var_used_by_4, "a4"));

        REQUIRE(var_used_by_5.size() == 0);

        REQUIRE(var_used_by_6.size() == 3);
        REQUIRE(Contains(var_used_by_6, "l"));
        REQUIRE(Contains(var_used_by_6, "m"));
        REQUIRE(Contains(var_used_by_6, "e9"));

        REQUIRE(var_used_by_7.size() == 2);
        REQUIRE(Contains(var_used_by_7, "m"));
        REQUIRE(Contains(var_used_by_7, "e9"));

        REQUIRE(var_used_by_8.size() == 0);

        REQUIRE(var_used_by_9.size() == 1);
        REQUIRE(Contains(var_used_by_9, "e9"));

        REQUIRE(var_used_by_10.size() == 0);
        REQUIRE(var_used_by_11.size() == 0);
      }
    }

    WHEN("Design extractor extracts statement entity types") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      std::unordered_set<std::string> stmt_entities = pkb.GetAllEntities();

      THEN("PKB gets populated with statement entity information") {
        REQUIRE(stmt_entities.size() == 5);
        REQUIRE(Contains(stmt_entities, "print"));
        REQUIRE(Contains(stmt_entities, "read"));
        REQUIRE(Contains(stmt_entities, "if"));
        REQUIRE(Contains(stmt_entities, "while"));
        REQUIRE(Contains(stmt_entities, "assign"));
      }
    }
  }
}
