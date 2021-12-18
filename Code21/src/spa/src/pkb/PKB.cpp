#include "PKB.h"

bool PKB::InsertVariable(const std::string& variable) {
  return var_table.Insert(variable);
}

bool PKB::InsertStatement(int stmt_index) {
  return stmt_table.Insert(stmt_index);
}

bool PKB::InsertConstant(int constant) {
  return const_table.Insert(constant);
}

bool PKB::InsertIf(int stmt_index, const std::vector<std::string>& variable_list) {
  return container_table.InsertIf(stmt_index, variable_list);
}

bool PKB::InsertWhile(int stmt_index, const std::vector<std::string>& variable_list) {
  return container_table.InsertWhile(stmt_index, variable_list);
}

bool PKB::InsertRead(int stmt_index, const std::string& var_name) {
  return read_table.Insert(stmt_index, var_name);
}

bool PKB::InsertPrint(int stmt_index, const std::string& var_name) {
  return print_table.Insert(stmt_index, var_name);
}

bool PKB::InsertCalls(int stmt_index, const std::string& proc_name) {
  return calls_table.InsertCalls(stmt_index, proc_name);
}

bool PKB::InsertCalls(const std::string& caller, const std::string& callee) {
  return calls_table.InsertCalls(caller, callee);
}

bool PKB::InsertCallsT(const std::string& caller, const std::string& callee) {
  return calls_table.InsertCallsT(caller, callee);
}

bool PKB::InsertAssignment(int stmt_index, const std::string& assigned_var, const source_processor::TokenList& token_list) {
  return assign_table.InsertAssign(stmt_index, assigned_var, token_list);
}

bool PKB::InsertProcedure(const std::string& proc_name, int start_index, int end_index) {
  return proc_table.InsertProc(proc_name, std::pair<int, int>(start_index, end_index));
}

bool PKB::InsertEntity(int stmt_index, const std::string& entity) {
  return entity_table.InsertEntity(stmt_index, entity);
}

bool PKB::InsertFollows(int stmt1, int stmt2) {
  return follows_table.InsertFollows(stmt1, stmt2);
}

bool PKB::InsertFollowsT(int stmt1, int stmt2) {
  return follows_T_table.InsertFollowsT(stmt1, stmt2);
}

bool PKB::InsertParent(int stmt1, int stmt2) {
  return parent_table.InsertParent(stmt1, stmt2);
}

bool PKB::InsertParentT(int stmt1, int stmt2) {
  return parent_T_table.InsertParentT(stmt1, stmt2);
}

bool PKB::InsertModifies(int stmt_index, const std::string& variable) {
  return modifies_table.InsertStmtModifies(stmt_index, variable);
}

bool PKB::InsertModifies(const std::string& proc_name, const std::string& variable) {
  return modifies_table.InsertProcModifies(proc_name, variable);
}

bool PKB::InsertUses(int stmt_index, const std::string& variable) {
  return uses_table.InsertStmtUses(stmt_index, variable);
}

bool PKB::InsertUses(const std::string& proc_name, const std::string& variable) {
  return uses_table.InsertProcUses(proc_name, variable);
}

bool PKB::InsertNext(int prog_line1, int prog_line2) {
  return next_table.InsertNext(prog_line1, prog_line2);
}

bool PKB::InsertNextT(int prog_line1, int prog_line2) {
  return next_table.InsertNextT(prog_line1, prog_line2);
}

bool PKB::IsNext(int prog_line1, int prog_line2) {
  return next_table.IsNext(prog_line1, prog_line2);
}

bool PKB::IsNextT(int prog_line1, int prog_line2) {
  return next_table.IsNextT(prog_line1, prog_line2);
}

bool PKB::IsFollows(int stmt1, int stmt2) {
  return follows_table.IsFollows(stmt1, stmt2);
}

bool PKB::IsFollowsT(int stmt1, int stmt2) {
  return follows_T_table.IsFollowsT(stmt1, stmt2);
}

bool PKB::IsParent(int stmt1, int stmt2) {
  return parent_table.IsParent(stmt1, stmt2);
}

bool PKB::IsParentT(int stmt1, int stmt2) {
  return parent_T_table.IsParentT(stmt1, stmt2);
}

bool PKB::IsModifies(int stmt_index, const std::string& variable) {
  return modifies_table.IsStmtModifies(stmt_index, variable);
}

bool PKB::IsModifies(const std::string& proc_name, const std::string& variable) {
  return modifies_table.IsProcModifies(proc_name, variable);
}

bool PKB::IsUses(int stmt_index, const std::string& variable) {
  return uses_table.IsStmtUses(stmt_index, variable);
}

bool PKB::IsUses(const std::string& proc_name, const std::string& variable) {
  return uses_table.IsProcUses(proc_name, variable);
}

std::unordered_set<std::string> PKB::GetVariablesUsedByIfStmt(int stmt_index) {
  return container_table.GetVariablesUsedByIfStmt(stmt_index);
}

std::string PKB::GetCallsProcName(int stmt_index) {
  return calls_table.GetCalledProcedure(stmt_index);
}

std::string PKB::GetPrintVarName(int stmt_index) {
  if (!print_table.Contains(stmt_index)) {
    return "";
  }
  return print_table.Get(stmt_index);
}

std::string PKB::GetReadVarName(int stmt_index) {
  if (!read_table.Contains(stmt_index)) {
    return "";
  }
  return read_table.Get(stmt_index);
}

std::unordered_set<std::string> PKB::GetVariablesUsedByWhileStmt(int stmt_index) {
  return container_table.GetVariablesUsedByWhileStmt(stmt_index);
}

std::pair<int, int> PKB::GetProcRange(const std::string& proc_name) {
  return proc_table.GetProcRange(proc_name);
}

bool PKB::IsCalls(const std::string& caller, const std::string& callee) {
  return calls_table.IsCalls(caller, callee);
}

bool PKB::IsCallsT(const std::string& caller, const std::string& callee) {
  return calls_table.IsCallsT(caller, callee);
}

int PKB::GetStmtFollowedBy(int stmt2) {
  return follows_table.GetStmtFollowedBy(stmt2);
}

std::unordered_set<int> PKB::GetStmtsFollowedTBy(int stmt2) {
  return follows_T_table.GetStmtsFollowedTBy(stmt2);
}

int PKB::GetStmtFollows(int stmt1) {
  return follows_table.GetStmtFollows(stmt1);
}

std::unordered_set<int> PKB::GetStmtsFollowsT(int stmt1) {
  return follows_T_table.GetStmtsFollowsT(stmt1);
}

std::unordered_set<int> PKB::GetAllFollowsStmts() {
  return follows_table.GetAllFollowsStmts();
}

std::unordered_set<int> PKB::GetAllFollowsTStmts() {
  return follows_T_table.GetAllFollowsTStmts();
}

std::unordered_set<int> PKB::GetAllFollowedStmts() {
  return follows_table.GetAllFollowedStmts();
}

std::unordered_set<int> PKB::GetAllFollowedTStmts() {
  return follows_T_table.GetAllFollowedTStmts();
}

int PKB::GetParentStatement(int stmt2) {
  return parent_table.GetParentStatement(stmt2);
}

std::unordered_set<int> PKB::GetChildrenStatements(int stmt1) {
  return parent_table.GetChildrenStatements(stmt1);
}

std::unordered_set<int> PKB::GetParentTStatements(int stmt2) {
  return parent_T_table.GetParentTStatements(stmt2);
}

std::unordered_set<int> PKB::GetChildrenTStatements(int stmt1) {
  return parent_T_table.GetChildrenTStatements(stmt1);
}

std::unordered_set<int> PKB::GetAllParentStmts() {
  return parent_table.GetAllParentStmts();
}

std::unordered_set<int> PKB::GetAllParentTStmts() {
  return parent_T_table.GetAllParentTStmts();
}

std::unordered_set<int> PKB::GetAllChildrenStmts() {
  return parent_table.GetAllChildrenStmts();
}

std::unordered_set<int> PKB::GetAllChildrenTStmts() {
  return parent_T_table.GetAllChildrenTStmts();
}

std::unordered_set<std::string> PKB::GetModifiedVariables(int stmt_index) {
  return modifies_table.GetModifiedStmtVariables(stmt_index);
}

std::unordered_set<std::string> PKB::GetModifiedVariables(const std::string& proc_name) {
  return modifies_table.GetModifiedProcVariables(proc_name);
}

