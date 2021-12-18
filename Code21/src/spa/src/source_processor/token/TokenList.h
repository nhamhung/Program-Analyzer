#pragma once

#include <list>
#include <string>

#include "Token.h"
#include "TokenType.h"

namespace source_processor {

/**
 * This class contains the FIFO data structure for the list
 * of tokens, where tokens are pushed to the back of the list
 * and popped from the front.
 */
class TokenList {
 private:
  std::list<Token> tokens;

 public:
  TokenList() = default;

  // Returns a const reference to the underlying std::list container.
  // For ease of iteration; prefer not to use this if possible.
  const std::list<Token>& GetUnderlyingList() const;

  // Returns true if the lists have the same lengths and similar tokens.
  friend bool operator==(const TokenList&, const TokenList&);
  // Returns true if the lists have different lengths or tokens.
  friend bool operator!=(const TokenList&, const TokenList&);
  // Returns true if the argument TokenList is a contiguous sublist.
  bool HasSublist(const TokenList&);

  void Clear();
  int GetSize() const;
  bool IsEmpty() const;
  TokenList& Push(const Token&);
  Token Peek();
  Token PeekLast();
  Token Pop();
  bool PeekMatch(TokenType);
  bool PeekMatch(TokenType, const std::string&);
  bool PeekLastMatch(TokenType);
  Token PopExpect(TokenType);
  Token PopExpect(TokenType, const std::string&);
};

}  // namespace source_processor
