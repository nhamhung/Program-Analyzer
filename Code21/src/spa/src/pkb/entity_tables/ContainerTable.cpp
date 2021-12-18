#include "ContainerTable.h"

bool ContainerTable::InsertIf(int stmt_index, const std::vector<std::string>& variable_list) {
  if (stmt_index <= 0) {
    return false;
  }

  if (variable_list.empty()) {
    return if_table.Insert(stmt_index, "");
  }

  for (const auto& variable : variable_list) {
    if_table.Insert(stmt_index, variable);
    inverse_if_table.Insert(variable, stmt_index);
  }

  return true;
}

std::unordered_set<std::string> ContainerTable::GetVariablesUsedByIfStmt(int stmt_index) {
  if (stmt_index <= 0 || !if_table.Contains(stmt_index)) {
    return std::unordered_set<std::string>();
  }

  return if_table.Get(stmt_index);
}

std::unordered_set<int> ContainerTable::GetIfStmts(const std::string& variable) {
  if (variable == "" || !inverse_if_table.Contains(variable)) {
    return std::unordered_set<int>();
  }

  return inverse_if_table.Get(variable);
}

std::unordered_set<int> ContainerTable::GetAllIfStmts() {
  return if_table.GetAllKeys();
}

std::unordered_set<std::string> ContainerTable::GetAllVariablesUsedByIf() {
  return inverse_if_table.GetAllKeys();
}

TableMultiple<int, std::string> ContainerTable::GetIfTable() {
  return if_table;
}

TableMultiple<std::string, int> ContainerTable::GetInverseIfTable() {
  return inverse_if_table;
}

bool ContainerTable::InsertWhile(int stmt_index, const std::vector<std::string>& variable_list) {
  if (stmt_index <= 0) {
    return false;
  }

  if (variable_list.empty()) {
    return while_table.Insert(stmt_index, "");
  }

  for (const auto& variable : variable_list) {
    while_table.Insert(stmt_index, variable);
    inverse_while_table.Insert(variable, stmt_index);
  }

  return true;
}

std::unordered_set<std::string> ContainerTable::GetVariablesUsedByWhileStmt(int stmt_index) {
  if (stmt_index <= 0 || !while_table.Contains(stmt_index)) {
    return std::unordered_set<std::string>();
  }

  return while_table.Get(stmt_index);
}

std::unordered_set<int> ContainerTable::GetWhileStmts(const std::string& variable) {
  if (variable == "" || !inverse_while_table.Contains(variable)) {
    return std::unordered_set<int>();
  }

  return inverse_while_table.Get(variable);
}

std::unordered_set<int> ContainerTable::GetAllWhileStmts() {
  return while_table.GetAllKeys();
}

std::unordered_set<std::string> ContainerTable::GetAllVariablesUsedByWhile() {
  return inverse_while_table.GetAllKeys();
}

TableMultiple<int, std::string> ContainerTable::GetWhileTable() {
  return while_table;
}

TableMultiple<std::string, int> ContainerTable::GetInverseWhileTable() {
  return inverse_while_table;
}

void ContainerTable::ClearTable() {
  if_table.ClearTable();
  inverse_if_table.ClearTable();
  while_table.ClearTable();
  inverse_while_table.ClearTable();
}
