#pragma once

#include <list>
#include <string>

#include "source_processor/ast/TNode.h"
#include "source_processor/token/TokenList.h"

namespace parser_utils {

class ParserUtils {
 public:
  // Returns a TokenList without checking for syntax validity.
  // Strictly for expressions only; throws runtime_error on any token that
  // is not found within an expression.
  static source_processor::TokenList TokeniseExpression(const std::string&);
  // Returns a TokenList without checking for syntax validity.
  // Strictly for conditional expressions only; throws runtime_error on any
  // token that is not found within a conditional expression.
  static source_processor::TokenList TokeniseConditionalExpression(const std::string&);

  // For a valid expression:
  // 1) Immediately after an operator cannot be a right parenthesis
  // 2) Immediately after an operand cannot be a left parenthesis
  // 3) Immediately after a right parenthesis cannot be an operand or left parenthesis
  // 4) Immediately after a left parenthesis cannot be an operator or right parenthesis
  //
  // This function returns true if such a pattern is maintained.
  // Needed since Shunting Yard cannot detect some invalid SIMPLE expressions.
  static bool HasValidParenthesisPattern(source_processor::TokenList);

  // For a valid expression:
  // 1) Operators and operands must be alternating (ignoring parentheses)
  // 2) First and last token cannot be operators
  //
  // This function returns true if such a pattern is maintained.
  // Needed since Shunting Yard cannot detect some invalid SIMPLE expressions.
  static bool HasAlternatingOperatorPattern(const source_processor::TokenList&);

  // Returns a pointer to the AST constructed using the RPN TokenList argument.
  static source_processor::TNode* ConstructExpressionAST(const source_processor::TokenList&, int);
};

}  // namespace parser_utils