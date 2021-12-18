#include "QueryEvaluatorUtils.h"

#include <stdexcept>

namespace query_processor {

Column QueryEvaluatorUtils::ConvertSetToColumn(const std::unordered_set<int>& stmts) {
  Column result;
  for (auto stmt : stmts) {
    result.push_back(TableElement(stmt));
  }
  return result;
}

Column QueryEvaluatorUtils::ConvertSetToColumn(const std::unordered_set<std::string>& names) {
  Column result;
  for (const auto& name : names) {
    result.push_back(TableElement(name));
  }
  return result;
}

Column QueryEvaluatorUtils::RemoveDuplicateTableElements(const Column& column) {
  if (column.empty()) {
    return column;
  }
  if (column.front().type == QueryResultType::STMTS) {
    std::unordered_set<int> elem_set;
    for (auto& elem : column) {
      if (elem.type != QueryResultType::STMTS) {
        throw std::runtime_error("Invalid table column with mixed element types.");
      }
      elem_set.insert(elem.stmt);
    }
    return ConvertSetToColumn(elem_set);
  } else {
    std::unordered_set<std::string> elem_set;
    for (auto& elem : column) {
      if (elem.type != QueryResultType::NAMES) {
        throw std::runtime_error("Invalid table column with mixed element types.");
      }
      elem_set.insert(elem.name);
    }
    return ConvertSetToColumn(elem_set);
  }
}

std::string QueryEvaluatorUtils::GetAttributeKey(AttributeType attr, DesignEntity& de) {
  std::string synonym = de.GetSynonym();
  if (attr == AttributeType::STMT_NO || attr == AttributeType::VALUE || attr == AttributeType::INTEGER) {
    return synonym + ".INTEGER";
  }
  if (attr == AttributeType::VAR_NAME || attr == AttributeType::PROC_NAME || attr == AttributeType::NAME) {
    return synonym + ".NAME";
  }
  return "";
}
QueryResult QueryEvaluatorUtils::ConvertColumnToQueryResult(const Column& column) {
  QueryResult result;
  std::unordered_set<int> stmts;
  std::unordered_set<std::string> names;
  for (TableElement elem : column) {
    if (elem.type == QueryResultType::STMTS) {
      stmts.insert(elem.stmt);
    }
    if (elem.type == QueryResultType::NAMES) {
      names.insert(elem.name);
    }
  }
  if (!(stmts.empty() || names.empty())) {
    throw std::runtime_error("Both statements and names have values.");
  }

  if (stmts.empty()) {
    result = QueryResult(names);
  }
  if (names.empty()) {
    result = QueryResult(stmts);
  }
  return result;
}

QueryResult QueryEvaluatorUtils::ConvertResultTableToTupleResult(ResultTable& final_table, std::vector<SelectedEntity> selected_synonyms) {
  std::vector<std::string> relevant_columns;
  relevant_columns.reserve(selected_synonyms.size());
  for (SelectedEntity entity : selected_synonyms) {
    std::string key = "";
    if (entity.entity_type == SelectedEntityType::ATTRIBUTE) {
      key = GetAttributeKey(entity.attribute.second, entity.attribute.first);
    }
    if (entity.entity_type == SelectedEntityType::DESIGN_ENTITY) {
      key = entity.design_entity.GetSynonym();
    }
    relevant_columns.push_back(key);
  }

  std::vector<std::vector<std::string>> final_list;

  for (int i = 0; i < final_table.GetHeight(); i++) {
    std::vector<std::string> row_tuple;
    for (const std::string& key : relevant_columns) {
      TableElement elem = final_table.GetColumn(key).at(i);
      if (elem.type == QueryResultType::NAMES) {
        row_tuple.push_back(elem.name);
      } else if (elem.type == QueryResultType::STMTS) {
        row_tuple.push_back(std::to_string(elem.stmt));
      }
    }
    final_list.push_back(row_tuple);
  }
  QueryResult result = QueryResult(final_list);
  return result;
}

DesignEntityType QueryEvaluatorUtils::ConvertAbstractionToWildcardType(DesignAbstraction abstraction) {
  switch (abstraction) {
    case DesignAbstraction::FOLLOWS:
    case DesignAbstraction::FOLLOWS_T:
    case DesignAbstraction::PARENT:
    case DesignAbstraction::PARENT_T:
    case DesignAbstraction::NEXT:
    case DesignAbstraction::NEXT_T:
    case DesignAbstraction::NEXTBIP:
    case DesignAbstraction::NEXTBIP_T:
      return DesignEntityType::STMT;
    case DesignAbstraction::USES:
    case DesignAbstraction::MODIFIES:
      return DesignEntityType::VARIABLE;
    case DesignAbstraction::CALLS:
    case DesignAbstraction::CALLS_T:
      return DesignEntityType::PROCEDURE;
    case DesignAbstraction::AFFECTS:
    case DesignAbstraction::AFFECTS_T:
    case DesignAbstraction::AFFECTSBIP:
    case DesignAbstraction::AFFECTSBIP_T:
      return DesignEntityType::ASSIGN;
    default:
      throw std::runtime_error("Design abstraction cannot have wildcard.");
  }
}

bool QueryEvaluatorUtils::IsDesignAbstractionWithNoSimilarParams(DesignAbstraction abstraction) {
  switch (abstraction) {
    case DesignAbstraction::FOLLOWS:
    case DesignAbstraction::FOLLOWS_T:
    case DesignAbstraction::PARENT:
    case DesignAbstraction::PARENT_T:
    case DesignAbstraction::USES:
    case DesignAbstraction::MODIFIES:
    case DesignAbstraction::CALLS:
    case DesignAbstraction::CALLS_T:
    case DesignAbstraction::NEXT:
    case DesignAbstraction::NEXTBIP:
      return true;
    default:
      return false;
  }
}
}  // namespace query_processor