#include "FollowsTable.h"

bool FollowsTable::InsertFollows(int stmt1, int stmt2) {
  if (follows_table.Contains(stmt1) || inverse_follows_table.Contains(stmt2)) {
    return false;
  }
  if (stmt1 >= stmt2 || stmt1 <= 0) {
    return false;
  }

  return follows_table.Insert(stmt1, stmt2) && inverse_follows_table.Insert(stmt2, stmt1);
}

bool FollowsTable::IsFollows(int stmt1, int stmt2) {
  if (!follows_table.Contains(stmt1)) {
    return false;
  }

  return follows_table.Get(stmt1) == stmt2;
}

int FollowsTable::GetStmtFollowedBy(int stmt2) {
  if (!inverse_follows_table.Contains(stmt2)) {
    return -1;
  }
  return inverse_follows_table.Get(stmt2);
}

int FollowsTable::GetStmtFollows(int stmt1) {
  if (!follows_table.Contains(stmt1)) {
    return -1;
  }
  return follows_table.Get(stmt1);
}

std::unordered_set<int> FollowsTable::GetAllFollowedStmts() {
  return follows_table.GetAllKeys();
};

std::unordered_set<int> FollowsTable::GetAllFollowsStmts() {
  return inverse_follows_table.GetAllKeys();
};

TableSingle<int, int> FollowsTable::GetFollowsTable() {
  return follows_table;
}

TableSingle<int, int> FollowsTable::GetInverseFollowsTable() {
  return inverse_follows_table;
}

void FollowsTable::ClearFollowsTable() {
  follows_table.ClearTable();
  inverse_follows_table.ClearTable();
}