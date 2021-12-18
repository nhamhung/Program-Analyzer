#pragma once

#include <map>
#include <unordered_set>

#include "design_extractor/graph_explosion/GENode.h"
#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

typedef std::map<GENode*, std::unordered_set<GENode*>> AffectsBipGraph;

class AffectsBipHandler {
 private:
  static AffectsBipGraph affects_bip_graph;
  static void ExtractAffectsBipForGraph(GENode* root_node, PKB& pkb);
  static void TraverseExplodedGraphFromSrc(GENode* src, PKB& pkb);
  static std::unordered_set<GENode*> GetReachableNodes(GENode* src, PKB& pkb);

 public:
  static void ExtractAffectsBip(PKB& pkb, const source_processor::TNode& root);
  static void ExtractAffectsBipT(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor