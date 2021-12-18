#pragma once

#include <string>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class ReadHandler {
 public:
  static void ExtractReadStmts(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor