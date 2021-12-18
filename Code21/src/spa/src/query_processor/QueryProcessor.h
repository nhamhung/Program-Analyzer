#pragma once

#include <list>
#include <string>

#include "pkb/PKB.h"

namespace query_processor {

class QueryProcessor {
 public:
  QueryProcessor();
  static std::list<std::string> ProcessQuery(std::string, PKB&);
};

}  // namespace query_processor