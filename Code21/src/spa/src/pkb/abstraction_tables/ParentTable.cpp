#include "ParentTable.h"

#include <algorithm>

bool ParentTable::InsertParent(int stmt1, int stmt2) {
  if (stmt1 <= 0 || stmt1 >= stmt2) {
    return false;
  }
  if (inverse_parent_table.Contains(stmt2)) {
    return false;
  }

  return parent_table.Insert(stmt1, stmt2) && inverse_parent_table.Insert(stmt2, stmt1);
}

bool ParentTable::IsParent(int stmt1, int stmt2) {
  if (stmt1 <= 0 || stmt1 >= stmt2) {
    return false;
  }
  if (!parent_table.Contains(stmt1)) {
    return false;
  }
  if (!inverse_parent_table.Contains(stmt2)) {
    return false;
  }
  return inverse_parent_table.Get(stmt2) == stmt1;
}

std::unordered_set<int> ParentTable::GetChildrenStatements(int stmt1) {
  if (stmt1 <= 0 || !parent_table.Contains(stmt1)) {
    return std::unordered_set<int>();
  }
  return parent_table.Get(stmt1);
}

int ParentTable::GetParentStatement(int stmt2) {
  if (stmt2 <= 1) {
    return -1;
  }
  if (!inverse_parent_table.Contains(stmt2)) {
    return -1;
  }
  return inverse_parent_table.Get(stmt2);
}

std::unordered_set<int> ParentTable::GetAllParentStmts() {
  return parent_table.GetAllKeys();
}

std::unordered_set<int> ParentTable::GetAllChildrenStmts() {
  return inverse_parent_table.GetAllKeys();
}

TableMultiple<int, int> ParentTable::GetParentTable() {
  return parent_table;
}

TableSingle<int, int> ParentTable::GetInverseParentTable() {
  return inverse_parent_table;
}

void ParentTable::ClearParentTable() {
  parent_table.ClearTable();
  inverse_parent_table.ClearTable();
}