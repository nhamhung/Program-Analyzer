#include "PKBStub.h"

#include "source_processor/token/Token.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;
/*
Currently, the PKBStub returns values based on Code 3 in Basic SPA Requirements.
    procedure sumDigits {
01     read number;
02     sum = 0;

03     while (number > 0) {
04         digit = number % 10;
05         sum = sum + digit;
06         number = number / 10;
       }

07     print sum;
    }
    procedure nestVar {
08     while (x > 0) {
09         while (y > 0) {
10              print x;
            }
11          x = x - 1;
12          call sumDigits
       }
   }
 */

PKBStub::PKBStub() = default;

bool PKBStub::IsFollows(int s1, int s2) {
  if (s1 == 1 && s2 == 2) {
    return true;
  }
  if (s1 == 2 && s2 == 3) {
    return true;
  }
  if (s1 == 3 && s2 == 7) {
    return true;
  }

  if (s1 == 4 && s2 == 5) {
    return true;
  }

  if (s1 == 5 && s2 == 6) {
    return true;
  }

  if (s1 == 9 && s2 == 11) {
    return true;
  }

  if (s1 == 11 && s2 == 12) {
    return true;
  }

  return false;
}

bool PKBStub::IsFollowsT(int s1, int s2) {
  if (IsFollows(s1, s2)) {
    return true;
  }

  if (s1 == 1 && s2 == 3) {
    return true;
  }

  if (s1 == 4 && s2 == 6) {
    return true;
  }

  if (s1 == 1 && s2 == 7) {
    return true;
  }

  if (s1 == 2 && s2 == 7) {
    return true;
  }

  if (s1 == 9 && s2 == 12) {
    return true;
  }
  return false;
}

bool PKBStub::IsNext(int s1, int s2) {
  if (s1 == 1 && s2 == 2) {
    return true;
  }

  if (s1 == 2 && s2 == 3) {
    return true;
  }

  if (s1 == 3 && s2 == 4) {
    return true;
  }

  if (s1 == 4 && s2 == 5) {
    return true;
  }

  if (s1 == 5 && s2 == 6) {
    return true;
  }

  if (s1 == 6 && s2 == 3) {
    return true;
  }

  if (s1 == 3 && s2 == 7) {
    return true;
  }

  if (s1 == 8 && s2 == 9) {
    return true;
  }

  if (s1 == 9 && s2 == 10) {
    return true;
  }

  if (s1 == 10 && s2 == 9) {
    return true;
  }

  if (s1 == 9 && s2 == 11) {
    return true;
  }

  if (s1 == 11 && s2 == 12) {
    return true;
  }

  if (s1 == 12 && s2 == 8) {
    return true;
  }

  return false;
}

