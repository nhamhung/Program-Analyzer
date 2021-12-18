#include "CFGBipHandler.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

#include "CFGHandler.h"
#include "DeUtils.h"
#include "design_extractor/handler/CallHandler.h"
#include "design_extractor/handler/EntityHandler.h"
#include "design_extractor/handler/StatementHandler.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

namespace design_extractor {

CFG CFGBipHandler::cfgbip;
std::unordered_map<std::string, std::unordered_set<int>> CFGBipHandler::exit_stmts;

int CFGBipHandler::GetProcEntryStmt(std::string proc, PKB& pkb) {
  return pkb.GetProcRange(proc).first;
  ;
}

/*
Detecting exist stmts is done by examining number of next stmts.
| stmt types | possible num out edges | how many out edges if it is exit pt |
| ---------- | ---------------------- | ----------------------------------- |
| read       | 0,1                    | 0                                   |
| print      | 0,1                    | 0                                   |
| assign     | 0,1                    | 0                                   |
| while      | 1,2                    | 1                                   |
| call       | 0,1                    | 0                                   |
| if         | 2                      | never an exit stmt                  |
*/
//Exit stmts are checked via the number of Next() stmts an sn has
bool CFGBipHandler::IsExitStmt(int sn, PKB& pkb) {
  auto type = pkb.GetStatementType(sn);
  auto next = pkb.GetNextStatements(sn);
  if (type == EntityHandler::kread_string && next.size() == 0) {
    return true;
  }
  if (type == EntityHandler::kprint_string && next.size() == 0) {
    return true;
  }
  if (type == EntityHandler::kassign_string && next.size() == 0) {
    return true;
  }
  if (type == EntityHandler::kwhile_string && next.size() == 1) {
    return true;
  }
  if (type == EntityHandler::kcall_string && next.size() == 0) {
    return true;
  }
  return false;
}

/*
Operate(string proc) will be called on procs in topo order of the reverse call graph
Operate needs to, for each procedure's CFG forest:
  - populate all exit stmts of this procedure (could be in other proc if a call stmt is an exit stmt)
  - establish BranchIns to other procs if this procedure calls any other procs
  - establish BranchBacks from called procs back to this proc, if its call stmts have Next stmts

ALGO:
find entry stmt#. (GetProcRange().first)
start traversal of cfg from that stmt node:
  if node is a call stmt:
    establish BranchIns to callee in cfgbip
    establish BranchBacks back to caller's next cfgbip

  if node is an exit stmt:
    if node is a call stmt:
      add all exit pts of the callee proc to my exit stmts
    else:
      add current stmt to my exit stmts

  if node is call stmt:
    delete edge to my next stmts in cfg

update exit_stmts for the current proc
*/
void CFGBipHandler::Operate(std::string proc, PKB& pkb) {
  int entry_sn = GetProcEntryStmt(proc, pkb);
  std::unordered_set<int> exits;

  std::queue<int> q;
  std::unordered_set<int> visited;

  //CFG can contain while loops (infinite cycles), so need to maintain visited nodes
  visited.insert(entry_sn);
  q.push(entry_sn);

  while (!q.empty()) {
    auto cur_sn = q.front();
    q.pop();
    auto next = pkb.GetNextStatements(cur_sn);

    //create BranchIn and BranchBack edges
    if (pkb.GetStatementType(cur_sn) == EntityHandler::kcall_string) {
      //create BranchIn edge to callee
      auto callee = pkb.GetCallsProcName(cur_sn);
      assert(callee != "");
      auto callee_entry = GetProcEntryStmt(callee, pkb);
      assert(std::find(cfgbip[cur_sn].begin(), cfgbip[cur_sn].end(), callee_entry) == cfgbip[cur_sn].end());  //BranchIn edge doesn't exist yet
      cfgbip[cur_sn].push_back(callee_entry);

      //create BranchBack edges
      //every exit stmt in the callee proc is joined to every next stmt (if any)
      for (int n : next) {
        for (int e : exit_stmts[callee]) {
          //it's possible that different call stmts to the same proc have the same next stmt
          //due to whiles or ifs. Only add BranchBack if it's not been added before
          if (std::find(cfgbip[e].begin(), cfgbip[e].end(), n) == cfgbip[e].end()) {
            cfgbip[e].push_back(n);
          }
        }
      }
    }

    //add the exit stmts of the current proc, if the current node is an exit stmt
    if (IsExitStmt(cur_sn, pkb)) {
      if (pkb.GetStatementType(cur_sn) == EntityHandler::kcall_string) {
        auto callee = pkb.GetCallsProcName(cur_sn);
        assert(callee != "");
        DeUtils::SetAddAll(exits, exit_stmts[callee]);
      } else {
        //else cur_sn is a single exit stmt in the current proc
        exits.insert(cur_sn);
      }
    }

    //delete all next edges from call stmts to their next stmts
    if (pkb.GetStatementType(cur_sn) == EntityHandler::kcall_string) {
      for (auto n : next) {
        auto nit = std::find(cfgbip[cur_sn].begin(), cfgbip[cur_sn].end(), n);
        assert(nit != cfgbip[cur_sn].end());  //this element should exist
        cfgbip[cur_sn].erase(nit);
      }
    }

    //continue  BFS
    for (int n : next) {
      if (visited.find(n) == visited.end()) {
        visited.insert(n);
        q.push(n);
      }
    }
  }

  //update exit_stmts
  exit_stmts[proc] = exits;
}

/*
ALGO for constructing CFGBip: 
clear cfgbip before population (single use DE)
clear exit_stmts

cfgbip = cfg (initially)
get toposort of inverse call graph
Operate on each proc in topo order.
*/
const CFG& CFGBipHandler::ConstructCFGBip(PKB& pkb) {
  cfgbip.clear();
  exit_stmts.clear();

  cfgbip = CFGHandler::GetCFG();  //should be deep copy. Look here if bugs occur

  graph inverse_call_graph = CallHandler::GetInverseCallGraph(pkb);
  auto topo_order = DeUtils::Toposort(inverse_call_graph);
  for (std::string p : topo_order) {
    Operate(p, pkb);
  }

  return cfgbip;
}

const CFG& CFGBipHandler::GetCFGBip() {
  if (cfgbip.empty()) {
    throw std::runtime_error("CFGBipHandler::GetCFGBip: cfgbip is not yet constructed");
  }
  return cfgbip;
}

}  // namespace design_extractor