#include "ConstantHandler.h"

#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

void ConstantHandler::ExtractConstants(PKB& pkb, const source_processor::TNode& root) {
  if (!root.IsType(source_processor::TNodeType::Constant)) {
    return;
  }
  pkb.InsertConstant(stoi(root.GetValue()));
}

}  // namespace design_extractor