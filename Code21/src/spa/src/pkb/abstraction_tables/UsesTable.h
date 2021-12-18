#include "pkb/templates/TableMultiple.h"

class UsesTable {
 private:
  TableMultiple<int, std::string> uses_stmt_table;
  TableMultiple<std::string, std::string> uses_proc_table;
  TableMultiple<std::string, int> inverse_uses_stmt_table;
  TableMultiple<std::string, std::string> inverse_uses_proc_table;

 public:
  UsesTable(){};

  bool InsertStmtUses(int, const std::string&);

  bool InsertProcUses(const std::string&, const std::string&);

  bool IsStmtUses(int, const std::string&);

  bool IsProcUses(const std::string&, const std::string&);

  std::unordered_set<std::string> GetUsedStmtVariables(int);

  std::unordered_set<std::string> GetUsedProcVariables(const std::string&);

  std::unordered_set<int> GetUsesStatements(const std::string&);

  std::unordered_set<std::string> GetUsesProcedures(const std::string&);

  std::unordered_set<int> GetAllUsesStatements();

  std::unordered_set<std::string> GetAllUsesProcedures();

  void ClearUsesTable();

  TableMultiple<int, std::string> GetUsesStmtTable();

  TableMultiple<std::string, std::string> GetUsesProcTable();

  TableMultiple<std::string, int> GetInverseUsesStmtTable();

  TableMultiple<std::string, std::string> GetInverseUsesProcTable();
};