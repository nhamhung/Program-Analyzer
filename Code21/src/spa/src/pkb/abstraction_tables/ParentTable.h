#include "pkb/templates/TableMultiple.h"
#include "pkb/templates/TableSingle.h"

class ParentTable {
 private:
  TableMultiple<int, int> parent_table;
  TableSingle<int, int> inverse_parent_table;

 public:
  ParentTable(){};

  bool InsertParent(int, int);

  bool IsParent(int, int);

  std::unordered_set<int> GetChildrenStatements(int);

  int GetParentStatement(int);

  std::unordered_set<int> GetAllParentStmts();

  std::unordered_set<int> GetAllChildrenStmts();

  TableMultiple<int, int> GetParentTable();

  TableSingle<int, int> GetInverseParentTable();

  void ClearParentTable();
};