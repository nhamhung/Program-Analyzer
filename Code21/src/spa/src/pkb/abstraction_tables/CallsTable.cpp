#include "CallsTable.h"

#include <algorithm>

bool CallsTable::InsertCalls(const std::string& caller, const std::string& callee) {
  if (caller == "" || callee == "") {
    return false;
  }

  return calls_table.Insert(caller, callee) && inverse_calls_table.Insert(callee, caller);
}

bool CallsTable::IsCalls(const std::string& caller, const std::string& callee) {
  if (caller == "" || callee == "") {
    return false;
  }

  if (!calls_table.Contains(caller)) {
    return false;
  }

  std::unordered_set<std::string>& callee_set = calls_table.Get(caller);
  return std::find(callee_set.begin(), callee_set.end(), callee) != callee_set.end();
}

std::unordered_set<std::string> CallsTable::GetProceduresThatCalls(const std::string& callee) {
  if (callee == "" || !inverse_calls_table.Contains(callee)) {
    return std::unordered_set<std::string>();
  }

  return inverse_calls_table.Get(callee);
}

std::unordered_set<std::string> CallsTable::GetProceduresCalled(const std::string& caller) {
  if (caller == "" || !calls_table.Contains(caller)) {
    return std::unordered_set<std::string>();
  }

  return calls_table.Get(caller);
}

std::unordered_set<std::string> CallsTable::GetAllCallsProcedures() {
  return calls_table.GetAllKeys();
}

std::unordered_set<std::string> CallsTable::GetAllCalledProcedures() {
  return inverse_calls_table.GetAllKeys();
}

TableMultiple<std::string, std::string> CallsTable::GetCallsTable() {
  return calls_table;
}

TableMultiple<std::string, std::string> CallsTable::GetInverseCallsTable() {
  return inverse_calls_table;
}

bool CallsTable::InsertCallsT(const std::string& caller, const std::string& callee) {
  if (caller == "" || callee == "") {
    return false;
  }

  return calls_T_table.Insert(caller, callee) && inverse_calls_T_table.Insert(callee, caller);
}

bool CallsTable::IsCallsT(const std::string& caller, const std::string& callee) {
  if (caller == "" || callee == "") {
    return false;
  }

  if (!calls_T_table.Contains(caller)) {
    return false;
  }

  std::unordered_set<std::string>& callee_set = calls_T_table.Get(caller);
  return std::find(callee_set.begin(), callee_set.end(), callee) != callee_set.end();
}

std::unordered_set<std::string> CallsTable::GetProceduresThatCallsT(const std::string& callee) {
  if (callee == "" || !inverse_calls_T_table.Contains(callee)) {
    return std::unordered_set<std::string>();
  }

  return inverse_calls_T_table.Get(callee);
}

std::unordered_set<std::string> CallsTable::GetProceduresCalledT(const std::string& caller) {
  if (caller == "" || !calls_T_table.Contains(caller)) {
    return std::unordered_set<std::string>();
  }

  return calls_T_table.Get(caller);
}

std::unordered_set<std::string> CallsTable::GetAllCallsTProcedures() {
  return calls_T_table.GetAllKeys();
}

std::unordered_set<std::string> CallsTable::GetAllCalledTProcedures() {
  return inverse_calls_T_table.GetAllKeys();
}

TableMultiple<std::string, std::string> CallsTable::GetCallsTTable() {
  return calls_T_table;
}

TableMultiple<std::string, std::string> CallsTable::GetInverseCallsTTable() {
  return inverse_calls_T_table;
}

bool CallsTable::InsertCalls(int stmt_index, const std::string& proc_name) {
  if (stmt_index <= 0 || proc_name == "") {
    return false;
  }

  return calls_stmt_table.Insert(stmt_index, proc_name) && inverse_calls_stmt_table.Insert(proc_name, stmt_index);
}

std::string CallsTable::GetCalledProcedure(int stmt_index) {
  if (stmt_index <= 0 || !calls_stmt_table.Contains(stmt_index)) {
    return "";
  }

  return calls_stmt_table.Get(stmt_index);
}

std::unordered_set<int> CallsTable::GetCallsStmtIndexes(const std::string& proc_name) {
  if (proc_name == "" || !inverse_calls_stmt_table.Contains(proc_name)) {
    return std::unordered_set<int>();
  }

  return inverse_calls_stmt_table.Get(proc_name);
}

std::unordered_set<int> CallsTable::GetAllCallsStmts() {
  return calls_stmt_table.GetAllKeys();
}

TableSingle<int, std::string> CallsTable::GetCallsStmtTable() {
  return calls_stmt_table;
}

TableMultiple<std::string, int> CallsTable::GetInverseCallsStmtTable() {
  return inverse_calls_stmt_table;
}

void CallsTable::ClearCallsTable() {
  calls_table.ClearTable();
  inverse_calls_table.ClearTable();
  calls_T_table.ClearTable();
  inverse_calls_T_table.ClearTable();
  calls_stmt_table.ClearTable();
  inverse_calls_stmt_table.ClearTable();
}
