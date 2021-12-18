#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "design_extractor/utils/DeUtils.h"
#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class CallHandler {
 private:
  //the key is a procedure that calls all the procedures in the value vector
  static std::unordered_map<std::string, std::unordered_set<std::string>> call_graph;

  static void GetDescendantsOf(std::string caller, std::unordered_set<std::string>& descendants);

 public:
  static void ExtractCallStmts(PKB& pkb, const source_processor::TNode& node);
  static void ExtractCallRelation(PKB& pkb, const source_processor::TNode& node);
  static void ExtractCallTRelation(PKB& pkb, const source_processor::TNode& node);
  static graph GetInverseCallGraph(PKB& pkb);
};

}  // namespace design_extractor