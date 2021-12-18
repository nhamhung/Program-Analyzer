#include "QueryUtils.h"
#include "utils/Extension.h"
#include <string>

namespace query_processor {

DesignEntityType QueryUtils::ConvertStringToDesignEntityType(std::string input_string) {
  return string_to_design_entity_type_mappings[input_string];
}

std::string QueryUtils::ConvertDesignEntityTypeToString(DesignEntityType input_type) {
  return design_entity_type_to_string_mappings[input_type];
}

DesignAbstraction QueryUtils::ConvertStringToDesignAbstraction(std::string input_string) {
  DesignAbstraction da = design_abstraction_mappings[input_string];
  if (!utils::Extension::HasNextBip && (da == DesignAbstraction::NEXTBIP || da == DesignAbstraction::NEXTBIP_T)) {
    throw std::runtime_error("NextBip/* has not been turned on as an extension");
  }
  if (!utils::Extension::HasAffectsBip && (da == DesignAbstraction::AFFECTSBIP || da == DesignAbstraction::AFFECTSBIP_T)) {
    throw std::runtime_error("AffectsBip/* has not been turned on as an extension");
  }
  return da;
}

std::string QueryUtils::ConvertClauseTypeToString(ClauseType input_type) {
  return clause_type_to_string_mappings[input_type];
}

AttributeType QueryUtils::ConvertStringToAttributeType(std::string attribute) {
  return synonym_attribute_type_mappings[attribute];
}

bool QueryUtils::IsStatementEntity(ClauseParam &param, bool is_wildcard_allowed = false) {
  ClauseParamType param_type = param.param_type;
  if (param_type == ClauseParamType::INDEX) {
    return true;
  }
  if (is_wildcard_allowed && param_type == ClauseParamType::WILDCARD) {
    return true;
  }
  if (param_type == ClauseParamType::DESIGN_ENTITY) {
    DesignEntityType de_type = param.design_entity.GetDesignEntityType();
    if (de_type == DesignEntityType::STMT || de_type == DesignEntityType::ASSIGN ||
        de_type == DesignEntityType::READ || de_type == DesignEntityType::PRINT ||
        de_type == DesignEntityType::IF || de_type == DesignEntityType::WHILE ||
        de_type == DesignEntityType::CALL || de_type == DesignEntityType::PROG_LINE) {
      return true;
    }
  }
  return false;
}

bool QueryUtils::IsAssignEntity(ClauseParam &param, bool is_wildcard_allowed = false) {
  ClauseParamType param_type = param.param_type;
  if (param_type == ClauseParamType::INDEX) {
    return true;
  }
  if (is_wildcard_allowed && param_type == ClauseParamType::WILDCARD) {
    return true;
  }
  if (param_type == ClauseParamType::DESIGN_ENTITY) {
    DesignEntityType de_type = param.design_entity.GetDesignEntityType();
    if (de_type == DesignEntityType::STMT || de_type == DesignEntityType::ASSIGN ||
        de_type == DesignEntityType::PROG_LINE) {
      return true;
    }
  }
  return false;
}

bool QueryUtils::IsProgLineEntity(ClauseParam &param, bool is_wildcard_allowed = false) {
  return IsStatementEntity(param, is_wildcard_allowed);
}

bool QueryUtils::IsProcedureEntity(ClauseParam &param, bool is_wildcard_allowed = false) {
  ClauseParamType param_type = param.param_type;
  if (param_type == ClauseParamType::NAME) {
    return true;
  }
  if (is_wildcard_allowed && param_type == ClauseParamType::WILDCARD) {
    return true;
  }
  if (param_type == ClauseParamType::DESIGN_ENTITY) {
    DesignEntityType de_type = param.design_entity.GetDesignEntityType();
    if (de_type == DesignEntityType::PROCEDURE) {
      return true;
    }
  }
  return false;
}

bool QueryUtils::IsVariableEntity(ClauseParam &param, bool is_wildcard_allowed = false) {
  ClauseParamType param_type = param.param_type;
  if (param_type == ClauseParamType::NAME) {
    return true;
  }
  if (is_wildcard_allowed && param_type == ClauseParamType::WILDCARD) {
    return true;
  }
  if (param_type == ClauseParamType::DESIGN_ENTITY) {
    DesignEntityType de_type = param.design_entity.GetDesignEntityType();
    if (de_type == DesignEntityType::VARIABLE) {
      return true;
    }
  }
  return false;
}

bool QueryUtils::IsValidAttributeType(DesignEntity &de, AttributeType attr_type) {
  DesignEntityType type = de.GetDesignEntityType();
  if (attr_type == AttributeType::STMT_NO) {
    if (type == DesignEntityType::STMT ||
        type == DesignEntityType::READ ||
        type == DesignEntityType::PRINT ||
        type == DesignEntityType::CALL ||
        type == DesignEntityType::WHILE ||
        type == DesignEntityType::IF ||
        type == DesignEntityType::ASSIGN) {
      return true;
    }
  }
  if (attr_type == AttributeType::VAR_NAME) {
    if (type == DesignEntityType::VARIABLE ||
        type == DesignEntityType::READ ||
        type == DesignEntityType::PRINT) {
      return true;
    }
  }
  if (attr_type == AttributeType::PROC_NAME) {
    if (type == DesignEntityType::PROCEDURE ||
        type == DesignEntityType::CALL) {
      return true;
    }
  }
  if (attr_type == AttributeType::VALUE) {
    if (type == DesignEntityType::CONSTANT) {
      return true;
    }
  }
  if (attr_type == AttributeType::NONE) {
    if (type == DesignEntityType::PROG_LINE) {
      return true;
    }
  }
  return false;
}

int QueryUtils::RankDesignAbstraction(DesignAbstraction da) {
  return design_abstraction_rank[da];
}

std::map<DesignAbstraction, int> QueryUtils::design_abstraction_rank = {
    {DesignAbstraction::CALLS, 1},
    {DesignAbstraction::CALLS_T, 2},
    {DesignAbstraction::AFFECTS, 3},
    {DesignAbstraction::FOLLOWS, 4},
    {DesignAbstraction::AFFECTSBIP, 5},
    {DesignAbstraction::PARENT, 6},
    {DesignAbstraction::AFFECTS_T, 7},
    {DesignAbstraction::NEXT, 8},
    {DesignAbstraction::NEXTBIP, 9},
    {DesignAbstraction::FOLLOWS_T, 10},
    {DesignAbstraction::PARENT_T, 11},
    {DesignAbstraction::MODIFIES, 12},
    {DesignAbstraction::USES, 13},
    {DesignAbstraction::AFFECTSBIP_T, 14},
    {DesignAbstraction::NEXT_T, 15},
    {DesignAbstraction::NEXTBIP_T, 16}};

std::map<std::string, DesignEntityType> QueryUtils::string_to_design_entity_type_mappings = {
    {"stmt", DesignEntityType::STMT},
    {"read", DesignEntityType::READ},
    {"print", DesignEntityType::PRINT},
    {"call", DesignEntityType::CALL},
    {"while", DesignEntityType::WHILE},
    {"if", DesignEntityType::IF},
    {"assign", DesignEntityType::ASSIGN},
    {"variable", DesignEntityType::VARIABLE},
    {"constant", DesignEntityType::CONSTANT},
    {"procedure", DesignEntityType::PROCEDURE},
    {"prog_line", DesignEntityType::PROG_LINE}};

std::map<DesignEntityType, std::string> QueryUtils::design_entity_type_to_string_mappings = {
    {DesignEntityType::STMT, "stmt"},
    {DesignEntityType::READ, "read"},
    {DesignEntityType::PRINT, "print"},
    {DesignEntityType::CALL, "call"},
    {DesignEntityType::WHILE, "while"},
    {DesignEntityType::IF, "if"},
    {DesignEntityType::ASSIGN, "assign"},
    {DesignEntityType::VARIABLE, "variable"},
    {DesignEntityType::CONSTANT, "constant"},
    {DesignEntityType::PROCEDURE, "procedure"},
    {DesignEntityType::PROG_LINE, "prog_line"}};

std::map<std::string, DesignAbstraction> QueryUtils::design_abstraction_mappings = {
    {"Follows", DesignAbstraction::FOLLOWS},
    {"Follows*", DesignAbstraction::FOLLOWS_T},
    {"Parent", DesignAbstraction::PARENT},
    {"Parent*", DesignAbstraction::PARENT_T},
    {"Modifies", DesignAbstraction::MODIFIES},
    {"Uses", DesignAbstraction::USES},
    {"Calls", DesignAbstraction::CALLS},
    {"Calls*", DesignAbstraction::CALLS_T},
    {"Next", DesignAbstraction::NEXT},
    {"Next*", DesignAbstraction::NEXT_T},
    {"Affects", DesignAbstraction::AFFECTS},
    {"Affects*", DesignAbstraction::AFFECTS_T},
    {"NextBip", DesignAbstraction::NEXTBIP},
    {"NextBip*", DesignAbstraction::NEXTBIP_T},
    {"AffectsBip", DesignAbstraction::AFFECTSBIP},
    {"AffectsBip*", DesignAbstraction::AFFECTSBIP_T}};

std::map<ClauseType, std::string> QueryUtils::clause_type_to_string_mappings = {
    {ClauseType::SUCHTHAT, "such that"},
    {ClauseType::PATTERN, "pattern"},
    {ClauseType::WITH, "with"}};

std::map<std::string, AttributeType> QueryUtils::synonym_attribute_type_mappings = {
    {"procName", AttributeType::PROC_NAME},
    {"varName", AttributeType::VAR_NAME},
    {"value", AttributeType::VALUE},
    {"stmt#", AttributeType::STMT_NO}};
}  // namespace query_processor
