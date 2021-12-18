#pragma once

#include <list>
#include <string>

#include "pkb/PKB.h"

class SPA {
 public:
  static void ParseSourceCode(const std::string&, PKB&);
  static void HandleQueries(const std::string&, std::list<std::string>&, PKB&);
};
