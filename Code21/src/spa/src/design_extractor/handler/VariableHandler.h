#pragma once

#include <string>
#include <unordered_set>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class VariableHandler {
 public:
  static void ExtractVariableStmts(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor