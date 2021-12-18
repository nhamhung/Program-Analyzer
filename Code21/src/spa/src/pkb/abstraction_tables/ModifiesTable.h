#include "pkb/templates/TableMultiple.h"

class ModifiesTable {
 private:
  TableMultiple<int, std::string> modifies_stmt_table;
  TableMultiple<std::string, std::string> modifies_proc_table;
  TableMultiple<std::string, int> inverse_modifies_stmt_table;
  TableMultiple<std::string, std::string> inverse_modifies_proc_table;

 public:
  ModifiesTable(){};

  bool InsertStmtModifies(int, const std::string&);

  bool InsertProcModifies(const std::string&, const std::string&);

  bool IsStmtModifies(int, const std::string&);

  bool IsProcModifies(const std::string&, const std::string&);

  std::unordered_set<std::string> GetModifiedStmtVariables(int);

  std::unordered_set<std::string> GetModifiedProcVariables(const std::string&);

  std::unordered_set<int> GetModifiesStatements(const std::string&);

  std::unordered_set<std::string> GetModifiesProcedures(const std::string&);

  std::unordered_set<std::string> GetAllModifiesProcedures();

  std::unordered_set<int> GetAllModifiesStatements();

  void ClearModifiesTable();

  TableMultiple<int, std::string> GetModifiesStmtTable();

  TableMultiple<int, std::string> GetModifiesProcTable();

  TableMultiple<std::string, int> GetInverseModifiesStmtTable();

  TableMultiple<std::string, std::string> GetInverseModifiesProcTable();
};