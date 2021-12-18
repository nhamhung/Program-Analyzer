#include "Query.h"

#include <stdexcept>

namespace query_processor {

Query::Query() {}
Query::Query(SelectedEntity entity) {
  this->selected_entities.push_back(entity);
}

Query::Query(std::vector<SelectedEntity> entities) {
  this->selected_entities = entities;
}

SelectedEntity& Query::GetSelectedEntity() {
  if (selected_entities.size() != 1) {
    throw std::runtime_error("GetSelectedEntity() is being called on a query containing a tuple");
  }
  return this->selected_entities.front();
}

std::vector<SelectedEntity>& Query::GetSelectedEntities() {
  return this->selected_entities;
}

bool Query::SetClauseList(std::vector<Clause> clause_list) {
  this->clause_list = clause_list;
  return true;
}

std::vector<Clause>& Query::GetClauseList() {
  return this->clause_list;
}

bool Query::AddClause(Clause new_clause) {
  this->clause_list.push_back(new_clause);
  return true;
}
}  // namespace query_processor