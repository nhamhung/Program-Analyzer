#include "pkb/templates/TableMultiple.h"

class AffectsBipTable {
 private:
  TableMultiple<int, int> affects_bip_table;
  TableMultiple<int, int> inverse_affects_bip_table;
  TableMultiple<int, int> affects_bip_T_table;
  TableMultiple<int, int> inverse_affects_bip_T_table;

 public:
  AffectsBipTable(){};

  /* APIs for Affects relationship */

  bool InsertAffectsBip(int, int);

  bool IsAffectsBip(int, int);

  std::unordered_set<int> GetStatementsThatAffectsBip(int);

  std::unordered_set<int> GetAffectedBipStatements(int);

  std::unordered_set<int> GetAllStatementsThatAffectsBip();

  std::unordered_set<int> GetAllAffectedBipStatements();

  TableMultiple<int, int> GetAffectsBipTable();

  TableMultiple<int, int> GetAffectedBipTable();

  /* APIs for AffectsT relationship */

  bool InsertAffectsBipT(int, int);

  bool IsAffectsBipT(int, int);

  std::unordered_set<int> GetStatementsThatAffectsBipT(int);

  std::unordered_set<int> GetAffectedBipTStatements(int);

  std::unordered_set<int> GetAllStatementsThatAffectsBipT();

  std::unordered_set<int> GetAllAffectedBipTStatements();

  TableMultiple<int, int> GetAffectsBipTTable();

  TableMultiple<int, int> GetAffectedBipTTable();

  void ClearAffectsBipTable();
};