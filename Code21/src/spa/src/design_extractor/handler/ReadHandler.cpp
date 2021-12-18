#include "ReadHandler.h"

#include <string>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

void ReadHandler::ExtractReadStmts(PKB& pkb, const source_processor::TNode& root) {
  if (!root.IsType(source_processor::TNodeType::Read)) {
    return;
  }
  pkb.InsertRead(root.GetStatementNumber(), root.GetVariableTNode().GetValue());
}

}  // namespace design_extractor