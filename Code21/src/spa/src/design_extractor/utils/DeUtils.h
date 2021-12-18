#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CFGHandler.h"
#include "source_processor/ast/TNode.h"

typedef std::unordered_map<std::string, std::unordered_set<std::string>> graph;

namespace design_extractor {

class DeUtils {
 public:
  /*
   `give` to be a const lvalue reference so it can accept both 
   rvalue and lvalue references for code cleanliness.
   */
  static void SetAddAll(std::unordered_set<std::string>& receive, const std::unordered_set<std::string>& give);
  static void SetAddAll(std::unordered_set<int>& receive, const std::unordered_set<int>& give);

  /*
   topological order goes from first to last element.
   Requires that every node in graph appears as a key, even if it doesn't call any procedure. Else 
    toposort will not have knowledge of all the nodes in the graph, and cannot return a correct toposort
   throws runtime error if cycle is detected.
   topo order returned is non-unique.
   */
  static std::vector<std::string> Toposort(graph& call_graph);

  /*
  BFS outward from src, marking all nodes that can be reached, without marking src as reached initially.
  In normal BFS, src node is marked as visited first.
  But if src is marked as visited, that means Relation(src, src) is true, which is not always true
  */
  static std::unordered_set<int> GetReachableNodes(int src, const CFG& cfg);

  static void PrintSet(std::string set_name, std::unordered_set<int> set);
};

}  // namespace design_extractor