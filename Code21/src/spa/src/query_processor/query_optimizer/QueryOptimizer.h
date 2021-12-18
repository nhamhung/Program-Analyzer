#pragma once

#include <vector>

#include "query_processor/commons/query/Query.h"
#include "query_processor/commons/query/entities/SelectedEntity.h"
#include "query_processor/query_evaluator/ResultTable.h"

namespace query_processor {

class QueryOptimizer {
 public:
  static Query OptimizeQuery(Query&, bool remove_repeated_clauses, bool sort_clauses);
  static std::vector<ResultTable> OptimizeMerging(std::vector<ResultTable>&, bool sort_before_bfs, bool sort_before_merge);

 private:
  static std::vector<Clause> RemoveRepeatedClauses(std::vector<Clause>&);
  static std::vector<Clause> SortClausesByNumberOfSynonyms(std::vector<Clause>&);
  static ResultTable BFSMerge(int node, std::vector<std::unordered_set<int>> adj_list, std::vector<bool>& merged, std::vector<ResultTable>&, bool sort_before_merge);
};

}  // namespace query_processor
