#include <iostream>

#include "TestUtils.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "source_processor/ast/TNode.h"
#include "utils/Extension.h"

SCENARIO("Test NextBip / NextBip*") {
  utils::Extension::HasNextBip = true;

  GIVEN("A program with a linear call hierarchy") {
    /* NUMBERED PROGRAM STRING
    procedure A {
      call B; //1
    }
    procedure B {
      call C; //2
    }
    procedure C {
      call D; //3
    }
    procedure D {
      print dummy;  //4
    }
    */
    const std::string test_program =
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

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with NextBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(1), {2}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(2), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(3), {4}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(4), {}));
      }

      THEN("PKB gets populated with NextBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(1), {2, 3, 4}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(2), {3, 4}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(3), {4}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(4), {}));
      }
    }
  }

  GIVEN("A program where a function is called from multiple locations") {
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
    const std::string test_program =
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

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with NextBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(1), {7}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(3), {7}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(5), {7}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(6), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(7), {2, 4, 6}));
      }

      THEN("PKB gets populated with NextBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(1), {7, 2}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(3), {7, 4}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(5), {7, 6}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(6), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(7), {2, 4, 6}));
      }
    }
  }

  GIVEN("A program where a called function contains a single while loop") {
    /* NUMBERED PROGRAM STRING
    proc A {
      call B;     //1
      print a;    //2
      print a;    //3
    }
    proc B {
      while (5 == 5) {  //4
          print a;      //5
      }
    }
    */
    const std::string test_program =
        "\
    procedure A {\
      call B;     \
      print a;    \
      print a;    \
    }\
    procedure B {\
      while (5 == 5) {  \
          print a;      \
      }\
    }";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with NextBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(1), {4}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(2), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(4), {5, 2}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(5), {4}));
      }

      THEN("PKB gets populated with NextBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(1), {2, 3, 4, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(2), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(4), {5, 4, 2, 3}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(5), {5, 4, 2, 3}));
      }
    }
  }

  GIVEN("A program from Wiki 'Extension b: General case of CFGBip and AffectsBip") {
    /* NUMBERED PROGRAM STRING
    procedure Bill {
    01      x = 5;
    02      call Mary;
    03      y = x + 6;
    04      call John;
    05      z = x * y + 2; }

    procedure Mary {
    06      y = x * 3;
    07      call John;
    08      z = x + y; }

    procedure John {
    09      if (i > 0) then {
    10              x = x + z; } }
            else {
    11              y = x * y; } }    
    */
    const std::string test_program =
        "\
    procedure Bill {      \
      x = 5;              \
      call Mary;          \
      y = x + 6;          \
      call John;          \
      z = x * y + 2;      \
    }                     \
                          \
    procedure Mary {       \
      y = x * 3;          \
      call John;          \
      z = x + y;          \
    }                     \
                          \
    procedure John {       \
      if (i > 0) then {   \
        x = x + z;        \
      } else {            \
        y = x * y;        \
      }                   \
    }";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with NextBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(1), {2}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(2), {6}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(3), {4}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(4), {9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(6), {7}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(7), {9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(8), {3}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(9), {10, 11}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(10), {5, 8}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(11), {5, 8}));
      }

      THEN("PKB gets populated with NextBipT relation properly") {
        //easier to see by looking at wiki prof iter 2 and 3 section
        //'1. Generalized CFGBip by graph explosion'
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(1), {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(2), {3, 4, 5, 6, 7, 8, 9, 10, 11}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(3), {4, 9, 10, 11, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(4), {9, 10, 11, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(6), {7, 9, 10, 11, 8, 3, 4, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(7), {9, 10, 11, 8, 3, 4, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(8), {3, 4, 9, 10, 11, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(9), {10, 11, 5, 8, 3, 4, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(10), {5, 8, 3, 4, 9, 10, 11}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(11), {5, 8, 3, 4, 9, 10, 11}));
      }
    }
  }
  GIVEN("A program from Wiki \'Extension Hints\'") {
    /* NUMBERED PROGRAM STRING
    procedure P {
    01      call Q;
    02      while (i > 0) {
    03              call Q1; }
    04      if (i > 0) then {
    05              call Q2; }
            else {
    06              call Q3; } }    

    procedure Q {
    07  print a;
    }
    procedure Q1 {
    08  print a;
    }
    procedure Q2 {
    09  print a;
    }
    procedure Q3 {
    10  print a;
    }
    */
    const std::string test_program =
        "\
    procedure P {         \
      call Q;             \
      while (i > 0) {     \
        call Q1;          \
      }                   \
      if (i > 0) then {   \
        call Q2; }        \
      else {              \
        call Q3; } }      \
                          \
    procedure Q {         \
      print a; }          \
                          \
    procedure Q1 {        \
      print a; }          \
                          \
    procedure Q2 {        \
      print a; }          \
                          \
    procedure Q3 {        \
      print a; }          \
    ";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with NextBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(1), {7}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(2), {3, 4}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(3), {8}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(4), {5, 6}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(5), {9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(6), {10}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(7), {2}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(8), {2}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipStatements(10), {}));
      }

      THEN("PKB gets populated with NextBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(1), {7, 2, 3, 8, 4, 5, 6, 9, 10}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(2), {3, 8, 2, 4, 5, 6, 9, 10}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(3), {8, 2, 3, 4, 5, 6, 9, 10}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(4), {5, 6, 9, 10}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(5), {9}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(6), {10}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(7), {2, 3, 8, 4, 5, 6, 9, 10}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(8), {2, 3, 8, 4, 5, 6, 9, 10}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetNextBipTStatements(10), {}));
      }
    }
  }
}

