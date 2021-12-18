#include <iostream>

#include "TestUtils.h"
#include "catch.hpp"
#include "design_extractor/handler/StatementHandler.h"
#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "source_processor/ast/TNode.h"

using namespace design_extractor;
using namespace source_processor;

SCENARIO("CFGHandler can create the proper CFG") {
  GIVEN("A valid source program with a single statement") {
    const auto program =
        "procedure main {\
          read x;\
        }";

    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with multiple non-container statements") {
    const auto program =
        "procedure main {\
          read x;\
          print x;\
          x = 1+2;\
          main = 90;\
        }";

    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2},
        /* 2 */ {3},
        /* 3 */ {4},
        /* 4 */ {},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with multiple non-container statements in multiple procedures") {
    const auto program =
        "procedure main {\
          read x;\
          print x;\
          x = 1+2;\
          main = 90;\
        }\
        procedure foo {\
          read x;\
          print x;\
          x = 1+2;\
          main = 90;\
        }\
        procedure bar {\
          read x;\
        }";

    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2},
        /* 2 */ {3},
        /* 3 */ {4},
        /* 4 */ {},
        /* 5 */ {6},
        /* 6 */ {7},
        /* 7 */ {8},
        /* 8 */ {},
        /* 9 */ {},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with single nesting level in container statements") {
    // taken from:
    // https://github.com/nus-cs3203/project-wiki/wiki/Full-(Basic-and-Advanced)-SPA-requirements#32-control-flow-graph
    const auto program =
        "procedure Second { \
          x = 0; \
          i = 5; \
          while (i!=0) { \
            x = x + 2*y; \
            print Third; \
            i = i - 1; \
          } \
          if (x==1) then { \
            x = x+1; \
          } else { \
            z = 1; \
          } \
          z = z + x + i; \
          y = z + 2; \
          x = x * y + z; \
        }";
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2},
        /* 2 */ {3},
        /* 3 */ {4, 7},
        /* 4 */ {5},
        /* 5 */ {6},
        /* 6 */ {3},
        /* 7 */ {8, 9},
        /* 8 */ {10},
        /* 9 */ {10},
        /* 10 */ {11},
        /* 11 */ {12},
        /* 12 */ {},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with multiple nested while with single statement") {
    const auto program =
        "procedure main { \
          while (1 == 1) {\
            while (1 == 1) {\
              while (1 == 1) {\
                x = 123;\
              }\
            }\
          }\
        }";
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2},
        /* 2 */ {1, 3},
        /* 3 */ {2, 4},
        /* 4 */ {3},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with multiple nested if with single statement") {
    const auto program =
        "procedure main { \
          if (1 == 1) then {\
            if (1 == 1) then {\
              if (1 == 1) then {\
                read x;\
              } else {\
                read x;\
              }\
            } else {\
              read x;\
            }\
          } else {\
            read x;\
          }\
        }";
    /*
      procedure main {
        if (1 == 1) then { // 1
          if (1 == 1) then { // 2
            if (1 == 1) then { // 3
              read x; // 4
            } else {
              read x; // 5
            }
          } else {
            read x; // 6
          }
        } else {
          read x; // 7
        }
      }
    */
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2, 7},
        /* 2 */ {3, 6},
        /* 3 */ {4, 5},
        /* 4 */ {},
        /* 5 */ {},
        /* 6 */ {},
        /* 7 */ {},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with multiple nested and complex container statements") {
    const auto program =
        "procedure main { \
          if (1 == 1) then {\
            read x;\
            while (1 == 1) {\
              read x;\
            }\
          } else {\
            if (1 == 1) then {\
              x = 123;\
              while (1 == 1) {\
                x = 123;\
              }\
              print y;\
            } else {\
              print x;\
              read y;\
            }\
            while (1 == 1) {\
              read x;\
            }\
          }\
          x = 123;\
          read x;\
        }";
    /*
      procedure main { 
        if (1 == 1) then { // 1
          read x; // 2
          while (1 == 1) { // 3
            read x; // 4
          }
        } else {
          if (1 == 1) then { // 5
            x = 123; // 6
            while (1 == 1) { // 7
              x = 123; // 8
            }
            print y; // 9
          } else {
            print x; // 10
            read y; // 11
          }
          while (1 == 1) { // 12
            read x; // 13
          }
        }
        x = 123; // 14
        read x; // 15
      }
    */
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2, 5},
        /* 2 */ {3},
        /* 3 */ {4, 14},
        /* 4 */ {3},
        /* 5 */ {6, 10},
        /* 6 */ {7},
        /* 7 */ {8, 9},
        /* 8 */ {7},
        /* 9 */ {12},
        /* 10 */ {11},
        /* 11 */ {12},
        /* 12 */ {13, 14},
        /* 13 */ {12},
        /* 14 */ {15},
        /* 15 */ {},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with an if statement nested in a while statment") {
    const auto program =
        "procedure main { \
          while (1 == 1) { \
            if (1 == 1) then { \
              read x; \
            } else { \
              while (1 == 1) { \
                read y; \
              } \
            } \
          } \
        }";
    /*
      procedure main {
        while (1 == 1) {     // 1
          if (1 == 1) then { // 2
            read x;          // 3
          } else {
            while (1 == 1) { // 4
              read y;        // 5
            }
          }
        }
      }
    */
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2},
        /* 2 */ {3, 4},
        /* 3 */ {1},
        /* 4 */ {1, 5},
        /* 5 */ {4},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with multiple nested if statements in a while statment (each container statement with only 1 statement)") {
    const auto program =
        "procedure main { \
          while (1 == 1) { \
            if (1 == 1) then { \
              if (1 == 1) then { \
                if (1 == 1) then { \
                  read a; \
                } else { \
                  read b; \
                } \
              } else { \
                read c;\
              } \
            } else { \
              if (1 == 1) then { \
                read d; \
              } else { \
                if (1 == 1) then { \
                  read e; \
                } else { \
                  read f; \
                } \
              } \
            } \
          } \
        }";
    /*
      procedure main {
        while (1 == 1) { // 1
          if (1 == 1) then { // 2
            if (1 == 1) then { // 3
              if (1 == 1) then { // 4
                read a; // 5
              } else {
                read b; // 6
              }
            } else {
              read c; // 7
            }
          } else {
            if (1 == 1) then { // 8
              read d; // 9
            } else {
              if (1 == 1) then { // 10
                read e; // 11
              } else {
                read f; // 12
              }
            }
          }
        }
      }
    */
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2},
        /* 2 */ {3, 8},
        /* 3 */ {4, 7},
        /* 4 */ {5, 6},
        /* 5 */ {1},
        /* 6 */ {1},
        /* 7 */ {1},
        /* 8 */ {9, 10},
        /* 9 */ {1},
        /* 10 */ {11, 12},
        /* 11 */ {1},
        /* 12 */ {1},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with multiple nested if statements in a while statment (each container statement with any amount of statements)") {
    const auto program =
        "procedure main { \
          while (1 == 1) { \
            a = 1; \
            if (1 == 1) then { \
              if (1 == 1) then { \
                a = 2; \
                if (1 == 1) then { \
                  a = 3; \
                  read a; \
                } else { \
                  read b; \
                } \
              } else { \
                while (1 == 1) { \
                  read c; \
                  a = 4; \
                } \
              } \
            } else { \
              while (1 == 1) { \
                if (1 == 1) then { \
                  read d; \
                } else { \
                  read e; \
                } \
              } \
            } \
          } \
          last = 420; \
        }";
    /*
      procedure main {
        while (1 == 1) { \\ 1
          a = 1; \\ 2
          if (1 == 1) then { \\ 3
            if (1 == 1) then { \\ 4
              a = 2; \\ 5
              if (1 == 1) then { \\ 6
                a = 3; \\ 7
                read a; \\ 8
              } else {
                read b; \\ 9
              }
            } else {
              while (1 == 1) { \\ 10
                read c; \\ 11
                a = 4; \\ 12
              }
            }
          } else {
            while (1 == 1) { \\ 13
              if (1 == 1) then { \\ 14
                read d; \\ 15
              } else {
                read e; \\ 16
              }
            }
          }
        }
        last = 420; \\ 17
      }
    */
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2, 17},
        /* 2 */ {3},
        /* 3 */ {4, 13},
        /* 4 */ {5, 10},
        /* 5 */ {6},
        /* 6 */ {7, 9},
        /* 7 */ {8},
        /* 8 */ {1},
        /* 9 */ {1},
        /* 10 */ {1, 11},
        /* 11 */ {12},
        /* 12 */ {10},
        /* 13 */ {1, 14},
        /* 14 */ {15, 16},
        /* 15 */ {13},
        /* 16 */ {13},
        /* 17 */ {},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A valid source program with an if statement nested in a while statment") {
    const auto program =
        "procedure main { \
          if (1 == 1) then {\
            if (1 == 1) then { \
              if (1 == 1) then { \
                read x; \
              } else { \
                read x; \
              } \
            } else { \
              if (1 == 1) then { \
                read x; \
              } else { \
                read x; \
              } \
            } \
            read x; \
          } else { \
            if (1 == 1) then { \
              read x; \
            } else { \
              read x; \
            } \
          } \
          read x; \
        } \
        ";
    /*
      procedure main {
        if (1 == 1) then { // 1
          if (1 == 1) then { // 2
            if (1 == 1) then { // 3
              read x; // 4
            } else {
              read x; // 5
            }
          } else {
            if (1 == 1) then { // 6
              read x; // 7
            } else {
              read x; // 8
            }
          }
          read x; // 9
        } else {
          if (1 == 1) then { // 10
            read x; // 11
          } else {
            read x; // 12
          }
        }
        read x; // 13
      }
    */
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {2, 10},
        /* 2 */ {3, 6},
        /* 3 */ {4, 5},
        /* 4 */ {9},
        /* 5 */ {9},
        /* 6 */ {7, 8},
        /* 7 */ {9},
        /* 8 */ {9},
        /* 9 */ {13},
        /* 10 */ {11, 12},
        /* 11 */ {13},
        /* 12 */ {13},
        /* 13 */ {},
    };

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    const auto graph = CFGHandler::ConstructCFG(ast);

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }
}