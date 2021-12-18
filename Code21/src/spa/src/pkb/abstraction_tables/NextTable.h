#include "pkb/templates/TableMultiple.h"

class NextTable {
 private:
  TableMultiple<int, int> next_table;
  TableMultiple<int, int> inverse_next_table;
  TableMultiple<int, int> next_T_table;
  TableMultiple<int, int> inverse_next_T_table;

 public:
  NextTable(){};

  /* APIs for Next relationship */

  bool InsertNext(int, int);

  bool IsNext(int, int);

  std::unordered_set<int> GetNextStatements(int);

  std::unordered_set<int> GetPreviousStatements(int);

  std::unordered_set<int> GetAllNextStatements();

  std::unordered_set<int> GetAllPreviousStatements();

  TableMultiple<int, int> GetNextTable();

  TableMultiple<int, int> GetInverseNextTable();

  /* API for NextT relationship */

  bool InsertNextT(int, int);

  bool IsNextT(int, int);

  std::unordered_set<int> GetNextTStatements(int);

  std::unordered_set<int> GetPreviousTStatements(int);

  std::unordered_set<int> GetAllNextTStatements();

  std::unordered_set<int> GetAllPreviousTStatements();

  TableMultiple<int, int> GetNextTTable();

  TableMultiple<int, int> GetInverseNextTTable();

  void ClearNextTable();
};