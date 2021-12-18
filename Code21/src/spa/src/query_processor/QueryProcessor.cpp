#include "QueryProcessor.h"

#include <iostream>
#include <stdexcept>

#include "query_processor/commons/BooleanSemanticError.h"
#include "query_processor/commons/query/Query.h"
#include "query_processor/query_evaluator/QueryEvaluator.h"
#include "query_processor/query_parser/QueryParser.h"
#include "query_processor/query_projector/QueryProjector.h"

namespace query_processor {

QueryProcessor::QueryProcessor() {}

std::list<std::string> QueryProcessor::ProcessQuery(std::string query_string, PKB& pkb) {
  Query query = QueryParser::ParseQuery(query_string);
  QueryResult query_result = QueryEvaluator::EvaluateQuery(query, &pkb, true);
  return QueryProjector::FormatResult(query_result);
}

}  // namespace query_processor