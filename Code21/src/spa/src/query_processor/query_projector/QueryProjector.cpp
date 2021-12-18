#include "QueryProjector.h"

#include <sstream>

namespace query_processor {
std::list<std::string> QueryProjector::FormatResult(QueryResult raw_result) {
  std::list<std::string> result_list;
  if (raw_result.result_type == QueryResultType::STMTS && !raw_result.statement_indexes_or_constants.empty()) {
    result_list = QueryProjector::ConstructString<int>(raw_result.statement_indexes_or_constants);
    return result_list;
  } else if (raw_result.result_type == QueryResultType::NAMES && !raw_result.var_proc_names.empty()) {
    result_list = QueryProjector::ConstructString<std::string>(raw_result.var_proc_names);
    return result_list;
  } else if (raw_result.result_type == QueryResultType::BOOLEAN) {
    result_list = QueryProjector::ConstructString(raw_result.boolean);
    return result_list;
  } else if (raw_result.result_type == QueryResultType::TUPLE && !raw_result.tuple_result.empty()) {
    result_list = QueryProjector::ConstructString(raw_result.tuple_result);
    return result_list;
  } else {
    return result_list;
  }
}

template <typename T>
std::list<std::string> QueryProjector::ConstructString(std::unordered_set<T> result_set) {
  std::list<std::string> result_list;
  typename std::unordered_set<T>::iterator iter;
  for (iter = result_set.begin(); iter != result_set.end(); iter++) {
    result_list.push_back(ToString(*iter));
  }
  return result_list;
}

std::list<std::string> QueryProjector::ConstructString(std::vector<std::vector<std::string>>& result_set) {
  std::list<std::string> result_list;
  for (const auto& tuple : result_set) {
    std::stringstream ss;
    for (const auto& var : tuple) {
      ss << var << ' ';
    }
    std::string tuple_string = ss.str();
    tuple_string.pop_back();
    result_list.push_back(tuple_string);
  }
  return result_list;
}

std::list<std::string> QueryProjector::ConstructString(bool boolean_result) {
  std::list<std::string> result_list;
  if (boolean_result) {
    result_list.push_back("TRUE");
  } else {
    result_list.push_back("FALSE");
  }
  return result_list;
}

template <typename T>
std::string QueryProjector::ToString(T t) {
  std::stringstream s;
  s << t;
  return s.str();
}
}  // namespace query_processor