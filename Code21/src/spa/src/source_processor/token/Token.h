#pragma once

#include <string>

#include "TokenType.h"

namespace source_processor {

class Token {
 protected:
  std::string value;
  TokenType type;

  bool ValidateValue();

 public:
  const std::string& GetValue() const { return value; };
  TokenType GetType() const { return type; };

  Token(std::string, TokenType);

  bool IsType(TokenType) const;
  bool IsValue(const std::string&) const;
  // Returns true if the tokens have the same value and type.
  friend bool operator==(const Token&, const Token&);
  // Returns true if the tokens have different value or type.
  friend bool operator!=(const Token&, const Token&);

  const Token& PrintDebugInfo() const;
};

}  // namespace source_processor