bool PKBStub::IsNextT(int s1, int s2) {
  if (IsNext(s1, s2)) {
    return true;
  }

  if (s1 == 1 && s2 == 3) {
    return true;
  }

  if (s1 == 1 && s2 == 4) {
    return true;
  }

  if (s1 == 1 && s2 == 5) {
    return true;
  }

  if (s1 == 1 && s2 == 6) {
    return true;
  }

  if (s1 == 1 && s2 == 7) {
    return true;
  }

  if (s1 == 2 && s2 == 4) {
    return true;
  }

  if (s1 == 2 && s2 == 5) {
    return true;
  }

  if (s1 == 2 && s2 == 6) {
    return true;
  }

  if (s1 == 2 && s2 == 7) {
    return true;
  }

  if (s1 == 3 && s2 == 3) {
    return true;
  }

  if (s1 == 3 && s2 == 5) {
    return true;
  }

  if (s1 == 3 && s2 == 6) {
    return true;
  }

  if (s1 == 3 && s2 == 7) {
    return true;
  }

  if (s1 == 4 && s2 == 3) {
    return true;
  }

  if (s1 == 4 && s2 == 4) {
    return true;
  }

  if (s1 == 4 && s2 == 6) {
    return true;
  }

  if (s1 == 4 && s2 == 7) {
    return true;
  }

  if (s1 == 5 && s2 == 3) {
    return true;
  }

  if (s1 == 5 && s2 == 4) {
    return true;
  }

  if (s1 == 5 && s2 == 5) {
    return true;
  }

  if (s1 == 5 && s2 == 7) {
    return true;
  }

  if (s1 == 6 && s2 == 4) {
    return true;
  }

  if (s1 == 6 && s2 == 5) {
    return true;
  }

  if (s1 == 6 && s2 == 6) {
    return true;
  }

  if (s1 == 6 && s2 == 7) {
    return true;
  }

  if (s1 == 8 && s2 == 8) {
    return true;
  }

  if (s1 == 8 && s2 == 10) {
    return true;
  }

  if (s1 == 8 && s2 == 11) {
    return true;
  }

  if (s1 == 8 && s2 == 12) {
    return true;
  }

  if (s1 == 9 && s2 == 8) {
    return true;
  }

  if (s1 == 9 && s2 == 9) {
    return true;
  }

  if (s1 == 9 && s2 == 11) {
    return true;
  }

  if (s1 == 9 && s2 == 12) {
    return true;
  }

  if (s1 == 10 && s2 == 8) {
    return true;
  }

  if (s1 == 10 && s2 == 9) {
    return true;
  }

  if (s1 == 10 && s2 == 10) {
    return true;
  }

  if (s1 == 10 && s2 == 11) {
    return true;
  }

  if (s1 == 10 && s2 == 12) {
    return true;
  }

  if (s1 == 11 && s2 == 8) {
    return true;
  }

  if (s1 == 11 && s2 == 9) {
    return true;
  }

  if (s1 == 11 && s2 == 10) {
    return true;
  }

  if (s1 == 11 && s2 == 11) {
    return true;
  }

  if (s1 == 12 && s2 == 9) {
    return true;
  }

  if (s1 == 12 && s2 == 10) {
    return true;
  }

  if (s1 == 12 && s2 == 11) {
    return true;
  }

  if (s1 == 12 && s2 == 12) {
    return true;
  }

  return false;
}

bool PKBStub::IsUses(int stmt, const std::string& var) {
  if (stmt == 3 && var == "number") {
    return true;
  }
  if (stmt == 3 && var == "digit") {
    return true;
  }
  if (stmt == 3 && var == "sum") {
    return true;
  }
  if (stmt == 4 && var == "number") {
    return true;
  }
  if (stmt == 5 && var == "sum") {
    return true;
  }
  if (stmt == 5 && var == "digit") {
    return true;
  }
  if (stmt == 6 && var == "number") {
    return true;
  }
  if (stmt == 7 && var == "sum") {
    return true;
  }
  if (stmt == 8 && var == "x") {
    return true;
  }
  if (stmt == 8 && var == "y") {
    return true;
  }
  if (stmt == 9 && var == "x") {
    return true;
  }
  if (stmt == 9 && var == "y") {
    return true;
  }
  if (stmt == 10 && var == "x") {
    return true;
  }
  if (stmt == 11 && var == "x") {
    return true;
  }
  if (stmt == 12 && var == "sum") {
    return true;
  }
  if (stmt == 12 && var == "digit") {
    return true;
  }
  if (stmt == 12 && var == "number") {
    return true;
  }
  return false;
}

bool PKBStub::IsModifies(int stmt, const std::string& var) {
  if (stmt == 1 && var == "number") {
    return true;
  }
  if (stmt == 2 && var == "sum") {
    return true;
  }
  if (stmt == 3 && var == "number") {
    return true;
  }
  if (stmt == 3 && var == "digit") {
    return true;
  }
  if (stmt == 3 && var == "sum") {
    return true;
  }
  if (stmt == 4 && var == "digit") {
    return true;
  }
  if (stmt == 5 && var == "sum") {
    return true;
  }
  if (stmt == 6 && var == "number") {
    return true;
  }
  if (stmt == 8 && var == "x") {
    return true;
  }
  if (stmt == 11 && var == "x") {
    return true;
  }
  if (stmt == 12 && var == "sum") {
    return true;
  }
  if (stmt == 12 && var == "digit") {
    return true;
  }
  if (stmt == 12 && var == "number") {
    return true;
  }
  return false;
}

bool PKBStub::IsUses(const std::string& proc_name, const std::string& var_name) {
  if (proc_name == "sumDigits" && var_name == "number") {
    return true;
  }
  if (proc_name == "sumDigits" && var_name == "sum") {
    return true;
  }
  if (proc_name == "sumDigits" && var_name == "digit") {
    return true;
  }
  if (proc_name == "nestVar" && var_name == "x") {
    return true;
  }
  if (proc_name == "nestVar" && var_name == "y") {
    return true;
  }
  return false;
}

bool PKBStub::IsModifies(const std::string& proc_name, const std::string& var_name) {
  if (proc_name == "sumDigits" && var_name == "number") {
    return true;
  }
  if (proc_name == "sumDigits" && var_name == "sum") {
    return true;
  }
  if (proc_name == "sumDigits" && var_name == "digit") {
    return true;
  }
  if (proc_name == "nestVar" && var_name == "x") {
    return true;
  }
  return false;
}

bool PKBStub::IsCalls(const std::string& p1, const std::string& p2) {
  if (p1 == "nestVar" && p2 == "sumDigits") {
    return true;
  }
  return false;
}

bool PKBStub::IsCallsT(const std::string& p1, const std::string& p2) {
  if (IsCalls(p1, p2)) {
    return true;
  }
  return false;
}

std::unordered_set<int> PKBStub::GetAllAssignStmtsThatMatches(TokenList& token_list) {
  Token zero("0", TokenType::ConstantValue);
  Token number("number", TokenType::VariableName);
  Token mod("%", TokenType::ExpressionOp);
  Token sum("sum", TokenType::VariableName);
  Token digit("digit", TokenType::VariableName);
  Token div("/", TokenType::ExpressionOp);
  Token plus("+", TokenType::ExpressionOp);
  Token ten("10", TokenType::ConstantValue);
  Token x("x", TokenType::VariableName);
  Token minus("-", TokenType::ExpressionOp);
  Token one("1", TokenType::ConstantValue);
  TokenList stmt2;
  TokenList stmt4;
  TokenList stmt5;
  TokenList stmt6;
  TokenList stmt11;
  stmt2.Push(zero);
  stmt4.Push(number).Push(mod).Push(ten);
  stmt5.Push(sum).Push(plus).Push(digit);
  stmt6.Push(number).Push(div).Push(ten);
  stmt11.Push(x).Push(minus).Push(one);

  if (token_list == stmt2) {
    return std::unordered_set<int>{2};
  }
  if (token_list == stmt4) {
    return std::unordered_set<int>{4};
  }
  if (token_list == stmt5) {
    return std::unordered_set<int>{5};
  }
  if (token_list == stmt6) {
    return std::unordered_set<int>{6};
  }
  if (token_list == stmt11) {
    return std::unordered_set<int>{11};
  }
  return std::unordered_set<int>{};
}

std::unordered_set<int> PKBStub::GetAllAssignStmtsThatModifies(const std::string& lhs_param) {
  if (lhs_param == "sum") {
    return std::unordered_set<int>{2, 5};
  }
  if (lhs_param == "digit") {
    return std::unordered_set<int>{4};
  }
  if (lhs_param == "number") {
    return std::unordered_set<int>{6};
  }
  if (lhs_param == "x") {
    return std::unordered_set<int>{11};
  }
  return std::unordered_set<int>{};
}

std::unordered_set<int> PKBStub::GetAllAssignStmtsThatContains(TokenList& token_list) {
  Token zero("0", TokenType::ConstantValue);
  Token number("number", TokenType::VariableName);
  Token mod("%", TokenType::ExpressionOp);
  Token sum("sum", TokenType::VariableName);
  Token digit("digit", TokenType::VariableName);
  Token div("/", TokenType::ExpressionOp);
  Token plus("+", TokenType::ExpressionOp);
  Token ten("10", TokenType::ConstantValue);

  TokenList stmt5;
  TokenList stmt5_partial;
  TokenList stmt5_partial2;
  TokenList stmt4_6_partial;
  TokenList stmt4_6_partial2;

  stmt5.Push(sum).Push(plus).Push(digit);
  stmt5_partial.Push(sum);
  stmt5_partial2.Push(digit);
  stmt4_6_partial.Push(ten);
  stmt4_6_partial2.Push(number);

  if (token_list == stmt5) {
    return std::unordered_set<int>{5};
  }

  if (token_list == stmt5_partial) {
    return std::unordered_set<int>{5};
  }

  if (token_list == stmt5_partial2) {
    return std::unordered_set<int>{5};
  }

  if (token_list == stmt4_6_partial) {
    return std::unordered_set<int>{4, 6};
  }

  if (token_list == stmt4_6_partial2) {
    return std::unordered_set<int>{4, 6};
  }

  return std::unordered_set<int>{};
}

