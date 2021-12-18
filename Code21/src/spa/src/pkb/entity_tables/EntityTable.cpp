#include "EntityTable.h"

#include <algorithm>

bool EntityTable::InsertEntity(int stmt_index, const std::string& entity) {
  if (stmt_index <= 0) {
    return false;
  }

  if (entity_table.Contains(stmt_index)) {
    return false;
  }

  return entity_table.Insert(stmt_index, entity) && inverse_entity_table.Insert(entity, stmt_index);
}

std::string EntityTable::GetStatementType(int stmt_index) {
  if (!entity_table.Contains(stmt_index)) {
    return "";
  }
  return entity_table.Get(stmt_index);
}

std::unordered_set<int> EntityTable::GetAllStmtsOfType(const std::string& entity) {
  if (!inverse_entity_table.Contains(entity)) {
    return std::unordered_set<int>();
  }
  return inverse_entity_table.Get(entity);
}

std::unordered_set<std::string> EntityTable::GetAllEntities() {
  return inverse_entity_table.GetAllKeys();
}

TableSingle<int, std::string> EntityTable::GetEntityTable() {
  return entity_table;
}

TableMultiple<std::string, int> EntityTable::GetInverseEntityTable() {
  return inverse_entity_table;
}

void EntityTable::ClearEntityTable() {
  entity_table.ClearTable();
  inverse_entity_table.ClearTable();
}