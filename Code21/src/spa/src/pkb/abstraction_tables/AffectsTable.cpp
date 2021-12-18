#include "AffectsTable.h"

#include <algorithm>

bool AffectsTable::InsertAffects(int assign_stmt1, int assign_stmt2) {
  if (assign_stmt1 <= 0 || assign_stmt2 <= 0) {
    return false;
  }

  return affects_table.Insert(assign_stmt1, assign_stmt2) && inverse_affects_table.Insert(assign_stmt2, assign_stmt1);
}

bool AffectsTable::IsAffects(int assign_stmt1, int assign_stmt2) {
  if (assign_stmt1 <= 0 || assign_stmt2 <= 0) {
    return false;
  }
  if (!affects_table.Contains(assign_stmt1)) {
    return false;
  }

  std::unordered_set<int>& assign_stmt1_affects_set = affects_table.Get(assign_stmt1);
  return std::find(assign_stmt1_affects_set.begin(), assign_stmt1_affects_set.end(), assign_stmt2) != assign_stmt1_affects_set.end();
}

std::unordered_set<int> AffectsTable::GetAffectedStatements(int assign_stmt1) {
  if (assign_stmt1 <= 0 || !affects_table.Contains(assign_stmt1)) {
    return std::unordered_set<int>();
  }
  return affects_table.Get(assign_stmt1);
}

std::unordered_set<int> AffectsTable::GetStatementsThatAffects(int assign_stmt2) {
  if (assign_stmt2 <= 0 || !inverse_affects_table.Contains(assign_stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_affects_table.Get(assign_stmt2);
}

std::unordered_set<int> AffectsTable::GetAllStatementsThatAffects() {
  return affects_table.GetAllKeys();
}

std::unordered_set<int> AffectsTable::GetAllAffectedStatements() {
  return inverse_affects_table.GetAllKeys();
}

TableMultiple<int, int> AffectsTable::GetAffectsTable() {
  return affects_table;
}

TableMultiple<int, int> AffectsTable::GetAffectedTable() {
  return inverse_affects_table;
}

bool AffectsTable::InsertAffectsT(int assign_stmt1, int assign_stmt2) {
  if (assign_stmt1 <= 0 || assign_stmt2 <= 0) {
    return false;
  }

  return affects_T_table.Insert(assign_stmt1, assign_stmt2) && inverse_affects_T_table.Insert(assign_stmt2, assign_stmt1);
}

bool AffectsTable::IsAffectsT(int assign_stmt1, int assign_stmt2) {
  if (assign_stmt1 <= 0 || assign_stmt2 <= 0) {
    return false;
  }
  if (!affects_T_table.Contains(assign_stmt1)) {
    return false;
  }

  std::unordered_set<int>& assign_stmt1_affects_set = affects_T_table.Get(assign_stmt1);
  return std::find(assign_stmt1_affects_set.begin(), assign_stmt1_affects_set.end(), assign_stmt2) != assign_stmt1_affects_set.end();
}

std::unordered_set<int> AffectsTable::GetAffectedTStatements(int assign_stmt1) {
  if (assign_stmt1 <= 0 || !affects_T_table.Contains(assign_stmt1)) {
    return std::unordered_set<int>();
  }
  return affects_T_table.Get(assign_stmt1);
}

std::unordered_set<int> AffectsTable::GetStatementsThatAffectsT(int assign_stmt2) {
  if (assign_stmt2 <= 0 || !inverse_affects_T_table.Contains(assign_stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_affects_T_table.Get(assign_stmt2);
}

std::unordered_set<int> AffectsTable::GetAllStatementsThatAffectsT() {
  return affects_T_table.GetAllKeys();
}

std::unordered_set<int> AffectsTable::GetAllAffectedTStatements() {
  return inverse_affects_T_table.GetAllKeys();
}

TableMultiple<int, int> AffectsTable::GetAffectsTTable() {
  return affects_T_table;
}

TableMultiple<int, int> AffectsTable::GetAffectedTTable() {
  return inverse_affects_T_table;
}

void AffectsTable::ClearAffectsTable() {
  affects_table.ClearTable();
  inverse_affects_table.ClearTable();
  affects_T_table.ClearTable();
  inverse_affects_T_table.ClearTable();
}