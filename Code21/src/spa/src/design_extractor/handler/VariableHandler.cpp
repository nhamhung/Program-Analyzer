#include "VariableHandler.h"

#include <string>
#include <unordered_set>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

void VariableHandler::ExtractVariableStmts(PKB& pkb, const source_processor::TNode& root) {
  if (!root.IsType(source_processor::TNodeType::Variable)) {
    return;
  }
  pkb.InsertVariable(root.GetValue());
}

}  // namespace design_extractor