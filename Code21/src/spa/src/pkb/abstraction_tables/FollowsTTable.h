#include "pkb/templates/TableMultiple.h"

class FollowsTTable {
 private:
  TableMultiple<int, int> follows_T_table;
  TableMultiple<int, int> inverse_follows_T_table;

 public:
  FollowsTTable(){};

  bool InsertFollowsT(int, int);

  bool IsFollowsT(int, int);

  std::unordered_set<int> GetStmtsFollowedTBy(int);

  std::unordered_set<int> GetStmtsFollowsT(int);

  std::unordered_set<int> GetAllFollowedTStmts();

  std::unordered_set<int> GetAllFollowsTStmts();

  TableMultiple<int, int> GetFollowsTTable();

  TableMultiple<int, int> GetInverseFollowsTTable();

  void ClearFollowsTTable();
};