std::unordered_set<int> PKB::GetModifiesStatements(const std::string& variable) {
  return modifies_table.GetModifiesStatements(variable);
}

std::unordered_set<std::string> PKB::GetModifiesProcedures(const std::string& variable) {
  return modifies_table.GetModifiesProcedures(variable);
}

std::unordered_set<int> PKB::GetAllModifiesStatements() {
  return modifies_table.GetAllModifiesStatements();
}

std::unordered_set<std::string> PKB::GetAllModifiesProcedures() {
  return modifies_table.GetAllModifiesProcedures();
}

std::unordered_set<std::string> PKB::GetUsedVariables(int stmt_index) {
  return uses_table.GetUsedStmtVariables(stmt_index);
}

std::unordered_set<std::string> PKB::GetUsedVariables(const std::string& proc_name) {
  return uses_table.GetUsedProcVariables(proc_name);
}

std::unordered_set<int> PKB::GetUsesStatements(const std::string& variable) {
  return uses_table.GetUsesStatements(variable);
}

std::unordered_set<std::string> PKB::GetUsesProcedures(const std::string& variable) {
  return uses_table.GetUsesProcedures(variable);
}

std::unordered_set<int> PKB::GetAllUsesStatements() {
  return uses_table.GetAllUsesStatements();
}

std::unordered_set<std::string> PKB::GetAllUsesProcedures() {
  return uses_table.GetAllUsesProcedures();
}

std::string PKB::GetAssignedVariable(int stmt_index) {
  return assign_table.GetAssignedVariable(stmt_index);
}

std::unordered_set<int> PKB::GetAllAssignStmtsThatMatches(source_processor::TokenList& rhs_expression) {
  return assign_table.GetAllAssignStmtsThatMatches(rhs_expression);
}

std::unordered_set<int> PKB::GetAllAssignStmtsThatContains(source_processor::TokenList& rhs_expression) {
  return assign_table.GetAllAssignStmtsThatContains(rhs_expression);
}

std::unordered_set<int> PKB::GetAllAssignStmtsThatModifies(const std::string& variable_string) {
  return assign_table.GetAllAssignStmtsThatModifies(variable_string);
}

std::string PKB::GetStatementType(int stmt_index) {
  return entity_table.GetStatementType(stmt_index);
}

std::unordered_set<int> PKB::GetAllStmtsOfType(const std::string& entity) {
  return entity_table.GetAllStmtsOfType(entity);
}

std::unordered_set<std::string> PKB::GetAllVariables() {
  return var_table.GetAll();
}

std::unordered_set<int> PKB::GetAllStmts() {
  return stmt_table.GetAll();
}

std::unordered_set<int> PKB::GetAllConstants() {
  return const_table.GetAll();
}

std::unordered_set<int> PKB::GetAllIfStmts() {
  return container_table.GetAllIfStmts();
}

std::unordered_set<int> PKB::GetAllWhileStmts() {
  return container_table.GetAllWhileStmts();
}

std::unordered_set<int> PKB::GetAllReadStmts() {
  return read_table.GetAllKeys();
}

std::unordered_set<int> PKB::GetAllPrintStmts() {
  return print_table.GetAllKeys();
}

std::unordered_set<int> PKB::GetAllCallsStmts() {
  return calls_table.GetAllCallsStmts();
}

std::unordered_set<std::string> PKB::GetProceduresThatCalls(const std::string& proc_name) {
  return calls_table.GetProceduresThatCalls(proc_name);
}

std::unordered_set<std::string> PKB::GetProceduresCalled(const std::string& proc_name) {
  return calls_table.GetProceduresCalled(proc_name);
}

std::unordered_set<std::string> PKB::GetAllProceduresThatCalls() {
  return calls_table.GetAllCallsProcedures();
}

std::unordered_set<std::string> PKB::GetAllProceduresCalled() {
  return calls_table.GetAllCalledProcedures();
}

std::unordered_set<std::string> PKB::GetProceduresThatCallsT(const std::string& proc_name) {
  return calls_table.GetProceduresThatCallsT(proc_name);
}

std::unordered_set<std::string> PKB::GetProceduresCalledT(const std::string& proc_name) {
  return calls_table.GetProceduresCalledT(proc_name);
}

std::unordered_set<std::string> PKB::GetAllProceduresThatCallsT() {
  return calls_table.GetAllCallsTProcedures();
}

std::unordered_set<std::string> PKB::GetAllProceduresCalledT() {
  return calls_table.GetAllCalledTProcedures();
}

std::unordered_set<int> PKB::GetStmtIndexesThatCalls(const std::string& proc_name) {
  return calls_table.GetCallsStmtIndexes(proc_name);
}

std::unordered_set<int> PKB::GetAllAssignStmts() {
  return assign_table.GetAllAssignStmts();
}

std::unordered_set<std::string> PKB::GetAllProcedures() {
  return proc_table.GetAllProcedures();
}

std::unordered_set<std::string> PKB::GetAllEntities() {
  return entity_table.GetAllEntities();
}

std::unordered_set<int> PKB::GetAllNextStatements() {
  return next_table.GetAllNextStatements();
}

std::unordered_set<int> PKB::GetAllPreviousStatements() {
  return next_table.GetAllPreviousStatements();
}

std::unordered_set<int> PKB::GetAllPreviousTStatements() {
  return next_table.GetAllPreviousTStatements();
}

std::unordered_set<int> PKB::GetAllNextTStatements() {
  return next_table.GetAllNextTStatements();
}

std::unordered_set<int> PKB::GetNextStatements(int prog_line) {
  return next_table.GetNextStatements(prog_line);
}

std::unordered_set<int> PKB::GetPreviousStatements(int prog_line) {
  return next_table.GetPreviousStatements(prog_line);
}

std::unordered_set<int> PKB::GetNextTStatements(int prog_line) {
  return next_table.GetNextTStatements(prog_line);
}

std::unordered_set<int> PKB::GetPreviousTStatements(int prog_line) {
  return next_table.GetPreviousTStatements(prog_line);
}

bool PKB::InsertAffects(int assign_stmt1, int assign_stmt2) {
  return affects_table.InsertAffects(assign_stmt1, assign_stmt2);
}

bool PKB::IsAffects(int assign_stmt1, int assign_stmt2) {
  return affects_table.IsAffects(assign_stmt1, assign_stmt2);
}

std::unordered_set<int> PKB::GetStatementsThatAffects(int assign_stmt2) {
  return affects_table.GetStatementsThatAffects(assign_stmt2);
}

std::unordered_set<int> PKB::GetAffectedStatements(int assign_stmt1) {
  return affects_table.GetAffectedStatements(assign_stmt1);
}

std::unordered_set<int> PKB::GetAllStatementsThatAffects() {
  return affects_table.GetAllStatementsThatAffects();
}

std::unordered_set<int> PKB::GetAllAffectedStatements() {
  return affects_table.GetAllAffectedStatements();
}

bool PKB::InsertAffectsT(int assign_stmt1, int assign_stmt2) {
  return affects_table.InsertAffectsT(assign_stmt1, assign_stmt2);
}

bool PKB::IsAffectsT(int assign_stmt1, int assign_stmt2) {
  return affects_table.IsAffectsT(assign_stmt1, assign_stmt2);
}

std::unordered_set<int> PKB::GetStatementsThatAffectsT(int assign_stmt2) {
  return affects_table.GetStatementsThatAffectsT(assign_stmt2);
}

std::unordered_set<int> PKB::GetAffectedTStatements(int assign_stmt1) {
  return affects_table.GetAffectedTStatements(assign_stmt1);
}

std::unordered_set<int> PKB::GetAllStatementsThatAffectsT() {
  return affects_table.GetAllStatementsThatAffectsT();
}

std::unordered_set<int> PKB::GetAllAffectedTStatements() {
  return affects_table.GetAllAffectedTStatements();
}

bool PKB::InsertNextBip(int prog_line1, int prog_line2) {
  return nextbip_table.InsertNextBip(prog_line1, prog_line2);
}

bool PKB::InsertNextBipT(int prog_line1, int prog_line2) {
  return nextbip_table.InsertNextBipT(prog_line1, prog_line2);
}

bool PKB::IsNextBip(int prog_line1, int prog_line2) {
  return nextbip_table.IsNextBip(prog_line1, prog_line2);
}

