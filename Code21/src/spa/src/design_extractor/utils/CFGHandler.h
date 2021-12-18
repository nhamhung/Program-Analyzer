#pragma once

#include <vector>

#include "source_processor/ast/TNode.h"

namespace design_extractor {

typedef std::vector<std::vector<int>> CFG;

class CFGHandler {
 private:
  static CFG graph;
  static void ConnectLastStmtOfWhile(CFG&, const int, const source_processor::TNode&);
  static void ConnectLastStmtsOfIf(CFG&, const int, const source_processor::TNode&);

 public:
  // called whenever the CFG needs to be constructed from scratch;
  // must be called after StatementHandler::ExtractStmtNums is called
  static const CFG& ConstructCFG(const source_processor::TNode&);
  // returns the cached CFG; throws error when CFG is not yet constructed
  static const CFG& GetCFG();
};

}  // namespace design_extractor