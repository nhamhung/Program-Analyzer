#pragma once

#include <string>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class ProcedureHandler {
 private:
  static std::pair<int, int> GetStmtNumRange(const source_processor::TNode& root);

 public:
  static void ExtractProcedureStmts(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor