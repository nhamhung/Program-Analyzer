#include "pkb/templates/TableMultiple.h"

class AffectsTable {
 private:
  TableMultiple<int, int> affects_table;
  TableMultiple<int, int> inverse_affects_table;
  TableMultiple<int, int> affects_T_table;
  TableMultiple<int, int> inverse_affects_T_table;

 public:
  AffectsTable(){};

  /* APIs for Affects relationship */

  bool InsertAffects(int, int);

  bool IsAffects(int, int);

  std::unordered_set<int> GetStatementsThatAffects(int);

  std::unordered_set<int> GetAffectedStatements(int);

  std::unordered_set<int> GetAllStatementsThatAffects();

  std::unordered_set<int> GetAllAffectedStatements();

  TableMultiple<int, int> GetAffectsTable();

  TableMultiple<int, int> GetAffectedTable();

  /* APIs for AffectsT relationship */

  bool InsertAffectsT(int, int);

  bool IsAffectsT(int, int);

  std::unordered_set<int> GetStatementsThatAffectsT(int);

  std::unordered_set<int> GetAffectedTStatements(int);

  std::unordered_set<int> GetAllStatementsThatAffectsT();

  std::unordered_set<int> GetAllAffectedTStatements();

  TableMultiple<int, int> GetAffectsTTable();

  TableMultiple<int, int> GetAffectedTTable();

  void ClearAffectsTable();
};