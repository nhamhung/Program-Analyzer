#include "GENode.h"

#include <iostream>
#include <list>
#include <unordered_set>

namespace design_extractor {

GENode::GENode(int statement_number) {
  this->statement_number = statement_number;
}

GENode& GENode::AddChild(GENode* child) {
  children.push_back(child);
  return *this;
}

void GENode::PrintDebugInfo(const GENode& root) {
  // traverse AST in BFS order
  std::list<const GENode*> queue;
  std::unordered_set<const GENode*> visited_set;
  queue.push_back(&root);
  visited_set.insert(&root);

  while (!queue.empty()) {
    auto current_node = queue.front();
    queue.pop_front();

    std::cout << current_node->GetStatementNumber()
              << " ("
              << current_node
              << ")"
              << ": ";

    // add children to queue
    for (const auto child : current_node->GetChildren()) {
      std::cout << child->GetStatementNumber()
                << " ("
                << child
                << ") ";
      if (visited_set.find(child) == visited_set.end()) {
        visited_set.insert(child);
        queue.push_back(child);
      }
    }

    std::cout << "\n";
  }
}

}  // namespace design_extractor