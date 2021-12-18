#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>

#include "TestUtils.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/graph_explosion/GEHandler.h"
#include "design_extractor/graph_explosion/GENode.h"
#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "utils/Extension.h"

using namespace design_extractor;
using namespace source_processor;

bool IsSimilarGraph(std::multiset<std::string> expected_graph_set, GENode* generated_graph) {
  // get the set of strings from generated_graph
  std::list<const GENode*> queue;
  std::unordered_set<const GENode*> visited_set;
  queue.push_back(generated_graph);
  visited_set.insert(generated_graph);
  std::multiset<std::string> generated_graph_set;

  while (!queue.empty()) {
    auto current_node = queue.front();
    queue.pop_front();
    std::stringstream stream;

    stream << current_node->GetStatementNumber() << ":";

    // add children to queue
    for (const auto child : current_node->GetChildren()) {
      stream << " " << child->GetStatementNumber();
      if (visited_set.find(child) == visited_set.end()) {
        visited_set.insert(child);
        queue.push_back(child);
      }
    }

    generated_graph_set.insert(stream.str());
  }

  // check if expected_graph_set has same elements as generated_graph_set
  if (expected_graph_set.size() != generated_graph_set.size()) {
    return false;
  }

  for (auto expected_str : expected_graph_set) {
    auto itr = generated_graph_set.find(expected_str);
    if (itr == generated_graph_set.end()) {
      return false;
    }
    generated_graph_set.erase(itr);
  }

  return true;
}

