#pragma once

#include <map>
#include <string>
#include <typeinfo>

#include "query_processor/commons/query/clause/AttributeType.h"
#include "query_processor/commons/query/clause/ClauseParam.h"
#include "query_processor/commons/query/clause/ClauseType.h"
#include "query_processor/commons/query/clause/DesignAbstraction.h"
#include "query_processor/commons/query/entities/DesignEntityType.h"

namespace query_processor {

class QueryUtils {
 public:
  static DesignEntityType ConvertStringToDesignEntityType(std::string);
  static std::string ConvertDesignEntityTypeToString(DesignEntityType);
  static DesignAbstraction ConvertStringToDesignAbstraction(std::string);
  static AttributeType ConvertStringToAttributeType(std::string);
  static std::string ConvertClauseTypeToString(ClauseType);
  static bool IsStatementEntity(ClauseParam&, bool);
  static bool IsAssignEntity(ClauseParam&, bool);
  static bool IsProgLineEntity(ClauseParam&, bool);
  static bool IsProcedureEntity(ClauseParam&, bool);
  static bool IsVariableEntity(ClauseParam&, bool);
  static bool IsValidAttributeType(DesignEntity&, AttributeType);
  static int RankDesignAbstraction(DesignAbstraction);

 private:
  static std::map<std::string, DesignEntityType> string_to_design_entity_type_mappings;
  static std::map<DesignEntityType, std::string> design_entity_type_to_string_mappings;
  static std::map<std::string, DesignAbstraction> design_abstraction_mappings;
  static std::map<ClauseType, std::string> clause_type_to_string_mappings;
  static std::map<std::string, AttributeType> synonym_attribute_type_mappings;
  static std::map<DesignAbstraction, int> design_abstraction_rank;
};
}  // namespace query_processor