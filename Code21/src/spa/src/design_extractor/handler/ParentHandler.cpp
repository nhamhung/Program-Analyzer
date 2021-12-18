#include "ParentHandler.h"

#include <cassert>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

namespace design_extractor {

std::vector<std::pair<int, int>> ParentHandler::parent_entries;   // each entry is a <stmt no, stmt no> pair
std::vector<std::pair<int, int>> ParentHandler::parentT_entries;  // each entry is a <stmt no, stmt no> pair
std::vector<int> ancestors;                                       //ancestors[0] is earliest ancestor, and last element is immediate parent

bool IsStmtNode(const source_processor::TNode& node) {
  return node.IsType(source_processor::TNodeType::Read) ||
         node.IsType(source_processor::TNodeType::Print) ||
         node.IsType(source_processor::TNodeType::While) ||
         node.IsType(source_processor::TNodeType::Assign) ||
         node.IsType(source_processor::TNodeType::If) ||
         node.IsType(source_processor::TNodeType::Call);
}

bool IsContainerNode(const source_processor::TNode& node) {
  return node.IsType(source_processor::TNodeType::While) ||
         node.IsType(source_processor::TNodeType::If);
}

void ParentHandler::ExtractParentAndParentTHelper(PKB& pkb, const source_processor::TNode& node) {
  //if is a stmt node, then add all parents / parents* entries
  if (IsStmtNode(node)) {
    if (ancestors.size() >= 1) {
      //add parent
      parent_entries.push_back({ancestors.back(), node.GetStatementNumber()});

      //add parentT
      for (int ancestor : ancestors) {
        parentT_entries.push_back({ancestor, node.GetStatementNumber()});
      }
    }
  }

  //if is a container stmt (which is also a stmt node)
  //add him to ancestors
  if (IsContainerNode(node)) {
    ancestors.push_back(node.GetStatementNumber());
  }

  //continue the dfs regardless of node type
  for (auto cptr : node.GetChildren()) {
    ExtractParentAndParentTHelper(pkb, *cptr);
  }

  //backtrack: leaving function to explore other branches where this node will no longer be an ancestor
  //ONLY pop if this is a possible parent node / was added to ancestor during this call
  if (IsContainerNode(node)) {
    assert(ancestors.size() >= 1 && "Popping from ancestors when none exist. At least 1 should exist");
    ancestors.pop_back();
  }
}

void ParentHandler::PopulateParentAndParentT(PKB& pkb) {
  for (auto e : parent_entries) {
    pkb.InsertParent(e.first, e.second);
  }
  for (auto e : parentT_entries) {
    pkb.InsertParentT(e.first, e.second);
  }
}

void ParentHandler::ExtractParentAndParentTStmts(PKB& pkb, const source_processor::TNode& node) {
  parent_entries.clear();
  parentT_entries.clear();
  ancestors.clear();
  ExtractParentAndParentTHelper(pkb, node);
  PopulateParentAndParentT(pkb);
}
}  // namespace design_extractor