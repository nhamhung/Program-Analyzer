#pragma once

#include <string>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class PrintHandler {
 public:
  static void ExtractPrintStmts(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor