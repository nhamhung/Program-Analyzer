#pragma once

#include <string>

#include "source_processor/ast/TNodeType.h"
#include "source_processor/token/TokenType.h"

namespace source_processor {

class TypeUtils {
 private:
  static const std::string tnode_type_map[];
  static const std::string token_type_map[];

 public:
  static const std::string& TNodeTypeToName(TNodeType);
  static const std::string& TokenTypeToName(TokenType);
};

}  // namespace source_processor
