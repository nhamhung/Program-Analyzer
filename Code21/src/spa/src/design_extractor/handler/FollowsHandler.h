#pragma once

#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class FollowsHandler {
 public:
  static void ExtractFollowsStmts(PKB& pkb, const source_processor::TNode& node);
  static void ExtractFollowsTStmts(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor