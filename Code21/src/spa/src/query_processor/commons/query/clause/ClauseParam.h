#pragma once

#include <source_processor/token/TokenList.h>

#include "query_processor/commons/query/entities/DesignEntity.h"

enum class ClauseParamType {
  DESIGN_ENTITY,
  NAME,
  INDEX,
  EXPR,
  WILDCARD,
};

namespace query_processor {
struct PatternExpression {
  source_processor::TokenList token_list;
  bool is_wild_card;

  PatternExpression() {}

  PatternExpression(source_processor::TokenList token_list) {
    this->token_list = token_list;
    this->is_wild_card = false;
  }

  PatternExpression(source_processor::TokenList token_list, bool is_wild_card) {
    this->token_list = token_list;
    this->is_wild_card = is_wild_card;
  }

  friend bool operator==(const PatternExpression& pe1, const PatternExpression& pe2) {
    return pe1.token_list == pe2.token_list && pe1.is_wild_card == pe2.is_wild_card;
  }
};

struct ClauseParam {
  DesignEntity design_entity;
  std::string var_proc_name;
  int statement_index;
  PatternExpression pattern_expr;
  ClauseParamType param_type;

  ClauseParam() {}

  ClauseParam(DesignEntity design_entity) {
    this->design_entity = design_entity;
    if (design_entity.GetDesignEntityType() == DesignEntityType::WILDCARD) {
      this->param_type = ClauseParamType::WILDCARD;
    } else {
      this->param_type = ClauseParamType::DESIGN_ENTITY;
    }
  }

  ClauseParam(std::string var_proc_name) {
    this->var_proc_name = var_proc_name;
    this->param_type = ClauseParamType::NAME;
  }

  ClauseParam(int statement_index) {
    this->statement_index = statement_index;
    this->param_type = ClauseParamType::INDEX;
  }

  ClauseParam(PatternExpression pattern_expr) {
    this->pattern_expr = pattern_expr;
    this->param_type = ClauseParamType::EXPR;
  }

  friend bool operator==(const ClauseParam& cp1, const ClauseParam& cp2) {
    if (cp1.param_type != cp2.param_type) {
      return false;
    }
    if (cp1.param_type == ClauseParamType::DESIGN_ENTITY) {
      return cp1.design_entity == cp2.design_entity;
    }
    if (cp1.param_type == ClauseParamType::INDEX) {
      return cp1.statement_index == cp2.statement_index;
    }
    if (cp1.param_type == ClauseParamType::NAME) {
      return cp1.var_proc_name == cp2.var_proc_name;
    }
    if (cp1.param_type == ClauseParamType::EXPR) {
      return cp1.pattern_expr == cp2.pattern_expr;
    }
    if (cp1.param_type == ClauseParamType::WILDCARD) {
      return cp2.param_type == ClauseParamType::WILDCARD;
    }
    return false;
  }
};
}  // namespace query_processor