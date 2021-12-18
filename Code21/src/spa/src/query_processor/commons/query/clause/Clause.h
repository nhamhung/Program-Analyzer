#pragma once
#include "ClauseParam.h"
#include "ClauseType.h"
#include "PatternClause.h"
#include "SuchThatClause.h"
#include "WithClause.h"

namespace query_processor {
class Clause {
 protected:
  SuchThatClause such_that_clause;
  PatternClause pattern_clause;
  WithClause with_clause;
  ClauseType type;

 public:
  Clause(){};
  Clause(SuchThatClause);
  Clause(PatternClause);
  Clause(WithClause);
  SuchThatClause &GetSuchThatClause();
  PatternClause &GetPatternClause();
  WithClause &GetWithClause();
  ClauseType GetClauseType();
  friend bool operator==(const Clause &clause1, const Clause &clause2);
  friend bool operator<(const Clause &clause1, const Clause &clause2);

 private:
  ClauseParamType GetFirstParamType() const;
  ClauseParamType GetSecondParamType() const;
  const DesignAbstraction GetSuchThatDesignAbstraction() const;
};
}  // namespace query_processor
