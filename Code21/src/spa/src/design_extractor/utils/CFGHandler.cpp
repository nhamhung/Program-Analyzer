#include "CFGHandler.h"

#include <list>
#include <stdexcept>
#include <vector>

#include "design_extractor/handler/StatementHandler.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

namespace design_extractor {

CFG CFGHandler::graph;

/*
Algorithm to generate the CFG:

ConstructCFG():
  undergo BFS of AST, when encounter stmt_list:
    for every stmt in stmt_list:
      if is not if_stmt:
        if next stmt exists:
          connect self to next stmt
      if is if_stmt:
        connect self to first stmt in then stmt_list
        connect self to first stmt in else stmt_list
        connect last stmt in then stmt_list to next stmt outside of container
        connect last stmt in else stmt_list to next stmt outside of container
      if is while_stmt:
        connect self to first stmt in stmt_list
        ConnectLastStmtOfWhile(graph, while_stmt_tnode):

ConnectLastStmtOfWhile(graph, while_stmt_node):
  if last_stmt of while_stmt_node is not if_stmt:
    connect last_stmt to while_stmt
  else:
    ConnectLastStmtOfWhile(graph, while_stmt_num, then_stmt_list)
    ConnectLastStmtOfWhile(graph, while_stmt_num, else_stmt_list)
*/
void CFGHandler::ConnectLastStmtOfWhile(CFG& graph, const int while_stmt_num,
                                        const source_processor::TNode& current_stmt_list_tnode) {
  if (!current_stmt_list_tnode.IsType(source_processor::TNodeType::StatementList)) {
    throw std::runtime_error("CFGHandler::ConnectLastStmtOfWhile: argument current_stmt_list_tnode not of type StatementList");
  }

  const auto last_stmt = *current_stmt_list_tnode.GetChildren().back();
  if (!last_stmt.IsType(source_processor::TNodeType::If)) {
    // last_stmt is NOT an if statement, just connect it
    graph[last_stmt.GetStatementNumber()].push_back(while_stmt_num);
  } else {
    // last_stmt is an if statement, recursively try to get the last statement of the if statement
    ConnectLastStmtOfWhile(graph, while_stmt_num, last_stmt.GetThenStatementListTNode());
    ConnectLastStmtOfWhile(graph, while_stmt_num, last_stmt.GetElseStatementListTNode());
  }
}

void CFGHandler::ConnectLastStmtsOfIf(CFG& graph, const int exit_stmt_num,
                                      const source_processor::TNode& if_tnode) {
  if (!if_tnode.IsType(source_processor::TNodeType::If)) {
    throw std::runtime_error("CFGHandler::ConnectLastStmtsOfIf: argument if_tnode not of type If");
  }

  auto last_then_stmt_node = if_tnode.GetThenStatementListTNode().GetChildren().back();
  if (last_then_stmt_node->IsType(source_processor::TNodeType::If)) {
    ConnectLastStmtsOfIf(graph, exit_stmt_num, *last_then_stmt_node);
  } else {
    graph[last_then_stmt_node->GetStatementNumber()].push_back(exit_stmt_num);
  }

  auto last_else_stmt_node = if_tnode.GetElseStatementListTNode().GetChildren().back();
  if (last_else_stmt_node->IsType(source_processor::TNodeType::If)) {
    ConnectLastStmtsOfIf(graph, exit_stmt_num, *last_else_stmt_node);
  } else {
    graph[last_else_stmt_node->GetStatementNumber()].push_back(exit_stmt_num);
  }
}

const CFG& CFGHandler::ConstructCFG(const source_processor::TNode& root) {
  // get max statement number
  int max_stmt_num = StatementHandler::GetMaxStmtNum(root);
  // graph is an adjacency list; needs exactly (max_stmt_num + 1) space
  graph.clear();
  graph.resize(max_stmt_num + 1);  // note: NOT reserve (will lead to segfault)

  // traverse AST in BFS order
  std::list<source_processor::TNode> queue;
  queue.push_back(root);
  while (!queue.empty()) {
    const auto current_node = queue.front();
    queue.pop_front();

    // add children to queue
    for (const auto child : current_node.GetChildren()) {
      queue.push_back(*child);
    }

    if (!current_node.IsType(source_processor::TNodeType::StatementList)) {
      continue;
    }

    const auto stmt_list = current_node.GetChildren();
    for (int i = 0; i < stmt_list.size(); i++) {
      const auto current_stmt = stmt_list[i];
      int current_stmt_num = current_stmt->GetStatementNumber();

      if (!current_stmt->IsType(source_processor::TNodeType::If)) {
        if (i + 1 < stmt_list.size()) {
          int next_stmt_num = stmt_list[i + 1]->GetStatementNumber();
          graph[current_stmt_num].push_back(next_stmt_num);
        }
      }

      if (current_stmt->IsType(source_processor::TNodeType::If)) {
        const auto then_stmt_list = current_stmt->GetThenStatementListTNode();
        const auto else_stmt_list = current_stmt->GetElseStatementListTNode();
        int first_then_stmt_num = then_stmt_list.GetChildren().front()->GetStatementNumber();
        int first_else_stmt_num = else_stmt_list.GetChildren().front()->GetStatementNumber();
        graph[current_stmt_num].push_back(first_then_stmt_num);
        graph[current_stmt_num].push_back(first_else_stmt_num);

        if (i + 1 < stmt_list.size()) {
          // if there exists a next statement outside this If container, connect
          // the correct last statement(s) to said statement
          int next_stmt_num = stmt_list[i + 1]->GetStatementNumber();
          ConnectLastStmtsOfIf(graph, next_stmt_num, *current_stmt);
        }
      }

      if (current_stmt->IsType(source_processor::TNodeType::While)) {
        const auto while_stmt_list = current_stmt->GetWhileStatementListTNode();
        const auto first_while_stmt_num = while_stmt_list.GetChildren().front()->GetStatementNumber();
        graph[current_stmt_num].push_back(first_while_stmt_num);
        ConnectLastStmtOfWhile(graph, current_stmt_num, while_stmt_list);
      }
    }
  }

  return graph;
}

const CFG& CFGHandler::GetCFG() {
  if (graph.empty()) {
    throw std::runtime_error("CFGHandler::GetCFG: graph is not yet constructed");
  }
  return graph;
}

}  // namespace design_extractor