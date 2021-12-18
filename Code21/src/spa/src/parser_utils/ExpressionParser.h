#pragma once

#include <list>
#include <string>

#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"

namespace parser_utils {

class ExpressionParser {
 private:
  static int GetOperatorPrecedence(const source_processor::Token&);

 public:
  // Takes a TokenList of expression tokens, checks for syntax validity
  // and returns an RPN TokenList. Throws an instance of runtime_error on
  // any syntax errors
  static source_processor::TokenList ParseExpression(const source_processor::TokenList&);

  // Takes a raw expression string, tokenises and checks for syntax validity
  // and returns an RPN TokenList. Throws an instance of runtime_error on
  // any syntax errors
  static source_processor::TokenList ParseExpression(const std::string&);
};

}  // namespace parser_utils
