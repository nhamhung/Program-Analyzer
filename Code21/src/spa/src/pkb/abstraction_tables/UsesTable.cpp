#include "UsesTable.h"

#include <algorithm>

bool UsesTable::InsertStmtUses(int stmt_index, const std::string& variable) {
  if (stmt_index <= 0 || variable == "") {
    return false;
  }

  return uses_stmt_table.Insert(stmt_index, variable) && inverse_uses_stmt_table.Insert(variable, stmt_index);
}

bool UsesTable::InsertProcUses(const std::string& proc_name, const std::string& variable) {
  if (proc_name == "" || variable == "") {
    return false;
  }

  return uses_proc_table.Insert(proc_name, variable) && inverse_uses_proc_table.Insert(variable, proc_name);
}

bool UsesTable::IsStmtUses(int stmt_index, const std::string& variable) {
  if (stmt_index <= 0 || variable == "") {
    return false;
  }
  if (!uses_stmt_table.Contains(stmt_index)) {
    return false;
  }

  std::unordered_set<std::string>& stmt_variable_set = uses_stmt_table.Get(stmt_index);
  return std::find(stmt_variable_set.begin(), stmt_variable_set.end(), variable) != stmt_variable_set.end();
}

bool UsesTable::IsProcUses(const std::string& proc_name, const std::string& variable) {
  if (proc_name == "" || variable == "") {
    return false;
  }
  if (!uses_proc_table.Contains(proc_name)) {
    return false;
  }

  std::unordered_set<std::string>& proc_variable_set = uses_proc_table.Get(proc_name);
  return std::find(proc_variable_set.begin(), proc_variable_set.end(), variable) != proc_variable_set.end();
}

std::unordered_set<std::string> UsesTable::GetUsedStmtVariables(int stmt_index) {
  if (!uses_stmt_table.Contains(stmt_index)) {
    return std::unordered_set<std::string>();
  }
  return uses_stmt_table.Get(stmt_index);
}

std::unordered_set<std::string> UsesTable::GetUsedProcVariables(const std::string& proc_name) {
  if (!uses_proc_table.Contains(proc_name)) {
    return std::unordered_set<std::string>();
  }
  return uses_proc_table.Get(proc_name);
}

std::unordered_set<int> UsesTable::GetUsesStatements(const std::string& variable) {
  if (!inverse_uses_stmt_table.Contains(variable)) {
    return std::unordered_set<int>();
  }
  return inverse_uses_stmt_table.Get(variable);
}

std::unordered_set<std::string> UsesTable::GetUsesProcedures(const std::string& variable) {
  if (!inverse_uses_proc_table.Contains(variable)) {
    return std::unordered_set<std::string>();
  }
  return inverse_uses_proc_table.Get(variable);
}

std::unordered_set<int> UsesTable::GetAllUsesStatements() {
  return uses_stmt_table.GetAllKeys();
}

std::unordered_set<std::string> UsesTable::GetAllUsesProcedures() {
  return uses_proc_table.GetAllKeys();
}

void UsesTable::ClearUsesTable() {
  uses_stmt_table.ClearTable();
  uses_proc_table.ClearTable();
  inverse_uses_stmt_table.ClearTable();
  inverse_uses_proc_table.ClearTable();
}

TableMultiple<int, std::string> UsesTable::GetUsesStmtTable() {
  return uses_stmt_table;
}

TableMultiple<std::string, std::string> UsesTable::GetUsesProcTable() {
  return uses_proc_table;
}

TableMultiple<std::string, int> UsesTable::GetInverseUsesStmtTable() {
  return inverse_uses_stmt_table;
}

TableMultiple<std::string, std::string> UsesTable::GetInverseUsesProcTable() {
  return inverse_uses_proc_table;
}
