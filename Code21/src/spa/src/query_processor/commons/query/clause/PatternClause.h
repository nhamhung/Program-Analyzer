#pragma once

#include "ClauseParam.h"
#include "query_processor/commons/query/entities/DesignEntity.h"

namespace query_processor {
class PatternClause {
 protected:
  DesignEntity design_entity;
  ClauseParam lhs_param;
  ClauseParam rhs_param;

 public:
  PatternClause(){};
  PatternClause(DesignEntity, ClauseParam, ClauseParam);

  const DesignEntity& GetDesignEntity();

  const ClauseParam& GetLHSParam() const;

  const ClauseParam& GetRHSParam() const;

  bool IsValidClause();

  friend bool operator==(const PatternClause& clause1, const PatternClause& clause2);
};
}  // namespace query_processor
