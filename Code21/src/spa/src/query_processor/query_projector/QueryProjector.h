#pragma once

#include <list>
#include <string>
#include <unordered_set>

#include "query_processor/commons/query_result/QueryResult.h"
#include "query_processor/query_evaluator/QueryEvaluator.h"

namespace query_processor {
class QueryProjector {
 public:
  static std::list<std::string> FormatResult(QueryResult);

 private:
  template <typename T>
  static std::list<std::string> ConstructString(std::unordered_set<T>);
  static std::list<std::string> ConstructString(bool);
  static std::list<std::string> ConstructString(std::vector<std::vector<std::string>>&);
  template <typename T>
  static std::string ToString(T);
};
}  // namespace query_processor