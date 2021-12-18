#pragma once

#include <map>
#include <string>
#include <vector>

#include "GENode.h"
#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class GEHandler {
 private:
  static std::vector<GENode*> graphs;

  // Returns the GEnode in the map if it exists. Else, creates one
  // and inserts it into the map and returns it. Similar to C++17 emplace.
  static GENode* GetOrInsert(std::map<int, GENode*>&, int);
  // Returns the names of the procedures which are the root of some
  // execution path (ie. they are not called by any other procedures).
  static const std::vector<std::string> GetStartingProcedureNames(PKB&);
  static std::map<int, GENode*> ExplodeGraph(
      PKB&, const CFG&, const source_processor::TNode&, const std::string&, GENode* = nullptr);

  // Connects the last executed statement(s) in a stmt list to the arg current_exit_node.
  // Using the procedure's statement list in the AST:
  // - If last statement is Call: terminate and do not connect
  // - If last statement is If: recursively call function with Then and Else stmt lists
  // - If last statement is anything else: connect it to the argument current_exit_node
  static void ConnectLastExecutedStmts(const source_processor::TNode&,
                                       std::map<int, GENode*>&, GENode*);

 public:
  // Constructs the CFG explosion and caches it in the static graphs var.
  // Should only be called once by DesignExtractor.
  static const std::vector<GENode*> ConstructGraphExplosion(PKB&, const source_processor::TNode&);
  // Should be called to get the created graph.
  static const std::vector<GENode*> GetGraphExplosion();
};

}  // namespace design_extractor