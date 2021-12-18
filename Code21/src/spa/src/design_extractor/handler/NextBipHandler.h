#pragma once

#include <unordered_map>

#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

//NextBipHandler must be called after Next, CFGHandler, EntityHandler
class NextBipHandler {
 private:
  static CFG cfgbip;

  static void PopulateNextBipRelation(PKB& pkb);
  static void PopulateNextBipTRelation(PKB& pkb);
  static void PopulateNextBipTRelationFromStmt(int from, PKB& pkb, int num_hops);
  static std::string GetProcStartingWith(int sn, PKB& pkb);

 public:
  //call after CFGHandler and NextHandler
  static void ExtractNextBipAndNextBipTRelation(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor