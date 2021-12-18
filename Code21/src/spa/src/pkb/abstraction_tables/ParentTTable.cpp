#include "ParentTTable.h"

#include <algorithm>

bool ParentTTable::InsertParentT(int stmt1, int stmt2) {
  if (stmt1 <= 0 || stmt1 >= stmt2) {
    return false;
  }
  return parent_T_table.Insert(stmt1, stmt2) && inverse_parent_T_table.Insert(stmt2, stmt1);
}

bool ParentTTable::IsParentT(int stmt1, int stmt2) {
  if (stmt1 <= 0 || stmt1 >= stmt2) {
    return false;
  }
  if (!parent_T_table.Contains(stmt1)) {
    return false;
  }

  std::unordered_set<int>& stmt1_children_set = parent_T_table.Get(stmt1);
  return std::find(stmt1_children_set.begin(), stmt1_children_set.end(), stmt2) != stmt1_children_set.end();
}

std::unordered_set<int> ParentTTable::GetChildrenTStatements(int stmt1) {
  if (stmt1 <= 0 || !parent_T_table.Contains(stmt1)) {
    return std::unordered_set<int>();
  }
  return parent_T_table.Get(stmt1);
}

std::unordered_set<int> ParentTTable::GetParentTStatements(int stmt2) {
  if (stmt2 <= 1 || !inverse_parent_T_table.Contains(stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_parent_T_table.Get(stmt2);
}

std::unordered_set<int> ParentTTable::GetAllParentTStmts() {
  return parent_T_table.GetAllKeys();
}

std::unordered_set<int> ParentTTable::GetAllChildrenTStmts() {
  return inverse_parent_T_table.GetAllKeys();
}

TableMultiple<int, int> ParentTTable::GetParentTTable() {
  return parent_T_table;
}

TableMultiple<int, int> ParentTTable::GetInverseParentTTable() {
  return inverse_parent_T_table;
}

void ParentTTable::ClearParentTTable() {
  parent_T_table.ClearTable();
  inverse_parent_T_table.ClearTable();
}
