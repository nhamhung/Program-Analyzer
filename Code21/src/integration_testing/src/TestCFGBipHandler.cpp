#include <iostream>

#include "TestUtils.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/handler/StatementHandler.h"
#include "design_extractor/utils/CFGBipHandler.h"
#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "source_processor/ast/TNode.h"

using namespace design_extractor;
using namespace source_processor;

SCENARIO("CFGBipHandler can create the proper CFGBip") {
  GIVEN("A valid source program with a single statement") {
    const auto program =
        "procedure main {\
          read x;\
        }";

    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {}};

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    PKB pkb = PKB();
    design_extractor::DesignExtractor::ExtractDesigns(pkb, ast);
    const auto graph = CFGBipHandler::GetCFGBip();

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A program with a linear call hierarchy") {
    const auto program =
        "\
      procedure A {\
        call B; \
      }\
      procedure B{\
        call C;\
      }\
      procedure C{\
        call D;\
      }\
      procedure D{\
        print dummy;\
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
    PKB pkb = PKB();
    design_extractor::DesignExtractor::ExtractDesigns(pkb, ast);
    const auto graph = CFGBipHandler::GetCFGBip();

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A program where a function is called from, and returns to, multiple locations") {
    /* NUMBERED PROGRAM STRING
    procedure A {
        call proc;  //1
        print a;    //2
    }
    procedure B {
        call proc;  //3
        print a;    //4
    }
    procedure C {
        call proc;  //5
        print a;    //6
    }
    procedure proc {
        print a;    //7
    }
    */
    const auto program =
        "\
      procedure A{            \
          call proc;          \
          print a;            \
      }                       \
      procedure B{            \
          call proc;          \
          print a;            \
      }                       \
      procedure C{            \
          call proc;          \
          print a;            \
      }                       \
      procedure proc{         \
          print a;            \
      }";

    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {7},
        /* 2 */ {},
        /* 3 */ {7},
        /* 4 */ {},
        /* 5 */ {7},
        /* 6 */ {},
        /* 7 */ {2, 4, 6}};

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    PKB pkb = PKB();
    design_extractor::DesignExtractor::ExtractDesigns(pkb, ast);
    const auto graph = CFGBipHandler::GetCFGBip();

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }

  GIVEN("A program which calls functions with multiple exits or are a single loop, throught an single-call-stmt intermediate function") {
    /* NUMBERED PROGRAM STRING
    procedure main {
      call middleman;         #1      
      print a;                #2
      call middlewoman;        #3
      print a;                #4
    }                         
    procedure middleman {     
      call singlewhile;       #5
    }                         
    procedure singlewhile {   
      while (5 == 5) {        #6
        print a;              #7
        print a;              #8
      }                       
    }                         
    procedure middlewoman {   
      call multipexit;        #9
    }                         
    procedure multipexit {    
      if (5 == 5) then {      #10
        if (5 == 5) then {    #11
          print a;            #12
        } else {              
          print a;            #13
        }                     
      } else {                
        if (5 == 5) then {    #14
          print a;            #15
        } else {              
          print a;            #16
        }                     
      }                       
    }        
    */
    const auto program =
        "\
    procedure main {        \
      call middleman;       \
      print a;              \
      call middlewoman;      \
      print a;              \
    }                       \
    procedure middleman {   \
      call singlewhile;     \
    }                       \
    procedure singlewhile { \
      while (5 == 5) {      \
        print a;            \
        print a;            \
      }                     \
    }                       \
    procedure middlewoman { \
      call multipexit;      \
    }                       \
    procedure multipexit {  \
      if (5 == 5) then {    \
        if (5 == 5) then {  \
          print a;          \
        } else {            \
          print a;          \
        }                   \
      } else {              \
        if (5 == 5) then {  \
          print a;          \
        } else {            \
          print a;          \
        }                   \
      }                     \
   }";
    CFG correct_graph = {
        /* 0 */ {},
        /* 1 */ {5},
        /* 2 */ {3},
        /* 3 */ {9},
        /* 4 */ {},
        /* 5 */ {6},
        /* 6 */ {7, 2},
        /* 7 */ {8},
        /* 8 */ {6},
        /* 9 */ {10},
        /* 10 */ {11, 14},
        /* 11 */ {12, 13},
        /* 12 */ {4},
        /* 13 */ {4},
        /* 14 */ {15, 16},
        /* 15 */ {4},
        /* 16 */ {4}};

    StatementHandler::ResetMaxStmtNum();
    const auto ast = Parser::Parse(program);
    PKB pkb = PKB();
    design_extractor::DesignExtractor::ExtractDesigns(pkb, ast);
    const auto graph = CFGBipHandler::GetCFGBip();

    REQUIRE(IsSimilarCFG(graph, correct_graph));
  }
}