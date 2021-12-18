#include "ModifiesTable.h"

#include <algorithm>

bool ModifiesTable::InsertStmtModifies(int stmt_index, const std::string& variable) {
  if (stmt_index <= 0 || variable == "") {
    return false;
  }

  return modifies_stmt_table.Insert(stmt_index, variable) && inverse_modifies_stmt_table.Insert(variable, stmt_index);
}

bool ModifiesTable::InsertProcModifies(const std::string& proc_name, const std::string& variable) {
  if (proc_name == "" || variable == "") {
    return false;
  }

  return modifies_proc_table.Insert(proc_name, variable) && inverse_modifies_proc_table.Insert(variable, proc_name);
}

bool ModifiesTable::IsStmtModifies(int stmt_index, const std::string& variable) {
  if (stmt_index <= 0 || variable == "") {
    return false;
  }
  if (!modifies_stmt_table.Contains(stmt_index)) {
    return false;
  }

  std::unordered_set<std::string>& stmt_variable_set = modifies_stmt_table.Get(stmt_index);
  return std::find(stmt_variable_set.begin(), stmt_variable_set.end(), variable) != stmt_variable_set.end();
}

bool ModifiesTable::IsProcModifies(const std::string& proc_name, const std::string& variable) {
  if (proc_name == "" || variable == "") {
    return false;
  }
  if (!modifies_proc_table.Contains(proc_name)) {
    return false;
  }

  std::unordered_set<std::string>& proc_variable_set = modifies_proc_table.Get(proc_name);
  return std::find(proc_variable_set.begin(), proc_variable_set.end(), variable) != proc_variable_set.end();
}

std::unordered_set<int> ModifiesTable::GetModifiesStatements(const std::string& variable) {
  if (!inverse_modifies_stmt_table.Contains(variable)) {
    return std::unordered_set<int>();
  }
  return inverse_modifies_stmt_table.Get(variable);
}

std::unordered_set<std::string> ModifiesTable::GetModifiesProcedures(const std::string& variable) {
  if (!inverse_modifies_proc_table.Contains(variable)) {
    return std::unordered_set<std::string>();
  }
  return inverse_modifies_proc_table.Get(variable);
}

std::unordered_set<std::string> ModifiesTable::GetModifiedStmtVariables(int stmt_index) {
  if (!modifies_stmt_table.Contains(stmt_index)) {
    return std::unordered_set<std::string>();
  }
  return modifies_stmt_table.Get(stmt_index);
}

std::unordered_set<std::string> ModifiesTable::GetModifiedProcVariables(const std::string& proc_name) {
  if (!modifies_proc_table.Contains(proc_name)) {
    return std::unordered_set<std::string>();
  }
  return modifies_proc_table.Get(proc_name);
}

std::unordered_set<int> ModifiesTable::GetAllModifiesStatements() {
  return modifies_stmt_table.GetAllKeys();
}

std::unordered_set<std::string> ModifiesTable::GetAllModifiesProcedures() {
  return modifies_proc_table.GetAllKeys();
}

void ModifiesTable::ClearModifiesTable() {
  modifies_stmt_table.ClearTable();
  modifies_proc_table.ClearTable();
  inverse_modifies_proc_table.ClearTable();
  inverse_modifies_stmt_table.ClearTable();
}

TableMultiple<int, std::string> ModifiesTable::GetModifiesStmtTable() {
  return modifies_stmt_table;
}

TableMultiple<int, std::string> ModifiesTable::GetModifiesProcTable() {
  return modifies_stmt_table;
}

TableMultiple<std::string, int> ModifiesTable::GetInverseModifiesStmtTable() {
  return inverse_modifies_stmt_table;
}

TableMultiple<std::string, std::string> ModifiesTable::GetInverseModifiesProcTable() {
  return inverse_modifies_proc_table;
}