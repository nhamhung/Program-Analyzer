#include "pkb/templates/TableMultiple.h"
#include "pkb/templates/TableSingle.h"

class EntityTable {
 private:
  TableSingle<int, std::string> entity_table;
  TableMultiple<std::string, int> inverse_entity_table;

 public:
  EntityTable(){};

  bool InsertEntity(int, const std::string&);

  std::string GetStatementType(int);

  std::unordered_set<int> GetAllStmtsOfType(const std::string&);

  std::unordered_set<std::string> GetAllEntities();

  TableSingle<int, std::string> GetEntityTable();

  TableMultiple<std::string, int> GetInverseEntityTable();

  void ClearEntityTable();
};
