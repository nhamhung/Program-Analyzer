#pragma once

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "pkb/abstraction_tables/AffectsBipTable.h"
#include "pkb/abstraction_tables/AffectsTable.h"
#include "pkb/abstraction_tables/CallsTable.h"
#include "pkb/abstraction_tables/FollowsTTable.h"
#include "pkb/abstraction_tables/FollowsTable.h"
#include "pkb/abstraction_tables/ModifiesTable.h"
#include "pkb/abstraction_tables/NextBipTable.h"
#include "pkb/abstraction_tables/NextTable.h"
#include "pkb/abstraction_tables/ParentTTable.h"
#include "pkb/abstraction_tables/ParentTable.h"
#include "pkb/abstraction_tables/UsesTable.h"
#include "pkb/entity_tables/AssignTable.h"
#include "pkb/entity_tables/ContainerTable.h"
#include "pkb/entity_tables/EntityTable.h"
#include "pkb/entity_tables/ProcTable.h"
#include "pkb/templates/Table.h"
#include "pkb/templates/TableSingle.h"
#include "source_processor/token/TokenList.h"

class PKB {
 private:
  Table<std::string> var_table;
  Table<int> stmt_table;
  Table<int> const_table;
  TableSingle<int, std::string> read_table;
  TableSingle<int, std::string> print_table;
  ContainerTable container_table;
  EntityTable entity_table;
  AssignTable assign_table;
  ProcTable proc_table;
  FollowsTable follows_table;
  FollowsTTable follows_T_table;
  ParentTable parent_table;
  ParentTTable parent_T_table;
  ModifiesTable modifies_table;
  UsesTable uses_table;
  CallsTable calls_table;
  NextTable next_table;
  AffectsTable affects_table;
  NextBipTable nextbip_table;
  AffectsBipTable affects_bip_table;

 public:
  PKB(){};

  /* ----------------------------------- All APIs related to Variables ----------------------------------- */

  /**
   * Inserts a variable into var_table
   * @params string var_name
   * @return bool
   */
  bool InsertVariable(const std::string &);

  /**
   * Gets all variables stored in var_table
   * @params
   * @return unordered_set<string> of var_name
   */
  virtual std::unordered_set<std::string> GetAllVariables();

  /* ----------------------------------- All APIs related to Statements ----------------------------------- */

  /**
   * Inserts a statement into stmt_table
   * @params int stmt_index
   * @return bool
   */
  bool InsertStatement(int);

  /**
   * Gets all statements stored in stmt_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllStmts();

  /* ----------------------------------- All APIs related to Constant ----------------------------------- */

  /**
   * Inserts a constant into const_table
   * @params int constant
   * @return bool
   */
  bool InsertConstant(int);

  /**
   * Gets all constants stored in const_table
   * @params
   * @return unordered_set<int> of constants
   */
  virtual std::unordered_set<int> GetAllConstants();

  /* ----------------------------------- All APIs related to If ----------------------------------- */

  /**
   * Inserts an if statement into if_table
   * @params int stmt_index
   * @return bool
   */
  bool InsertIf(int stmt_index, const std::vector<std::string> &variable_list);

  /**
   * Gets all if statements stored in container_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllIfStmts();

  /**
   * Get all variables used in the conditional block of an If statement
   * @int stmt_index of If
   * @return unordered_set<string> of variables
   */
  virtual std::unordered_set<std::string> GetVariablesUsedByIfStmt(int);

  /* ----------------------------------- All APIs related to While ----------------------------------- */

  /**
   * Inserts a while statement into while_table
   * @params int stmt_index
   * @return bool
   */
  bool InsertWhile(int stmt_index, const std::vector<std::string> &variable_list);

  /**
   * Gets all while statements stored in container_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllWhileStmts();

  /**
   * Get all variables used in the conditional block of a While statement
   * @int stmt_index of While
   * @return unordered_set<string> of variables
   */
  virtual std::unordered_set<std::string> GetVariablesUsedByWhileStmt(int);

  /* ----------------------------------- All APIs related to Read ----------------------------------- */

  /**
   * Inserts a read statement into read_table
   * @params int stmt_index, string var_name
   * @return bool
   */
  bool InsertRead(int, const std::string &);

  /**
   * Get the variable name read in a given Read statement index
   * @params int stmt_index
   * @return string variable
   */
  virtual std::string GetReadVarName(int);

