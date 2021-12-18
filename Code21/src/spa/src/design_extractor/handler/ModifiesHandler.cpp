#include "ModifiesHandler.h"

#include <cassert>
#include <unordered_map>
#include <vector>

#include "CallHandler.h"
#include "design_extractor/utils/DeUtils.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

namespace design_extractor {

std::unordered_map<int, std::unordered_set<std::string>> ModifiesHandler::vars_modified_by;

//extractModifies is called once on root
//and will populate `vars_modified_by` with all vars modified by all stmtnos

//wishful thinking: For all stmtnos contained within subtree rooted at node,
//extractModifies will populate vars_modified_by with all vars modified by those stmtnos
void ModifiesHandler::ExtractModifiesSWithoutCallsStmtsHelper(const source_processor::TNode& node) {
  int cur_sn = node.GetStatementNumber();  //cur stmt no

  //base cases, can get vars modified by Read and Assignment stmts directly
  if (node.IsType(source_processor::TNodeType::Read)) {
    /*modifies var on RHS */
    std::unordered_set<std::string> vars_modded;
    vars_modded.insert(node.GetVariableTNode().GetValue());

    vars_modified_by[cur_sn] = vars_modded;
  }

  if (node.IsType(source_processor::TNodeType::Assign)) {
    //modifies var on LHS
    std::unordered_set<std::string> vars_modded;
    vars_modded.insert(node.GetAssigneeTNode().GetValue());

    vars_modified_by[cur_sn] = vars_modded;
  }

  //container statements
  //container stmt modifies the union of all vars modified by each of its child's stmtnos
  //so have to populate vars modified by its children first.
  //aka continue dfs on it's children first, and populate vars_modified_by once it has returned
  if (node.IsType(source_processor::TNodeType::While) ||
      node.IsType(source_processor::TNodeType::If)) {
    //do nothing
  }

  //implicit:
  //non-statement nodes don't trigger anything - we continue traversing the AST

  //continue dfs
  for (auto cptr : node.GetChildren()) {
    ExtractModifiesSWithoutCallsStmtsHelper(*cptr);
  }

  //dfs has been completed on all nodes in the subtree rooted at `node`
  //by guarantee of extractModifies, vars modified by all its children have been populated
  //now we just retreive and union all of them
  if (node.IsType(source_processor::TNodeType::While)) {
    for (auto cptr : node.GetWhileStatementListTNode().GetChildren()) {
      int child_sn = cptr->GetStatementNumber();
      auto& child_mod = vars_modified_by[child_sn];

      //Important: cur_sn doesn't exist as a key in the map yet
      //operator[] will create a default value for us
      //else will throw invalid key error
      DeUtils::SetAddAll(vars_modified_by[cur_sn], child_mod);
    }
  }

  if (node.IsType(source_processor::TNodeType::If)) {
    for (auto cptr : node.GetElseStatementListTNode().GetChildren()) {
      int child_sn = cptr->GetStatementNumber();
      auto& child_mod = vars_modified_by[child_sn];

      //Important: cur_sn doesn't exist as a key in the map yet
      //operator[] will create a default value for us
      //else will throw invalid key error
      DeUtils::SetAddAll(vars_modified_by[cur_sn], child_mod);
    }

    for (auto cptr : node.GetThenStatementListTNode().GetChildren()) {
      int child_sn = cptr->GetStatementNumber();
      auto& child_mod = vars_modified_by[child_sn];

      //Important: cur_sn doesn't exist as a key in the map yet
      //operator[] will create a default value for us
      //else will throw invalid key error
      DeUtils::SetAddAll(vars_modified_by[cur_sn], child_mod);
    }
  }
}

void ModifiesHandler::PopulateModifiesSWithoutCallsStmts(PKB& pkb) {
  //entry is pair of <stmtno, unordered_set<string> modified by this stmtno>
  for (auto& entry : vars_modified_by) {
    for (auto& var : entry.second) {
      pkb.InsertModifies(entry.first, var);
    }
  }
}

void ModifiesHandler::ExtractModifiesSWithoutCallsStmts(PKB& pkb, const source_processor::TNode& node) {
  vars_modified_by.clear();
  ExtractModifiesSWithoutCallsStmtsHelper(node);
  PopulateModifiesSWithoutCallsStmts(pkb);
}

/**
  * Populate all ModifiesP(proc, v) for a given proc.
  * Also Populate all ModifiesS(sn, v) where sn is a call stmt that calls proc.
  * Precondition: all call stmts within proc must already have had their
  * ModifiesP(..) populated
  */
void ModifiesHandler::PopulateModifiesSForCallsAndModifiesP(PKB& pkb, graph call_graph, std::string proc) {
  auto p = pkb.GetProcRange(proc);
  int lo = p.first;
  int hi = p.second;

  std::unordered_set<std::string> vars_modded_p;
  //
  for (int i = lo; i <= hi; ++i) {
    //if this is a call stmt, pkb would have been populated with the
    //variables that the called procedure modifies
    DeUtils::SetAddAll(vars_modded_p, pkb.GetModifiedVariables(i));
  }
  /* vars_modded_p has been populated correctly */

  //proc modifies all variables modified in its body
  //every stmt which is a call to proc also modifies all variables modified by proc
  for (auto& v : vars_modded_p) {
    //the procedure modifies the variable
    pkb.InsertModifies(proc, v);
    //all calls to the procedure also modifies the variable
    //all ancestors of all call statement also modifies the variable
    for (int sn : pkb.GetStmtIndexesThatCalls(proc)) {
      pkb.InsertModifies(sn, v);  //for the call stmt which calls this fn
      for (int pn : pkb.GetParentTStatements(sn)) {
        pkb.InsertModifies(pn, v);  //for the ParentT of the call stmt
      }
    }
  }
}

void ModifiesHandler::ExtractModifiesSForCallsAndModifiesP(PKB& pkb, const source_processor::TNode& node) {
  graph inverse_call_graph = CallHandler::GetInverseCallGraph(pkb);

  std::vector<std::string> topo_order = DeUtils::Toposort(inverse_call_graph);

  for (auto& p : topo_order) {
    PopulateModifiesSForCallsAndModifiesP(pkb, inverse_call_graph, p);
  }
}

}  // namespace design_extractor