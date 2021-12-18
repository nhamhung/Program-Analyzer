#pragma once

#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class ConstantHandler {
 public:
  static void ExtractConstants(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor