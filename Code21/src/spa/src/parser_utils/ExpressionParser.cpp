#include "ExpressionParser.h"

#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>

#include "ParserUtils.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;

namespace parser_utils {

int ExpressionParser::GetOperatorPrecedence(const Token& token) {
  if (!token.IsType(TokenType::ExpressionOp)) {
    throw std::runtime_error("ExpressionParser::GetOperatorPrecedence: argument is not an expression operator");
  }

  if (token.IsValue("+") || token.IsValue("-")) {
    return 1;
  } else {  // for `*`, `/`, `%`
    return 2;
  }
}

TokenList ExpressionParser::ParseExpression(const TokenList& tokens) {
  if (!ParserUtils::HasValidParenthesisPattern(tokens) ||
      !ParserUtils::HasAlternatingOperatorPattern(tokens)) {
    throw std::runtime_error("ExpressionParser::TokeniseExpression: invalid expression detected");
  }

  // commence shunting yard algo
  // see: https://en.wikipedia.org/wiki/Shunting-yard_algorithm#The_algorithm_in_detail
  TokenList rpn_tokens;
  std::stack<Token> op_stack;

  for (auto token : tokens.GetUnderlyingList()) {
    if (token.IsType(TokenType::ConstantValue) ||
        token.IsType(TokenType::VariableName)) {
      rpn_tokens.Push(token);
    } else if (token.IsType(TokenType::ExpressionOp)) {
      while (!op_stack.empty() && op_stack.top().IsType(TokenType::ExpressionOp) &&
             GetOperatorPrecedence(op_stack.top()) >= GetOperatorPrecedence(token)) {
        rpn_tokens.Push(op_stack.top());
        op_stack.pop();
      }
      op_stack.push(token);
    } else if (token.IsType(TokenType::Parenthesis)) {
      if (token.IsValue("(")) {
        op_stack.push(token);
      } else if (token.IsValue(")")) {
        while (true) {
          if (op_stack.empty()) {
            throw std::runtime_error("ExpressionParser::ParseExpression: mismatched parentheses in expression");
          }
          if (op_stack.top().IsValue("(")) {
            // remove and ignore the right parenthesis
            op_stack.pop();
            break;
          }
          rpn_tokens.Push(op_stack.top());
          op_stack.pop();
        }
      }
    } else {
      // invalid token found
      std::stringstream err_msg;
      err_msg << "Parser::ParseStatement: invalid token "
              << "[" << token.GetValue() << "] "
              << "in expression";
      throw std::runtime_error(err_msg.str());
    }
  }

  while (!op_stack.empty()) {
    auto op = op_stack.top();
    if (op.IsType(TokenType::Parenthesis)) {
      throw std::runtime_error("ExpressionParser::ParseExpression: mismatched parentheses in expression");
    }
    rpn_tokens.Push(op);
    op_stack.pop();
  }

  return rpn_tokens;
}

TokenList ExpressionParser::ParseExpression(const std::string& expr) {
  return ParseExpression(ParserUtils::TokeniseExpression(expr));
}

}  // namespace parser_utils