#include <string>
#include <vector>

#include "pkb/templates/TableMultiple.h"

class ContainerTable {
 private:
  TableMultiple<int, std::string> if_table;
  TableMultiple<std::string, int> inverse_if_table;
  TableMultiple<int, std::string> while_table;
  TableMultiple<std::string, int> inverse_while_table;

 public:
  ContainerTable(){};

  /* APIs for If */

  bool InsertIf(int, const std::vector<std::string>&);

  std::unordered_set<std::string> GetVariablesUsedByIfStmt(int);

  std::unordered_set<int> GetIfStmts(const std::string&);

  std::unordered_set<int> GetAllIfStmts();

  std::unordered_set<std::string> GetAllVariablesUsedByIf();

  TableMultiple<int, std::string> GetIfTable();

  TableMultiple<std::string, int> GetInverseIfTable();

  /* APIs for While */

  bool InsertWhile(int, const std::vector<std::string>&);

  std::unordered_set<std::string> GetVariablesUsedByWhileStmt(int);

  std::unordered_set<int> GetWhileStmts(const std::string&);

  std::unordered_set<int> GetAllWhileStmts();

  std::unordered_set<std::string> GetAllVariablesUsedByWhile();

  TableMultiple<int, std::string> GetWhileTable();

  TableMultiple<std::string, int> GetInverseWhileTable();

  void ClearTable();
};