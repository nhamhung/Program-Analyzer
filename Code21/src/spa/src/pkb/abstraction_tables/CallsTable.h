#include "pkb/templates/TableMultiple.h"
#include "pkb/templates/TableSingle.h"

class CallsTable {
 private:
  TableMultiple<std::string, std::string> calls_table;
  TableMultiple<std::string, std::string> inverse_calls_table;
  TableMultiple<std::string, std::string> calls_T_table;
  TableMultiple<std::string, std::string> inverse_calls_T_table;
  TableSingle<int, std::string> calls_stmt_table;
  TableMultiple<std::string, int> inverse_calls_stmt_table;

 public:
  CallsTable(){};

  /* APIs for Calls relationship */

  bool InsertCalls(const std::string&, const std::string&);

  bool IsCalls(const std::string&, const std::string&);

  std::unordered_set<std::string> GetProceduresThatCalls(const std::string&);

  std::unordered_set<std::string> GetProceduresCalled(const std::string&);

  std::unordered_set<std::string> GetAllCallsProcedures();

  std::unordered_set<std::string> GetAllCalledProcedures();

  TableMultiple<std::string, std::string> GetCallsTable();

  TableMultiple<std::string, std::string> GetInverseCallsTable();

  /* APIs for CallsT relationship */

  bool InsertCallsT(const std::string&, const std::string&);

  std::unordered_set<std::string> GetProceduresThatCallsT(const std::string&);

  std::unordered_set<std::string> GetProceduresCalledT(const std::string&);

  std::unordered_set<std::string> GetAllCallsTProcedures();

  std::unordered_set<std::string> GetAllCalledTProcedures();

  TableMultiple<std::string, std::string> GetCallsTTable();

  TableMultiple<std::string, std::string> GetInverseCallsTTable();

  /* APIs for call_stmt_table */

  bool InsertCalls(int, const std::string&);

  bool IsCallsT(const std::string&, const std::string&);

  std::string GetCalledProcedure(int);

  std::unordered_set<int> GetCallsStmtIndexes(const std::string&);

  std::unordered_set<int> GetAllCallsStmts();

  TableSingle<int, std::string> GetCallsStmtTable();

  TableMultiple<std::string, int> GetInverseCallsStmtTable();

  void ClearCallsTable();
};