  /**
   * Gets all read statements stored in read_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllReadStmts();

  /* ----------------------------------- All APIs related to Print ----------------------------------- */

  /**
   * Inserts a print statement into print_table
   * @params int stmt_index, string var_name
   * @return bool
   */
  bool InsertPrint(int, const std::string &);

  /**
   * Get the variable name printed in a given Print statement index
   * @params int stmt_index
   * @return string variable
   */
  virtual std::string GetPrintVarName(int);

  /**
   * Gets all print statements stored in print_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllPrintStmts();

  /* ----------------------------------- All APIs related to Assignment ----------------------------------- */

  /**
   * Inserts an assignment statement into assign_table
   * @params int stmt_index, string assigned_var, TokenList rhs_expression
   * @return bool
   */
  bool InsertAssignment(int, const std::string &, const source_processor::TokenList &);

  /**
   * Gets all assign statements stored in assign_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllAssignStmts();

  /**
   * Gets all assigned variables on LHS of assign statements stored in assign_table
   * @params
   * @return unordered_set<string> of var_name
   */
  std::unordered_set<std::string> GetAllAssignedVariables();

  /**
   * Gets the assigned variable on LHS of a given assign statement
   * @params
   * @return string var_name
   */
  std::string GetAssignedVariable(int);

  /**
   * Gets all assign statements that modifies a given variable
   * @params string var_name
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllAssignStmtsThatModifies(const std::string &);

  /**
   * Gets all assign statements whose RHS matches exactly with a given rhs_expression
   * @params TokenList rhs_expression
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllAssignStmtsThatMatches(source_processor::TokenList &);

  /**
   * Gets all assign statements whose RHS contains a given rhs_sub_expression
   * @params TokenList rhs_sub_expression
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllAssignStmtsThatContains(source_processor::TokenList &);

  /* ----------------------------------- All APIs related to Procedure ----------------------------------- */

  /**
   * Inserts a procedure into proc_table
   * @params string proc_name, int start_index, int end_index
   * @return bool
   */
  bool InsertProcedure(const std::string &, int, int);

  /**
   * Get the start and end statement indexes of a given procedure
   * @params string proc_name
   * @return pair(int, int) of start and end indexes
   */
  std::pair<int, int> GetProcRange(const std::string &);

  /**
   * Gets all procedures stored in proc_table
   * @params
   * @return unordered_set<string> of proc_name
   */
  virtual std::unordered_set<std::string> GetAllProcedures();

  /* ----------------------------------- All APIs related to Entity ----------------------------------- */

  /**
   * Inserts an entity type into entity_table
   * @params int stmt_index, string entity
   * @return bool
   */
  bool InsertEntity(int, const std::string &);

  /**
   * Gets all entity types stored in entity_table
   * @params
   * @return unordered_set<string> of entity
   */
  virtual std::unordered_set<std::string> GetAllEntities();

  /**
   * Get the entity type of a given statement
   * @params int stmt_index
   * @return string entity
   */
  virtual std::string GetStatementType(int);

  /**
   * Get all statements of a given entity type
   * @params string type
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllStmtsOfType(const std::string &);

  /* ----------------------------------- All APIs related to Follows/FollowsT ----------------------------------- */

  /**
   * Inserts a Follows relationship into follows_table
   * @params int stmt1, int stmt2
   * @return bool
   */
  bool InsertFollows(int, int);

  /**
   * Inserts a Follows* relationship into follows_T_table
   * @params int stmt1, int stmt2
   * @return bool
   */
  bool InsertFollowsT(int, int);

  /**
   * Checks if Follows(stmt1, stmt2) relationship holds
   * @params int stmt1, int stmt2
   * @return bool
   */
  virtual bool IsFollows(int, int);

  /**
   * Checks if Follows*(stmt1, stmt2) relationship holds
   * @params int stmt1, int stmt2
   * @return bool
   */
  virtual bool IsFollowsT(int, int);

  /**
   * Gets the statement stmt1 such that Follows(stmt1, stmt2) holds (stmt1 followed by stmt2)
   * @params int stmt2
   * @return int stmt1
   */
  virtual int GetStmtFollowedBy(int);

  /**
   * Gets the statement stmt2 such that Follows(stmt1, stmt2) holds (stmt2 follows stmt1)
   * @params int stmt1
   * @return int stmt2
   */
  virtual int GetStmtFollows(int);

