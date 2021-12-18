
#include "QueryResult.h"

namespace query_processor {
QueryResult::QueryResult(std::unordered_set<int> s_idx) {
  this->statement_indexes_or_constants = s_idx;
  this->result_type = QueryResultType::STMTS;
}

QueryResult::QueryResult(std::unordered_set<std::string> v_p_b) {
  this->var_proc_names = v_p_b;
  this->result_type = QueryResultType::NAMES;
}

QueryResult::QueryResult(bool boolean) {
  this->boolean = boolean;
  this->result_type = QueryResultType::BOOLEAN;
}

QueryResult::QueryResult(std::vector<std::vector<std::string>> tuple_result) {
  this->tuple_result = tuple_result;
  this->result_type = QueryResultType::TUPLE;
}

}  // namespace query_processor