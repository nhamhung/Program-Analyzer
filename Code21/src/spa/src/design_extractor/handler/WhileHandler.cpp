#include "WhileHandler.h"

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

namespace design_extractor {

// BFS to get all values of variables only
std::vector<std::string> WhileHandler::GetVariableValues(const source_processor::TNode& node) {
  std::vector<std::string> vars_used;
  std::list<source_processor::TNode> queue;
  queue.push_back(node);

  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop_front();

    if (node.IsType(source_processor::TNodeType::Variable)) {
      vars_used.push_back(node.GetValue());
    }

    for (auto child : node.GetChildren()) {
      queue.push_back(*child);
    }
  }
  return vars_used;
}

void WhileHandler::ExtractWhileStmts(PKB& pkb, const source_processor::TNode& node) {
  if (!node.IsType(source_processor::TNodeType::While)) {
    return;
  }

  std::vector<std::string> vars_used = GetVariableValues(node.GetRootConditionalExpressionTNode());
  pkb.InsertWhile(node.GetStatementNumber(), vars_used);
}
}  // namespace design_extractor