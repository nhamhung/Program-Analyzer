#pragma once

namespace source_processor {

enum class TNodeType {
  Program,
  Procedure,
  StatementList,

  // statements:
  Call,
  Read,
  Print,
  Assign,
  While,
  If,

  // user-defined:
  Variable,  // LETTER (LETTER | DIGIT)*
  Constant,  // INTEGER

  // operators:
  ExpressionOp,             // for `+`, `-`, `*`, `/`, `%`
  ConditionalExpressionOp,  // for `!`, `&&`, `||`
  RelativeExpressionOp,     // for `>`, `>=`, `<`, `<=`, `==`, `!=`
};

}