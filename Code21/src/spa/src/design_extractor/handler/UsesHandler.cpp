#include "UsesHandler.h"

#include "CallHandler.h"
#include "design_extractor/utils/DeUtils.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

// BFS to get all values of variables only
std::vector<std::string> UsesHandler::GetVariableValues(const source_processor::TNode& node) {
  std::vector<std::string> var_used;
  std::list<source_processor::TNode> queue;
  queue.push_back(node);

  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop_front();

    if (node.IsType(source_processor::TNodeType::Variable)) {
      var_used.push_back(node.GetValue());
    }

    for (auto child : node.GetChildren()) {
      queue.push_back(*child);
    }
  }
  return var_used;
}

void UsesHandler::ExtractUsesStmtsForParentT(PKB& pkb, std::unordered_set<int> parentT_list, std::string value) {
  for (int parent_stmt_num : parentT_list) {
    pkb.InsertUses(parent_stmt_num, value);
  }
}

void UsesHandler::ExtractUsesStmtsForParentT(PKB& pkb, std::unordered_set<int> parentT_list, std::vector<std::string> value_list) {
  for (auto value : value_list) {
    ExtractUsesStmtsForParentT(pkb, parentT_list, value);
  }
}

void UsesHandler::ExtractUsesStmtsWithoutCalls(PKB& pkb, const source_processor::TNode& node) {
  if (!node.IsType(source_processor::TNodeType::Print) &&
      !node.IsType(source_processor::TNodeType::Assign) &&
      !node.IsType(source_processor::TNodeType::If) &&
      !node.IsType(source_processor::TNodeType::While)) {
    return;
  }

  std::vector<std::string> value_list;
  std::unordered_set<int> parentT_list;

  if (node.IsType(source_processor::TNodeType::Print)) {
    value_list.push_back(node.GetVariableTNode().GetValue());

  } else if (node.IsType(source_processor::TNodeType::Assign)) {
    value_list = GetVariableValues(node.GetRootExpressionTNode());

  } else if (node.IsType(source_processor::TNodeType::If) || node.IsType(source_processor::TNodeType::While)) {
    value_list = GetVariableValues(node.GetRootConditionalExpressionTNode());
  }

  int stmt_num = node.GetStatementNumber();
  for (auto value : value_list) {
    pkb.InsertUses(stmt_num, value);
  }
  parentT_list = pkb.GetParentTStatements(stmt_num);
  ExtractUsesStmtsForParentT(pkb, parentT_list, value_list);
}

void UsesHandler::ExtractUsesSCallsAndUsesP(PKB& pkb, const source_processor::TNode& node) {
  graph inverse_call_graph = CallHandler::GetInverseCallGraph(pkb);

  std::vector<std::string> topo_order = DeUtils::Toposort(inverse_call_graph);

  for (auto& p : topo_order) {
    auto node = pkb.GetProcRange(p);
    int lo = node.first;
    int hi = node.second;

    std::unordered_set<std::string> vars_used_p;
    for (int i = lo; i <= hi; ++i) {
      //if this is a call stmt, pkb would have been populated with the
      //variables that the called procedure modifies
      DeUtils::SetAddAll(vars_used_p, pkb.GetUsedVariables(i));
    }
    /* vars_used_p has been populated correctly */

    //proc modifies all variables modified in its body
    //every stmt which is a call to proc also modifies all variables modified by proc
    for (auto& v : vars_used_p) {
      //the procedure modifies the variable
      pkb.InsertUses(p, v);
      //all calls to the procedure also modifies the variable
      //all ancestors of all call statement also modifies the variable
      for (int sn : pkb.GetStmtIndexesThatCalls(p)) {
        pkb.InsertUses(sn, v);  //for the call stmt which calls this fn
        for (int pn : pkb.GetParentTStatements(sn)) {
          pkb.InsertUses(pn, v);  //for the ParentT of the call stmt
        }
      }
    }
  }
}

}  // namespace design_extractor