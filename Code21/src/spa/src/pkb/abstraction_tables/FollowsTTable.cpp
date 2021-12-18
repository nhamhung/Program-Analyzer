#include "FollowsTTable.h"

#include <algorithm>

bool FollowsTTable::InsertFollowsT(int stmt1, int stmt2) {
  if (stmt1 >= stmt2 || stmt1 <= 0) {
    return false;
  }
  return follows_T_table.Insert(stmt1, stmt2) && inverse_follows_T_table.Insert(stmt2, stmt1);
}

bool FollowsTTable::IsFollowsT(int stmt1, int stmt2) {
  if (stmt1 >= stmt2 || stmt1 <= 0) {
    return false;
  }
  if (!follows_T_table.Contains(stmt1)) {
    return false;
  }

  std::unordered_set<int>& stmt1_follows_set = follows_T_table.Get(stmt1);
  return std::find(stmt1_follows_set.begin(), stmt1_follows_set.end(), stmt2) != stmt1_follows_set.end();
}

std::unordered_set<int> FollowsTTable::GetStmtsFollowedTBy(int stmt2) {
  if (stmt2 <= 1 || !inverse_follows_T_table.Contains(stmt2)) {
    return std::unordered_set<int>();
  }
  return inverse_follows_T_table.Get(stmt2);
}

std::unordered_set<int> FollowsTTable::GetStmtsFollowsT(int stmt_index) {
  if (stmt_index <= 0 || !follows_T_table.Contains(stmt_index)) {
    return std::unordered_set<int>();
  }
  return follows_T_table.Get(stmt_index);
}

std::unordered_set<int> FollowsTTable::GetAllFollowedTStmts() {
  return follows_T_table.GetAllKeys();
}

std::unordered_set<int> FollowsTTable::GetAllFollowsTStmts() {
  return inverse_follows_T_table.GetAllKeys();
}

TableMultiple<int, int> FollowsTTable::GetFollowsTTable() {
  return follows_T_table;
}

TableMultiple<int, int> FollowsTTable::GetInverseFollowsTTable() {
  return inverse_follows_T_table;
}

void FollowsTTable::ClearFollowsTTable() {
  follows_T_table.ClearTable();
  inverse_follows_T_table.ClearTable();
}