  /**
   * Gets statements which are followed* by a given stmt_index
   * @params int stmt2
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetStmtsFollowedTBy(int);

  /**
   * Gets statements which follows* a given stmt_index
   * @params int stmt1
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetStmtsFollowsT(int);

  /**
   * Gets all statements which Follows some other statements stored in follows_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllFollowsStmts();

  /**
   * Gets all statements which are Followed by some other statements stored in follows_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  std::unordered_set<int> GetAllFollowedStmts();

  /**
   * Gets all statements which Follows* some other statements stored in follows_T_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllFollowsTStmts();

  /**
   * Gets all statements which are Followed* by some other statements stored in follows_T_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  std::unordered_set<int> GetAllFollowedTStmts();

  /* ----------------------------------- All APIs related to Parent/ParentT ----------------------------------- */

  /**
   * Inserts a Parent relationship into parent_table
   * @params int stmt1, int stmt2
   * @return bool
   */
  bool InsertParent(int, int);

  /**
   * Inserts a Parent* relationship into parent_T_table
   * @params int stmt1, int stmt2
   * @return bool
   */
  bool InsertParentT(int, int);

  /**
   * Checks if Parent(stmt1, stmt2) relationship holds
   * @params int stmt1, int stmt2
   * @return bool
   */
  virtual bool IsParent(int, int);

  /**
   * Checks if Parent*(stmt1, stmt2) relationship holds
   * @params int stmt1, int stmt2
   * @return bool
   */
  virtual bool IsParentT(int, int);

  /**
   * Get the parent statement of a given statement
   * @params int child
   * @return int parent
   */
  virtual int GetParentStatement(int);

  /**
   * Get all children statements of a given parent statement
   * @params int parent
   * @return unordered_set<int> of children_index
   */
  virtual std::unordered_set<int> GetChildrenStatements(int);

  /**
   * Get all Parent* statements of a given child statement
   * @params int child
   * @return unordered_set<int> of parent*_index
   */
  virtual std::unordered_set<int> GetParentTStatements(int);

  /**
   * Get all children statements of a given Parent* statement
   * @params int parent*
   * @return unordered_set<int> of children_index
   */
  virtual std::unordered_set<int> GetChildrenTStatements(int);

  /**
   * Get all statements which are Parent to some other statements
   * @params
   * @return unordered_set<int> parent_index
   */
  virtual std::unordered_set<int> GetAllParentStmts();

  /**
   * Get all statements which are Parent* to some other statements
   * @params
   * @return unordered_set<int> of parent*_index
   */
  virtual std::unordered_set<int> GetAllParentTStmts();

  /**
   * Get all children statements of some Parent statements
   * @params
   * @return unordered_set<int> of child_index
   */
  std::unordered_set<int> GetAllChildrenStmts();

  /**
   * Get all children statements of some Parent* statements
   * @params
   * @return unordered_set<int> of child_index
   */
  std::unordered_set<int> GetAllChildrenTStmts();

  /* ----------------------------------- All APIs related to Modifies ----------------------------------- */

  /**
   * Inserts a Modifies(statement, variable) relationship into modifies_table
   * @params int stmt_index, string var_name
   * @return bool
   */
  bool InsertModifies(int, const std::string &);

  /**
   * Inserts a Modifies(procedure, variable) relationship into modifies_table
   * @params string proc_name, string var_name
   * @return bool
   */
  bool InsertModifies(const std::string &, const std::string &);

  /**
   * Checks if Modifies(stmt_index, var_name) relationship holds
   * @params int stmt_index, string var_name
   * @return bool
   */
  virtual bool IsModifies(int, const std::string &);

  /**
   * Check if Modifies(proc_name, var_name) relationship holds
   * @params string proc_name, string var_name
   * @return bool
   */

  virtual bool IsModifies(const std::string &, const std::string &);

  /**
   * Get all variables which are modified by a given statement
   * @params int stmt_index
   * @return unordered_set<string> of var_name
   */
  virtual std::unordered_set<std::string> GetModifiedVariables(int);

  /**
   * Get all variables which are modified by a given procedure
   * @params string proc_name
   * @return unordered_set<string> of var_name
   */
  virtual std::unordered_set<std::string> GetModifiedVariables(const std::string &);

