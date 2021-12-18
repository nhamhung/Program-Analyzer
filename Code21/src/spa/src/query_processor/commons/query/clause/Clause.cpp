#include "Clause.h"

#include <stdexcept>

#include "../utils/QueryUtils.h"

namespace query_processor {

Clause::Clause(SuchThatClause such_that_clause) {
  this->such_that_clause = such_that_clause;
  this->type = ClauseType::SUCHTHAT;
}

Clause::Clause(PatternClause pattern_clause) {
  this->pattern_clause = pattern_clause;
  this->type = ClauseType::PATTERN;
}

Clause::Clause(WithClause with_clause) {
  this->with_clause = with_clause;
  this->type = ClauseType::WITH;
}

SuchThatClause &Clause::GetSuchThatClause() {
  if (this->type != ClauseType::SUCHTHAT) {
    throw std::runtime_error("Getting clause with incorrect type");
  }
  return this->such_that_clause;
}

PatternClause &Clause::GetPatternClause() {
  if (this->type != ClauseType::PATTERN) {
    throw std::runtime_error("Getting clause with incorrect type");
  }
  return this->pattern_clause;
}

WithClause &Clause::GetWithClause() {
  if (this->type != ClauseType::WITH) {
    throw std::runtime_error("Getting clause with incorrect type");
  }
  return this->with_clause;
}

ClauseType Clause::GetClauseType() {
  return this->type;
}

ClauseParamType Clause::GetFirstParamType() const {
  switch (type) {
    case ClauseType::SUCHTHAT:
      return such_that_clause.GetLHSParam().param_type;
    case ClauseType::PATTERN:
      // For pattern clauses, since the RHS param is either a wildcard or a
      // pattern expression, the design entity attached to is it considered instead
      return ClauseParamType::DESIGN_ENTITY;
    case ClauseType::WITH:
      return with_clause.GetLHSParam().param_type;
    default:
      throw std::runtime_error("Invalid Clause type");
  }
}

ClauseParamType Clause::GetSecondParamType() const {
  switch (type) {
    case ClauseType::SUCHTHAT:
      return such_that_clause.GetRHSParam().param_type;
    case ClauseType::PATTERN:
      // The LHS parameter is the second parameter that is considered
      return pattern_clause.GetLHSParam().param_type;
    case ClauseType::WITH:
      return with_clause.GetRHSParam().param_type;
    default:
      throw std::runtime_error("Invalid Clause type");
  }
}

const DesignAbstraction Clause::GetSuchThatDesignAbstraction() const {
  if (type != ClauseType::SUCHTHAT) {
    throw std::runtime_error("Getting DesignAbstraction of non Such That Clause");
  }
  return such_that_clause.GetDesignAbstraction();
}

bool operator==(const Clause &clause1, const Clause &clause2) {
  if (clause1.type != clause2.type) {
    return false;
  }

  if (clause1.type == ClauseType::SUCHTHAT) {
    return clause1.such_that_clause == clause2.such_that_clause;
  }

  if (clause1.type == ClauseType::PATTERN) {
    return clause1.pattern_clause == clause2.pattern_clause;
  }

  if (clause1.type == ClauseType::WITH) {
    return clause1.with_clause == clause2.with_clause;
  }
  return false;
}

bool operator<(const Clause &clause1, const Clause &clause2) {
  int num_synonyms_clause_1 = 0;
  int num_synonyms_clause_2 = 0;
  if (clause1.GetFirstParamType() == ClauseParamType::DESIGN_ENTITY || clause1.GetFirstParamType() == ClauseParamType::WILDCARD) {
    num_synonyms_clause_1 += 1;
  }
  if (clause1.GetSecondParamType() == ClauseParamType::DESIGN_ENTITY || clause1.GetSecondParamType() == ClauseParamType::WILDCARD) {
    num_synonyms_clause_1 += 1;
  }
  if (clause2.GetFirstParamType() == ClauseParamType::DESIGN_ENTITY || clause2.GetFirstParamType() == ClauseParamType::WILDCARD) {
    num_synonyms_clause_2 += 1;
  }
  if (clause2.GetSecondParamType() == ClauseParamType::DESIGN_ENTITY || clause2.GetSecondParamType() == ClauseParamType::WILDCARD) {
    num_synonyms_clause_2 += 1;
  }

  if (num_synonyms_clause_1 < num_synonyms_clause_2) {
    return true;
  } else if (num_synonyms_clause_1 == num_synonyms_clause_2) {
    // WithClause < PatternClause < SuchThatClause
    if (clause1.type == ClauseType::WITH && clause2.type != ClauseType::WITH) {
      return true;
    }
    if (clause1.type == ClauseType::PATTERN && clause2.type == ClauseType::SUCHTHAT) {
      return true;
    }
    if (clause1.type == ClauseType::SUCHTHAT && clause2.type == ClauseType::SUCHTHAT) {
      return QueryUtils::RankDesignAbstraction(clause1.GetSuchThatDesignAbstraction()) < QueryUtils::RankDesignAbstraction(clause2.GetSuchThatDesignAbstraction());
    }
  }
  return false;
}
}  // namespace query_processor
