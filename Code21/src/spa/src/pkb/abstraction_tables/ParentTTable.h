#include "pkb/templates/TableMultiple.h"

class ParentTTable {
 private:
  TableMultiple<int, int> parent_T_table;
  TableMultiple<int, int> inverse_parent_T_table;

 public:
  ParentTTable(){};

  bool InsertParentT(int, int);

  bool IsParentT(int, int);

  std::unordered_set<int> GetChildrenTStatements(int);

  std::unordered_set<int> GetParentTStatements(int);

  std::unordered_set<int> GetAllParentTStmts();

  std::unordered_set<int> GetAllChildrenTStmts();

  TableMultiple<int, int> GetParentTTable();

  TableMultiple<int, int> GetInverseParentTTable();

  void ClearParentTTable();
};