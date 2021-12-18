#include "NextBipHandler.h"

#include <cassert>
#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

#include "CallHandler.h"
#include "EntityHandler.h"
#include "design_extractor/utils/CFGBipHandler.h"
#include "design_extractor/utils/CFGHandler.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

//Meant to be Process Control Block (PCB) from CS2106, which
//stores all info needed at each node of the CFGBip to resume execution
//from that point.

//cur_sn: the stmt# of the current node that is popped off the DFS stack
//hops_left: for reasons explained in the execution-aware traversal, we will only do traversal of path length
//n-1. 0 hops_left means we process this current node, but do not continue traversal further
//return_to: it's the call stack of the current execution. When it's time to return to a caller, if any,
//the top of the stack will be the stmt# to execute next / return to
struct PCB {
  int cur_sn;
  std::stack<int> return_to;  //stack.top would be the stmt no. a returning process should execute next
  int hops_left;              //when hops_left == 0, cur_sn is processed, but not any further
};

namespace design_extractor {

//initializing static member variables
CFG NextBipHandler::cfgbip;

//NextBip(a, b) is true for all edges from a -> b in cfgbip
void design_extractor::NextBipHandler::PopulateNextBipRelation(PKB& pkb) {
  for (int from = 0; from < cfgbip.size(); ++from) {
    for (auto to : cfgbip[from]) {
      //std::cout << "NextBip(" << from << ", " << to << ")\n";
      pkb.InsertNextBip(from, to);
    }
  }
}

/*
  Does a execution-aware traversal starting from stmt no. `from`.
  We cannot maintain a visited state, bc while loops mean that each node migh be visited more than once.
  i.e. CFGBip is not a DAG. Hence to prevent infinite DFS, it is known that there is a path
  from each node to all nodes it can reach in at most n-1 hops.
  Hence this function traverses only paths of length n-1, without maintaining visited nodes, to prevent
  infinite DFS.

  Tries to populate NextBipT(from, s), for all s.
  Note that NextBipT(from, from) is not automatically true
  */
void design_extractor::NextBipHandler::PopulateNextBipTRelationFromStmt(int from, PKB& pkb, int num_hops) {
  std::unordered_set<int> reachable;
  std::stack<PCB> s;
  std::vector<int> visits_left;

  visits_left.assign(cfgbip.size(), 1);
  ++visits_left[from];

  //when we start traversal we have an empty call stack
  //Edge case: `from` itself is not automatically nextbipT itself. so we maintain counter of how
  //many times it is seen
  int times_from_seen = 0;
  s.push(PCB{from, std::stack<int>{}, num_hops});

  while (!s.empty()) {
    PCB cur = s.top();
    s.pop();

    auto cur_sn = cur.cur_sn;
    auto return_to = cur.return_to;  //should be deep copy the stack
    auto hops_left = cur.hops_left;

    if (visits_left[cur_sn] <= 0) {
      continue;
    }
    --visits_left[cur_sn];

    //std::cout << cur_sn << " " << hops_left << "\n";

    //if hops_left == 0, this is the last node in this path we process
    //then we just push the node with hops_left == -1, and it will terminate in the
    //next loop here
    if (hops_left < 0) {
      continue;
    }
    assert(hops_left >= 0);
    if (cur_sn == from) {
      ++times_from_seen;
    }
    reachable.insert(cur_sn);

    //Traverse branchin edge next?
    if (pkb.GetStatementType(cur_sn) == EntityHandler::kcall_string) {
      //call can only have at most 1 branchin edge in cfgbip, which is to the beginning stmt no of the called proc
      //but call can have no Next stmts
      if (CFGBipHandler::IsExitStmt(cur_sn, pkb)) {
        //if this call stmt is also an exit stmt, then it has no next stmt for the callee
        //to return to. so callee should return to my caller, which is already on the stack
        //if my current call stack is empty, then callee can return to anywhere it can reach
        std::stack<int> new_stack = return_to;
        s.push(PCB{cfgbip[cur_sn][0], new_stack, hops_left - 1});
      } else {
        //cur_sn must have only 1 next statement and outgoing branchin, which callee must return to
        auto nexts = pkb.GetNextStatements(cur_sn);
        assert(nexts.size() == 1);
        assert(cfgbip[cur_sn].size() == 1);  //this is where dfs can go to next
        auto return_sn = *nexts.begin();     //this is where callee must return to
        std::stack<int> new_stack = return_to;
        new_stack.push(return_sn);
        s.push(PCB{cfgbip[cur_sn][0], new_stack, hops_left - 1});
      }
      //increment by 1 all stmts in the callee proc
      auto callee = GetProcStartingWith(cfgbip[cur_sn][0], pkb);
      auto range = pkb.GetProcRange(callee);
      for (int i = range.first; i <= range.second; ++i) {
        ++visits_left[i];
      }
    }
    //are we at a non-call exit stmt of a proc
    //and our only cfgbip edges are branchback edges, if any.
    //any stmt except if-stmts can be exit statements, but there could be no branchback edges if
    //no procedure calls this one
    else if (CFGBipHandler::IsExitStmt(cur_sn, pkb)) {
      //we are a non-call and exit stmt. The possible types, #Next stmts are, #cfgbip out-edges are:
      //type, #Next stmts are, #cfgbip out-edges are
      //read    0     c
      //print   0     c
      //assign  0     c
      //while   1     c+1
      //where c = #stmts this could possible return to
      //for the first 3 types, we must return to the caller if possible, or all possible callers
      //for the while stmt, we both return to the caller if possible, or all possible callers
      //and also duplicate PCB to continue DFS within our whileStmtList
      if (pkb.GetStatementType(cur_sn) == EntityHandler::kwhile_string) {  //while
        if (return_to.empty()) {
          //since we have no callers on call stack, we can return to anywhere that possibly called us,
          //or called procedures which had call stmts as exit stmts that called us
          for (int next_sn : cfgbip[cur_sn]) {
            std::stack<int> new_stack = return_to;  //just make duplicate of the empty stack
            s.push(PCB{next_sn, new_stack, hops_left - 1});
          }
        } else {
          //we were called from somewhere, so we can only return to somewhere in the caller
          //the return point is the top of the `return_to` call stack
          std::stack<int> new_stack = return_to;
          int next_sn = new_stack.top();
          new_stack.pop();
          s.push(PCB{next_sn, new_stack, hops_left - 1});

          //*also have to continue dfs in our own stmtLst
          auto nexts = pkb.GetNextStatements(cur_sn);
          assert(nexts.size() == 1);  //exit while stmt should only be able to enter its stmtlist
          next_sn = *nexts.begin();   //should be ok to reuse the ident once struct created
          std::stack<int> dup_stack = return_to;
          s.push(PCB{next_sn, dup_stack, hops_left - 1});
        }
      } else {  //read / print / assign stmts
        if (return_to.empty()) {
          //since we have no callers on call stack, we can return to anywhere that possibly called us,
          //or called procedures which had call stmts as exit stmts that called us
          for (int next_sn : cfgbip[cur_sn]) {
            std::stack<int> new_stack = return_to;  //just make duplicate of the empty stack
            s.push(PCB{next_sn, new_stack, hops_left - 1});
          }
        } else {
          //we were called from somewhere, so we can only return to somewhere in the caller
          //the return point is the top of the `return_to` call stack
          std::stack<int> new_stack = return_to;
          int next_sn = new_stack.top();
          new_stack.pop();
          s.push(PCB{next_sn, new_stack, hops_left - 1});
        }
      }
    }
    //we are a non-exit and non-call stmt. The possible types and #Next stmts are:
    //node type, #next stmts:
    //read    1
    //print   1
    //assign  1
    //if      2
    //while   2
    //for the first 3 types, we continue normal DFS
    //for the branching control flow, we duplicate the PCB
    else {
      if (pkb.GetStatementType(cur_sn) == EntityHandler::kread_string ||
          pkb.GetStatementType(cur_sn) == EntityHandler::kprint_string ||
          pkb.GetStatementType(cur_sn) == EntityHandler::kassign_string) {
        assert(cfgbip[cur_sn].size() == 1);
        std::stack<int> new_stack = return_to;
        s.push(PCB{cfgbip[cur_sn][0], new_stack, hops_left - 1});
      } else if (pkb.GetStatementType(cur_sn) == EntityHandler::kif_string ||
                 pkb.GetStatementType(cur_sn) == EntityHandler::kwhile_string) {
        //std::cout << "cur sn: " << cur_sn << " " << cfgbip[cur_sn].size() << std::endl;
        assert(cfgbip[cur_sn].size() == 2);
        for (int next_sn : cfgbip[cur_sn]) {
          std::stack<int> new_stack = return_to;
          s.push(PCB{next_sn, new_stack, hops_left - 1});
        }
      } else {
        //should never reach here if all cases handled properly, and cur_sn is a valid stmt number
        assert(cur_sn != 0 &&
               "In NextBipHandler, traversing CFGBip, \
            encountered unexpected node type or statement number");
      }
    }
  }

  if (times_from_seen == 1) {
    //it was only seen when it started from the source
    //so it's not reachable from itself
    reachable.erase(from);
  }
  for (auto r : reachable) {
    pkb.InsertNextBipT(from, r);
  }
}

std::string NextBipHandler::GetProcStartingWith(int sn, PKB& pkb) {
  auto procs = pkb.GetAllProcedures();
  for (auto p : procs) {
    auto range = pkb.GetProcRange(p);
    if (range.first == sn) {
      return p;
    }
  }
  assert(false && "there must exist at least 1 procedure with such a starting stmt#");
}

void design_extractor::NextBipHandler::PopulateNextBipTRelation(PKB& pkb) {
  int num_nodes = cfgbip.size();  //has 1 node more than num stmts bc of node 0, but it doesn't affect correctedness
  for (int from = 1; from < num_nodes; ++from) {
    //must start from stmt 1, bc 0 is an invalid stmt num
    //std::cout << "populating nb* for " << from << std::endl;
    PopulateNextBipTRelationFromStmt(from, pkb, num_nodes - 1);
    //std::cout << "populated nb* for " << from << std::endl;
  }
}

void NextBipHandler::ExtractNextBipAndNextBipTRelation(PKB& pkb, const source_processor::TNode& node) {
  cfgbip.clear();
  cfgbip = CFGBipHandler::GetCFGBip();
  PopulateNextBipRelation(pkb);
  PopulateNextBipTRelation(pkb);
}

}  // namespace design_extractor