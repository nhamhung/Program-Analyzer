#include "GEHandler.h"

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "GENode.h"
#include "design_extractor/handler/CallHandler.h"
#include "design_extractor/utils/CFGHandler.h"
#include "design_extractor/utils/DeUtils.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

namespace design_extractor {

std::vector<GENode*> GEHandler::graphs;

GENode* GEHandler::GetOrInsert(
    std::map<int, GENode*>& stmt_num_to_ptr_map, int stmt_num) {
  if (stmt_num_to_ptr_map.find(stmt_num) == stmt_num_to_ptr_map.end()) {
    stmt_num_to_ptr_map[stmt_num] = new GENode(stmt_num);
  }
  return stmt_num_to_ptr_map[stmt_num];
}

const std::vector<std::string> GEHandler::GetStartingProcedureNames(PKB& pkb) {
  std::vector<std::string> pnames;

  auto called_procs = pkb.GetAllProceduresCalled();
  for (auto pname : pkb.GetAllProcedures()) {
    if (called_procs.find(pname) == called_procs.end()) {
      pnames.push_back(pname);
    }
  }

  return pnames;
}

void GEHandler::ConnectLastExecutedStmts(
    const source_processor::TNode& stmt_list_tnode,
    std::map<int, GENode*>& stmt_num_to_ptr_map,
    GENode* exit_node) {
  // Return if there's nothing to connect to
  if (exit_node == nullptr) {
    return;
  }

  auto last_stmt = stmt_list_tnode.GetChildren().back();

  // If last statement is Call: terminate and do not connect
  if (last_stmt->IsType(source_processor::TNodeType::Call)) {
    return;
  }

  // If last statement is If: recursively call function with Then and Else stmt lists
  if (last_stmt->IsType(source_processor::TNodeType::If)) {
    auto then_stmt_lst_tnode = last_stmt->GetThenStatementListTNode();
    auto else_stmt_lst_tnode = last_stmt->GetElseStatementListTNode();
    ConnectLastExecutedStmts(then_stmt_lst_tnode, stmt_num_to_ptr_map, exit_node);
    ConnectLastExecutedStmts(else_stmt_lst_tnode, stmt_num_to_ptr_map, exit_node);
    return;
  }

  // If last statement is anything else: connect it to the argument exit_node
  auto last_stmt_num = last_stmt->GetStatementNumber();
  stmt_num_to_ptr_map[last_stmt_num]->AddChild(exit_node);
}

std::map<int, GENode*> GEHandler::ExplodeGraph(
    PKB& pkb, const CFG& cfg, const source_processor::TNode& ast,
    const std::string& current_pname, GENode* current_exit_node) {
  auto call_stmts = pkb.GetAllCallsStmts();
  auto stmt_range = pkb.GetProcRange(current_pname);
  int start_stmt_num = stmt_range.first;
  int end_stmt_num = stmt_range.second;

  // map to store the GENode ptrs needed
  std::map<int, GENode*> stmt_num_to_ptr_map;

  for (int stmt_num = start_stmt_num; stmt_num <= end_stmt_num; stmt_num++) {
    GENode* current_node = GetOrInsert(stmt_num_to_ptr_map, stmt_num);
    if (call_stmts.find(stmt_num) != call_stmts.end()) {
      // If is currently call_stmt, recursively call ExplodeGraph
      GENode* exit_node =
          cfg[stmt_num].empty()
              // if no next statement, the exit_node must be the current one
              ? current_exit_node
              // else, use the next statement as the exit node
              : GetOrInsert(stmt_num_to_ptr_map, cfg[stmt_num][0]);
      auto pname_called = pkb.GetCallsProcName(stmt_num);
      auto pcalled_map = ExplodeGraph(pkb, cfg, ast, pname_called, exit_node);
      auto root_node = pcalled_map[pkb.GetProcRange(pname_called).first];
      current_node->AddChild(root_node);

      source_processor::TNode pcalled_stmt_list_tnode =
          source_processor::TNode::GetProcedureStmtLst(ast, pname_called);
      ConnectLastExecutedStmts(pcalled_stmt_list_tnode, pcalled_map, exit_node);
    } else {
      // if any other statement, create graph accordingly to CFG
      for (int child_stmt_num : cfg[stmt_num]) {
        current_node->AddChild(GetOrInsert(stmt_num_to_ptr_map, child_stmt_num));
      }
    }
  }

  // first statement in procedure is root of exploded graph
  return stmt_num_to_ptr_map;
}

const std::vector<GENode*> GEHandler::ConstructGraphExplosion(
    PKB& pkb, const source_processor::TNode& ast) {
  CFG cfg = CFGHandler::GetCFG();
  auto starting_pnames = GetStartingProcedureNames(pkb);
  graphs.clear();

  for (auto pname : starting_pnames) {
    // Get the stmt_num to GENode ptr map of the current procedure
    auto stmt_num_to_ptr_map = ExplodeGraph(pkb, cfg, ast, pname, nullptr);
    // First statement of the procedure must be the root of the exploded graph
    auto root_node = stmt_num_to_ptr_map[pkb.GetProcRange(pname).first];
    graphs.push_back(root_node);
  }

  return graphs;
}

const std::vector<GENode*> GEHandler::GetGraphExplosion() {
  if (graphs.empty()) {
    throw std::runtime_error("GEHandler::GetGraphExplosion: graphs are not yet constructed");
  }
  return graphs;
}

}  // namespace design_extractor
