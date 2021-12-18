#include <utility>

#include "pkb/templates/TableMultiple.h"
#include "pkb/templates/TableSingle.h"
#include "source_processor/token/TokenList.h"

class AssignTable {
 private:
  TableSingle<int, source_processor::TokenList> assign_table;  // this table stores mapping of stmt to a token list on the RHS
  TableSingle<int, std::string> assigned_table;                // this table stores mapping of stmt to assigned var on the LHS
  TableMultiple<std::string, int> inverse_assigned_table;      // this table stores mapping of assigned var on the LHS to statement number

 public:
  AssignTable(){};

  TableSingle<int, std::string> GetAssignedTable();

  TableSingle<int, source_processor::TokenList> GetAssignTable();

  TableMultiple<std::string, int> GetInverseAssignedTable();

  bool InsertAssign(int, const std::string&, const source_processor::TokenList&);

  std::unordered_set<int> GetAllAssignStmts();

  std::unordered_set<std::string> GetAllAssignedVariables();

  std::string GetAssignedVariable(int);

  std::unordered_set<int> GetAllAssignStmtsThatModifies(std::string);

  std::unordered_set<int> GetAllAssignStmtsThatMatches(const source_processor::TokenList&);

  std::unordered_set<int> GetAllAssignStmtsThatContains(const source_processor::TokenList&);

  void ClearAssignTable();
};