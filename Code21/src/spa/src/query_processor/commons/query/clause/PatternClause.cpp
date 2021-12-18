#include "PatternClause.h"

#include <stdexcept>

#include "query_processor/commons/query/entities/DesignEntityType.h"
#include "query_processor/commons/query/utils/QueryUtils.h"

namespace query_processor {

PatternClause::PatternClause(DesignEntity design_entity, ClauseParam lhs_param, ClauseParam rhs_param) {
  this->design_entity = design_entity;
  this->lhs_param = lhs_param;
  this->rhs_param = rhs_param;
}

const DesignEntity& PatternClause::GetDesignEntity() {
  return this->design_entity;
}

const ClauseParam& PatternClause::GetLHSParam() const {
  return this->lhs_param;
}

const ClauseParam& PatternClause::GetRHSParam() const {
  return this->rhs_param;
}

bool PatternClause::IsValidClause() {
  DesignEntityType type = design_entity.GetDesignEntityType();
  if (type != DesignEntityType::ASSIGN && type != DesignEntityType::WHILE && type != DesignEntityType::IF) {
    throw std::runtime_error("Pattern clause has to use an assign, while or if synonym");
  }
  if (!QueryUtils::IsVariableEntity(lhs_param, true)) {
    throw std::runtime_error("LHS param needs to be a variable (name or variable entity) or wildcard.");
  }

  if (type == DesignEntityType::WHILE || type == DesignEntityType::IF) {
    if (rhs_param.param_type != ClauseParamType::WILDCARD) {
      throw std::runtime_error("Right hand side of while and if pattern clauses must be a wildcard.");
    }
  }
  if (rhs_param.param_type == ClauseParamType::DESIGN_ENTITY) {
    throw std::runtime_error("Right hand side cannot have any other Design Entity apart from a wildcard.");
  } else if (rhs_param.param_type != ClauseParamType::EXPR && rhs_param.param_type != ClauseParamType::WILDCARD) {
    throw std::runtime_error("Pattern clause needs to contain an expression or a wildcard on the right hand side.");
  }
  return true;
}

bool operator==(const PatternClause& clause1, const PatternClause& clause2) {
  DesignEntity de1 = clause1.design_entity;
  DesignEntity de2 = clause2.design_entity;
  if (!(de1 == de2)) {
    return false;
  }

  ClauseParam lhs1 = clause1.lhs_param;
  ClauseParam lhs2 = clause2.lhs_param;
  ClauseParam rhs1 = clause1.rhs_param;
  ClauseParam rhs2 = clause2.rhs_param;
  return lhs1 == lhs2 && rhs1 == rhs2;
}

}  // namespace query_processor
