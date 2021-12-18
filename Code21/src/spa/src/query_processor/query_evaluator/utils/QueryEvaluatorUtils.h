#pragma once

#include <string>
#include <unordered_set>

#include "../ResultTable.h"
#include "query_processor/commons/query/clause/DesignAbstraction.h"
#include "query_processor/commons/query/entities/SelectedEntity.h"
#include "query_processor/commons/query_result/QueryResult.h"

namespace query_processor {

class QueryEvaluatorUtils {
 public:
  static Column ConvertSetToColumn(const std::unordered_set<int>&);
  static Column ConvertSetToColumn(const std::unordered_set<std::string>&);
  static Column RemoveDuplicateTableElements(const Column&);
  static std::string GetAttributeKey(AttributeType, DesignEntity& de);
  static QueryResult ConvertColumnToQueryResult(const Column&);
  static QueryResult ConvertResultTableToTupleResult(ResultTable&, std::vector<SelectedEntity>);
  static DesignEntityType ConvertAbstractionToWildcardType(DesignAbstraction);
  static bool IsDesignAbstractionWithNoSimilarParams(DesignAbstraction);
};
}  // namespace query_processor
