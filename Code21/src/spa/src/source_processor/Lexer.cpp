#include "Lexer.h"

#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

#include "parser_utils/ParserUtils.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"
#include "source_processor/token/TokenType.h"

namespace source_processor {

// initialise the static TokenList
TokenList Lexer::tokens = TokenList();

const std::regex kAnyWhitespaces("\\s+");

// These regex are ordered according to their precedence. The ones at the top
// has higher precedence and must be tested for first in Lexer::Tokenise.
// Some of the regex are made to be more lax; the strict checking of variables
// is handled when constructing the Token object itself.
// The last capture group must be the next chunk of raw input source to test.
const std::regex kAssignNameTokenAndExpression("^\\s*([a-zA-Z][a-zA-Z0-9]*)\\s*=\\s*([\\s\\S]*?)\\s*(;\\s*[\\s\\S]*)");
const std::regex kWhileConditionalExpression("^\\s*while\\s*(\\([\\s\\S]*?)\\s*(\\{[\\s\\S]*)");
const std::regex kIfConditionalExpression("^\\s*if\\s*(\\([\\s\\S]*?)\\s*(then\\s*\\{[\\s\\S]*)");
const std::regex kReservedKeywordToken("^\\s*(procedure|call|read|print|then|else)(\\s+[\\s\\S]*|\\{[\\s\\S]*)");
const std::regex kNameToken("^\\s*([a-zA-Z0-9]+)\\s*([\\s\\S]*)");
const std::regex kConstantValueToken("^\\s*([0-9]+)\\s*([\\s\\S]*)");
const std::regex kParenthesisToken("^\\s*([{}()])\\s*([\\s\\S]*)");
const std::regex kSemicolonToken("^\\s*;\\s*([\\s\\S]*)");

void Lexer::HandleProcedureDeclarationTokens(const std::string& pname) {
  // procedure keyword
  tokens.Push(Token("procedure", TokenType::Procedure));
  // procedure name
  tokens.Push(Token(pname, TokenType::ProcedureName));
}

void Lexer::HandleAssignStatementTokens(const std::string& vname,
                                        const std::string& raw_expr) {
  // variable name
  tokens.Push(Token(vname, TokenType::VariableName));
  // assignment operator
  tokens.Push(Token("=", TokenType::AssignmentOp));

  auto expr_tokens = parser_utils::ParserUtils::TokeniseExpression(raw_expr);
  while (!expr_tokens.IsEmpty()) {
    tokens.Push(expr_tokens.Pop());
  }
}

void Lexer::HandleWhileConditionalExpressionTokens(const std::string& cond_expr) {
  // while keyword
  tokens.Push(Token("while", TokenType::While));

  auto expr_tokens = parser_utils::ParserUtils::TokeniseConditionalExpression(cond_expr);
  while (!expr_tokens.IsEmpty()) {
    tokens.Push(expr_tokens.Pop());
  }
}

void Lexer::HandleIfConditionalExpressionTokens(const std::string& cond_expr) {
  // if keyword
  tokens.Push(Token("if", TokenType::If));

  auto expr_tokens = parser_utils::ParserUtils::TokeniseConditionalExpression(cond_expr);
  while (!expr_tokens.IsEmpty()) {
    tokens.Push(expr_tokens.Pop());
  }
}

void Lexer::HandleReservedKeywordToken(const std::string& tvalue) {
  if (!tokens.IsEmpty()) {
    if ((tokens.PeekLastMatch(TokenType::Read) ||
         tokens.PeekLastMatch(TokenType::Print))) {
      // if previous token is Read/Print, treat this token as VariableName
      tokens.Push(Token(tvalue, TokenType::VariableName));
      return;
    } else if (tokens.PeekLastMatch(TokenType::Procedure) ||
               tokens.PeekLastMatch(TokenType::Call)) {
      // if previous token is Procedure/Call, treat this token as ProcedureName
      tokens.Push(Token(tvalue, TokenType::ProcedureName));
      return;
    }
  }

  TokenType ttype;

  if (tvalue == "procedure") {
    ttype = TokenType::Procedure;
  } else if (tvalue == "call") {
    ttype = TokenType::Call;
  } else if (tvalue == "read") {
    ttype = TokenType::Read;
  } else if (tvalue == "print") {
    ttype = TokenType::Print;
  } else if (tvalue == "then") {
    ttype = TokenType::Then;
  } else if (tvalue == "else") {
    ttype = TokenType::Else;
  } else {
    throw std::runtime_error("Lexer::HandleReservedKeywordsToken: no valid reserved keywords found");
  }

  tokens.Push(Token(tvalue, ttype));
}

void Lexer::HandleNameToken(const std::string& name) {
  if (!tokens.IsEmpty() && (tokens.PeekLastMatch(TokenType::Procedure) ||
                            tokens.PeekLastMatch(TokenType::Call))) {
    // if previous token is Procedure/Call, treat this token as ProcedureName
    tokens.Push(Token(name, TokenType::ProcedureName));
  } else {
    tokens.Push(Token(name, TokenType::VariableName));
  }
}

void Lexer::HandleConstantValueToken(const std::string& cvalue) {
  tokens.Push(Token(cvalue, TokenType::ConstantValue));
}

void Lexer::HandleParenthesisToken(const std::string& paren) {
  tokens.Push(Token(paren, TokenType::Parenthesis));
}

void Lexer::HandleSemicolonToken() {
  tokens.Push(Token(";", TokenType::Semicolon));
}

std::string SanitiseRawInput(const std::string& raw_input) {
  // replace all consecutive whitespaces with a single whitespace
  return std::regex_replace(raw_input, kAnyWhitespaces, " ");
}

// Main entry point; returns a TokenList if supplied raw_input can be
// tokenised successfully. Otherwise, a runtime exception is thrown.
TokenList Lexer::Tokenise(const std::string& raw_input) {
  // clear all current tokens:
  tokens.Clear();

  // start the tokenising process:
  std::string current = SanitiseRawInput(raw_input);
  std::smatch matches;

  while (current.length() > 0) {
    if (std::regex_search(current, matches, kAssignNameTokenAndExpression)) {
      HandleAssignStatementTokens(matches[1], matches[2]);
    } else if (std::regex_search(current, matches, kWhileConditionalExpression)) {
      HandleWhileConditionalExpressionTokens(matches[1]);
    } else if (std::regex_search(current, matches, kIfConditionalExpression)) {
      HandleIfConditionalExpressionTokens(matches[1]);
    } else if (std::regex_search(current, matches, kReservedKeywordToken)) {
      HandleReservedKeywordToken(matches[1]);
    } else if (std::regex_search(current, matches, kNameToken)) {
      HandleNameToken(matches[1]);
    } else if (std::regex_search(current, matches, kConstantValueToken)) {
      HandleConstantValueToken(matches[1]);
    } else if (std::regex_search(current, matches, kParenthesisToken)) {
      HandleParenthesisToken(matches[1]);
    } else if (std::regex_search(current, matches, kSemicolonToken)) {
      HandleSemicolonToken();
    } else {
      std::stringstream err_msg;
      err_msg << "Lexer::Tokenise: invalid syntax found near ["
              << (current.length() > 25
                      ? current.substr(0, 25) + "...<truncated>"
                      : current)
              << "]";
      throw std::runtime_error(err_msg.str());
    }
    // last element in matches is next chunk of raw input source to test
    current = matches[matches.size() - 1];
  }

  return tokens;
}

}  // namespace source_processor