  /**
   * Get all statements which Modifies a given variable
   * @params string var_name
   * @return unordered_set<int> of stmt_index
   */
  std::unordered_set<int> GetModifiesStatements(const std::string &);

  /**
   * Get all procedures which Modifies a given variable
   * @params string proc_name
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetModifiesProcedures(const std::string &);

  /**
   * Get all Modifies statements stored in modifies_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  std::unordered_set<int> GetAllModifiesStatements();

  /**
   * Get all Modifies procedures stored in modifies_table
   * @params
   * @return unordered_set<std::string> of proc_name
   */
  std::unordered_set<std::string> GetAllModifiesProcedures();

  /* ----------------------------------- All APIs related to Uses ----------------------------------- */

  /**
   * Inserts a Uses(statement, variable) relationship into uses_table
   * @params int stmt_index, string var_name
   * @return bool
   */
  bool InsertUses(int, const std::string &);

  /**
   * Checks if Uses(stmt_index, var_name) relationship holds
   * @params int stmt_index, string var_name
   * @return bool
   */
  virtual bool IsUses(int, const std::string &);

  /**
   * Check if Uses(proc_name, var_name) relationship holds
   * @params string proc_name, string var_name
   * @return bool
   */
  virtual bool IsUses(const std::string &, const std::string &);

  /**
   * Get all variables which are used by a given statement
   * @params int stmt_index
   * @return unordered_set<string> of var_name
   */
  virtual std::unordered_set<std::string> GetUsedVariables(int);

  /**
   * Get all variables which are used by a given procedure
   * @params string proc_name
   * @return unordered_set<string> of var_name
   */
  virtual std::unordered_set<std::string> GetUsedVariables(const std::string &);

  /**
   * Get all statements which Uses a given variable
   * @params string var_name
   * @return unordered_set<int> of stmt_index
   */
  std::unordered_set<int> GetUsesStatements(const std::string &);

  /**
   * Get all procedures which Uses a given variable
   * @params string var_name
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetUsesProcedures(const std::string &);

  /**
   * Get all Uses statements stored in uses_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  std::unordered_set<int> GetAllUsesStatements();

  /**
   * Get all Uses procedures stored in uses_table
   * @params
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetAllUsesProcedures();

  /**
   * Insert a Uses(procedure, variable) relationship into uses_table
   * @params string proc_name, string var_name
   * @return bool
   */
  bool InsertUses(const std::string &, const std::string &);

  /* ----------------------------------- All APIs related to Calls/CallsT ----------------------------------- */

  /**
   * Inserts a stmt_index and proc_name into calls_stmt_table
   * @params int stmt_index, string proc_name
   * @return bool
   */
  bool InsertCalls(int, const std::string &);

  /**
   * Get the procedure called in a given statement index
   * @params int stmt_index
   * @return string proc_name
   */
  virtual std::string GetCallsProcName(int);

  /**
   * Get all the statement indexes that call a given procedure
   * @params string proc_name
   * @return
   */
  std::unordered_set<int> GetStmtIndexesThatCalls(const std::string &proc_name);

  /**
   * Gets all call statements stored in calls_table
   * @params
   * @return unordered_set<int> of stmt_index
   */
  virtual std::unordered_set<int> GetAllCallsStmts();

  /**
   * Inserts a Call(caller, callee) relationship into calls_table
   * @params string caller, string callee
   * @return bool
   */
  bool InsertCalls(const std::string &, const std::string &);

  /**
   * Inserts a CallT(caller, callee) relationship into calls_T_table
   * @params string caller, string callee
   * @return bool
   */
  bool InsertCallsT(const std::string &, const std::string &);

  /**
   * Checks if Calls(caller, callee) relationship holds
   * @params string caller, string callee
   * @return bool
   */
  virtual bool IsCalls(const std::string &, const std::string &);

  /**
   * Checks if CallsT(caller, callee) relationship holds
   * @params string caller, string callee
   * @return bool
   */
  virtual bool IsCallsT(const std::string &, const std::string &);

  /**
   * Get procedures which Calls a given procedure
   * @params
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetProceduresThatCalls(const std::string &);

  /**
   * Get procedure which is Called by a given procedure
   * @params string proc_name
   * @return string proc_name
   */
  std::unordered_set<std::string> GetProceduresCalled(const std::string &);

