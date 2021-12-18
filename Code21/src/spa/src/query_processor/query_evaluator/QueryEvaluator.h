#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ResultTable.h"
#include "pkb/PKB.h"
#include "query_processor/commons/query/Query.h"
#include "query_processor/commons/query/clause/PatternClause.h"
#include "query_processor/commons/query/clause/SuchThatClause.h"
#include "query_processor/commons/query/clause/WithClause.h"
#include "query_processor/commons/query/entities/DesignEntity.h"
#include "query_processor/commons/query_result/QueryResult.h"

namespace query_processor {
typedef std::vector<ResultTable> Database;

class QueryEvaluator {
 protected:
  static PKB* pkb;

 public:
  static QueryResult EvaluateQuery(Query, PKB*, bool);
  static void SetPKB(PKB*);
  static bool EvaluatePatternClause(PatternClause&, Database&);
  static bool EvaluateSuchThatClause(SuchThatClause&, Database&);
  static bool EvaluateWithClause(WithClause&, Database&);

 private:
  static QueryResult SelectTuple(std::vector<SelectedEntity>&, Database&);
  static bool EvaluateConditionalPatternClause(PatternClause&, Database&);
  static bool EvaluateSuchThatWildcardClause(SuchThatClause&);
  static Column GetSmallestDesignEntitySet(DesignEntity&, ResultTable&);
  static Column GetSmallestDesignEntitySet(DesignEntity&, Database&);
  static Column GetDesignEntityTable(DesignEntityType);
  static Column ConvertClauseParamToColumn(ClauseParam&, DesignEntityType, Database&);
  static TableElement ConvertToAttribute(TableElement&, ClauseParam&, AttributeType);
  static TableElement ConvertToAttribute(TableElement&, DesignEntityType, AttributeType);
  static bool IsSimilarParams(ClauseParam&, ClauseParam&);
  static bool IsWildcardParams(ClauseParam&, ClauseParam&);
  static bool ApplyPKBFunction(TableElement&, TableElement&, DesignAbstraction);
  static ResultTable GenerateTable(ClauseParam&, ClauseParam&, Column&, Column&);
  static ResultTable ConvertClauseToResultTable(ClauseParam&, ClauseParam&, DesignEntityType, Database&);
};
}  // namespace query_processor