#pragma once

#include <unordered_set>

#include "pkb/PKB.h"
#include "source_processor/token/TokenList.h"

class PKBStub : public PKB {
 public:
  PKBStub();
  bool IsFollows(int, int) override;
  bool IsFollowsT(int, int) override;
  bool IsParent(int, int) override;
  bool IsParentT(int, int) override;
  bool IsNext(int, int) override;
  bool IsNextT(int, int) override;
  bool IsUses(int, const std::string&) override;
  bool IsModifies(int, const std::string&) override;
  bool IsUses(const std::string&, const std::string&) override;
  bool IsModifies(const std::string&, const std::string&) override;
  bool IsCalls(const std::string&, const std::string&) override;
  bool IsCallsT(const std::string&, const std::string&) override;

  std::unordered_set<int> GetAllAssignStmtsThatMatches(source_processor::TokenList&) override;
  std::unordered_set<int> GetAllAssignStmtsThatContains(source_processor::TokenList&) override;
  std::unordered_set<int> GetAllAssignStmtsThatModifies(const std::string&) override;
  std::unordered_set<std::string> GetVariablesUsedByWhileStmt(int) override;
  std::unordered_set<std::string> GetVariablesUsedByIfStmt(int) override;

  std::string GetCallsProcName(int) override;
  std::string GetPrintVarName(int) override;
  std::string GetReadVarName(int) override;

  std::unordered_set<int> GetAllStmts() override;
  std::unordered_set<int> GetAllWhileStmts() override;
  std::unordered_set<int> GetAllReadStmts() override;
  std::unordered_set<int> GetAllAssignStmts() override;
  std::unordered_set<int> GetAllPrintStmts() override;
  std::unordered_set<int> GetAllCallsStmts() override;
  std::unordered_set<int> GetAllConstants() override;
  std::unordered_set<std::string> GetUsedVariables(int) override;
  std::unordered_set<std::string> GetModifiedVariables(int) override;
  std::unordered_set<std::string> GetAllVariables() override;
  std::unordered_set<std::string> GetAllProcedures() override;
};
