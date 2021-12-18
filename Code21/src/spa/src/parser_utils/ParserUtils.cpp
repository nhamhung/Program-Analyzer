#include "ParserUtils.h"

#include <ctype.h>

#include <list>
#include <regex>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;

namespace parser_utils {

// `+`, `-`, `*`, `/`, `%`, `(`, `)`, var_name, const_value
const std::regex kValidExpressionToken("^\\s*([+\\-*/%()]|[a-zA-Z0-9]+)\\s*([\\s\\S]*)");
// `>`, `>=`, `<`, `<=`, `==`, `!=`, `!`, `&&`,`||`, and all tokens from kValidExpressionTokens
const std::regex kValidConditionalExpressionToken("^\\s*(&&|\\|\\||>=|<=|==|!=|[><!+\\-*/%()]|[a-zA-Z0-9]+|[0-9]+)\\s*([\\s\\S]*)");

TokenList ParserUtils::TokeniseExpression(const std::string& expr) {
  if (expr.length() == 0) {
    throw std::runtime_error("ExpressionParser::TokeniseExpression: expression argument is empty");
  }

  std::smatch matches;
  std::string current = expr;
  TokenList tokens;

  while (current.length() > 0) {
    // regex only matches with a variable name, number, parenthesis, or an operator
    if (!std::regex_search(current, matches, kValidExpressionToken)) {
      throw std::runtime_error("ParserUtils::TokeniseExpression: invalid tokens detected in expression");
    }
    const auto token = matches[1].str();

    if (isalpha(token.at(0))) {
      // first char is alphabet, assume token is variable name
      tokens.Push(Token(token, TokenType::VariableName));
    } else if (isdigit(token.at(0))) {
      // first char is digit, assume token is number
      tokens.Push(Token(token, TokenType::ConstantValue));
    } else if (token == "(" || token == ")") {
      // token is parenthesis
      tokens.Push(Token(token, TokenType::Parenthesis));
    } else {
      // token is math expression operator
      tokens.Push(Token(token, TokenType::ExpressionOp));
    }
    current = matches[2];
  }

  return tokens;
}

TokenList ParserUtils::TokeniseConditionalExpression(const std::string& cond_expr) {
  if (cond_expr.length() == 0) {
    throw std::runtime_error("ParserUtils::TokeniseConditionalExpression: conditional expression argument is empty");
  }

  std::smatch matches;
  std::string current = cond_expr;
  TokenList tokens;

  while (current.length() > 0) {
    // regex only matches with a variable name, number, parenthesis, or an operator
    if (!std::regex_search(current, matches, kValidConditionalExpressionToken)) {
      throw std::runtime_error("ParserUtils::TokeniseConditionalExpression: invalid tokens detected in conditional expression");
    }
    const auto token = matches[1].str();

    if (isalpha(token.at(0))) {
      // first char is alphabet, assume token is variable name
      tokens.Push(Token(token, TokenType::VariableName));
    } else if (isdigit(token.at(0))) {
      // first char is digit, assume token is number
      tokens.Push(Token(token, TokenType::ConstantValue));
    } else if (token == "(" || token == ")") {
      // token is parenthesis
      tokens.Push(Token(token, TokenType::Parenthesis));
    } else if (token == "+" || token == "-" || token == "*" ||
               token == "/" || token == "%") {
      // token is math expression operator
      tokens.Push(Token(token, TokenType::ExpressionOp));
    } else if (token == "!" || token == "&&" || token == "||") {
      // token is conditional expression operator
      tokens.Push(Token(token, TokenType::ConditionalExpressionOp));
    } else {
      // token is relative expression operator
      tokens.Push(Token(token, TokenType::RelativeExpressionOp));
    }
    current = matches[2];
  }

  return tokens;
}

bool ParserUtils::HasValidParenthesisPattern(TokenList tokens) {
  while (tokens.GetSize() > 1) {
    Token token = tokens.Pop();
    if (token.IsType(TokenType::ExpressionOp)) {
      // right after an operator cannot be right parenthesis
      if (tokens.PeekMatch(TokenType::Parenthesis, ")")) {
        return false;
      }
    } else if (token.IsType(TokenType::ConstantValue) ||
               token.IsType(TokenType::VariableName)) {
      // right after an operand cannot be left parenthesis
      if (tokens.PeekMatch(TokenType::Parenthesis, "(")) {
        return false;
      }
    } else if (token.IsType(TokenType::Parenthesis)) {
      if (token.IsValue("(")) {
        // right after left parenthesis cannot be operator or right parenthesis
        if (tokens.PeekMatch(TokenType::ExpressionOp) ||
            tokens.PeekMatch(TokenType::Parenthesis, ")")) {
          return false;
        }
      } else {
        // right after right parenthesis cannot be operand or right parenthesis
        if (tokens.PeekMatch(TokenType::ConstantValue) ||
            tokens.PeekMatch(TokenType::VariableName) ||
            tokens.PeekMatch(TokenType::Parenthesis, "(")) {
          return false;
        }
      }
    } else {
      throw std::runtime_error("ParserUtils::HasValidParenthesisPattern: invalid token in expression");
    }
  }

  return true;
}

bool ParserUtils::HasAlternatingOperatorPattern(const TokenList& tokens) {
  bool is_next_token_op = false;

  for (const auto token : tokens.GetUnderlyingList()) {
    if (token.IsType(TokenType::Parenthesis)) {
      // ignore all parenthesis
      continue;
    }

    if (token.IsType(TokenType::ExpressionOp)) {
      // currently we are seeing an ExpressionOp
      if (is_next_token_op) {
        is_next_token_op = false;
      } else {
        return false;
      }
    } else if (token.IsType(TokenType::VariableName) ||
               token.IsType(TokenType::ConstantValue)) {
      // currently we are seeing a variable/constant value
      if (is_next_token_op) {
        return false;
      } else {
        is_next_token_op = true;
      }
    }
  }

  // since we must end with a var/constant, the next token (if any) must be an operator
  return is_next_token_op;
}

TNode* ParserUtils::ConstructExpressionAST(const TokenList& rpn_tokens,
                                           int statement_number) {
  std::stack<TNode*> stack;

  for (auto token : rpn_tokens.GetUnderlyingList()) {
    if (token.IsType(TokenType::ConstantValue)) {
      stack.push(new TNode(TNodeType::Constant, statement_number,
                           token.GetValue()));
    } else if (token.IsType(TokenType::VariableName)) {
      stack.push(new TNode(TNodeType::Variable, statement_number,
                           token.GetValue()));
    } else if (token.IsType(TokenType::ExpressionOp)) {
      TNode* rhs = stack.top();
      stack.pop();
      TNode* lhs = stack.top();
      stack.pop();

      TNode* current_root = new TNode(TNodeType::ExpressionOp, statement_number,
                                      token.GetValue());

      current_root->AddChild(lhs);
      current_root->AddChild(rhs);
      stack.push(current_root);
    } else {
      throw std::runtime_error("ParserUtils::ConstructExpressionAST: invalid TokenType detected");
    }
  }

  if (stack.size() != 1) {
    throw std::runtime_error("ParserUtils::ConstructExpressionAST: output stack does not have exactly 1 element");
  }

  return stack.top();
}

}  // namespace parser_utils