std::unordered_set<std::string> PKBStub::GetVariablesUsedByWhileStmt(int stmt) {
  if (stmt == 3) {
    return std::unordered_set<std::string>{"number"};
  }

  if (stmt == 8) {
    return std::unordered_set<std::string>{"x"};
  }

  if (stmt == 9) {
    return std::unordered_set<std::string>{"y"};
  }

  return std::unordered_set<std::string>{};
}

std::unordered_set<std::string> PKBStub::GetVariablesUsedByIfStmt(int stmt) {
  return std::unordered_set<std::string>{};
}

std::string PKBStub::GetCallsProcName(int stmt) {
  if (stmt == 12) {
    return "sumDigits";
  }
  return "";
}

std::string PKBStub::GetPrintVarName(int stmt) {
  if (stmt == 7) {
    return "sum";
  }
  if (stmt == 10) {
    return "x";
  }
  return "";
}

std::string PKBStub::GetReadVarName(int stmt) {
  if (stmt == 1) {
    return "number";
  }
  return "";
}

bool PKBStub::IsParent(int s1, int s2) {
  if (s1 == 3 && s2 == 4) {
    return true;
  }
  if (s1 == 3 && s2 == 5) {
    return true;
  }

  if (s1 == 3 && s2 == 6) {
    return true;
  }

  if (s1 == 8 && s2 == 9) {
    return true;
  }

  if (s1 == 8 && s2 == 11) {
    return true;
  }

  if (s1 == 8 && s2 == 12) {
    return true;
  }

  if (s1 == 9 && s2 == 10) {
    return true;
  }
  return false;
}

bool PKBStub::IsParentT(int s1, int s2) {
  if (IsParent(s1, s2)) {
    return true;
  }
  if (s1 == 8 && s2 == 10) {
    return true;
  }
  return false;
}

std::unordered_set<int> PKBStub::GetAllStmts() {
  std::unordered_set<int> statement_set;
  for (int i = 1; i <= 12; i++) {
    statement_set.insert(i);
  }
  return statement_set;
}

std::unordered_set<int> PKBStub::GetAllReadStmts() {
  return std::unordered_set<int>{1};
}

std::unordered_set<int> PKBStub::GetAllAssignStmts() {
  return std::unordered_set<int>{2, 4, 5, 6, 11};
}

std::unordered_set<int> PKBStub::GetAllWhileStmts() {
  return std::unordered_set<int>{3, 8, 9};
}

std::unordered_set<int> PKBStub::GetAllPrintStmts() {
  return std::unordered_set<int>{7, 10};
}

std::unordered_set<int> PKBStub::GetAllCallsStmts() {
  return std::unordered_set<int>{12};
}

std::unordered_set<int> PKBStub::GetAllConstants() {
  return std::unordered_set<int>{0, 1, 10};
}

std::unordered_set<std::string> PKBStub::GetUsedVariables(int s) {
  switch (s) {
    case 3:
    case 12:
      return std::unordered_set<std::string>{"sum", "digit", "number"};
    case 4:
      return std::unordered_set<std::string>{"number"};
    case 5:
      return std::unordered_set<std::string>{"sum, digit"};
    case 6:
      return std::unordered_set<std::string>{"number"};
    case 7:
      return std::unordered_set<std::string>{"sum"};
    case 8:
    case 9:
      return std::unordered_set<std::string>{"x", "y"};
    case 10:
    case 11:
      return std::unordered_set<std::string>{"x"};
    default:
      return {};
  }
}

std::unordered_set<std::string> PKBStub::GetModifiedVariables(int s) {
  switch (s) {
    case 1:
      return std::unordered_set<std::string>{"number"};
    case 3:
    case 12:
      return std::unordered_set<std::string>{"digit, sum, number"};
    case 4:
      return std::unordered_set<std::string>{"digit"};
    case 5:
      return std::unordered_set<std::string>{"sum"};
    case 6:
      return std::unordered_set<std::string>{"number"};
    case 8:
    case 11:
      return std::unordered_set<std::string>{"x"};
    default:
      return {};
  }
}

std::unordered_set<std::string> PKBStub::GetAllVariables() {
  return std::unordered_set<std::string>{"sum", "digit", "number", "x", "y"};
}

std::unordered_set<std::string> PKBStub::GetAllProcedures() {
  return std::unordered_set<std::string>{"sumDigits", "nestVar"};
}