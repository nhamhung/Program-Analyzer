#include "AssignTable.h"

bool AssignTable::InsertAssign(int stmt_index, const std::string& assigned_var, const source_processor::TokenList& token_list) {
  if (stmt_index <= 0) {
    return false;
  }

  if (assigned_table.Contains(stmt_index)) {
    return false;
  }

  if (assign_table.Contains(stmt_index)) {
    return false;
  }

  return assigned_table.Insert(stmt_index, assigned_var) && inverse_assigned_table.Insert(assigned_var, stmt_index) && assign_table.Insert(stmt_index, token_list);
}

std::unordered_set<int> AssignTable::GetAllAssignStmts() {
  return assigned_table.GetAllKeys();
}

std::unordered_set<std::string> AssignTable::GetAllAssignedVariables() {
  return inverse_assigned_table.GetAllKeys();
}

std::string AssignTable::GetAssignedVariable(int stmt_index) {
  if (!assigned_table.Contains(stmt_index)) {
    return "";
  }
  return assigned_table.Get(stmt_index);
}

std::unordered_set<int> AssignTable::GetAllAssignStmtsThatModifies(std::string variable) {
  if (!inverse_assigned_table.Contains(variable)) {
    return std::unordered_set<int>();
  }
  std::unordered_set<int> all_assign_statements_that_modifies;
  for (int stmt_index : inverse_assigned_table.Get(variable)) {
    all_assign_statements_that_modifies.insert(stmt_index);
  }
  return all_assign_statements_that_modifies;
}

std::unordered_set<int> AssignTable::GetAllAssignStmtsThatMatches(const source_processor::TokenList& token_list) {
  std::unordered_set<int> all_assign_stmts_that_matches;
  std::unordered_map<int, source_processor::TokenList>& assign_map = assign_table.GetTable();
  for (auto& it : assign_map) {
    if (it.second == token_list) {
      all_assign_stmts_that_matches.insert(it.first);
    }
  }
  return all_assign_stmts_that_matches;
}

std::unordered_set<int> AssignTable::GetAllAssignStmtsThatContains(const source_processor::TokenList& token_list) {
  std::unordered_set<int> all_assign_stmts_that_contains;
  std::unordered_map<int, source_processor::TokenList>& assign_map = assign_table.GetTable();
  for (auto& it : assign_map) {
    if (it.second.HasSublist(token_list)) {
      all_assign_stmts_that_contains.insert(it.first);
    }
  }
  return all_assign_stmts_that_contains;
}

TableSingle<int, std::string> AssignTable::GetAssignedTable() {
  return assigned_table;
}

TableSingle<int, source_processor::TokenList> AssignTable::GetAssignTable() {
  return assign_table;
}

TableMultiple<std::string, int> AssignTable::GetInverseAssignedTable() {
  return inverse_assigned_table;
}

void AssignTable::ClearAssignTable() {
  assign_table.ClearTable();
  assigned_table.ClearTable();
  inverse_assigned_table.ClearTable();
}