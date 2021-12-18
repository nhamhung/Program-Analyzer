#include <string>

#include "catch.hpp"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;

TEST_CASE("Overloaded operators") {
  SECTION("Similar tokens should return true for ==") {
    REQUIRE(Token("v", TokenType::VariableName) == Token("v", TokenType::VariableName));
    REQUIRE(Token("p", TokenType::ProcedureName) == Token("p", TokenType::ProcedureName));
  }

  SECTION("Different tokens should return false for ==") {
    REQUIRE((Token("v1", TokenType::VariableName) == Token("v2", TokenType::VariableName)) == false);
    REQUIRE((Token("p1", TokenType::ProcedureName) == Token("p2", TokenType::ProcedureName)) == false);
    REQUIRE((Token("p1", TokenType::ProcedureName) == Token("v2", TokenType::VariableName)) == false);
  }

  SECTION("Similar tokens should return false for !=") {
    REQUIRE((Token("v", TokenType::VariableName) != Token("v", TokenType::VariableName)) == false);
    REQUIRE((Token("p", TokenType::ProcedureName) != Token("p", TokenType::ProcedureName)) == false);
  }

  SECTION("Different tokens should return true for !=") {
    REQUIRE(Token("v1", TokenType::VariableName) != Token("v2", TokenType::VariableName));
    REQUIRE(Token("p1", TokenType::ProcedureName) != Token("p2", TokenType::ProcedureName));
    REQUIRE(Token("p1", TokenType::ProcedureName) != Token("v2", TokenType::VariableName));
  }
}

TEST_CASE("Validation of ProcedureName and VariableName") {
  auto pname_ttype = source_processor::TokenType::ProcedureName;
  auto vname_ttype = source_processor::TokenType::VariableName;

  const std::string kValidName1 = "a";
  const std::string kValidName2 = "a1";
  const std::string kValidName3 = "a1a2d3";

  const std::string kInvalidName1 = "";
  const std::string kInvalidName2 = "1";
  const std::string kInvalidName3 = "da{sda";

  SECTION("Valid names should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidName1, pname_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidName2, pname_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidName3, pname_ttype));

    REQUIRE_NOTHROW(source_processor::Token(kValidName1, vname_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidName2, vname_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidName3, vname_ttype));
  }

  SECTION("Invalid names should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidName1, pname_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidName2, pname_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidName3, pname_ttype));

    REQUIRE_THROWS(source_processor::Token(kInvalidName1, vname_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidName2, vname_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidName3, vname_ttype));
  }
}

TEST_CASE("Validation of ConstantValue") {
  auto cvalue_ttype = source_processor::TokenType::ConstantValue;

  const std::string kValidValue1 = "0";
  const std::string kValidValue2 = "1";
  const std::string kValidValue3 = "2060";
  const std::string kValidValue4 = "1234567890";

  const std::string kInvalidValue1 = "";
  const std::string kInvalidValue2 = "01";  // no leading 0
  const std::string kInvalidValue3 = "1a";
  const std::string kInvalidValue4 = "12./34";

  SECTION("Valid numbers should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidValue1, cvalue_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidValue2, cvalue_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidValue3, cvalue_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidValue4, cvalue_ttype));
  }

  SECTION("Invalid numbers should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidValue1, cvalue_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidValue2, cvalue_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidValue3, cvalue_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidValue4, cvalue_ttype));
  }
}

TEST_CASE("Validation of parentheses") {
  auto parenthesis_ttype = source_processor::TokenType::Parenthesis;

  const std::string kValidParenthesis1 = "(";
  const std::string kValidParenthesis2 = ")";
  const std::string kValidParenthesis3 = "{";
  const std::string kValidParenthesis4 = "}";

  const std::string kInvalidParenthesis1 = "()";
  const std::string kInvalidParenthesis2 = "P";
  const std::string kInvalidParenthesis3 = "{p";
  const std::string kInvalidParenthesis4 = "}1";
  const std::string kInvalidParenthesis5 = "";

  SECTION("Valid parenthesis should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidParenthesis1, parenthesis_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidParenthesis2, parenthesis_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidParenthesis3, parenthesis_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidParenthesis4, parenthesis_ttype));
  }

  SECTION("Invalid parenthesis should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidParenthesis1, parenthesis_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidParenthesis2, parenthesis_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidParenthesis3, parenthesis_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidParenthesis4, parenthesis_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidParenthesis5, parenthesis_ttype));
  }
}

TEST_CASE("Validation of semicolon") {
  auto semicolon_ttype = source_processor::TokenType::Semicolon;

  const std::string kValidSemicolon = ";";

  const std::string kInvalidSemicolon1 = "";
  const std::string kInvalidSemicolon2 = ":";
  const std::string kInvalidSemicolon3 = "'";
  const std::string kInvalidSemicolon4 = ";1";

  SECTION("Valid semicolon should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidSemicolon, semicolon_ttype));
  }

  SECTION("Invalid semicolon should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidSemicolon1, semicolon_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidSemicolon2, semicolon_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidSemicolon3, semicolon_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidSemicolon4, semicolon_ttype));
  }
}

TEST_CASE("Validation of assignment operator (=)") {
  auto assignment_ttype = source_processor::TokenType::AssignmentOp;

  const std::string kValidAssignmentOp = "=";

  const std::string kInvalidAssignmentOp1 = "";
  const std::string kInvalidAssignmentOp2 = "+";
  const std::string kInvalidAssignmentOp3 = "'";
  const std::string kInvalidAssignmentOp4 = "1";

  SECTION("Valid assignment op should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidAssignmentOp, assignment_ttype));
  }

  SECTION("Invalid assignment op should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidAssignmentOp1, assignment_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidAssignmentOp2, assignment_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidAssignmentOp3, assignment_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidAssignmentOp4, assignment_ttype));
  }
}

TEST_CASE("Validation of expression operator (+,-,*,/,%)") {
  auto expression_ttype = source_processor::TokenType::ExpressionOp;

  const std::string kValidExpressionOp1 = "+";
  const std::string kValidExpressionOp2 = "-";
  const std::string kValidExpressionOp3 = "*";
  const std::string kValidExpressionOp4 = "/";
  const std::string kValidExpressionOp5 = "%";

  const std::string kInvalidExpressionOp1 = "";
  const std::string kInvalidExpressionOp2 = "(";
  const std::string kInvalidExpressionOp3 = "-a";
  const std::string kInvalidExpressionOp4 = "+2";

  SECTION("Valid expression op should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp1, expression_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp2, expression_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp3, expression_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp4, expression_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp5, expression_ttype));
  }

  SECTION("Invalid expression op should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp1, expression_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp2, expression_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp3, expression_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp4, expression_ttype));
  }
}

TEST_CASE("Validation of conditional expression operator (!,&&,||)") {
  auto cond_expr_ttype = source_processor::TokenType::ConditionalExpressionOp;

  const std::string kValidExpressionOp1 = "!";
  const std::string kValidExpressionOp2 = "&&";
  const std::string kValidExpressionOp3 = "||";

  const std::string kInvalidExpressionOp1 = "";
  const std::string kInvalidExpressionOp2 = "&";
  const std::string kInvalidExpressionOp3 = "|";
  const std::string kInvalidExpressionOp4 = "!=";
  const std::string kInvalidExpressionOp5 = "& ";
  const std::string kInvalidExpressionOp6 = " |";
  const std::string kInvalidExpressionOp7 = "!&";
  const std::string kInvalidExpressionOp8 = "!|";

  SECTION("Valid conditional expression op should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp1, cond_expr_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp2, cond_expr_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp3, cond_expr_ttype));
  }

  SECTION("Invalid conditional expression op should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp1, cond_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp2, cond_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp3, cond_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp4, cond_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp5, cond_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp6, cond_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp7, cond_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp8, cond_expr_ttype));
  }
}

TEST_CASE("Validation of conditional expression operator (>,>=,<,<=,==,!=)") {
  auto rel_expr_ttype = source_processor::TokenType::RelativeExpressionOp;

  const std::string kValidExpressionOp1 = ">";
  const std::string kValidExpressionOp2 = ">=";
  const std::string kValidExpressionOp3 = "<";
  const std::string kValidExpressionOp4 = "<=";
  const std::string kValidExpressionOp5 = "==";
  const std::string kValidExpressionOp6 = "!=";

  const std::string kInvalidExpressionOp1 = "";
  const std::string kInvalidExpressionOp2 = "=>";
  const std::string kInvalidExpressionOp3 = "=<";
  const std::string kInvalidExpressionOp4 = "!";
  const std::string kInvalidExpressionOp5 = "= =";
  const std::string kInvalidExpressionOp6 = "! =";
  const std::string kInvalidExpressionOp7 = "> =";
  const std::string kInvalidExpressionOp8 = "< =";

  SECTION("Valid conditional expression op should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp1, rel_expr_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp2, rel_expr_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp3, rel_expr_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp4, rel_expr_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp5, rel_expr_ttype));
    REQUIRE_NOTHROW(source_processor::Token(kValidExpressionOp6, rel_expr_ttype));
  }

  SECTION("Invalid conditional expression op should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp1, rel_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp2, rel_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp3, rel_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp4, rel_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp5, rel_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp6, rel_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp7, rel_expr_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidExpressionOp8, rel_expr_ttype));
  }
}

TEST_CASE("Validation of read keyword") {
  auto read_ttype = source_processor::TokenType::Read;

  const std::string kValidRead = "read";

  const std::string kInvalidRead1 = "";
  const std::string kInvalidRead2 = "Read";
  const std::string kInvalidRead3 = "ReAD";
  const std::string kInvalidRead4 = "reAd";

  SECTION("Valid read should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidRead, read_ttype));
  }

  SECTION("Invalid read should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidRead1, read_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidRead2, read_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidRead3, read_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidRead4, read_ttype));
  }
}

TEST_CASE("Validation of print keyword") {
  auto print_ttype = source_processor::TokenType::Print;

  const std::string kValidPrint = "print";

  const std::string kInvalidPrint1 = "";
  const std::string kInvalidPrint2 = "Print";
  const std::string kInvalidPrint3 = "PrINt";
  const std::string kInvalidPrint4 = "pr1nt";

  SECTION("Valid print should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidPrint, print_ttype));
  }

  SECTION("Invalid print should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidPrint1, print_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidPrint2, print_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidPrint3, print_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidPrint4, print_ttype));
  }
}

TEST_CASE("Validation of procedure keyword") {
  auto procedure_ttype = source_processor::TokenType::Procedure;

  const std::string kValidProcedure = "procedure";

  const std::string kInvalidProcedure1 = "";
  const std::string kInvalidProcedure2 = "Procedure";
  const std::string kInvalidProcedure3 = "ProcEdURe";
  const std::string kInvalidProcedure4 = "Proc3dure";

  SECTION("Valid procedure should not throw") {
    REQUIRE_NOTHROW(source_processor::Token(kValidProcedure, procedure_ttype));
  }

  SECTION("Invalid procedure should throw") {
    REQUIRE_THROWS(source_processor::Token(kInvalidProcedure1, procedure_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidProcedure2, procedure_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidProcedure3, procedure_ttype));
    REQUIRE_THROWS(source_processor::Token(kInvalidProcedure4, procedure_ttype));
  }
}
