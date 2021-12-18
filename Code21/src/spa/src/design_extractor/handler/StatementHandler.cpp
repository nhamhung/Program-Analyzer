#include "StatementHandler.h"

#include <algorithm>
#include <climits>
#include <stack>
#include <unordered_set>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

int StatementHandler::max_stmt_num = INT_MIN;
std::unordered_set<int> StatementHandler::statement_entries;

void StatementHandler::ResetMaxStmtNum() {
  max_stmt_num = INT_MIN;
}

int StatementHandler::GetMaxStmtNum(const source_processor::TNode& root) {
  // return max_stmt_num if it is already computed
  if (max_stmt_num != INT_MIN) {
    return max_stmt_num;
  }

  // traverse AST in DFS order to get max_stmt_num
  std::stack<source_processor::TNode> stack;
  stack.push(root);

  max_stmt_num = INT_MIN;
  while (!stack.empty()) {
    source_processor::TNode cur = stack.top();
    stack.pop();

    int stmt_num = cur.GetStatementNumber();
    if (stmt_num != -1) {
      max_stmt_num = std::max(max_stmt_num, stmt_num);
    }

    //add children to stack
    for (auto c : cur.GetChildren()) {
      stack.push(*c);
    }
  }

  return max_stmt_num;
}

void StatementHandler::ExtractStmtNums(PKB& pkb, const source_processor::TNode& root) {
  // reset static var to INT_MIN before calling GetMaxStmtNum
  ResetMaxStmtNum();
  GetMaxStmtNum(root);
  for (int i = 1; i <= max_stmt_num; ++i) {
    StatementHandler::statement_entries.insert(i);
    pkb.InsertStatement(i);
  }
}

}  // namespace design_extractor