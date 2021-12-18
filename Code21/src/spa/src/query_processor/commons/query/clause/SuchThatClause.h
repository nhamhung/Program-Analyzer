#pragma once

#include <string>
#include <typeinfo>

#include "ClauseParam.h"
#include "DesignAbstraction.h"
#include "query_processor/commons/query/entities/DesignEntity.h"
#include "source_processor/token/TokenList.h"

namespace query_processor {

class SuchThatClause {
 protected:
  DesignAbstraction design_abstraction;
  ClauseParam lhs_param;
  ClauseParam rhs_param;

 public:
  SuchThatClause(){};
  SuchThatClause(DesignAbstraction, ClauseParam, ClauseParam);
  const DesignAbstraction GetDesignAbstraction() const;
  const ClauseParam& GetLHSParam() const;
  const ClauseParam& GetRHSParam() const;
  bool IsValidClause();
  friend bool operator==(const SuchThatClause& clause1, const SuchThatClause& clause2);
};
}  // namespace query_processor
