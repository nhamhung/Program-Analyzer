
#include "WithClause.h"

#include <stdexcept>

#include "query_processor/commons/query/utils/QueryUtils.h"

namespace query_processor {

WithClause::WithClause(std::pair<ClauseParam, AttributeType> lhs, std::pair<ClauseParam, AttributeType> rhs) {
  this->lhs_pair = lhs;
  this->rhs_pair = rhs;
}

const ClauseParam& WithClause::GetLHSParam() const {
  return this->lhs_pair.first;
}

const ClauseParam& WithClause::GetRHSParam() const {
  return this->rhs_pair.first;
}

AttributeType WithClause::GetLHSAttributeType() {
  return this->lhs_pair.second;
}

AttributeType WithClause::GetRHSAttributeType() {
  return this->rhs_pair.second;
}

bool WithClause::IsValidClause() {
  ClauseParam& lhs_param = lhs_pair.first;
  ClauseParam& rhs_param = rhs_pair.first;
  bool valid_lhs = (lhs_param.param_type == ClauseParamType::INDEX && lhs_pair.second == AttributeType::INTEGER) ||
                   (lhs_param.param_type == ClauseParamType::NAME && lhs_pair.second == AttributeType::NAME) ||
                   (lhs_param.param_type == ClauseParamType::DESIGN_ENTITY && QueryUtils::IsValidAttributeType(lhs_param.design_entity, lhs_pair.second));
  bool valid_rhs = (rhs_param.param_type == ClauseParamType::INDEX && rhs_pair.second == AttributeType::INTEGER) ||
                   (rhs_param.param_type == ClauseParamType::NAME && rhs_pair.second == AttributeType::NAME) ||
                   (rhs_param.param_type == ClauseParamType::DESIGN_ENTITY && QueryUtils::IsValidAttributeType(rhs_param.design_entity, rhs_pair.second));
  if (valid_lhs && valid_rhs) {
    return true;
  } else {
    throw std::runtime_error("Invalid with clause with bad ClauseParamType or attribute type");
  }
}

bool operator==(const WithClause& clause1, const WithClause& clause2) {
  if (clause1.lhs_pair != clause2.lhs_pair) {
    return false;
  }

  if (clause1.rhs_pair != clause2.rhs_pair) {
    return false;
  }

  return true;
}
}  // namespace query_processor
