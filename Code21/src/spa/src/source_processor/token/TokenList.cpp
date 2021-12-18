#include "TokenList.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Token.h"
#include "TokenType.h"
#include "source_processor/utils/TypeUtils.h"

namespace source_processor {

const std::list<Token>& TokenList::GetUnderlyingList() const {
  return tokens;
}

bool operator==(const TokenList& lhs, const TokenList& rhs) {
  if (lhs.GetSize() != rhs.GetSize()) {
    return false;
  }

  // Check each token is similar
  const auto xs = lhs.GetUnderlyingList();
  const auto ys = rhs.GetUnderlyingList();
  for (auto i1 = xs.begin(), i2 = ys.begin(); i1 != xs.end(); ++i1, ++i2) {
    if (*i1 != *i2) {
      return false;
    }
  }

  return true;
}

bool operator!=(const TokenList& lhs, const TokenList& rhs) {
  return (lhs == rhs) == false;
}

bool TokenList::HasSublist(const TokenList& sublist) {
  if (sublist.IsEmpty()) {
    // empty sublist is always a valid sublist
    return true;
  }
  if (sublist.GetSize() > GetSize()) {
    // sublist cannot be longer than main list
    return false;
  }

  auto main = GetUnderlyingList();
  auto sub = sublist.GetUnderlyingList();
  auto pos = std::search(main.begin(), main.end(), sub.begin(), sub.end());

  // if pos is not the end of main list, then the sublist is valid
  return pos != main.end();
}

void TokenList::Clear() {
  return tokens.clear();
}

bool TokenList::IsEmpty() const {
  return tokens.empty();
}

int TokenList::GetSize() const {
  return tokens.size();
}

// Inserts the Token into the back of the list.
TokenList& TokenList::Push(const Token& token) {
  tokens.push_back(token);
  return *this;
}

// Returns the first Token in the list.
Token TokenList::Peek() {
  if (IsEmpty()) {
    throw std::runtime_error("TokenList.Peek: list is empty");
  }
  return tokens.front();
}

// Returns the last Token in the list.
Token TokenList::PeekLast() {
  if (IsEmpty()) {
    throw std::runtime_error("TokenList.Peek: list is empty");
  }
  return tokens.back();
}

// Returns and removes the first Token in the list.
Token TokenList::Pop() {
  if (IsEmpty()) {
    throw std::runtime_error("TokenList.Pop: list is empty");
  }
  auto token = Peek();
  tokens.pop_front();
  return token;
}

// Returns true if the first token in the list has the same TokenType.
bool TokenList::PeekMatch(TokenType type) {
  if (IsEmpty()) {
    throw std::runtime_error("TokenList.PeekMatch: list is empty");
  }
  return Peek().IsType(type);
}

bool TokenList::PeekMatch(TokenType type, const std::string& value) {
  return PeekMatch(type) && (Peek().GetValue() == value);
}

// Returns true if the last token in the list has the same TokenType.
bool TokenList::PeekLastMatch(TokenType type) {
  if (IsEmpty()) {
    throw std::runtime_error("TokenList.PeekMatch: list is empty");
  }
  return PeekLast().IsType(type);
}

// Returns the first token in the list if it has the same TokenType.
// Otherwise, exception is thrown.
Token TokenList::PopExpect(TokenType type) {
  if (!PeekMatch(type)) {
    std::stringstream err_msg;
    err_msg << "TokenList.PopExpect: expected "
            << TypeUtils::TokenTypeToName(type) << " token "
            << "but encountered "
            << TypeUtils::TokenTypeToName(Peek().GetType()) << " token "
            << "[" << Peek().GetValue() << "] ";
    throw std::runtime_error(err_msg.str());
  }
  return Pop();
}

Token TokenList::PopExpect(TokenType type, const std::string& value) {
  if (!PeekMatch(type, value)) {
    std::stringstream err_msg;
    err_msg << "TokenList.PopExpect: expected "
            << TypeUtils::TokenTypeToName(type) << " token "
            << "[" << value << "] "
            << "but encountered "
            << TypeUtils::TokenTypeToName(Peek().GetType()) << " token "
            << "[" << Peek().GetValue() << "] ";
    throw std::runtime_error(err_msg.str());
  }
  return Pop();
}

}  // namespace source_processor