#pragma once

#include <set>
#include <string>
#include <unordered_set>
#include <vector>

namespace query_processor {

enum class QueryResultType {
  STMTS,
  NAMES,
  BOOLEAN,
  TUPLE,
};

class QueryResult {
 public:
  std::unordered_set<int> statement_indexes_or_constants;
  std::unordered_set<std::string> var_proc_names;
  std::vector<std::vector<std::string>> tuple_result;
  bool boolean{};
  QueryResultType result_type;

  QueryResult() = default;
  ;
  explicit QueryResult(std::unordered_set<int>);
  explicit QueryResult(std::unordered_set<std::string>);
  explicit QueryResult(std::vector<std::vector<std::string>>);
  explicit QueryResult(bool);
};
}  // namespace query_processor
