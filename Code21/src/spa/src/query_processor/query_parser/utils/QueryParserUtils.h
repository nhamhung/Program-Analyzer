#pragma once

#include <regex>
#include <string>
#include <vector>

#include "query_processor/commons/query/Query.h"

namespace query_processor {
class QueryParserUtils {
 public:

  static std::string& StripWhitespace(std::string&);
  static std::vector<std::string> SplitStringAlongChar(std::string&, char);
  static std::vector<std::string> GetAllMatchesFromString(std::string&, const std::regex&);
  static std::pair<std::string, std::string> PopMatchFromFront(std::string&, const std::regex&);

};
}  // namespace query_processor