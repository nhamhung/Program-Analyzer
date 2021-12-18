#pragma once

#include <list>
#include <string>

#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"

namespace source_processor {

class Lexer {
 private:
  static TokenList tokens;

  static std::string SanitiseInput(const std::string&);

  // given their corresponding regex matches, these functions will extract
  // the tokens, push it into tokens, and return the next chunk of raw
  // input string to test.
  static void HandleProcedureDeclarationTokens(const std::string&);
  static void HandleAssignStatementTokens(const std::string&, const std::string&);
  static void HandleWhileConditionalExpressionTokens(const std::string&);
  static void HandleIfConditionalExpressionTokens(const std::string&);
  static void HandleReservedKeywordToken(const std::string&);
  static void HandleNameToken(const std::string&);
  static void HandleConstantValueToken(const std::string&);
  static void HandleParenthesisToken(const std::string&);
  static void HandleSemicolonToken();

 public:
  static TokenList Tokenise(const std::string&);
};

}  // namespace source_processor
