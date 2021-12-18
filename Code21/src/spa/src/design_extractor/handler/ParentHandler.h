#pragma once

#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class ParentHandler {
 private:
  static std::vector<std::pair<int, int>> parent_entries;   // each entry is a <stmt no, stmt no> pair
  static std::vector<std::pair<int, int>> parentT_entries;  // each entry is a <stmt no, stmt no> pair
  static void PopulateParentAndParentT(PKB& pkb);
  static void ExtractParentAndParentTHelper(PKB& pkb, const source_processor::TNode& node);

 public:
  static void ExtractParentAndParentTStmts(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor