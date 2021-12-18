#include "DeUtils.h"

#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "design_extractor/handler/StatementHandler.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

namespace design_extractor {
struct HashPair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

//Adds all strings from `give` into receive
void DeUtils::SetAddAll(std::unordered_set<std::string>& receive, const std::unordered_set<std::string>& give) {
  for (auto& e : give) {
    receive.insert(e);
  }
}

//Adds all ints from `give` into receive
void DeUtils::SetAddAll(std::unordered_set<int>& receive, const std::unordered_set<int>& give) {
  for (auto& e : give) {
    receive.insert(e);
  }
}

//Uses Kahn's algorithm for toposort from wikipedia
std::vector<std::string> DeUtils::Toposort(graph& call_graph) {
  std::unordered_map<std::string, int> incident_edges;
  std::unordered_set<std::pair<std::string, std::string>, HashPair> edge_set;  //directed edges

  //populate incident edges with all graph nodes
  for (auto& p : call_graph) {
    incident_edges[p.first] = 0;
  }

  //precomputing number of incident edges on each callee node
  for (auto& caller_callee : call_graph) {
    auto& caller = caller_callee.first;
    for (auto& callee : caller_callee.second) {
      if (incident_edges.find(callee) == incident_edges.end()) {
        incident_edges[callee] = 1;
      } else {
        ++incident_edges[callee];
      }

      edge_set.insert({caller, callee});
    }
  }

  std::vector<std::string> sorted_order;
  std::queue<std::string> no_incoming;

  //initialise set of vertices with no incoming edges
  for (auto& caller_callee : call_graph) {
    auto caller = caller_callee.first;
    if (incident_edges[caller] == 0) {
      no_incoming.push(caller);
    }
  }

  while (!no_incoming.empty()) {
    auto caller = no_incoming.front();  //cannot use reference. pop() will invalidate queue references.
    no_incoming.pop();
    sorted_order.push_back(caller);
    for (auto& callee : call_graph[caller]) {
      --incident_edges[callee];
      int e = edge_set.erase({caller, callee});
      if (incident_edges[callee] == 0) {
        no_incoming.push(callee);
      }
    }
  }

  if (!edge_set.empty()) {
    throw std::runtime_error("DeUtils::Toposort: graph contains cycles. Topological sort does not exist.");
  } else {
    return sorted_order;
  }
}

//BFS outward from src, marking all nodes that can be reached
std::unordered_set<int> DeUtils::GetReachableNodes(int src, const CFG& cfg) {
  std::unordered_set<int> reachable_nodes;
  std::queue<int> q;

  //in normal BFS, src node is marked as visited first
  //but if src is marked as visited, that means Next*(src, src) is true,
  //which is not always true
  //reachable_nodes.insert(src);
  q.push(src);

  while (!q.empty()) {
    auto cur = q.front();
    q.pop();
    for (auto e : cfg[cur]) {
      if (reachable_nodes.find(e) == reachable_nodes.end()) {
        reachable_nodes.insert(e);
        q.push(e);
      }
    }
  }
  return reachable_nodes;
}

void DeUtils::PrintSet(std::string set_name, std::unordered_set<int> set) {
  std::cout << "in " << set_name << " : ";
  for (auto e : set) {
    std::cout << e << " ";
  }
  std::cout << "\n";
}

}  // namespace design_extractor