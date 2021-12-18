#include "ResultTable.h"

#include <functional>
#include <map>
#include <stdexcept>

namespace query_processor {

ResultTable::ResultTable() = default;

ResultTable::ResultTable(std::unordered_set<std::string> synonyms) {
  for (const std::string& synonym : synonyms) {
    Column new_column;
    columns[synonym] = new_column;
  }
}

Column& ResultTable::GetColumn(const std::string& synonym) {
  if (!this->Contains(synonym)) {
    throw std::runtime_error("Synonym could not be found in the table.");
  }
  return columns[synonym];
}

Column& ResultTable::GetColumn(ClauseParam& clause_param) {
  switch (clause_param.param_type) {
    case ClauseParamType::DESIGN_ENTITY:
      return this->GetColumn(clause_param.design_entity.GetSynonym());
    case ClauseParamType::NAME:
      return this->GetColumn(clause_param.var_proc_name);
    case ClauseParamType::INDEX:
      return this->GetColumn(std::to_string(clause_param.statement_index));
    default:
      throw std::runtime_error("Invalid clause param to get column");
  }
}

bool ResultTable::IsEmpty() {
  return columns.empty();
}

int ResultTable::GetSize() {
  return columns.size();
}

int ResultTable::GetHeight() {
  // All columns are the same height. Loop just to access any column in the unordered_map.
  for (const auto& column : columns) {
    return column.second.size();
  }
  return 0;
}
bool ResultTable::Contains(const std::string& synonym) {
  return columns.find(synonym) != columns.end();
}

bool ResultTable::Contains(ClauseParam& clause) {
  if (clause.param_type != ClauseParamType::DESIGN_ENTITY) {
    return false;
  }
  if (clause.design_entity.GetDesignEntityType() == DesignEntityType::WILDCARD) {
    return false;
  }
  std::string synonym = clause.design_entity.GetSynonym();
  return this->Contains(synonym);
}

bool ResultTable::AddColumn(const std::string& synonym, Column& data) {
  if (!this->IsEmpty() && (data.size() != this->GetHeight())) {
    throw std::runtime_error("Column not the same height as rest of the table");
  }
  columns[synonym] = data;
  return true;
}

bool ResultTable::AddRow(const std::unordered_map<std::string, TableElement>& row_data) {
  if (row_data.size() != columns.size()) {
    throw std::runtime_error("Row size and table size does not match.");
  }
  for (auto& data : row_data) {
    if (!Contains(data.first)) {
      throw std::runtime_error("Row has a synonym not in the table.");
    } else {
      columns[data.first].push_back(data.second);
    }
  }
  return true;
}

Row ResultTable::GetRowAt(int index) {
  if (index >= this->GetHeight()) {
    throw std::runtime_error("Invalid out of index Row");
  }
  Row returned_row;
  for (auto& column : columns) {
    returned_row[column.first] = column.second.at(index);
  }
  return returned_row;
}

std::unordered_set<std::string> ResultTable::GetHeaders() {
  std::unordered_set<std::string> synonyms;
  for (const auto& it : columns) {
    synonyms.insert(it.first);
  }
  return synonyms;
}

void ResultTable::Clear() {
  columns.clear();
}

std::unordered_map<std::string, Column>& ResultTable::GetTable() {
  return this->columns;
}

ResultTable ResultTable::MergeTable(ResultTable& other) {
  std::vector<std::string> intersecting_synonyms = this->FindIntersectingHeaders(other);
  if (intersecting_synonyms.empty()) {
    return this->CrossTable(other);
  }
  if (intersecting_synonyms.size() == 1) {
    return InnerJoin(other, intersecting_synonyms.front());
  }
  return InnerJoin(other, intersecting_synonyms);
}

ResultTable ResultTable::MergeColumn(std::string header, Column& other_col) {
  ResultTable col_table;
  col_table.AddColumn(header, other_col);
  return this->MergeTable(col_table);
}
ResultTable ResultTable::CrossTable(ResultTable& other) {
  std::unordered_map<std::string, Column>& other_table = other.GetTable();

  if (this->IsEmpty()) {
    return other;
  }
  if (other.IsEmpty()) {
    return *this;
  }

  ResultTable new_table;
  int lhs_height = this->GetHeight();
  int rhs_height = other.GetHeight();
  int new_height = lhs_height * rhs_height;
  for (const auto& lhs_col : columns) {
    Column new_col;
    new_col.reserve(new_height);
    for (int i = 0; i < lhs_height; i++) {
      for (int j = 0; j < rhs_height; j++) {
        new_col.push_back(lhs_col.second.at(i));
      }
    }
    new_table.AddColumn(lhs_col.first, new_col);
  }

  for (const auto& rhs_col : other_table) {
    Column new_col;
    new_col.reserve(new_height);
    for (int i = 0; i < lhs_height; i++) {
      for (int j = 0; j < rhs_height; j++) {
        new_col.push_back(rhs_col.second.at(j));
      }
    }
    new_table.AddColumn(rhs_col.first, new_col);
  }
  return new_table;
}

ResultTable ResultTable::InnerJoin(ResultTable& other, std::string matching_synonym) {
  if (!this->Contains(matching_synonym) || !other.Contains(matching_synonym)) {
    throw std::runtime_error("Tables do not share matching synonym");
  }

  std::hash<std::string> hash_func;
  // Initialise result table with correct headers
  std::unordered_set<std::string> synonyms;
  for (const auto& it : columns) {
    synonyms.insert(it.first);
  }
  for (const auto& it : other.GetTable()) {
    synonyms.insert(it.first);
  }
  ResultTable result_table = ResultTable(synonyms);
  std::unordered_multimap<size_t, int> hash_map;

  // Hashing
  Column left_join_col = this->GetColumn(matching_synonym);
  for (int i = 0; i < this->GetHeight(); i++) {
    TableElement& elem = left_join_col.at(i);
    size_t key;
    if (elem.type == QueryResultType::STMTS) {
      key = elem.stmt;
    } else {
      key = hash_func(elem.name);  //returns std::size_t
    }
    hash_map.insert(std::make_pair(key, i));
  }

  // Matching
  Column right_join_col = other.GetColumn(matching_synonym);
  for (int i = 0; i < other.GetHeight(); i++) {
    TableElement& elem = right_join_col.at(i);
    size_t key;
    if (elem.type == QueryResultType::STMTS) {
      key = elem.stmt;
    } else {
      key = hash_func(elem.name);  //returns std::size_t
    }
    auto same_key = hash_map.equal_range(key);
    for (auto it = same_key.first; it != same_key.second; ++it) {
      Row left_row = this->GetRowAt(it->second);
      Row right_row = other.GetRowAt(i);
      Row merged_row = left_row;
      merged_row.insert(right_row.begin(), right_row.end());
      result_table.AddRow(merged_row);
    }
  }
  return result_table;
}
ResultTable ResultTable::InnerJoin(ResultTable& other, const std::vector<std::string>& intersecting_synonyms) {
  for (const auto& matching_synonym : intersecting_synonyms) {
    if (!this->Contains(matching_synonym) || !other.Contains(matching_synonym)) {
      throw std::runtime_error("Tables do not share matching synonym");
    }
  }

  std::hash<std::string> hash_func;
  // Initialise result table with correct headers
  std::unordered_set<std::string> synonyms;
  for (const auto& it : columns) {
    synonyms.insert(it.first);
  }
  for (const auto& it : other.GetTable()) {
    synonyms.insert(it.first);
  }
  ResultTable result_table = ResultTable(synonyms);
  std::multimap<std::vector<size_t>, int> hash_map;

  // Hashing
  std::vector<Column> left_join_cols;
  left_join_cols.reserve(intersecting_synonyms.size());
  for (const auto& synonym : intersecting_synonyms) {
    left_join_cols.push_back(this->GetColumn(synonym));
  }
  for (int i = 0; i < this->GetHeight(); i++) {
    std::vector<size_t> row_of_elem;
    for (auto& column : left_join_cols) {
      TableElement& elem = column.at(i);
      if (elem.type == QueryResultType::STMTS) {
        row_of_elem.push_back(elem.stmt);
      } else {
        row_of_elem.push_back(hash_func(elem.name));
      }
    }
    hash_map.insert(std::make_pair(row_of_elem, i));
  }

  // Matching
  std::vector<Column> right_join_cols;
  right_join_cols.reserve(intersecting_synonyms.size());
  for (const auto& synonym : intersecting_synonyms) {
    right_join_cols.push_back(other.GetColumn(synonym));
  }
  for (int i = 0; i < other.GetHeight(); i++) {
    std::vector<size_t> row_of_elem;
    for (auto& column : right_join_cols) {
      TableElement& elem = column.at(i);
      if (elem.type == QueryResultType::STMTS) {
        row_of_elem.push_back(elem.stmt);
      } else {
        row_of_elem.push_back(hash_func(elem.name));
      }
    }

    auto same_key = hash_map.equal_range(row_of_elem);
    for (auto it = same_key.first; it != same_key.second; ++it) {
      Row left_row = this->GetRowAt(it->second);
      Row right_row = other.GetRowAt(i);
      Row merged_row = left_row;
      merged_row.insert(right_row.begin(), right_row.end());
      result_table.AddRow(merged_row);
    }
  }
  return result_table;
}

std::vector<std::string> ResultTable::FindIntersectingHeaders(ResultTable& other) {
  std::vector<std::string> intersecting_headers;
  std::unordered_set<std::string> this_headers = this->GetHeaders();
  for (const auto& synonym : this_headers) {
    if (other.Contains(synonym)) {
      intersecting_headers.push_back(synonym);
    }
  }
  return intersecting_headers;
}
}  // namespace query_processor