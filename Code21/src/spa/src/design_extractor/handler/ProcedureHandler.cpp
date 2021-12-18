#include "ProcedureHandler.h"

#include <algorithm>
#include <climits>
#include <stack>
#include <string>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

//helper functions
std::pair<int, int> ProcedureHandler::GetStmtNumRange(const source_processor::TNode& root) {
  //traverse AST in DFS order
  std::stack<source_processor::TNode> stack;
  stack.push(root);

  int min_stmt_no = INT_MAX;
  int max_stmt_no = INT_MIN;
  while (!stack.empty()) {
    source_processor::TNode cur = stack.top();
    stack.pop();

    int stmt_no = cur.GetStatementNumber();
    if (stmt_no != -1) {
      min_stmt_no = std::min(min_stmt_no, stmt_no);
      max_stmt_no = std::max(max_stmt_no, stmt_no);
    }

    //add children to stack
    for (auto c : cur.GetChildren()) {
      stack.push(*c);
    }
  }

  return {min_stmt_no, max_stmt_no};
}

void ProcedureHandler::ExtractProcedureStmts(PKB& pkb, const source_processor::TNode& root) {
  if (!root.IsType(source_processor::TNodeType::Procedure)) {
    return;
  }
  std::pair<int, int> stmt_no_range = GetStmtNumRange(root);
  pkb.InsertProcedure(root.GetValue(), stmt_no_range.first, stmt_no_range.second);
}

}  // namespace design_extractor