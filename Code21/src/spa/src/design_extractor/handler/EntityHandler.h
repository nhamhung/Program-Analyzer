#pragma once

#include <string>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

class EntityHandler {
 public:
  static const std::string kprint_string;
  static const std::string kread_string;
  static const std::string kwhile_string;
  static const std::string kif_string;
  static const std::string kassign_string;
  static const std::string kcall_string;

  static void ExtractStmtEntityType(PKB& pkb, const source_processor::TNode& node);
};

}  // namespace design_extractor