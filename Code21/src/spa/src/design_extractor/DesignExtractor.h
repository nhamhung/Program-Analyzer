#pragma once

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

namespace design_extractor {

class DesignExtractor {
 private:
  static void BreadthFirstTraversal(PKB& pkb, const source_processor::TNode& root);

 public:
  // design extractor entry point
  static void ExtractDesigns(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor