#pragma once

#include <map>
#include <string>
#include <vector>

#include "query_processor/commons/query/Query.h"

namespace query_processor {

class QueryParser {
 public:
  static Query ParseQuery(std::string&);
  static Query GenerateQuery(std::string&);

 private:
  static void ParseDeclarationClauses(std::string&, std::map<std::string, DesignEntityType>&);
  static Query ParseSelectPhrase(std::string&, std::map<std::string, DesignEntityType>&);
  static Query ParseConditionalClauses(std::string&, std::map<std::string, DesignEntityType>&, Query);
  static SuchThatClause ParseSuchThatClause(std::string&, std::map<std::string, DesignEntityType>&);
  static PatternClause ParsePatternClause(std::string&, std::map<std::string, DesignEntityType>&);
  static WithClause ParseWithClause(std::string&, std::map<std::string, DesignEntityType>&);
  static std::pair<DesignEntity, AttributeType> CreateAttributeSelectedEntityFromString(std::string&, std::map<std::string, DesignEntityType>&);
  static ClauseParam CreateClauseParamFromString(std::string&, std::map<std::string, DesignEntityType>&);
  static ClauseParam CreatePatternExpressionClauseParamFromString(std::string&);
  static std::pair<ClauseParam, AttributeType> CreateWithClauseParamFromString(std::string&, std::map<std::string, DesignEntityType>&);
  static std::pair<std::string, AttributeType> ParseSynonymAttribute(std::string&, std::map<std::string, DesignEntityType>&);
  static std::string& ReplaceKeywordAnd(std::string&, ClauseType);
  static void ThrowSemanticError(std::string);
  static void ResetBooleans();
  static bool is_boolean_query;
  static bool is_semantic_error;
  static bool has_declaration_error;
};

}  // namespace query_processor