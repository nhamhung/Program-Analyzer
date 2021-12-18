#pragma once

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class NextHandler {
 private:
 public:
  static void ExtractNextRelation(PKB& pkb, const source_processor::TNode& node);
  static void ExtractNextTRelation(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor