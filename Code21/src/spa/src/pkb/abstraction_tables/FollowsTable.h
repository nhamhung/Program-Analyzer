#include "pkb/templates/TableSingle.h"

class FollowsTable {
 private:
  TableSingle<int, int> follows_table;
  TableSingle<int, int> inverse_follows_table;

 public:
  FollowsTable(){};

  bool InsertFollows(int, int);

  bool IsFollows(int, int);

  int GetStmtFollowedBy(int);

  int GetStmtFollows(int);

  std::unordered_set<int> GetAllFollowsStmts();

  std::unordered_set<int> GetAllFollowedStmts();

  TableSingle<int, int> GetFollowsTable();

  TableSingle<int, int> GetInverseFollowsTable();

  void ClearFollowsTable();
};