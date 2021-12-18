#include "AffectsBipHandler.h"

#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <unordered_set>

#include "EntityHandler.h"
#include "design_extractor/graph_explosion/GEHandler.h"
#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

AffectsBipGraph AffectsBipHandler::affects_bip_graph;

// DFS outward from src, marking all nodes that can be reached
void AffectsBipHandler::TraverseExplodedGraphFromSrc(GENode* src, PKB& pkb) {
  std::unordered_set<GENode*> reachable_nodes;
  std::stack<GENode*> stack;

  int src_stmt_num = src->GetStatementNumber();
  std::string modified_var = pkb.GetAssignedVariable(src_stmt_num);

  stack.push(src);

  while (!stack.empty()) {
    auto cur = stack.top();
    stack.pop();
    bool path_exists = true;
    int cur_stmt_num = cur->GetStatementNumber();

    // if stmt is reachable
    if (reachable_nodes.find(cur) != reachable_nodes.end()) {
      // only insert if it is assign stmt && stmt uses LHS
      if (pkb.GetAllAssignStmts().find(cur_stmt_num) != pkb.GetAllAssignStmts().end() && (pkb.IsUses(cur_stmt_num, modified_var))) {
        // populate affectsbip graph
        affects_bip_graph[src].insert(cur);
        pkb.InsertAffectsBip(src_stmt_num, cur_stmt_num);
      }

      // if stmt (assign/read) modify LHS
      std::string stmt_type = pkb.GetStatementType(cur_stmt_num);
      if (stmt_type == EntityHandler::kassign_string || stmt_type == EntityHandler::kread_string) {
        if (pkb.IsModifies(cur_stmt_num, modified_var)) {
          path_exists = false;
        }
      }
    }

    for (auto e : cur->GetChildren()) {
      if (reachable_nodes.find(e) == reachable_nodes.end() && path_exists == true) {
        reachable_nodes.insert(e);
        stack.push(e);
      }
    }
  }
}

void AffectsBipHandler::ExtractAffectsBipForGraph(GENode* root_node, PKB& pkb) {
  std::unordered_set<GENode*> reachable_nodes;
  std::queue<GENode*> q;

  // BFS traverse exploded graph
  q.push(root_node);
  while (!q.empty()) {
    auto cur = q.front();
    q.pop();

    // if meet assign stmt, send it to TraverseExplodedGraphFromSrc
    int cur_stmt_num = cur->GetStatementNumber();
    if (pkb.GetAllAssignStmts().find(cur_stmt_num) != pkb.GetAllAssignStmts().end()) {
      TraverseExplodedGraphFromSrc(cur, pkb);
    }

    for (auto e : cur->GetChildren()) {
      if (reachable_nodes.find(e) == reachable_nodes.end()) {
        reachable_nodes.insert(e);
        q.push(e);
      }
    }
  }
}

void AffectsBipHandler::ExtractAffectsBip(PKB& pkb, const source_processor::TNode& root) {
  std::vector<GENode*> exploded_graphs = GEHandler::GetGraphExplosion();
  affects_bip_graph.clear();

  for (auto graph : exploded_graphs) {
    // extracts and populates affectsbip graph
    ExtractAffectsBipForGraph(graph, pkb);
  }
}

//BFS outward from src, marking all nodes that can be reached and is affected
std::unordered_set<GENode*> AffectsBipHandler::GetReachableNodes(GENode* src, PKB& pkb) {
  std::unordered_set<GENode*> reachable_nodes;
  std::queue<GENode*> q;

  q.push(src);

  while (!q.empty()) {
    auto cur = q.front();
    q.pop();
    for (auto e : affects_bip_graph[cur]) {
      int cur_stmt_num = cur->GetStatementNumber();
      int child_stmt_num = e->GetStatementNumber();

      if (reachable_nodes.find(e) == reachable_nodes.end() && pkb.IsAffectsBip(cur_stmt_num, child_stmt_num)) {
        reachable_nodes.insert(e);
        q.push(e);
      }
    }
  }
  return reachable_nodes;
}

void AffectsBipHandler::ExtractAffectsBipT(PKB& pkb, const source_processor::TNode& root) {
  // assumes affectsbip graph has been populated by ExtractAffectsBip
  for (auto src : affects_bip_graph) {
    auto reachable_nodes = GetReachableNodes(src.first, pkb);
    int src_stmt_num = src.first->GetStatementNumber();
    for (auto dest : reachable_nodes) {
      pkb.InsertAffectsBipT(src_stmt_num, dest->GetStatementNumber());
    }
  }
}

}  // namespace design_extractor