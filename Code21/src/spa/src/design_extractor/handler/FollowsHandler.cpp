#include "FollowsHandler.h"

#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

void FollowsHandler::ExtractFollowsStmts(PKB& pkb, const source_processor::TNode& node) {
  if (!node.IsType(source_processor::TNodeType::StatementList)) {
    return;
  }

  const auto children = node.GetChildren();
  int length = children.size();
  if (length <= 1) {
    return;
  }

  for (int i = 0; i < length - 1; i++) {
    int first = children[i]->GetStatementNumber();
    int second = children[i + 1]->GetStatementNumber();
    pkb.InsertFollows(first, second);
  }
}

void FollowsHandler::ExtractFollowsTStmts(PKB& pkb, const source_processor::TNode& node) {
  if (!node.IsType(source_processor::TNodeType::StatementList)) {
    return;
  }

  const auto children = node.GetChildren();
  int length = children.size();
  if (length <= 1) {
    return;
  }

  for (int i = 0; i < length - 1; i++) {
    for (int j = i + 1; j < length; j++) {
      int first = children[i]->GetStatementNumber();
      int second = children[j]->GetStatementNumber();
      pkb.InsertFollowsT(first, second);
    }
  }
}

}  // namespace design_extractor