SCENARIO("GEHandler can create the proper graph explosions") {
  utils::Extension::HasAffectsBip = true;

  GIVEN("A valid source program with a single procedure") {
    const auto program =
        "procedure main {\
          read x;\
          read x;\
          read x;\
          read x;\
        }";

    PKB pkb = PKB();
    const auto ast = Parser::Parse(program);
    DesignExtractor::ExtractDesigns(pkb, ast);

    auto generated_graph = GEHandler::GetGraphExplosion()[0];
    std::multiset<std::string> expected_graph_set = {
        "1: 2",
        "2: 3",
        "3: 4",
        "4:",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set, generated_graph));
  }

  GIVEN("A valid source program with single exploded graph TC1") {
    const auto program =
        "procedure main {\
          call B;\
          call B;\
          call B;\
        }\
        procedure B {\
          read x;\
          read x;\
          read x;\
          read x;\
        }\
        ";

    PKB pkb = PKB();
    const auto ast = Parser::Parse(program);
    DesignExtractor::ExtractDesigns(pkb, ast);

    auto generated_graph = GEHandler::GetGraphExplosion()[0];
    std::multiset<std::string> expected_graph_set = {
        "1: 4",
        "4: 5",
        "5: 6",
        "6: 7",
        "7: 2",
        "2: 4",
        "4: 5",
        "5: 6",
        "6: 7",
        "7: 3",
        "3: 4",
        "4: 5",
        "5: 6",
        "6: 7",
        "7:",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set, generated_graph));
  }

  GIVEN("A valid source program with single exploded graph TC2") {
    const auto program =
        "procedure A {\
          call B;\
          x = 2;\
        }\
        procedure B {\
          call C;\
          x = 4;\
        }\
        procedure C {\
          x = 5;\
        }\
        ";

    PKB pkb = PKB();
    const auto ast = Parser::Parse(program);
    DesignExtractor::ExtractDesigns(pkb, ast);

    auto generated_graph = GEHandler::GetGraphExplosion()[0];
    std::multiset<std::string> expected_graph_set = {
        "1: 3",
        "3: 5",
        "5: 4",
        "4: 2",
        "2:",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set, generated_graph));
  }

  GIVEN("A valid source program with single exploded graph TC3") {
    const auto program =
        "procedure gay {\
          call A;\
          x = 2;\
        }\
        procedure A {\
          call B;\
        }\
        procedure B {\
          call C;\
        }\
        procedure C {\
          if (1 == 1) then {\
            x = 6;\
          } else {\
            call D;\
          }\
        }\
        procedure D {\
          x = 8;\
        }\
        ";

    PKB pkb = PKB();
    const auto ast = Parser::Parse(program);
    DesignExtractor::ExtractDesigns(pkb, ast);

    auto generated_graph = GEHandler::GetGraphExplosion()[0];
    std::multiset<std::string> expected_graph_set = {
        "1: 3",
        "3: 4",
        "4: 5",
        "5: 6 7",
        "6: 2",
        "7: 8",
        "8: 2",
        "2:",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set, generated_graph));
  }

  GIVEN("A valid source program with single exploded graph TC4") {
    const auto program =
        "procedure gay {\
          call A;\
          x = 2;\
        }\
        procedure A {\
          if (1 == 1) then {\
            while (1 == 1) {\
              x = 5;\
              call D;\
            }\
          } else {\
            if (1 == 1) then {\
              call D;\
              x = 9;\
            } else {\
              call D;\
            }\
          }\
        }\
        procedure D {\
          x = 11;\
        }\
        ";

    PKB pkb = PKB();
    const auto ast = Parser::Parse(program);
    DesignExtractor::ExtractDesigns(pkb, ast);

    auto generated_graph = GEHandler::GetGraphExplosion()[0];
    std::multiset<std::string> expected_graph_set = {
        "1: 3",
        "3: 4 7",
        "4: 5 2",
        "7: 8 10",
        "5: 6",
        "2:",
        "8: 11",
        "10: 11",
        "6: 11",
        "11: 9",
        "11: 2",
        "11: 4",
        "9: 2",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set, generated_graph));
  }

  GIVEN("A valid source program with single exploded graph TC5") {
    const auto program =
        "procedure A {\
          if (x == 1) then {\
            if (x == 2) then {\
              call D;\
            } else {\
              call D;\
            }\
          } else {\
            if (x == 5) then {\
              call D;\
            } else {\
              call D;\
            }\
          }\
          call D; \
        }\
        procedure D {\
          if (x == 9) then {\
            x = 10; \
          } else {\
            x = 11; \
          }\
        }\
        ";

    PKB pkb = PKB();
    const auto ast = Parser::Parse(program);
    DesignExtractor::ExtractDesigns(pkb, ast);

    auto generated_graph = GEHandler::GetGraphExplosion()[0];
    std::multiset<std::string> expected_graph_set = {
        "1: 2 5",
        "2: 3 4",
        "5: 6 7",
        "3: 9",
        "4: 9",
        "6: 9",
        "7: 9",
        "9: 10 11",
        "9: 10 11",
        "9: 10 11",
        "9: 10 11",
        "10: 8",
        "11: 8",
        "10: 8",
        "11: 8",
        "10: 8",
        "11: 8",
        "10: 8",
        "11: 8",
        "8: 9",
        "9: 10 11",
        "10:",
        "11:",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set, generated_graph));
  }

  GIVEN("A valid source program with single exploded graph TC6") {
    const auto program =
        "procedure gay {\
          while (x == 1) {\
            if (x == 2) then {\
              call A;\
            } else {\
              call A;\
            }\
          }\
          x = 5;\
        }\
        procedure A {\
          if (x == 6) then {\
            while (x == 7) {\
              call D;\
            }\
          } else {\
            if (x == 9) then {\
              if (x == 10) then {\
                call D;\
              } else {\
                call D;\
              }\
            } else {\
              if (x == 13) then {\
                call D;\
              } else {\
                call D;\
              }\
            }\
          }\
          x = 16; \
        }\
        procedure D {\
          if (x == 17) then {\
            x = 18;\
          } else {\
            x = 19;\
          }\
        }\
        ";

    PKB pkb = PKB();
    const auto ast = Parser::Parse(program);
    DesignExtractor::ExtractDesigns(pkb, ast);

    auto generated_graph = GEHandler::GetGraphExplosion()[0];
    std::multiset<std::string> expected_graph_set = {
        // procedure gay
        "1: 5 2",
        "5:",
        "2: 3 4",
        // first call to A
        "3: 6",
        "6: 7 9",
        "7: 16 8",
        "8: 17",
        "17: 18 19",
        "18: 7",
        "19: 7",
        "9: 10 13",
        "10: 11 12",
        "13: 14 15",
        "11: 17",
        "12: 17",
        "14: 17",
        "15: 17",
        "17: 18 19",
        "17: 18 19",
        "17: 18 19",
        "17: 18 19",
        "18: 16",
        "19: 16",
        "18: 16",
        "19: 16",
        "18: 16",
        "19: 16",
        "18: 16",
        "19: 16",
        "16: 1",
        // second call to A
        "4: 6",
        "6: 7 9",
        "7: 16 8",
        "8: 17",
        "17: 18 19",
        "18: 7",
        "19: 7",
        "9: 10 13",
        "10: 11 12",
        "13: 14 15",
        "11: 17",
        "12: 17",
        "14: 17",
        "15: 17",
        "17: 18 19",
        "17: 18 19",
        "17: 18 19",
        "17: 18 19",
        "18: 16",
        "19: 16",
        "18: 16",
        "19: 16",
        "18: 16",
        "19: 16",
        "18: 16",
        "19: 16",
        "16: 1",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set, generated_graph));
  }

  GIVEN("A valid source program with multiple exploded graphs TC1") {
    const auto program =
        "procedure gay {\
          call A;\
          x = 2;\
        }\
        procedure A {\
          call B;\
        }\
        procedure B {\
          x = 4;\
        }\
        procedure C {\
          if (1 == 1) then {\
            x = 6;\
          } else {\
            call D;\
          }\
          x = 8;\
        }\
        procedure D {\
          x = 9;\
        }\
        ";

    PKB pkb = PKB();
    const auto ast = Parser::Parse(program);
    DesignExtractor::ExtractDesigns(pkb, ast);

    auto generated_graph_1 = GEHandler::GetGraphExplosion()[0];
    std::multiset<std::string> expected_graph_set_1 = {
        "1: 3",
        "3: 4",
        "4: 2",
        "2:",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set_1, generated_graph_1));

    auto generated_graph_2 = GEHandler::GetGraphExplosion()[1];
    std::multiset<std::string> expected_graph_set_2 = {
        "5: 6 7",
        "6: 8",
        "7: 9",
        "8:",
        "9: 8",
    };
    REQUIRE(IsSimilarGraph(expected_graph_set_2, generated_graph_2));
  }
}