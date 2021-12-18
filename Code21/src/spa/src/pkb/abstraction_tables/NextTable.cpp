#include "NextTable.h"

#include <algorithm>

bool NextTable::InsertNext(int stmt1, int stmt2) {
  if (stmt1 == stmt2 || stmt1 <= 0 || stmt2 <= 0) {
    return false;
  }
  return next_table.Insert(stmt1, stmt2) && inverse_next_table.Insert(stmt2, stmt1);
}

bool NextTable::IsNext(int stmt1, int stmt2) {
  if (stmt1 == stmt2 || stmt1 <= 0 || stmt2 <= 0) {
    return false;
  }
  if (!next_table.Contains(stmt1)) {
    return false;
  }

  std::unordered_set<int>& stmt1_next_set = next_table.Get(stmt1);
  return std::find(stmt1_next_set.begin(), stmt1_next_set.end(), stmt2) != stmt1_next_set.end();
}

std::unordered_set<int> NextTable::GetNextStatements(int stmt_index) {
  if (stmt_index <= 0 || !next_table.Contains(stmt_index)) {
    return std::unordered_set<int>();
  }
  return next_table.Get(stmt_index);
}

std::unordered_set<int> NextTable::GetPreviousStatements(int stmt2) {
  if (stmt2 <= 0 || !inverse_next_table.Contains(stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_next_table.Get(stmt2);
}

std::unordered_set<int> NextTable::GetAllNextStatements() {
  return inverse_next_table.GetAllKeys();
}

std::unordered_set<int> NextTable::GetAllPreviousStatements() {
  return next_table.GetAllKeys();
}

TableMultiple<int, int> NextTable::GetNextTable() {
  return next_table;
}

TableMultiple<int, int> NextTable::GetInverseNextTable() {
  return inverse_next_table;
}

bool NextTable::InsertNextT(int stmt1, int stmt2) {
  if (stmt1 <= 0 || stmt2 <= 0) {
    return false;
  }
  return next_T_table.Insert(stmt1, stmt2) && inverse_next_T_table.Insert(stmt2, stmt1);
}

bool NextTable::IsNextT(int stmt1, int stmt2) {
  if (stmt1 <= 0 || stmt2 <= 0) {
    return false;
  }
  if (!next_T_table.Contains(stmt1)) {
    return false;
  }

  std::unordered_set<int>& stmt1_next_T_set = next_T_table.Get(stmt1);
  return std::find(stmt1_next_T_set.begin(), stmt1_next_T_set.end(), stmt2) != stmt1_next_T_set.end();
}

std::unordered_set<int> NextTable::GetNextTStatements(int stmt_index) {
  if (stmt_index <= 0 || !next_T_table.Contains(stmt_index)) {
    return std::unordered_set<int>();
  }
  return next_T_table.Get(stmt_index);
}

std::unordered_set<int> NextTable::GetPreviousTStatements(int stmt2) {
  if (stmt2 <= 0 || !inverse_next_T_table.Contains(stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_next_T_table.Get(stmt2);
}

std::unordered_set<int> NextTable::GetAllNextTStatements() {
  return inverse_next_T_table.GetAllKeys();
}

std::unordered_set<int> NextTable::GetAllPreviousTStatements() {
  return next_T_table.GetAllKeys();
}

TableMultiple<int, int> NextTable::GetNextTTable() {
  return next_T_table;
}

TableMultiple<int, int> NextTable::GetInverseNextTTable() {
  return inverse_next_T_table;
}

void NextTable::ClearNextTable() {
  next_table.ClearTable();
  inverse_next_table.ClearTable();
  next_T_table.ClearTable();
  inverse_next_T_table.ClearTable();
}