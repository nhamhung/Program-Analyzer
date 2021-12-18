#include "QueryOptimizer.h"

#include <algorithm>
#include <unordered_map>
#include <utility>

namespace query_processor {
Query QueryOptimizer::OptimizeQuery(Query& query, bool remove_repeated_clauses = false, bool sort_clauses = false) {
  if (remove_repeated_clauses) {
    std::vector<Clause>& clause_list = query.GetClauseList();
    query.SetClauseList(RemoveRepeatedClauses(clause_list));
  }
  if (sort_clauses) {
    std::vector<Clause>& clause_list = query.GetClauseList();
    query.SetClauseList(SortClausesByNumberOfSynonyms(clause_list));
  }
  return query;
}

std::vector<ResultTable> QueryOptimizer::OptimizeMerging(std::vector<ResultTable>& database, bool sort_before_bfs, bool sort_before_merge) {
  int num_of_nodes = database.size();

  // Create adjacency list based on common synonyms
  std::vector<std::unordered_set<int>> adj_list;
  for (int i = 0; i < num_of_nodes; i++) {
    adj_list.push_back(std::unordered_set<int>{});
  }

  std::unordered_map<std::string, std::unordered_set<int>> distribution;
  for (int i = 0; i < num_of_nodes; i++) {
    ResultTable& table = database.at(i);
    std::unordered_set<std::string> synonyms = table.GetHeaders();
    for (auto& synonym : synonyms) {
      if (distribution.find(synonym) != distribution.end()) {
        for (int node : distribution[synonym]) {
          adj_list.at(node).insert(i);
          adj_list.at(i).insert(node);
        }
      }
      distribution[synonym].insert(i);
    }
  }

  std::vector<ResultTable> merged_tables;

  // BFS
  std::vector<bool> merged(num_of_nodes);
  for (int i = 0; i < num_of_nodes; i++) {
    merged.at(i) = false;
  }

  // Optimization where all tables are sorted before performing BFS
  // BFS then starts at the node with the smallest table size
  if (sort_before_bfs) {
    // Store as pairs of <size, int>
    std::vector<std::pair<int, int>> database_sizes;
    for (int i = 0; i < num_of_nodes; i++) {
      database_sizes.push_back(std::make_pair(database.at(i).GetHeight(), i));
    }

    sort(database_sizes.begin(), database_sizes.end());

    for (auto table_info : database_sizes) {
      int index = table_info.second;
      if (!merged.at(index)) {
        merged_tables.push_back(BFSMerge(index, adj_list, merged, database, sort_before_merge));
      }
    }
  } else {
    for (int i = 0; i < num_of_nodes; i++) {
      if (!merged.at(i)) {
        merged_tables.push_back(BFSMerge(i, adj_list, merged, database, sort_before_merge));
      }
    }
  }

  return merged_tables;
}

ResultTable QueryOptimizer::BFSMerge(int node, std::vector<std::unordered_set<int>> adj_list, std::vector<bool>& merged, std::vector<ResultTable>& database, bool sort_before_merge) {
  ResultTable group_table;
  std::vector<int> queue;
  group_table = group_table.MergeTable(database.at(node));
  merged.at(node) = true;
  queue.push_back(node);
  while (!queue.empty()) {
    node = queue.back();
    queue.pop_back();
    std::unordered_set<int>& node_adj = adj_list.at(node);
    if (sort_before_merge) {
      // Neighbours are pairs stored by <size, index>
      std::vector<std::pair<int, int>> neighbours;
      for (int neighbour : node_adj) {
        if (!merged.at(neighbour)) {
          neighbours.push_back(std::make_pair(database.at(neighbour).GetHeight(), neighbour));
        }
      }

      sort(neighbours.begin(), neighbours.end());

      for (auto sorted_neighbour : neighbours) {
        ResultTable& neighbour_table = database.at(sorted_neighbour.second);
        if (group_table.GetHeight() < sorted_neighbour.first) {
          group_table = group_table.MergeTable(neighbour_table);
        } else {
          group_table = neighbour_table.MergeTable(group_table);
        }
        merged.at(sorted_neighbour.second) = true;
        queue.push_back(sorted_neighbour.second);
      }
    } else {
      for (int neighbour : node_adj) {
        if (!merged.at(neighbour)) {
          ResultTable& neighbour_table = database.at(neighbour);
          if (group_table.GetHeight() < neighbour_table.GetHeight()) {
            group_table = group_table.MergeTable(neighbour_table);
          } else {
            group_table = neighbour_table.MergeTable(group_table);
          }
          merged.at(neighbour) = true;
          queue.push_back(neighbour);
        }
      }
    }
  }
  return group_table;
}

std::vector<Clause> QueryOptimizer::RemoveRepeatedClauses(std::vector<Clause>& clause_list) {
  std::vector<Clause> set_clause_list;
  for (auto& clause : clause_list) {
    if (std::find(set_clause_list.begin(), set_clause_list.end(), clause) == set_clause_list.end()) {
      set_clause_list.push_back(clause);
    }
  }

  return set_clause_list;
}

std::vector<Clause> QueryOptimizer::SortClausesByNumberOfSynonyms(std::vector<Clause>& clause_list) {
  std::sort(clause_list.begin(), clause_list.end());
  return clause_list;
}
}  // namespace query_processor