SCENARIO("Test AffectsBip/*") {
  utils::Extension::HasAffectsBip = true;

  GIVEN("Procedures with single call hierarchy") {
    /* NUMBERED PROGRAM STRING
    procedure Bill {
      x = 5; //1
      call Mary; //2
      y = x + 6; //3
      x = 5; //4
      z = x * y + 2; //5
    }
    procedure Mary {
      y = x * 3; //6
      call John; //7
      z = x + y; //8
    }
    procedure John {
     if (i > 0) then {  //9
      x = x + z; //10
     } else {
       y = x * y; //11
     }
    }
    */
    const std::string test_program =
        "\
      procedure Bill {\
        x = 5; \
        call Mary; \
        y = x + 6; \
        x = 5; \
        z = x * y + 2; \
      }\
      procedure Mary {\
        y = x * 3; \
        call John; \
        z = x + y; \
      }\
      procedure John {\
      if (i > 0) then {  \
        x = x + z; \
      } else {\
        y = x * y; \
      }\
    }";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(1), {6, 10, 11, 8, 3}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(3), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(4), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(6), {11, 8}));  // doesnt affect lines 3 - 5 as y is modified
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(7), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(10), {8, 3}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(11), {8}));
      }
    }

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(1), {6, 5, 10, 11, 8, 3}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(3), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(4), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(6), {11, 8}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(7), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(10), {8, 3, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(11), {8}));
      }
    }
  }

  GIVEN("Multiple calls to a procedure from different procedures") {
    /* NUMBERED PROGRAM STRING
    procedure Bill {
      x = 5; //1
      call Mary; //2
      y = x + 6; //3
      call John; //4
      z = x * y + 2; //5
    }
    procedure Mary {
      y = x * 3; //6
      call John; //7
      z = x + y; //8
    }
    procedure John {
     if (i > 0) then {  //9
      x = x + z; //10
     } else {
       y = x * y; //11
     }
    }

    EXPANDED:
    procedure Bill {
      x = 5; //1
      call Mary; //2
      y = x * 3; //6
      call John; //7
      if (i > 0) then {  //9
      x = x + z; //10
     } else {
       y = x * y; //11
     }
      z = x + y; //8
      y = x + 6; //3
      call John; //4
      if (i > 0) then {  //9
      x = x + z; //10
     } else {
       y = x * y; //11
     }
      z = x * y + 2; //5
    }
    */
    const std::string test_program =
        "\
      procedure Bill {\
      x = 5; \
      call Mary; \
      y = x + 6; \
      call John; \
      z = x * y + 2; \
      }\
      procedure Mary {\
        y = x * 3; \
        call John; \
        z = x + y; \
      }\
      procedure John {\
      if (i > 0) then {  \
        x = x + z; \
      } else {\
        y = x * y; \
      }\
    }";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(1), {6, 10, 11, 8, 3, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(3), {11, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(6), {11, 8}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(7), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(8), {10}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(10), {8, 5, 3, 10, 11}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(11), {8, 5}));
      }
    }

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(1), {6, 10, 11, 8, 3, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(3), {11, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(6), {11, 8, 5, 10}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(7), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(8), {10, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(10), {8, 5, 3, 10, 11}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(11), {8, 5, 10}));
      }
    }
  }

  GIVEN("Multiple calls to a procedure from the same procedure (simple)") {
    /* NUMBERED PROGRAM STRING
    procedure B {
      call C; //1
      call C; //2
      call C; //3
    }
    procedure C {
     d = a; //4
     a = b; //5
     b = c; //6
     c = d; //7
    }
    */
    const std::string test_program =
        "\
      procedure B {\
        call C;\
        call C;\
        call C;\
      }\
      procedure C {\
        d = a;\
        a = b;\
        b = c;\
        c = d;\
      }";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(4), {7}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(5), {4}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(6), {5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(7), {6}));
      }
    }

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(4), {7, 6, 5}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(5), {4, 7, 6}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(6), {5, 4, 7}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(7), {6, 5}));
      }
    }
  }

  GIVEN("Multiple calls to a procedure from the same procedure (more complex)") {
    /* NUMBERED PROGRAM STRING
    procedure B {
      call C; //1
      while (i > 0) { //2
        call C; //3
      }
      if (i > 0) then { //4
        call C; //5
      } else {
        call C; //6
      }
    }
    procedure C {
     d = a; //7
     a = b; //8
     b = c; //9
     c = d; //10
    }
    */
    const std::string test_program =
        "\
      procedure B {\
        call C; \
        while (i > 0) { \
          call C; \
        }\
        if (i > 0) then { \
          call C;\
        } else {\
          call C;\
        }\
      }\
      procedure C {\
        d = a;\
        a = b;\
        b = c;\
        c = d;\
      }";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(6), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(7), {10}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(8), {7}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(9), {8}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(10), {9}));
      }
    }
    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(6), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(7), {10, 9, 8, 7}));  // because of the while loop
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(8), {10, 9, 8, 7}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(9), {10, 9, 8, 7}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(10), {10, 9, 8, 7}));
      }
    }
  }

  GIVEN("Multiple calls to a procedure from the same procedure (nested ifs)") {
    /* NUMBERED PROGRAM STRING
    procedure A {
      if (x == 1) then { // 1
        if (x == 2) then { // 2
          call D; //3
          y = x + 10; //4
        } else { 
          call D; //5
          z = y - z; //6
        }
        x = y + z; //7
      } else {
        if (x == 5) then { //8
          call D; //9
          read x; //10
          x = y + z; //11
        } else {
          call D; //12
          read y; //13
        }
      }
      call D; //14
    }
    procedure D {
      if (x == 9) then {//15
        if (v > 1) then {//16
          y = 1 + z;//17
        } else {
          read y;//18
        }
        x = z / 2; //19
      } else { 
        if (a > 1) then {//20
          print y;//21
        } else {
          read z;//22
        }
        y = 2 + y - a; //23
        z = 11 - x; //24
      }
    }
    */
    const std::string test_program =
        "procedure A {\
          if (x == 1) then {\
            if (x == 2) then {\
              call D;\
              y = x + 10;\
            } else {\
              call D;\
              z = y + z;\
            }\
            x = y - z;\
          } else {\
            if (x == 5) then {\
              call D;\
              read x;\
              x = y + z;\
            } else {\
              call D;\
              read y;\
            }\
          }\
          call D; \
        }\
        procedure D {\
          if (x == 9) then {\
            if (v > 1) then {\
              y = 1 + z;\
            } else {\
              read y;\
            }\
            x = z / 2; \
          } else { \
            if (a > 1) then {\
              print y;\
            } else {\
              read z;\
            }\
            y = 2 + y - a;\
            z = 11 - x;\
          }\
        }";
    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      THEN("PKB gets populated with AffectsBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(4), {7, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(6), {7, 17, 19}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(7), {24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(10), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(11), {24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(12), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(13), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(14), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(15), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(16), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(17), {6, 7, 11, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(18), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(19), {4, 24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(20), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(21), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(22), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(23), {6, 7, 11, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(24), {6, 7, 11, 17, 19}));
      }
    }
    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);
      THEN("PKB gets populated with AffectsBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(3), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(4), {7, 23, 24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(5), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(6), {7, 17, 19, 24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(7), {24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(9), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(10), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(11), {24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(12), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(13), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(14), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(15), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(16), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(17), {6, 19, 17, 7, 24, 11, 23}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(18), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(19), {4, 7, 23, 24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(20), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(21), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(22), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(23), {6, 7, 17, 19, 11, 23, 24}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(24), {6, 7, 11, 17, 19, 24}));
      }
    }
  }

  GIVEN("Multiple calls to a procedure from same procedure (nested whiles)") {
    /* NUMBERED PROGRAM STRING
    procedure B { 
      call C; //1 
      while (i > 0) {   //2
        a = 3;  //3
        call C; //4
        b = d * v;//5
        while (b > 1) {//6
          read c;//7
          call C;//8
          c = c + 1;//9
          while (x == 2) {//10
            call C;//11
          }
        }
      }
      a = b + c + d;//12
    }
    procedure C {
      d = a;//13
      a = b;//14
      b = c;//15
      c = d;//16
    }

    EXPANDED
    procedure B { 
      call C; //1 
      while (i > 0) {   //2
        a = 3;  //3
        call C; //4
        d = a;//13
        a = b;//14
        b = c;//15
        c = d;//16
        
        b = d * v;//5
        while (b > 1) {//6
          read c;//7
          call C;//8
          
          d = a;//13
          a = b;//14
          b = c;//15
          c = d;//16
          
          c = c + 1;//9
          while (x == 2) {//10
            call C;//11
            d = a;//13
            a = b;//14
            b = c;//15
            c = d;//16
          }
        }
      }
      a = b + c + d;//12
    }
    */
    const std::string test_program =
        "\
      procedure B {\
        call C;\
        while (i > 0) { \
          a = 3;\
          call C; \
          b = d * v;\
          while (b > 1) {\
            read c;\
            call C;\
            c = c + 1;\
            while (x == 2) {\
              call C;\
            }\
          }\
        }\
        a = b + c + d;\
      }\
      procedure C {\
        d = a;\
        a = b;\
        b = c;\
        c = d;\
      }";

    source_processor::TNode root = source_processor::Parser::Parse(test_program);

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBip relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(3), {13}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(5), {14, 12}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(6), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(7), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(9), {12, 15}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(10), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(11), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(12), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(13), {16, 5, 12}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(14), {13}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(15), {12, 14}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipStatements(16), {9, 12, 15}));
      }
    }

    WHEN("Design extractor extracts all designs") {
      PKB pkb = PKB();
      pkb.ClearAllTables();
      design_extractor::DesignExtractor::ExtractDesigns(pkb, root);

      THEN("PKB gets populated with AffectsBipT relation properly") {
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(1), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(2), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(3), {13, 5, 14, 15, 16, 12, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(4), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(5), {14, 12, 13, 15, 16, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(6), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(7), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(8), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(9), {9, 12, 13, 14, 15, 16}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(10), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(11), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(12), {}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(13), {16, 5, 12, 13, 14, 15, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(14), {13, 14, 15, 16, 12, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(15), {12, 14, 13, 15, 16, 9}));
        REQUIRE(ContainsExactly<int>(pkb.GetAffectedBipTStatements(16), {9, 12, 15, 13, 14, 16}));
      }
    }
  }
}