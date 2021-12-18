#pragma once

#include <unordered_set>

#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

typedef std::unordered_map<int, std::unordered_set<int>> AffectsGraph;

class AffectsHandler {
 private:
  static AffectsGraph graph;
  static std::unordered_set<int> assign_stmts;
  static std::unordered_set<int> GetReachableNodes(int src);
  static void TraverseCFGFromSource(int src, std::string& modified_var, const CFG& cfg, PKB& pkb);

 public:
  static void ExtractAffects(PKB& pkb, const source_processor::TNode& root);
  static void ExtractAffectsT(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor