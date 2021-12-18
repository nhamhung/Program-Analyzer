#include "NextBipTable.h"

#include <algorithm>

bool NextBipTable::InsertNextBip(int stmt1, int stmt2) {
  if (stmt1 == stmt2 || stmt1 <= 0 || stmt2 <= 0) {
    return false;
  }
  return nextbip_table.Insert(stmt1, stmt2) && inverse_nextbip_table.Insert(stmt2, stmt1);
}

bool NextBipTable::IsNextBip(int stmt1, int stmt2) {
  if (stmt1 == stmt2 || stmt1 <= 0 || stmt2 <= 0) {
    return false;
  }
  if (!nextbip_table.Contains(stmt1)) {
    return false;
  }

  std::unordered_set<int>& stmt1_next_set = nextbip_table.Get(stmt1);
  return std::find(stmt1_next_set.begin(), stmt1_next_set.end(), stmt2) != stmt1_next_set.end();
}

std::unordered_set<int> NextBipTable::GetNextBipStatements(int stmt_index) {
  if (stmt_index <= 0 || !nextbip_table.Contains(stmt_index)) {
    return std::unordered_set<int>();
  }
  return nextbip_table.Get(stmt_index);
}

std::unordered_set<int> NextBipTable::GetPreviousBipStatements(int stmt2) {
  if (stmt2 <= 0 || !inverse_nextbip_table.Contains(stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_nextbip_table.Get(stmt2);
}

std::unordered_set<int> NextBipTable::GetAllNextBipStatements() {
  return inverse_nextbip_table.GetAllKeys();
}

std::unordered_set<int> NextBipTable::GetAllPreviousBipStatements() {
  return nextbip_table.GetAllKeys();
}

TableMultiple<int, int> NextBipTable::GetNextBipTable() {
  return nextbip_table;
}

TableMultiple<int, int> NextBipTable::GetInverseNextBipTable() {
  return inverse_nextbip_table;
}

bool NextBipTable::InsertNextBipT(int stmt1, int stmt2) {
  if (stmt1 <= 0 || stmt2 <= 0) {
    return false;
  }
  return nextbip_T_table.Insert(stmt1, stmt2) && inverse_nextbip_T_table.Insert(stmt2, stmt1);
}

bool NextBipTable::IsNextBipT(int stmt1, int stmt2) {
  if (stmt1 <= 0 || stmt2 <= 0) {
    return false;
  }
  if (!nextbip_T_table.Contains(stmt1)) {
    return false;
  }

  std::unordered_set<int>& stmt1_next_T_set = nextbip_T_table.Get(stmt1);
  return std::find(stmt1_next_T_set.begin(), stmt1_next_T_set.end(), stmt2) != stmt1_next_T_set.end();
}

std::unordered_set<int> NextBipTable::GetNextBipTStatements(int stmt_index) {
  if (stmt_index <= 0 || !nextbip_T_table.Contains(stmt_index)) {
    return std::unordered_set<int>();
  }
  return nextbip_T_table.Get(stmt_index);
}

std::unordered_set<int> NextBipTable::GetPreviousBipTStatements(int stmt2) {
  if (stmt2 <= 0 || !inverse_nextbip_T_table.Contains(stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_nextbip_T_table.Get(stmt2);
}

std::unordered_set<int> NextBipTable::GetAllNextBipTStatements() {
  return inverse_nextbip_T_table.GetAllKeys();
}

std::unordered_set<int> NextBipTable::GetAllPreviousBipTStatements() {
  return nextbip_T_table.GetAllKeys();
}

TableMultiple<int, int> NextBipTable::GetNextBipTTable() {
  return nextbip_T_table;
}

TableMultiple<int, int> NextBipTable::GetInverseNextBipTTable() {
  return inverse_nextbip_T_table;
}

void NextBipTable::ClearNextBipTable() {
  nextbip_table.ClearTable();
  inverse_nextbip_table.ClearTable();
  nextbip_T_table.ClearTable();
  inverse_nextbip_T_table.ClearTable();
}