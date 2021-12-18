
#include "QueryEvaluator.h"

#include <iostream>
#include <stdexcept>

#include "query_processor/commons/BooleanSemanticError.h"
#include "query_processor/commons/query/clause/SuchThatClause.h"
#include "query_processor/commons/query/entities/DesignEntityType.h"
#include "query_processor/commons/query/entities/SelectedEntity.h"
#include "query_processor/commons/query/utils/QueryUtils.h"
#include "query_processor/query_evaluator/utils/QueryEvaluatorUtils.h"
#include "query_processor/query_optimizer/QueryOptimizer.h"

namespace query_processor {

PKB empty_pkb = PKB();
PKB* QueryEvaluator::pkb = &empty_pkb;

const std::string LEFT_KEY = "LEFT";
const std::string RIGHT_KEY = "RIGHT";

/* Query Optimizer Flags
 * Note: If the optimize_query flag is set to false when calling EvaluateQuery, all optimizations will be disabled. */

const bool REMOVE_DUPLICATE_CLAUSE = true;    // Removes duplicated clauses of the same type, same design abstraction and same params.
const bool SORT_CLAUSES = true;               // Sorts clauses based on number of synonyms, followed by type of clause (With Clause < Such That Clause < Pattern Clause)
const bool GROUP_BEFORE_MERGE = true;         // Groups Result Tables based on overlapping synonyms. Clauses with no related synonyms will not be merged.
const bool SORT_TABLES_BEFORE_BFS = true;     // Sorts result tables based on table size before BFS. BFS will start from the smallest table.

// default: false for this optimisation
const bool SORT_TABLES_BEFORE_MERGE = false;  // During BFS, sort each node's neighbours based on table size. Merging will start from the smallest table.

void QueryEvaluator::SetPKB(PKB* input_pkb) {
  pkb = input_pkb;
}
/**
 * Evaluates a Query object based on the input PKB. The EvaluateQuery function first evaluates each clause independently.
 * A ResultTable containing data for any design entity evaluated is created for each clause and stored in
 * the database. If any of the clauses evaluate to false independently, the function terminates and returns false or
 * an empty QueryResult(). If not, all tables in the database will be merged into a single table or merged by the Query
 * Optimizer and any selected design entity, attribute or tuple will either be retrieved from the table or from the PKB
 * (if it was never evaluated).
 * @param query A Query object should minimally house at least one Selected Entity in a list of Selected Entities,
 * as well as an optional list of Clauses
 * @param input_pkb The current state of the PKB populated with the Design Abstractions from the SIMPLE source code.
 * @param optimize_query Set flag to true if the query should be optimized
 * @return QueryResult object, which either stores an unordered set of string or integers, or a boolean.
 */
QueryResult QueryEvaluator::EvaluateQuery(Query query, PKB* input_pkb, bool optimize_query) {
  if (optimize_query) {
    query = QueryOptimizer::OptimizeQuery(query, REMOVE_DUPLICATE_CLAUSE, SORT_CLAUSES);
  }
  SetPKB(input_pkb);
  Database database;
  std::vector<SelectedEntity> selected_entities = query.GetSelectedEntities();
  bool is_tuple = selected_entities.size() > 1;
  bool is_boolean_result = selected_entities.size() == 1 && query.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN;

  std::vector<Clause> clause_list = query.GetClauseList();
  Column result;

  try {
    // Evaluation of clauses here. If any of the clauses are false, return empty QueryResult or false.
    for (Clause clause : clause_list) {
      ClauseType clause_type = clause.GetClauseType();
      bool is_clause_true;
      switch (clause_type) {
        case ClauseType::SUCHTHAT:
          is_clause_true = EvaluateSuchThatClause(clause.GetSuchThatClause(), database);
          break;
        case ClauseType::PATTERN:
          is_clause_true = EvaluatePatternClause(clause.GetPatternClause(), database);
          break;
        case ClauseType::WITH:
          is_clause_true = EvaluateWithClause(clause.GetWithClause(), database);
          break;
        default:
          throw std::runtime_error("Invalid clause type");
      }
      if (!is_clause_true) {
        if (is_boolean_result) {
          return QueryResult(false);
        } else {
          return QueryResult();
        }
      }
    }
  } catch (const std::runtime_error& error) {
    // Errors thrown in evaluation of queries are semantic errors, return empty result or FALSE
    if (is_boolean_result) {
      throw BooleanSemanticError(error.what(), 0, 0);
    } else {
      throw error;
    }
  }

  std::vector<ResultTable> result_tables;
  if (optimize_query && GROUP_BEFORE_MERGE) {
    result_tables = QueryOptimizer::OptimizeMerging(database, SORT_TABLES_BEFORE_BFS, SORT_TABLES_BEFORE_MERGE);
  } else {
    // Inner join all tables on their intersecting headers trivially
    ResultTable final_table = ResultTable();
    for (ResultTable table : database) {
      final_table = table.MergeTable(final_table);
    }
    result_tables.push_back(final_table);
  }

  // Evaluation of selection
  bool is_not_null_result = true;
  for (auto& table : result_tables) {
    is_not_null_result = is_not_null_result && (!table.IsEmpty() && table.GetHeight() != 0);
  }
  bool all_clauses_return_true = database.empty() || is_not_null_result;

  if (is_tuple && all_clauses_return_true) {
    return SelectTuple(selected_entities, result_tables);
  }

  SelectedEntity selected_entity = query.GetSelectedEntity();
  switch (selected_entity.entity_type) {
    case SelectedEntityType::BOOLEAN:
      return QueryResult(all_clauses_return_true);

    case SelectedEntityType::DESIGN_ENTITY:
      if (all_clauses_return_true) {
        DesignEntity design_entity = selected_entity.design_entity;
        result = GetSmallestDesignEntitySet(design_entity, result_tables);
        return QueryEvaluatorUtils::ConvertColumnToQueryResult(result);
      } else {
        // Design entities were evaluated in the process but the resulting columns are all empty.
        return QueryResult();
      }

    case SelectedEntityType::ATTRIBUTE:
      if (all_clauses_return_true) {
        DesignEntity design_entity = selected_entity.attribute.first;
        Column design_entity_column = GetSmallestDesignEntitySet(design_entity, result_tables);
        for (auto elem : design_entity_column) {
          result.push_back(ConvertToAttribute(elem, design_entity.GetDesignEntityType(),
                                              selected_entity.attribute.second));
        }
        return QueryEvaluatorUtils::ConvertColumnToQueryResult(result);
      }
    default:
      throw std::runtime_error("Invalid selected entity type");
  }
}

QueryResult QueryEvaluator::SelectTuple(std::vector<SelectedEntity>& selected_entities, Database& result_tables) {
  ResultTable tuple_table;
  for (auto& table : result_tables) {
    ResultTable temp_table;
    for (auto& entity : selected_entities) {
      if (entity.entity_type != SelectedEntityType::DESIGN_ENTITY && entity.entity_type != SelectedEntityType::ATTRIBUTE) {
        throw std::runtime_error("Incorrect entity type within a tuple.");
      }

      // Obtain the relevant design entity for the synonym or the attribute
      DesignEntity de;
      if (entity.entity_type == SelectedEntityType::DESIGN_ENTITY) {
        de = entity.design_entity;
      } else {
        de = entity.attribute.first;
      }

      // If the design entity is not in this result table, look in the next one.
      if (!table.Contains(de.GetSynonym())) {
        continue;
      }

      // Obtain key for the column based on whether it is an attribute or a design entity.
      std::string key;
      if (entity.entity_type == SelectedEntityType::ATTRIBUTE) {
        key = QueryEvaluatorUtils::GetAttributeKey(entity.attribute.second, de);
      } else {
        key = de.GetSynonym();
      }

      // If the selected entity was previously already selected, move on.
      if (temp_table.Contains(key)) {
        continue;
      }

      // Add the column to the temporary table without cross product since all columns came from the same table.
      Column de_col = table.GetColumn(de.GetSynonym());
      if (entity.entity_type == SelectedEntityType::ATTRIBUTE) {
        Column attribute_col;
        for (auto elem : de_col) {
          attribute_col.push_back(ConvertToAttribute(elem, de.GetDesignEntityType(), entity.attribute.second));
        }
        temp_table.AddColumn(key, attribute_col);
      } else {
        temp_table.AddColumn(key, de_col);
      }
    }
    tuple_table = tuple_table.MergeTable(temp_table);
  }

  for (auto& entity : selected_entities) {
    if (entity.entity_type != SelectedEntityType::DESIGN_ENTITY && entity.entity_type != SelectedEntityType::ATTRIBUTE) {
      throw std::runtime_error("Incorrect entity type within a tuple.");
    }

    DesignEntity de;
    if (entity.entity_type == SelectedEntityType::DESIGN_ENTITY) {
      de = entity.design_entity;
    } else {
      de = entity.attribute.first;
    }

    std::string key;
    if (entity.entity_type == SelectedEntityType::ATTRIBUTE) {
      key = QueryEvaluatorUtils::GetAttributeKey(entity.attribute.second, de);
    } else {
      key = de.GetSynonym();
    }

    // Selected Entity was not in any table
    if (!tuple_table.Contains(key)) {
      Column selected_col = GetDesignEntityTable(de.GetDesignEntityType());
      if (entity.entity_type == SelectedEntityType::ATTRIBUTE) {
        Column attribute_col;
        for (auto elem : selected_col) {
          attribute_col.push_back(
              ConvertToAttribute(elem, de.GetDesignEntityType(), entity.attribute.second));
        }
        selected_col = attribute_col;
      }
      if (tuple_table.Contains(de.GetSynonym()) ||
          tuple_table.Contains(QueryEvaluatorUtils::GetAttributeKey(AttributeType::INTEGER, de)) ||
          tuple_table.Contains(QueryEvaluatorUtils::GetAttributeKey(AttributeType::NAME, de))) {
        tuple_table.AddColumn(key, selected_col);
      } else {
        tuple_table = tuple_table.MergeColumn(key, selected_col);
      }
    }
  }
  return QueryEvaluatorUtils::ConvertResultTableToTupleResult(tuple_table, selected_entities);
}

bool QueryEvaluator::ApplyPKBFunction(TableElement& lhs, TableElement& rhs, DesignAbstraction da) {
  switch (da) {
    case DesignAbstraction::FOLLOWS:
      return pkb->IsFollows(lhs.stmt, rhs.stmt);
    case DesignAbstraction::FOLLOWS_T:
      return pkb->IsFollowsT(lhs.stmt, rhs.stmt);
    case DesignAbstraction::PARENT:
      return pkb->IsParent(lhs.stmt, rhs.stmt);
    case DesignAbstraction::PARENT_T:
      return pkb->IsParentT(lhs.stmt, rhs.stmt);
    case DesignAbstraction::USES:
      if (lhs.type == QueryResultType::STMTS) {
        return pkb->IsUses(lhs.stmt, rhs.name);
      } else {
        return pkb->IsUses(lhs.name, rhs.name);
      }
    case DesignAbstraction::MODIFIES:
      if (lhs.type == QueryResultType::STMTS) {
        return pkb->IsModifies(lhs.stmt, rhs.name);
      } else {
        return pkb->IsModifies(lhs.name, rhs.name);
      }
    case DesignAbstraction::CALLS:
      return pkb->IsCalls(lhs.name, rhs.name);
    case DesignAbstraction::CALLS_T:
      return pkb->IsCallsT(lhs.name, rhs.name);
    case DesignAbstraction::NEXT:
      return pkb->IsNext(lhs.stmt, rhs.stmt);
    case DesignAbstraction::NEXT_T:
      return pkb->IsNextT(lhs.stmt, rhs.stmt);
    case DesignAbstraction::NEXTBIP:
      return pkb->IsNextBip(lhs.stmt, rhs.stmt);
    case DesignAbstraction::NEXTBIP_T:
      return pkb->IsNextBipT(lhs.stmt, rhs.stmt);
    case DesignAbstraction::AFFECTS:
      return pkb->IsAffects(lhs.stmt, rhs.stmt);
    case DesignAbstraction::AFFECTS_T:
      return pkb->IsAffectsT(lhs.stmt, rhs.stmt);
    case DesignAbstraction::AFFECTSBIP:
      return pkb->IsAffectsBip(lhs.stmt, rhs.stmt);
    case DesignAbstraction::AFFECTSBIP_T:
      return pkb->IsAffectsBipT(lhs.stmt, rhs.stmt);
    default:
      return true;
  }
}

bool QueryEvaluator::EvaluateSuchThatWildcardClause(SuchThatClause& clause) {
  DesignAbstraction design_abstraction = clause.GetDesignAbstraction();
  DesignEntityType de_type = QueryEvaluatorUtils::ConvertAbstractionToWildcardType(design_abstraction);
  Column de_col = GetDesignEntityTable(de_type);
  for (auto lhs_elem : de_col) {
    for (auto rhs_elem : de_col) {
      if (ApplyPKBFunction(lhs_elem, rhs_elem, design_abstraction)) {
        return true;
      }
    }
  }
  return false;
}

bool QueryEvaluator::EvaluateSuchThatClause(SuchThatClause& clause, Database& database) {
  if (!clause.IsValidClause()) {
    return false;
  }
  DesignAbstraction design_abstraction = clause.GetDesignAbstraction();
  ClauseParam lhs_param = clause.GetLHSParam();
  ClauseParam rhs_param = clause.GetRHSParam();

  if (QueryEvaluatorUtils::IsDesignAbstractionWithNoSimilarParams(design_abstraction)) {
    // The above design abstractions can evaluate to true for some clauses with similar parameters
    if (IsSimilarParams(lhs_param, rhs_param)) {
      return false;
    }
  }

  if (IsWildcardParams(lhs_param, rhs_param)) {
    return EvaluateSuchThatWildcardClause(clause);
  }

  ResultTable clause_param_table = ConvertClauseToResultTable(lhs_param, rhs_param,
                                                              QueryEvaluatorUtils::ConvertAbstractionToWildcardType(design_abstraction),
                                                              database);
  int table_height = clause_param_table.GetHeight();
  Column& lhs_col = clause_param_table.GetColumn(LEFT_KEY);
  Column& rhs_col = clause_param_table.GetColumn(RIGHT_KEY);
  Column lhs_valid;
  Column rhs_valid;
  for (int i = 0; i < table_height; i++) {
    if (ApplyPKBFunction(lhs_col.at(i), rhs_col.at(i), design_abstraction)) {
      lhs_valid.push_back(lhs_col.at(i));
      rhs_valid.push_back(rhs_col.at(i));
    }
  }

  ResultTable result_table = GenerateTable(lhs_param, rhs_param, lhs_valid, rhs_valid);
  if (result_table.IsEmpty()) {
    // No design entities were involved in the process
    return !lhs_valid.empty() || !rhs_valid.empty();
  } else {
    database.push_back(result_table);
    return result_table.GetHeight() != 0;
  }
}

bool QueryEvaluator::EvaluateConditionalPatternClause(PatternClause& clause, Database& database) {
  DesignEntity de = clause.GetDesignEntity();
  ClauseParam pattern_param = ClauseParam(de);
  ClauseParam lhs_param = clause.GetLHSParam();
  ResultTable clause_param_table = ConvertClauseToResultTable(pattern_param, lhs_param,
                                                              QueryEvaluatorUtils::ConvertAbstractionToWildcardType(DesignAbstraction::MODIFIES),
                                                              database);
  int table_height = clause_param_table.GetHeight();
  Column& lhs_col = clause_param_table.GetColumn(LEFT_KEY);
  Column& rhs_col = clause_param_table.GetColumn(RIGHT_KEY);
  Column pattern_valid;
  Column var_valid;
  for (int i = 0; i < table_height; i++) {
    std::unordered_set<std::string> vars_used_by_conditional;
    if (de.GetDesignEntityType() == DesignEntityType::WHILE) {
      vars_used_by_conditional = pkb->GetVariablesUsedByWhileStmt(lhs_col.at(i).stmt);
    } else {
      vars_used_by_conditional = pkb->GetVariablesUsedByIfStmt(lhs_col.at(i).stmt);
    }
    if (vars_used_by_conditional.find(rhs_col.at(i).name) != vars_used_by_conditional.end()) {
      pattern_valid.push_back(lhs_col.at(i));
      var_valid.push_back(rhs_col.at(i));
    }
  }
  ResultTable result_table = GenerateTable(pattern_param, lhs_param, pattern_valid, var_valid);
  database.push_back(result_table);
  return result_table.GetHeight() != 0;
}

bool QueryEvaluator::EvaluatePatternClause(PatternClause& clause, Database& database) {
  if (!clause.IsValidClause()) {
    return false;
  }

  DesignEntity de = clause.GetDesignEntity();
  ClauseParam pattern_param = ClauseParam(de);
  ClauseParam lhs_param = clause.GetLHSParam();
  ClauseParam rhs_param = clause.GetRHSParam();

  if (de.GetDesignEntityType() != DesignEntityType::ASSIGN) {
    return EvaluateConditionalPatternClause(clause, database);
  }

  if (IsWildcardParams(lhs_param, rhs_param)) {
    Column de_col = ConvertClauseParamToColumn(pattern_param, de.GetDesignEntityType(), database);
    return !de_col.empty();
  }

  // Evaluate Modifies(pattern_param, lhs_param)
  ResultTable clause_param_table = ConvertClauseToResultTable(pattern_param, lhs_param,
                                                              QueryEvaluatorUtils::ConvertAbstractionToWildcardType(DesignAbstraction::MODIFIES),
                                                              database);
  int table_height = clause_param_table.GetHeight();
  Column& lhs_col = clause_param_table.GetColumn(LEFT_KEY);
  Column& rhs_col = clause_param_table.GetColumn(RIGHT_KEY);
  Column pattern_valid;
  Column var_valid;
  for (int i = 0; i < table_height; i++) {
    if (ApplyPKBFunction(lhs_col.at(i), rhs_col.at(i), DesignAbstraction::MODIFIES)) {
      pattern_valid.push_back(lhs_col.at(i));
      var_valid.push_back(rhs_col.at(i));
    }
  }

  // Generate ResultTable based on previous evaluation
  ResultTable result_table = GenerateTable(pattern_param, lhs_param, pattern_valid, var_valid);
  if (rhs_param.param_type == ClauseParamType::WILDCARD) {
    database.push_back(result_table);
    return result_table.GetHeight() != 0;
  }

  ResultTable updated_table = ResultTable(result_table.GetHeaders());
  if (rhs_param.param_type == ClauseParamType::EXPR) {
    std::unordered_set<int> assign_stmts_match_expr;
    if (rhs_param.pattern_expr.is_wild_card) {
      assign_stmts_match_expr = pkb->GetAllAssignStmtsThatContains(rhs_param.pattern_expr.token_list);
    } else {
      assign_stmts_match_expr = pkb->GetAllAssignStmtsThatMatches(rhs_param.pattern_expr.token_list);
    }
    for (int i = 0; i < result_table.GetHeight(); i++) {
      // if the assignment statement in the current ResultTable fulfills the expression
      if (assign_stmts_match_expr.find(pattern_valid.at(i).stmt) != assign_stmts_match_expr.end()) {
        updated_table.AddRow(result_table.GetRowAt(i));
      }
    }
  } else {
    throw std::runtime_error("RHS param in pattern clause cannot be handled");
  }
  database.push_back(updated_table);
  return updated_table.GetHeight() != 0;
}

bool QueryEvaluator::EvaluateWithClause(WithClause& clause, Database& database) {
  if (!clause.IsValidClause()) {
    return false;
  }

  ClauseParam lhs_param = clause.GetLHSParam();
  ClauseParam rhs_param = clause.GetRHSParam();
  AttributeType lhs_attr_type = clause.GetLHSAttributeType();
  AttributeType rhs_attr_type = clause.GetRHSAttributeType();
  if (IsSimilarParams(lhs_param, rhs_param)) {
    Column param_col = ConvertClauseParamToColumn(lhs_param, DesignEntityType::WILDCARD, database);
    return !param_col.empty();
  }

  // A wildcard design entity type is used as a placeholder here since there aren't actually wildcards in with clauses.
  ResultTable clause_param_table = ConvertClauseToResultTable(lhs_param, rhs_param, DesignEntityType::WILDCARD,
                                                              database);
  int table_height = clause_param_table.GetHeight();
  Column& lhs_col = clause_param_table.GetColumn(LEFT_KEY);
  Column& rhs_col = clause_param_table.GetColumn(RIGHT_KEY);
  Column lhs_valid;
  Column rhs_valid;

  for (int i = 0; i < table_height; i++) {
    TableElement lhs_attr = ConvertToAttribute(lhs_col.at(i), lhs_param, lhs_attr_type);
    TableElement rhs_attr = ConvertToAttribute(rhs_col.at(i), rhs_param, rhs_attr_type);
    if (lhs_attr == rhs_attr) {
      lhs_valid.push_back(lhs_col.at(i));
      rhs_valid.push_back(rhs_col.at(i));
    }
  }

  ResultTable result_table = GenerateTable(lhs_param, rhs_param, lhs_valid, rhs_valid);
  if (result_table.IsEmpty()) {
    // No design entities were involved in the process
    return !lhs_valid.empty() || !rhs_valid.empty();
  } else {
    database.push_back(result_table);
    return result_table.GetHeight() != 0;
  }
}
bool QueryEvaluator::IsSimilarParams(ClauseParam& lhs_param, ClauseParam& rhs_param) {
  if (lhs_param == rhs_param) {
    if (lhs_param.param_type != ClauseParamType::WILDCARD) {
      return true;
    }
  }
  return false;
}

bool QueryEvaluator::IsWildcardParams(ClauseParam& lhs_param, ClauseParam& rhs_param) {
  if (lhs_param.param_type == ClauseParamType::WILDCARD && rhs_param.param_type == ClauseParamType::WILDCARD) {
    return true;
  }
  return false;
}

/*
 * Generates a Table based on their params and their respective columns. The columns are only added to the table if
 * their corresponding parameter is a DesignEntity with a synonym.
 */
ResultTable QueryEvaluator::GenerateTable(ClauseParam& lhs_param, ClauseParam& rhs_param, Column& lhs_column, Column& rhs_column) {
  ClauseParamType lhs_type = lhs_param.param_type;
  ClauseParamType rhs_type = rhs_param.param_type;
  ResultTable new_table;
  if (lhs_type == ClauseParamType::DESIGN_ENTITY && rhs_type == ClauseParamType::DESIGN_ENTITY) {
    new_table.AddColumn(lhs_param.design_entity.GetSynonym(), lhs_column);
    new_table.AddColumn(rhs_param.design_entity.GetSynonym(), rhs_column);
  } else if (lhs_type == ClauseParamType::DESIGN_ENTITY) {
    lhs_column = QueryEvaluatorUtils::RemoveDuplicateTableElements(lhs_column);
    new_table.AddColumn(lhs_param.design_entity.GetSynonym(), lhs_column);
  } else if (rhs_type == ClauseParamType::DESIGN_ENTITY) {
    rhs_column = QueryEvaluatorUtils::RemoveDuplicateTableElements(rhs_column);
    new_table.AddColumn(rhs_param.design_entity.GetSynonym(), rhs_column);
  }
  return new_table;
}

Column QueryEvaluator::GetSmallestDesignEntitySet(DesignEntity& de, ResultTable& result_table) {
  std::string synonym = de.GetSynonym();
  if (result_table.Contains(synonym)) {
    return result_table.GetColumn(synonym);
  } else {
    return GetDesignEntityTable(de.GetDesignEntityType());
  }
}

Column QueryEvaluator::GetSmallestDesignEntitySet(DesignEntity& de, Database& database) {
  std::string synonym = de.GetSynonym();
  for (auto iter = database.rbegin(); iter != database.rend(); ++iter) {
    // Iterate through database from the back to search for ResultTables with the synonym.
    ResultTable& table = *iter;
    if (table.Contains(synonym)) {
      return table.GetColumn(synonym);
    }
  }
  // Synonym not present in database, retrieve design entity from PKB
  return GetDesignEntityTable(de.GetDesignEntityType());
}

TableElement QueryEvaluator::ConvertToAttribute(TableElement& elem, ClauseParam& param, AttributeType attr) {
  if (param.param_type != ClauseParamType::DESIGN_ENTITY) {
    // Integers and names should remain as it is.
    return elem;
  }

  DesignEntityType de_type = param.design_entity.GetDesignEntityType();
  return ConvertToAttribute(elem, de_type, attr);
}

TableElement QueryEvaluator::ConvertToAttribute(TableElement& elem, DesignEntityType de_type, AttributeType attr) {
  if (attr == AttributeType::STMT_NO || attr == AttributeType::VALUE ||
      attr == AttributeType::INTEGER || attr == AttributeType::NAME ||
      attr == AttributeType::NONE) {
    // All the above attribute types already correspond to their original data type.
    return elem;
  }
  switch (de_type) {
    case DesignEntityType::PROCEDURE:
    case DesignEntityType::VARIABLE:
      return elem;
    case DesignEntityType::CALL:
      return TableElement(pkb->GetCallsProcName(elem.stmt));
    case DesignEntityType::PRINT:
      return TableElement(pkb->GetPrintVarName(elem.stmt));
    case DesignEntityType::READ:
      return TableElement(pkb->GetReadVarName(elem.stmt));
    default:
      throw std::runtime_error("There should not be other design entity types with AttributeType NAME");
  }
}

Column QueryEvaluator::ConvertClauseParamToColumn(ClauseParam& clause_param, DesignEntityType wildcard_type, Database& database) {
  switch (clause_param.param_type) {
    case ClauseParamType::DESIGN_ENTITY: {
      return GetSmallestDesignEntitySet(clause_param.design_entity, database);
    }
    case ClauseParamType::WILDCARD: {
      return GetDesignEntityTable(wildcard_type);
    }
    case ClauseParamType::INDEX: {
      Column index_result;
      index_result.push_back(TableElement(clause_param.statement_index));
      return index_result;
    }
    case ClauseParamType::NAME: {
      Column name_result;
      name_result.push_back(TableElement({clause_param.var_proc_name}));
      return name_result;
    }
    case ClauseParamType::EXPR: {
      throw std::runtime_error("An expression ClauseParam cannot be converted to a Column.");
    }
    default:
      throw std::runtime_error("Invalid ClauseParam.");
  }
}

/*
 * If there already exists a table with both params inside, we return the two columns.
 * If not, we will retrieve each column individually (either from table or PKB) and cross product the two columns.
 */
ResultTable QueryEvaluator::ConvertClauseToResultTable(ClauseParam& lhs_param, ClauseParam& rhs_param, DesignEntityType wildcard_type, Database& database) {
  Column lhs_col;
  Column rhs_col;
  ResultTable final_table;
  for (int i = 0; i < database.size(); i++) {
    ResultTable& table = database.at(i);
    if (table.Contains(lhs_param) && table.Contains(rhs_param)) {
      lhs_col = table.GetColumn(lhs_param);
      rhs_col = table.GetColumn(rhs_param);
      final_table.AddColumn(LEFT_KEY, lhs_col);
      final_table.AddColumn(RIGHT_KEY, rhs_col);
      database.erase(database.begin() + i);
      return final_table;
    }
  }

  lhs_col = ConvertClauseParamToColumn(lhs_param, wildcard_type, database);
  rhs_col = ConvertClauseParamToColumn(rhs_param, wildcard_type, database);
  if (IsSimilarParams(lhs_param, rhs_param)) {
    // This is specially for Next and Affects where it is possible to have similar parameters
    final_table.AddColumn(LEFT_KEY, lhs_col);
    final_table.AddColumn(RIGHT_KEY, rhs_col);
    return final_table;
  }
  // Since only one column is being retrieved, it's converted to a set to remove any duplicate elements
  // resulting from previous cross products
  lhs_col = QueryEvaluatorUtils::RemoveDuplicateTableElements(lhs_col);
  rhs_col = QueryEvaluatorUtils::RemoveDuplicateTableElements(rhs_col);
  ResultTable lhs_table;
  ResultTable rhs_table;
  lhs_table.AddColumn(LEFT_KEY, lhs_col);
  rhs_table.AddColumn(RIGHT_KEY, rhs_col);
  final_table = lhs_table.CrossTable(rhs_table);

  return final_table;
}

/*
 * Gets the full set that corresponds to the DesignEntity from the PKB and returns them as a Column.
 */
Column QueryEvaluator::GetDesignEntityTable(DesignEntityType entity) {
  Column design_entity_col;
  switch (entity) {
    case DesignEntityType::STMT:
    case DesignEntityType::PROG_LINE:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllStmts());
      break;
    case DesignEntityType::VARIABLE:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllVariables());
      break;
    case DesignEntityType::ASSIGN:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllAssignStmts());
      break;
    case DesignEntityType::CALL:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllCallsStmts());
      break;
    case DesignEntityType::CONSTANT:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllConstants());
      break;
    case DesignEntityType::PRINT:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllPrintStmts());
      break;
    case DesignEntityType::WHILE:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllWhileStmts());
      break;
    case DesignEntityType::IF:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllIfStmts());
      break;
    case DesignEntityType::PROCEDURE:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllProcedures());
      break;
    case DesignEntityType::READ:
      design_entity_col = QueryEvaluatorUtils::ConvertSetToColumn(pkb->GetAllReadStmts());
      break;
    default:
      throw std::runtime_error("Invalid design entity");
  }
  return design_entity_col;
}

}  // namespace query_processor
