#pragma once

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

typedef std::unordered_map<std::string, std::unordered_set<std::string>> graph;

namespace design_extractor {

class UsesHandler {
 private:
  static std::vector<std::string> GetVariableValues(const source_processor::TNode& node);
  static void ExtractUsesStmtsForParentT(PKB& pkb, std::unordered_set<int> parentT_list, std::string value);
  static void ExtractUsesStmtsForParentT(PKB& pkb, std::unordered_set<int> parentT_list, std::vector<std::string> value_list);

 public:
  static void ExtractUsesStmtsWithoutCalls(PKB& pkb, const source_processor::TNode& node);
  static void ExtractUsesSCallsAndUsesP(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor