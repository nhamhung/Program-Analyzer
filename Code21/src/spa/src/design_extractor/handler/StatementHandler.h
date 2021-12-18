#pragma once

#include <unordered_set>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class StatementHandler {
 private:
  static int max_stmt_num;
  static std::unordered_set<int> statement_entries;  // each entry is an int from 1 to max stmt num

 public:
  static void ResetMaxStmtNum();  // resets the static max_stmt_num to INT_MIN
  static int GetMaxStmtNum(const source_processor::TNode& root);
  static void ExtractStmtNums(PKB& pkb, const source_processor::TNode& root);
};

}  // namespace design_extractor