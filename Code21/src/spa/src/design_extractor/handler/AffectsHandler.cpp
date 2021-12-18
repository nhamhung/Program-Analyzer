#include "AffectsHandler.h"

#include <iostream>
#include <queue>
#include <stack>

#include "EntityHandler.h"
#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

std::unordered_set<int> AffectsHandler::assign_stmts;
AffectsGraph AffectsHandler::graph;

//DFS outward from src, marking all nodes that can be reached
void AffectsHandler::TraverseCFGFromSource(int src, std::string& modified_var, const CFG& cfg, PKB& pkb) {
  std::unordered_set<int> reachable_nodes;
  std::stack<int> stack;

  stack.push(src);

  while (!stack.empty()) {
    auto cur = stack.top();
    stack.pop();
    bool path_exists = true;

    // if stmt is reachable
    if (reachable_nodes.find(cur) != reachable_nodes.end()) {
      // only insert if it is assign stmt && stmt uses LHS
      if (assign_stmts.find(cur) != assign_stmts.end() && (pkb.IsUses(cur, modified_var))) {
        pkb.InsertAffects(src, cur);
      }

      // if stmt (assign/read/proc call) modify LHS
      std::string stmt_type = pkb.GetStatementType(cur);
      if (stmt_type == EntityHandler::kassign_string || stmt_type == EntityHandler::kread_string || stmt_type == EntityHandler::kcall_string) {
        if (pkb.IsModifies(cur, modified_var)) {
          path_exists = false;
        }
      }
    }

    for (auto e : cfg[cur]) {
      if (reachable_nodes.find(e) == reachable_nodes.end() && path_exists == true) {
        reachable_nodes.insert(e);
        stack.push(e);
      }
    }
  }
}

void AffectsHandler::ExtractAffects(PKB& pkb, const source_processor::TNode& root) {
  CFG cfg = CFGHandler::GetCFG();
  assign_stmts = pkb.GetAllAssignStmts();

  for (auto stmt_num : assign_stmts) {
    std::string modified_var = pkb.GetAssignedVariable(stmt_num);
    TraverseCFGFromSource(stmt_num, modified_var, cfg, pkb);
  }
}

//BFS outward from src, marking all nodes that can be reached
std::unordered_set<int> AffectsHandler::GetReachableNodes(int src) {
  std::unordered_set<int> reachable_nodes;
  std::queue<int> q;

  q.push(src);

  while (!q.empty()) {
    auto cur = q.front();
    q.pop();
    for (auto e : graph[cur]) {
      if (reachable_nodes.find(e) == reachable_nodes.end()) {
        reachable_nodes.insert(e);
        q.push(e);
      }
    }
  }
  return reachable_nodes;
}

void AffectsHandler::ExtractAffectsT(PKB& pkb, const source_processor::TNode& root) {
  assign_stmts = pkb.GetAllAssignStmts();
  graph.clear();

  // populate affects graph
  for (int stmt_num : assign_stmts) {
    std::unordered_set<int> affected_stmts = pkb.GetAffectedStatements(stmt_num);
    for (auto affected_stmt : affected_stmts) {
      graph[stmt_num].insert(affected_stmt);
    }
  }

  // get reachable nodes
  for (int src = 0; src < graph.size(); src++) {
    auto reachable_nodes = GetReachableNodes(src);
    for (auto to : reachable_nodes) {
      pkb.InsertAffectsT(src, to);
    }
  }
}

}  // namespace design_extractor