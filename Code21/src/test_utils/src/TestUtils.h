#pragma once

#include <unordered_set>
#include <vector>

#include "design_extractor/utils/CFGHandler.h"
#include "query_processor/commons/query/entities/DesignEntityType.h"
#include "query_processor/query_evaluator/ResultTable.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"

bool IsSimilarToken(const source_processor::Token&, const source_processor::Token&);
bool HasSimilarTokens(source_processor::TokenList, source_processor::TokenList);
bool IsSimilarTNode(const source_processor::TNode&, const source_processor::TNode&);
bool IsSimilarAST(const source_processor::TNode&, const source_processor::TNode&);
bool IsSimilarCFG(const design_extractor::CFG& g1, const design_extractor::CFG& g2);
bool IsSimilarColumn(std::vector<query_processor::TableElement>, std::vector<query_processor::TableElement>);
template <typename T>
bool ContainsExactly(std::unordered_set<T> set, std::vector<T> expected_vals);
bool Contains(std::unordered_set<std::string>, std::string);
bool Contains(std::unordered_set<int>, int);
bool Contains(std::list<std::string>, std::string);
bool Contains(std::vector<std::string>, std::string);
bool Contains(std::vector<int>, int);
bool Contains(std::vector<query_processor::TableElement>, int);
bool Contains(std::vector<std::vector<std::string>>, std::vector<std::string>);
query_processor::ResultTable CreateSingleColumnResultTable(std::string, std::unordered_set<int>);
query_processor::ResultTable CreateSingleColumnResultTable(std::string, std::unordered_set<std::string>);