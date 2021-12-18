#include "AssignmentHandler.h"

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

void AssignmentHandler::ExtractAssignStmts(PKB& pkb, const source_processor::TNode& node) {
  if (!node.IsType(source_processor::TNodeType::Assign)) {
    return;
  }

  // std::cout << node.GetStatementNumber() << " " << node.GetAssigneeTNode().GetValue() << " " << node.GetRpnTokens().GetSize() << std::endl;

  pkb.InsertAssignment(node.GetStatementNumber(),
                       node.GetAssigneeTNode().GetValue(),
                       node.GetRpnTokens());
}

}  // namespace design_extractor