#include "pkb/templates/TableMultiple.h"

class NextBipTable {
 private:
  TableMultiple<int, int> nextbip_table;
  TableMultiple<int, int> inverse_nextbip_table;
  TableMultiple<int, int> nextbip_T_table;
  TableMultiple<int, int> inverse_nextbip_T_table;

 public:
  NextBipTable(){};

  /* APIs for Next relationship */

  bool InsertNextBip(int, int);

  bool IsNextBip(int, int);

  std::unordered_set<int> GetNextBipStatements(int);

  std::unordered_set<int> GetPreviousBipStatements(int);

  std::unordered_set<int> GetAllNextBipStatements();

  std::unordered_set<int> GetAllPreviousBipStatements();

  TableMultiple<int, int> GetNextBipTable();

  TableMultiple<int, int> GetInverseNextBipTable();

  /* API for NextT relationship */

  bool InsertNextBipT(int, int);

  bool IsNextBipT(int, int);

  std::unordered_set<int> GetNextBipTStatements(int);

  std::unordered_set<int> GetPreviousBipTStatements(int);

  std::unordered_set<int> GetAllNextBipTStatements();

  std::unordered_set<int> GetAllPreviousBipTStatements();

  TableMultiple<int, int> GetNextBipTTable();

  TableMultiple<int, int> GetInverseNextBipTTable();

  void ClearNextBipTable();
};