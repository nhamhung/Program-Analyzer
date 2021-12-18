#include "Token.h"

#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

#include "TokenType.h"
#include "source_processor/utils/TypeUtils.h"

namespace source_processor {

const std::regex kNameRegex("^[a-zA-Z][a-zA-Z0-9]*$");
const std::regex kConstantValueRegex("^0|[1-9][0-9]*$");
const std::regex kParenthesisRegex("^[{}()]$");
const std::regex kExpressionOpRegex("^[+\\-*/%]$");
const std::regex kConditionalExpressionOpRegex("^!|&&|\\|\\|$");
const std::regex kRelativeExpressionOpRegex("^>|>=|<|<=|==|!=$");

bool Token::ValidateValue() {
  std::smatch matches;

  switch (type) {
    // user-defined names:
    case TokenType::ProcedureName:
    case TokenType::VariableName:
      return std::regex_match(value, matches, kNameRegex);
    case TokenType::ConstantValue:
      return std::regex_match(value, matches, kConstantValueRegex);

    // delimitters:
    case TokenType::Semicolon:
      return value == std::string(";");
    case TokenType::Parenthesis:
      return std::regex_match(value, matches, kParenthesisRegex);

    // operators:
    case TokenType::AssignmentOp:
      return value == std::string("=");
    case TokenType::ExpressionOp:
      return std::regex_match(value, matches, kExpressionOpRegex);
    case TokenType::ConditionalExpressionOp:
      return std::regex_match(value, matches, kConditionalExpressionOpRegex);
    case TokenType::RelativeExpressionOp:
      return std::regex_match(value, matches, kRelativeExpressionOpRegex);

    // reserved keywords in SIMPLE:
    case TokenType::Procedure:
      return value == std::string("procedure");
    case TokenType::Call:
      return value == std::string("call");
    case TokenType::Read:
      return value == std::string("read");
    case TokenType::Print:
      return value == std::string("print");

    default:
      return true;
  }
}

bool Token::IsType(TokenType type) const {
  return this->type == type;
}

bool Token::IsValue(const std::string& value) const {
  return this->value == value;
}

bool operator==(const Token& lhs, const Token& rhs) {
  return lhs.IsType(rhs.GetType()) && lhs.IsValue(rhs.GetValue());
}

bool operator!=(const Token& lhs, const Token& rhs) {
  return (lhs == rhs) == false;
}

// Default constructor
Token::Token(std::string value, TokenType type) {
  this->value = value;
  this->type = type;
  if (!ValidateValue()) {
    std::stringstream err_msg;
    err_msg << "Token: invalid value [" << value << "] "
            << "for token type " << TypeUtils::TokenTypeToName(type);
    throw std::runtime_error(err_msg.str());
  }
}

// Prints the current token and its type for debugging purposes
const Token& Token::PrintDebugInfo() const {
  std::cout << TypeUtils::TokenTypeToName(type)
            << " [" << value << "]\n";

  return *this;
}

}  // namespace source_processor
