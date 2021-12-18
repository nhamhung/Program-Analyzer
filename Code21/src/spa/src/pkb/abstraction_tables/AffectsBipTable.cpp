#include "AffectsBipTable.h"

#include <algorithm>

bool AffectsBipTable::InsertAffectsBip(int assign_stmt1, int assign_stmt2) {
  if (assign_stmt1 <= 0 || assign_stmt2 <= 0) {
    return false;
  }

  return affects_bip_table.Insert(assign_stmt1, assign_stmt2) && inverse_affects_bip_table.Insert(assign_stmt2, assign_stmt1);
}

bool AffectsBipTable::IsAffectsBip(int assign_stmt1, int assign_stmt2) {
  if (assign_stmt1 <= 0 || assign_stmt2 <= 0) {
    return false;
  }
  if (!affects_bip_table.Contains(assign_stmt1)) {
    return false;
  }

  std::unordered_set<int>& assign_stmt1_affects_set = affects_bip_table.Get(assign_stmt1);
  return std::find(assign_stmt1_affects_set.begin(), assign_stmt1_affects_set.end(), assign_stmt2) != assign_stmt1_affects_set.end();
}

std::unordered_set<int> AffectsBipTable::GetAffectedBipStatements(int assign_stmt1) {
  if (assign_stmt1 <= 0 || !affects_bip_table.Contains(assign_stmt1)) {
    return std::unordered_set<int>();
  }
  return affects_bip_table.Get(assign_stmt1);
}

std::unordered_set<int> AffectsBipTable::GetStatementsThatAffectsBip(int assign_stmt2) {
  if (assign_stmt2 <= 0 || !inverse_affects_bip_table.Contains(assign_stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_affects_bip_table.Get(assign_stmt2);
}

std::unordered_set<int> AffectsBipTable::GetAllStatementsThatAffectsBip() {
  return affects_bip_table.GetAllKeys();
}

std::unordered_set<int> AffectsBipTable::GetAllAffectedBipStatements() {
  return inverse_affects_bip_table.GetAllKeys();
}

TableMultiple<int, int> AffectsBipTable::GetAffectsBipTable() {
  return affects_bip_table;
}

TableMultiple<int, int> AffectsBipTable::GetAffectedBipTable() {
  return inverse_affects_bip_table;
}

bool AffectsBipTable::InsertAffectsBipT(int assign_stmt1, int assign_stmt2) {
  if (assign_stmt1 <= 0 || assign_stmt2 <= 0) {
    return false;
  }

  return affects_bip_T_table.Insert(assign_stmt1, assign_stmt2) && inverse_affects_bip_T_table.Insert(assign_stmt2, assign_stmt1);
}

bool AffectsBipTable::IsAffectsBipT(int assign_stmt1, int assign_stmt2) {
  if (assign_stmt1 <= 0 || assign_stmt2 <= 0) {
    return false;
  }
  if (!affects_bip_T_table.Contains(assign_stmt1)) {
    return false;
  }

  std::unordered_set<int>& assign_stmt1_affects_set = affects_bip_T_table.Get(assign_stmt1);
  return std::find(assign_stmt1_affects_set.begin(), assign_stmt1_affects_set.end(), assign_stmt2) != assign_stmt1_affects_set.end();
}

std::unordered_set<int> AffectsBipTable::GetAffectedBipTStatements(int assign_stmt1) {
  if (assign_stmt1 <= 0 || !affects_bip_T_table.Contains(assign_stmt1)) {
    return std::unordered_set<int>();
  }
  return affects_bip_T_table.Get(assign_stmt1);
}

std::unordered_set<int> AffectsBipTable::GetStatementsThatAffectsBipT(int assign_stmt2) {
  if (assign_stmt2 <= 0 || !inverse_affects_bip_T_table.Contains(assign_stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_affects_bip_T_table.Get(assign_stmt2);
}

std::unordered_set<int> AffectsBipTable::GetAllStatementsThatAffectsBipT() {
  return affects_bip_T_table.GetAllKeys();
}

std::unordered_set<int> AffectsBipTable::GetAllAffectedBipTStatements() {
  return inverse_affects_bip_T_table.GetAllKeys();
}

TableMultiple<int, int> AffectsBipTable::GetAffectsBipTTable() {
  return affects_bip_T_table;
}

TableMultiple<int, int> AffectsBipTable::GetAffectedBipTTable() {
  return inverse_affects_bip_T_table;
}

void AffectsBipTable::ClearAffectsBipTable() {
  affects_bip_table.ClearTable();
  inverse_affects_bip_table.ClearTable();
  affects_bip_T_table.ClearTable();
  inverse_affects_bip_T_table.ClearTable();
}