bool PKB::IsNextBipT(int prog_line1, int prog_line2) {
  return nextbip_table.IsNextBipT(prog_line1, prog_line2);
}

std::unordered_set<int> PKB::GetAllNextBipStatements() {
  return nextbip_table.GetAllNextBipStatements();
}

std::unordered_set<int> PKB::GetAllPreviousBipStatements() {
  return nextbip_table.GetAllPreviousBipStatements();
}

std::unordered_set<int> PKB::GetAllPreviousBipTStatements() {
  return nextbip_table.GetAllPreviousBipTStatements();
}

std::unordered_set<int> PKB::GetAllNextBipTStatements() {
  return nextbip_table.GetAllNextBipTStatements();
}

std::unordered_set<int> PKB::GetNextBipStatements(int prog_line) {
  return nextbip_table.GetNextBipStatements(prog_line);
}

std::unordered_set<int> PKB::GetPreviousBipStatements(int prog_line) {
  return nextbip_table.GetPreviousBipStatements(prog_line);
}

std::unordered_set<int> PKB::GetNextBipTStatements(int prog_line) {
  return nextbip_table.GetNextBipTStatements(prog_line);
}

std::unordered_set<int> PKB::GetPreviousBipTStatements(int prog_line) {
  return nextbip_table.GetPreviousBipTStatements(prog_line);
}

bool PKB::InsertAffectsBip(int assign_stmt1, int assign_stmt2) {
  return affects_bip_table.InsertAffectsBip(assign_stmt1, assign_stmt2);
}

bool PKB::IsAffectsBip(int assign_stmt1, int assign_stmt2) {
  return affects_bip_table.IsAffectsBip(assign_stmt1, assign_stmt2);
}

std::unordered_set<int> PKB::GetStatementsThatAffectsBip(int assign_stmt2) {
  return affects_bip_table.GetStatementsThatAffectsBip(assign_stmt2);
}

std::unordered_set<int> PKB::GetAffectedBipStatements(int assign_stmt1) {
  return affects_bip_table.GetAffectedBipStatements(assign_stmt1);
}

std::unordered_set<int> PKB::GetAllStatementsThatAffectsBip() {
  return affects_bip_table.GetAllStatementsThatAffectsBip();
}

std::unordered_set<int> PKB::GetAllAffectedBipStatements() {
  return affects_bip_table.GetAllAffectedBipStatements();
}

bool PKB::InsertAffectsBipT(int assign_stmt1, int assign_stmt2) {
  return affects_bip_table.InsertAffectsBipT(assign_stmt1, assign_stmt2);
}

bool PKB::IsAffectsBipT(int assign_stmt1, int assign_stmt2) {
  return affects_bip_table.IsAffectsBipT(assign_stmt1, assign_stmt2);
}

std::unordered_set<int> PKB::GetStatementsThatAffectsBipT(int assign_stmt2) {
  return affects_bip_table.GetStatementsThatAffectsBipT(assign_stmt2);
}

std::unordered_set<int> PKB::GetAffectedBipTStatements(int assign_stmt1) {
  return affects_bip_table.GetAffectedBipTStatements(assign_stmt1);
}

std::unordered_set<int> PKB::GetAllStatementsThatAffectsBipT() {
  return affects_bip_table.GetAllStatementsThatAffectsBipT();
}

std::unordered_set<int> PKB::GetAllAffectedBipTStatements() {
  return affects_bip_table.GetAllAffectedBipTStatements();
}

void PKB::ClearAllTables() {
  var_table.ClearTable();
  stmt_table.ClearTable();
  const_table.ClearTable();
  container_table.ClearTable();
  read_table.ClearTable();
  print_table.ClearTable();
  entity_table.ClearEntityTable();
  assign_table.ClearAssignTable();
  proc_table.ClearProcTable();
  parent_table.ClearParentTable();
  parent_T_table.ClearParentTTable();
  follows_table.ClearFollowsTable();
  follows_T_table.ClearFollowsTTable();
  modifies_table.ClearModifiesTable();
  uses_table.ClearUsesTable();
  calls_table.ClearCallsTable();
  next_table.ClearNextTable();
  affects_table.ClearAffectsTable();
  nextbip_table.ClearNextBipTable();
  affects_bip_table.ClearAffectsBipTable();
}