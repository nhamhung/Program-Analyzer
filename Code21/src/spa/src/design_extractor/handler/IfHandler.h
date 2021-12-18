#pragma once

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class IfHandler {
 private:
  static std::vector<std::string> GetVariableValues(const source_processor::TNode& node);

 public:
  static void ExtractIfStmts(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor