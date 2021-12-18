#pragma once

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class AssignmentHandler {
 public:
  static void ExtractAssignStmts(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor