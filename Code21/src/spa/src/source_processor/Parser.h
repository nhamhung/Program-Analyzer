#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "source_processor/ast/TNode.h"
#include "source_processor/token/TokenList.h"

namespace source_processor {

class Parser {
 private:
  static int statement_number;
  static TokenList tokens;
  static std::string current_pname;
  // map of procedure declaration to procedures called in the declaration
  static std::unordered_map<std::string, std::unordered_set<std::string>>
      pdeclared_to_pcalled;

  static TokenList ExtractExpressionTokenList(bool is_lhs);
  static bool IsRelativeExpression();

  static void Reset();
  static void ValidateExistentCalls();
  static bool ValidateNonRecursiveCallsUtils(
      const std::string&, std::unordered_set<std::string>&, std::unordered_set<std::string>&);
  static void ValidateNonRecursiveCalls();
  static TNode* ParseProgram();
  static void ParseProcedure(TNode*);
  static void ParseStatementList(TNode*);
  static void ParseStatement(TNode*);
  static void ParseCallStatement(TNode*);
  static void ParsePrintStatement(TNode*);
  static void ParseReadStatement(TNode*);
  static void ParseAssignStatement(TNode*);
  static void ParseConditionalExpression(TNode*);
  static void ParseRelativeExpression(TNode*);
  static void ParseWhileStatement(TNode*);
  static void ParseIfStatement(TNode*);

 public:
  static const TNode& Parse(const std::string&);
};

}  // namespace source_processor
