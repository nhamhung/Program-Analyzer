
#include "SuchThatClause.h"

#include <stdexcept>

#include "query_processor/commons/query/entities/DesignEntityType.h"
#include "query_processor/commons/query/utils/QueryUtils.h"

namespace query_processor {

SuchThatClause::SuchThatClause(DesignAbstraction design_abstraction, ClauseParam lhs_param, ClauseParam rhs_param) {
  this->design_abstraction = design_abstraction;
  this->lhs_param = lhs_param;
  this->rhs_param = rhs_param;
}

const DesignAbstraction SuchThatClause::GetDesignAbstraction() const {
  return this->design_abstraction;
}

const ClauseParam& SuchThatClause::GetLHSParam() const {
  return this->lhs_param;
}

const ClauseParam& SuchThatClause::GetRHSParam() const {
  return this->rhs_param;
}

bool SuchThatClause::IsValidClause() {
  switch (design_abstraction) {
    case DesignAbstraction::FOLLOWS:
    case DesignAbstraction::FOLLOWS_T:
    case DesignAbstraction::PARENT:
    case DesignAbstraction::PARENT_T: {
      if (!QueryUtils::IsStatementEntity(lhs_param, true)) {
        throw std::runtime_error("Invalid LHS parameter -- must be of type statement (index or statement entity) or wildcard.");
      }
      if (!QueryUtils::IsStatementEntity(rhs_param, true)) {
        throw std::runtime_error("Invalid RHS parameter -- must be of type statement (index or statement entity) or wildcard.");
      }
      return true;
    }
    case DesignAbstraction::USES:
    case DesignAbstraction::MODIFIES: {
      if (!(QueryUtils::IsStatementEntity(lhs_param, false) || QueryUtils::IsProcedureEntity(lhs_param, false))) {
        throw std::runtime_error("Invalid LHS parameter -- must be of type statement (index or statement entity) or procedure");
      }

      if (rhs_param.param_type == ClauseParamType::INDEX || rhs_param.param_type == ClauseParamType::EXPR) {
        throw std::runtime_error("Uses need to have a variable or wildcard on the RHS.");
      }

      if (rhs_param.param_type == ClauseParamType::DESIGN_ENTITY) {
        if (rhs_param.design_entity.GetDesignEntityType() != DesignEntityType::VARIABLE) {
          throw std::runtime_error("Uses need to have a variable or wildcard on the RHS.");
        }
      }
      return true;
    }
    case DesignAbstraction::CALLS:
    case DesignAbstraction::CALLS_T: {
      if (!(QueryUtils::IsProcedureEntity(lhs_param, true))) {
        throw std::runtime_error("Invalid LHS parameter -- must be of type procedure (name or procedure entity)");
      }

      if (!(QueryUtils::IsProcedureEntity(rhs_param, true))) {
        throw std::runtime_error("Invalid RHS parameter -- must be of type procedure (name or procedure entity)");
      }

      return true;
    }

    case DesignAbstraction::NEXT:
    case DesignAbstraction::NEXT_T:
    case DesignAbstraction::NEXTBIP:
    case DesignAbstraction::NEXTBIP_T: {
      if (!(QueryUtils::IsProgLineEntity(lhs_param, true))) {
        throw std::runtime_error("Invalid LHS parameter -- must be of type progline (index or progline entity) or wildcard");
      }
      if (!(QueryUtils::IsProgLineEntity(rhs_param, true))) {
        throw std::runtime_error("Invalid RHS parameter -- must be of type progline (index or progline entity) or wildcard");
      }

      return true;
    }

    case DesignAbstraction::AFFECTS:
    case DesignAbstraction::AFFECTS_T:
    case DesignAbstraction::AFFECTSBIP:
    case DesignAbstraction::AFFECTSBIP_T: {
      if (!(QueryUtils::IsAssignEntity(lhs_param, true))) {
        throw std::runtime_error("Invalid LHS parameter -- must be an assignment statement or wildcard.");
      }
      if (!(QueryUtils::IsAssignEntity(rhs_param, true))) {
        throw std::runtime_error("Invalid RHS parameter -- must be an assignment statement or wildcard.");
      }

      return true;
    }
    default:
      throw std::runtime_error("Invalid Design Abstraction");
  }
}

bool operator==(const SuchThatClause& clause1, const SuchThatClause& clause2) {
  DesignAbstraction da1 = clause1.design_abstraction;
  DesignAbstraction da2 = clause2.design_abstraction;
  if (da1 != da2) {
    return false;
  }

  ClauseParam lhs1 = clause1.lhs_param;
  ClauseParam lhs2 = clause2.lhs_param;
  ClauseParam rhs1 = clause1.rhs_param;
  ClauseParam rhs2 = clause2.rhs_param;
  return lhs1 == lhs2 && rhs1 == rhs2;
}

}  // namespace query_processor
