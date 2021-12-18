#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

typedef std::unordered_map<std::string, std::unordered_set<std::string>> graph;

namespace design_extractor {

class ModifiesHandler {
 private:
  //stmt no modifies the corresponding set of variables
  static std::unordered_map<int, std::unordered_set<std::string>> vars_modified_by;

  static void ExtractModifiesSWithoutCallsStmtsHelper(const source_processor::TNode& node);
  static void PopulateModifiesSWithoutCallsStmts(PKB& pkb);
  static void PopulateModifiesSForCallsAndModifiesP(PKB& pkb, graph call_graph, std::string proc);

 public:
  static void ExtractModifiesSWithoutCallsStmts(PKB& pkb, const source_processor::TNode& node);
  static void ExtractModifiesSForCallsAndModifiesP(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor