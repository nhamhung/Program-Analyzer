#include "pkb/templates/TableSingle.h"

class ProcTable {
 private:
  TableSingle<std::string, std::pair<int, int>> proc_table;  // mapping of proc_name to start - end indexes

 public:
  ProcTable(){};

  bool InsertProc(const std::string&, const std::pair<int, int>&);

  std::unordered_set<std::string> GetAllProcedures();

  std::pair<int, int> GetProcRange(const std::string&);

  void ClearProcTable();

  TableSingle<std::string, std::pair<int, int>> GetProcTable();
};