#pragma once

#include <string>
#include <vector>

#include "CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class CFGBipHandler {
 private:
  static CFG cfgbip;
  //stores all exit stmt#s of each procedure
  static std::unordered_map<std::string, std::unordered_set<int>> exit_stmts;

  static void Operate(std::string proc, PKB& pkb);
  static int GetProcEntryStmt(std::string proc, PKB& pkb);

 public:
  // called whenever the CFGBip needs to be constructed from the CFG;
  // must be called after CallHandler, NextHandler, CFGHandler, EntityHandler is called
  static const CFG& ConstructCFGBip(PKB& pkb);
  // returns the cached CFGBip; throws error when CFGBip is not yet constructed
  static const CFG& GetCFGBip();
  static bool IsExitStmt(int sn, PKB& pkb);
};

}  // namespace design_extractor