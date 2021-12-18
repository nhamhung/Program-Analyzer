#pragma once

#include <vector>

#include "query_processor/commons/query/clause/Clause.h"
#include "query_processor/commons/query/entities/DesignEntity.h"
#include "query_processor/commons/query/entities/SelectedEntity.h"

namespace query_processor {

class Query {
 public:
  Query();
  Query(SelectedEntity);
  Query(std::vector<SelectedEntity>);
  SelectedEntity& GetSelectedEntity();
  std::vector<SelectedEntity>& GetSelectedEntities();
  bool AddClause(Clause);
  bool SetClauseList(std::vector<Clause>);
  std::vector<Clause>& GetClauseList();

 protected:
  std::vector<SelectedEntity> selected_entities;
  std::vector<Clause> clause_list;
};
}  // namespace query_processor