  /**
   * Get all procedures which Calls another procedure
   * @params
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetAllProceduresThatCalls();

  /**
   * Get all procedures which are Called (Calls relationship) by another procedure
   * @params
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetAllProceduresCalled();

  /**
   * Get all procedures which CallsT a given procedure
   * @params string proc_name
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetProceduresThatCallsT(const std::string &);

  /**
   * Get all procedures which are CalledT (CallsT relationship) by a given procedure
   * @params string proc_name
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetProceduresCalledT(const std::string &);

  /**
   * Get all procedures which CallsT some other procedure
   * @params
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetAllProceduresThatCallsT();

  /**
   * Get all procedures which are CalledT by some other procedure
   * @params
   * @return unordered_set<string> of proc_name
   */
  std::unordered_set<std::string> GetAllProceduresCalledT();

  /* ----------------------------------- All APIs related to Next/NextT ----------------------------------- */

  /**
   * Inserts a Next(prog_line1, prog_line2) relationship into next_table
   * @params int prog_line1, int prog_line2
   * @return bool
   */
  bool InsertNext(int, int);

  /**
   * Inserts a NextT(prog_line1, prog_line2) relationship into next_table
   * @params int prog_line1, int prog_line2
   * @return bool
   */
  bool InsertNextT(int, int);

  /**
   * Check if Next(prog_line1, prog_line2) relationship holds
   * @params int prog_line1, int prog_line2
   * @return bool
   */
  virtual bool IsNext(int, int);

  /**
   * Check if NextT(prog_line1, prog_line2) relationship holds
   * @params int prog_line1, int prog_line2
   * @return bool
   */
  virtual bool IsNextT(int, int);

  /**
   * Get all program lines which are Next to a given program line
   * @params int prog_line
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetNextStatements(int);

  /**
   * Get all program lines which are Previous to a given program line
   * @params int prog_line
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetPreviousStatements(int);

  /**
   * Get all program lines which are NextT to a given program line
   * @params int prog_line
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetNextTStatements(int);

  /**
   * Get all program lines which are PreviousT to a given program line
   * @params int prog_line
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetPreviousTStatements(int);

  /**
   * Get all program lines which are Next to one or another program line
   * @param
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetAllNextStatements();

  /**
   * Get all program lines which are Previous to one or another program line
   * @param
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetAllPreviousStatements();

  /**
   * Get all program lines which are NextT to one or another program line
   * @param
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetAllNextTStatements();

  /**
   * Get all program lines which are PreviousT to one or another program line
   * @param
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetAllPreviousTStatements();

  /* ----------------------------------- All APIs related to Affects/AffectsT ----------------------------------- */

  /**
   * Inserts Affects(assign_stmt1, assign_stmt2) into affects_table
   * @params int assign_stmt1, int assign_stmt2
   * @return bool
   */
  bool InsertAffects(int, int);

  /**
   * Checks if Affects(assign_stmt1, assign_stmt2) holds
   * @params int assign_stmt1, int assign_stmt2
   * @return bool
   */
  bool IsAffects(int, int);

  /**
   * Get all assign statements that Affects a given assign statement
   * @params int assign_stmt2
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetStatementsThatAffects(int);

  /**
   * Get all assign statements that are Affected by a given assign statement
   * @params int assign_stmt1
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAffectedStatements(int);

  /**
   * Get all assign statements that Affects one or more other assign statements
   * @params
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAllStatementsThatAffects();

  /**
   * Get all assign statements that are Affected by one or more other assign statements
   * @params
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAllAffectedStatements();

  /**
   * Inserts AffectsT(assign_stmt1, assign_stmt2) into affects_T_table
   * @params int assign_stmt1, int assign_stmt2
   * @return bool
   */
  bool InsertAffectsT(int, int);

  /**
   * Checks if AffectsT(assign_stmt1, assign_stmt2) holds
   * @params int assign_stmt1, int assign_stmt2
   * @return bool
   */
  bool IsAffectsT(int, int);

  /**
   * Get all assign statements that AffectsT a given assign statement
   * @params int assign_stmt2
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetStatementsThatAffectsT(int);

  /**
   * Get all assign statements that are AffectedT by a given assign statement
   * @params int assign_stmt1
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAffectedTStatements(int);

  /**
   * Get all assign statements that AffectsT one or more other assign statements
   * @params
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAllStatementsThatAffectsT();

  /**
   * Get all assign statements that are AffectedT by one or more other assign statements
   * @params
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAllAffectedTStatements();

  /* ----------------------------------- All APIs related to NextBip/NextBipT ----------------------------------- */

