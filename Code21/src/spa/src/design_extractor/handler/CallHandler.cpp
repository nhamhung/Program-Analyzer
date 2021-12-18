#include "CallHandler.h"

#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "design_extractor/utils/DeUtils.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/utils/TypeUtils.h"

namespace design_extractor {

std::unordered_map<std::string, std::unordered_set<std::string>> CallHandler::call_graph;

void CallHandler::ExtractCallStmts(PKB& pkb, const source_processor::TNode& node) {
  if (!node.IsType(source_processor::TNodeType::Call)) {
    return;
  }

  //std::cout << "extract call stmt " << node.GetStatementNumber() << " " << node.GetValue() << std::endl;
  pkb.InsertCalls(node.GetStatementNumber(), node.GetValue());
}

/* Called on procedure TNodes.
* Will populate call_graph with all the procedures that are called
* within this procedure node
*/
void CallHandler::ExtractCallRelation(PKB& pkb, const source_processor::TNode& node) {
  if (!node.IsType(source_processor::TNodeType::Procedure)) {
    return;
  }
  assert(node.IsType(source_processor::TNodeType::Procedure) &&
         "ExtractCallRelation should only be called on a procedure node");

  std::list<source_processor::TNode> queue;
  queue.push_back(node);

  while (!queue.empty()) {
    auto cur = queue.front();  //for some reason, auto cannot be changed to TNode&, else getChildren() will not return any children.
    queue.pop_front();

    if (cur.IsType(source_processor::TNodeType::Call)) {
      pkb.InsertCalls(node.GetValue(), cur.GetValue());
      //std::cout << node.GetValue() << " calls " << cur.GetValue() << std::endl;
      call_graph[node.GetValue()].insert(cur.GetValue());  //node is the parent of cur in the call graph
    }

    //assert(cur.GetChildren().size() >= 1);
    for (auto child : cur.GetChildren()) {
      queue.push_back(*child);
    }
  }
}

/* Precondition: call_graph must already be populated, 
* aka ExtractCallRelation() must have been called prior
*/
void CallHandler::ExtractCallTRelation(PKB& pkb, const source_processor::TNode& node) {
  for (auto& pair : call_graph) {
    assert(!pair.second.empty() && "If call_graph has this entry, it should have non-empty descendants");

    //descendants = union of all GetDescendants(children)
    //for every d in descendant, calls*(cur, d) is true
    std::unordered_set<std::string> descendants;
    GetDescendantsOf(pair.first, descendants);

    for (std::string d : descendants) {
      //std::cout << pair.first << " calls* " << d << std::endl;
      pkb.InsertCallsT(pair.first, d);
    }
  }

  //clear the static data structures after every run
  //ExtractCallTRelation() will be called last among all the call handler functions, and will only
  //be called once per SIMPLE program
  call_graph.clear();
}

// will return direct (children) and indirect descendants (grandchildren onwards) of caller
void CallHandler::GetDescendantsOf(std::string caller,
                                   std::unordered_set<std::string>& descendants) {
  //if this caller has no descendants, there is nothing to populate descendants with
  if (call_graph.find(caller) == call_graph.end()) {
    return;
  }

  //descendants of caller is the union of all descendants of caller's children
  //operator[] creates an empty set for the k-v pair if key doesn't exist
  //we avoid this by doing existence check before this
  for (std::string child : call_graph[caller]) {
    std::unordered_set<std::string> childs_descendants;
    GetDescendantsOf(child, childs_descendants);
    descendants.insert(child);  //insert the

    for (std::string cd : childs_descendants) {
      descendants.insert(cd);
    }
  }
  return;
}

graph CallHandler::GetInverseCallGraph(PKB& pkb) {
  std::unordered_set<std::string> procedures = pkb.GetAllProcedures();

  //if proc A calls proc B in call graph,
  //if proc B calls proc A in inverse call graph,
  graph inverse_call_graph;

  for (auto p : procedures) {
    //every procedure must appear as a key in a call graph, even if it calls no other procedures
    inverse_call_graph[p] = std::unordered_set<std::string>{};
  }

  for (auto p : procedures) {
    auto callees = pkb.GetProceduresCalled(p);
    for (auto c : callees) {
      inverse_call_graph[c].insert(p);
    }
  }
  //for (auto& p : procedures) {
  //  auto callees = pkb.GetProceduresCalled(p);
  //  for (auto c : callees) {
  //    if (inverse_call_graph.count(c) == 0) {
  //      inverse_call_graph[c] = std::unordered_set<std::string>{p};
  //    } else {
  //      inverse_call_graph[c].insert(p);
  //    }
  //  }
  //}
  assert(procedures.size() == inverse_call_graph.size());
  return inverse_call_graph;
}

}  // namespace design_extractor