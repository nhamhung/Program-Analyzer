#pragma once

namespace source_processor {

// This defines the types of the lexical keywords that are
// found in the Basic SPA language
enum class TokenType {
  // user-defined names:
  ProcedureName,  // LETTER (LETTER | DIGIT)*
  VariableName,   // LETTER (LETTER | DIGIT)*
  ConstantValue,  // INTEGER

  // delimitters:
  Semicolon,    // for `;`
  Parenthesis,  // for `(`, `)`, `{`, `}`

  // operators:
  AssignmentOp,             // for `=`
  ExpressionOp,             // for `+`, `-`, `*`, `/`, `%`
  ConditionalExpressionOp,  // for `!`, `&&`, `||`
  RelativeExpressionOp,     // for `>`, `>=`, `<`, `<=`, `==`, `!=`

  // reserved keywords in SIMPLE:
  Procedure,
  Read,
  Print,
  Call,
  While,
  If,
  Then,
  Else,
};

}  // namespace source_processor
