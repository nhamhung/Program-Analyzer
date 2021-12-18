#include "NextHandler.h"

#include <iostream>
#include <queue>
#include <vector>

#include "design_extractor/utils/CFGHandler.h"
#include "design_extractor/utils/DeUtils.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

// every edge on CFG is a next relation.
// Next(a, b) is true iff there is an edge from a to b in the CFG
void NextHandler::ExtractNextRelation(PKB& pkb, const source_processor::TNode& node) {
  const CFG& cfg = CFGHandler::GetCFG();
  for (int from = 0; from < cfg.size(); ++from) {
    for (auto to : cfg[from]) {
      //std::cout << "next(" << from << ", " << to << ")\n";
      pkb.InsertNext(from, to);
    }
  }
}

//for a node A, Next*(A, B) is true for all B where
//B can be reached from A in the CFG
void NextHandler::ExtractNextTRelation(PKB& pkb, const source_processor::TNode& node) {
  const CFG& cfg = CFGHandler::GetCFG();
  for (int from = 0; from < cfg.size(); ++from) {
    auto reachable_nodes = DeUtils::GetReachableNodes(from, cfg);
    for (auto to : reachable_nodes) {
      //std::cout << "next*(" << from << ", " << to << ")\n";
      pkb.InsertNextT(from, to);
    }
  }
}

}  // namespace design_extractor