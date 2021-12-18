#include "DesignExtractor.h"

#include <list>

#include "graph_explosion/GEHandler.h"
#include "handler/AffectsBipHandler.h"
#include "handler/AffectsHandler.h"
#include "handler/AssignmentHandler.h"
#include "handler/CallHandler.h"
#include "handler/ConstantHandler.h"
#include "handler/EntityHandler.h"
#include "handler/FollowsHandler.h"
#include "handler/IfHandler.h"
#include "handler/ModifiesHandler.h"
#include "handler/NextBipHandler.h"
#include "handler/NextHandler.h"
#include "handler/ParentHandler.h"
#include "handler/PrintHandler.h"
#include "handler/ProcedureHandler.h"
#include "handler/ReadHandler.h"
#include "handler/StatementHandler.h"
#include "handler/UsesHandler.h"
#include "handler/VariableHandler.h"
#include "handler/WhileHandler.h"
#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/utils/TypeUtils.h"
#include "utils/CFGBipHandler.h"
#include "utils/CFGHandler.h"
#include "utils/Extension.h"

namespace design_extractor {

// A generic BFS traversal for AST to extract all relevant relationships
// No type checks should be done within BFS, checks should be done within their respective
// functions to respect SRP
void DesignExtractor::BreadthFirstTraversal(PKB& pkb, const source_processor::TNode& root) {
  std::list<source_processor::TNode> queue;
  queue.push_back(root);

  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop_front();

    // function calls to extract respective relationships
    PrintHandler::ExtractPrintStmts(pkb, node);
    ReadHandler::ExtractReadStmts(pkb, node);
    VariableHandler::ExtractVariableStmts(pkb, node);
    ProcedureHandler::ExtractProcedureStmts(pkb, node);
    ConstantHandler::ExtractConstants(pkb, node);
    FollowsHandler::ExtractFollowsStmts(pkb, node);
    FollowsHandler::ExtractFollowsTStmts(pkb, node);
    AssignmentHandler::ExtractAssignStmts(pkb, node);
    IfHandler::ExtractIfStmts(pkb, node);
    WhileHandler::ExtractWhileStmts(pkb, node);
    UsesHandler::ExtractUsesStmtsWithoutCalls(pkb, node);
    EntityHandler::ExtractStmtEntityType(pkb, node);
    CallHandler::ExtractCallStmts(pkb, node);
    CallHandler::ExtractCallRelation(pkb, node);

    for (auto child : node.GetChildren()) {
      queue.push_back(*child);
    }
  }
}

// call each individual design abstraction extractor function here
void DesignExtractor::ExtractDesigns(PKB& pkb, const source_processor::TNode& root) {
  StatementHandler::ExtractStmtNums(pkb, root);            // all stmt numbers in a program only needs to be extracted once
  ParentHandler::ExtractParentAndParentTStmts(pkb, root);  // parent handler has to be called BEFORE the BreadthFirstTraversal
  ModifiesHandler::ExtractModifiesSWithoutCallsStmts(pkb, root);
  CFGHandler::ConstructCFG(root);                // must be called after StatementHandler::ExtractStmtNums
  NextHandler::ExtractNextRelation(pkb, root);   // must be called after CFGHandler::ConstructCFG
  NextHandler::ExtractNextTRelation(pkb, root);  // must be called after CFGHandler::ConstructCFG

  BreadthFirstTraversal(pkb, root);

  CallHandler::ExtractCallTRelation(pkb, root);                      // ExtractCallTRelation must be called AFTER the CallHandler::ExtractCallRelation in BreadthFirstTraversal
  UsesHandler::ExtractUsesSCallsAndUsesP(pkb, root);                 // must be called AFTER ExtractCallTRelation() and BreadthFirstTraversal
  ModifiesHandler::ExtractModifiesSForCallsAndModifiesP(pkb, root);  // must be called after ModifiesHandler::ExtractModifiesSWithoutCallsStmts AND CallHandler::ExtractCallTRelation
  AffectsHandler::ExtractAffects(pkb, root);                         // must be called after UsesHandler::ExtractUsesSCallsAndUsesP, ModifiesHandler::ExtractModifiesSForCallsAndModifiesP and CFGHandler::ConstructCFG
  AffectsHandler::ExtractAffectsT(pkb, root);

  // Extensions:
  if (utils::Extension::HasNextBip) {
    CFGBipHandler::ConstructCFGBip(pkb);                           // must be called after CallHandler, NextHandler, CFGHandler, EntityHandler
    NextBipHandler::ExtractNextBipAndNextBipTRelation(pkb, root);  // must be called after CFGBipHandler
  }
  if (utils::Extension::HasAffectsBip) {
    GEHandler::ConstructGraphExplosion(pkb, root);     // must be called after CFGHandler and CallHandler
    AffectsBipHandler::ExtractAffectsBip(pkb, root);   // must be called after ConstructGraphExplosion
    AffectsBipHandler::ExtractAffectsBipT(pkb, root);  // must be called after ExtractAffectsBip
  }
}

};  // namespace design_extractor