  /**
   * Inserts a NextBip(prog_line1, prog_line2) relationship into nextbip_table
   * @params int prog_line1, int prog_line2
   * @return bool
   */
  bool InsertNextBip(int, int);

  /**
   * Inserts a NextBipT(prog_line1, prog_line2) relationship into nextbip_table
   * @params int prog_line1, int prog_line2
   * @return bool
   */
  bool InsertNextBipT(int, int);

  /**
   * Check if NextBip(prog_line1, prog_line2) relationship holds
   * @params int prog_line1, int prog_line2
   * @return bool
   */
  virtual bool IsNextBip(int, int);

  /**
   * Check if NextBipT(prog_line1, prog_line2) relationship holds
   * @params int prog_line1, int prog_line2
   * @return bool
   */
  virtual bool IsNextBipT(int, int);

  /**
   * Get all program lines which are NextBip to a given program line
   * @params int prog_line
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetNextBipStatements(int);

  /**
   * Get all program lines which are PreviousBip to a given program line
   * @params int prog_line
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetPreviousBipStatements(int);

  /**
   * Get all program lines which are NextBipT to a given program line
   * @params int prog_line
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetNextBipTStatements(int);

  /**
   * Get all program lines which are PreviousBipT to a given program line
   * @params int prog_line
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetPreviousBipTStatements(int);

  /**
   * Get all program lines which are NextBip to one or another program line
   * @param
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetAllNextBipStatements();

  /**
   * Get all program lines which are PreviousBip to one or another program line
   * @param
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetAllPreviousBipStatements();

  /**
   * Get all program lines which are NextBipT to one or another program line
   * @param
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetAllNextBipTStatements();

  /**
   * Get all program lines which are PreviousBipT to one or another program line
   * @param
   * @return unordered_set<int> of prog_line
   */
  std::unordered_set<int> GetAllPreviousBipTStatements();

  /* ----------------------------------- All APIs related to AffectsBip/AffectsBipT ----------------------------------- */

  /**
   * Inserts Affects(assign_stmt1, assign_stmt2) into affects_table
   * @params int assign_stmt1, int assign_stmt2
   * @return bool
   */
  bool InsertAffectsBip(int, int);

  /**
   * Checks if Affects(assign_stmt1, assign_stmt2) holds
   * @params int assign_stmt1, int assign_stmt2
   * @return bool
   */
  bool IsAffectsBip(int, int);

  /**
   * Get all assign statements that Affects a given assign statement
   * @params int assign_stmt2
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetStatementsThatAffectsBip(int);

  /**
   * Get all assign statements that are Affected by a given assign statement
   * @params int assign_stmt1
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAffectedBipStatements(int);

  /**
   * Get all assign statements that Affects one or more other assign statements
   * @params
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAllStatementsThatAffectsBip();

  /**
   * Get all assign statements that are Affected by one or more other assign statements
   * @params
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAllAffectedBipStatements();

  /**
   * Inserts AffectsT(assign_stmt1, assign_stmt2) into affects_T_table
   * @params int assign_stmt1, int assign_stmt2
   * @return bool
   */
  bool InsertAffectsBipT(int, int);

  /**
   * Checks if AffectsT(assign_stmt1, assign_stmt2) holds
   * @params int assign_stmt1, int assign_stmt2
   * @return bool
   */
  bool IsAffectsBipT(int, int);

  /**
   * Get all assign statements that AffectsT a given assign statement
   * @params int assign_stmt2
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetStatementsThatAffectsBipT(int);

  /**
   * Get all assign statements that are AffectedT by a given assign statement
   * @params int assign_stmt1
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAffectedBipTStatements(int);

  /**
   * Get all assign statements that AffectsT one or more other assign statements
   * @params
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAllStatementsThatAffectsBipT();

  /**
   * Get all assign statements that are AffectedT by one or more other assign statements
   * @params
   * @return unordered_set<int> of assign statements
   */
  std::unordered_set<int> GetAllAffectedBipTStatements();

  /**
   * Clears all underlying tables of the PKB to size 0
   * @params
   * @return
   */
  void ClearAllTables();
};