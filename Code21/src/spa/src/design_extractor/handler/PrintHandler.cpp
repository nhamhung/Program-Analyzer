#include "PrintHandler.h"

#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

void PrintHandler::ExtractPrintStmts(PKB& pkb, const source_processor::TNode& root) {
  if (!root.IsType(source_processor::TNodeType::Print)) {
    return;
  }
  pkb.InsertPrint(root.GetStatementNumber(), root.GetVariableTNode().GetValue());
}

}