#include <string>

#include "TestUtils.h"
#include "catch.hpp"
#include "parser_utils/ParserUtils.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;
using namespace parser_utils;

// Tests here should only be for TokeniseExpression and TokeniseConditionalExpression.
// Invalid patterns should be tested in TestExpressionParser using ParseExpression.

SCENARIO("Lexer can tokenise a valid expression") {
  GIVEN("1 constant") {
    TokenList correct_tokens;
    correct_tokens.Push(Token("69", TokenType::ConstantValue));

    WHEN("There are no whitespaces") {
      std::string expression("69");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression(" \n \n\n  69   \n \n \n ");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }
  }

  GIVEN("1 variable") {
    TokenList correct_tokens;
    correct_tokens.Push(Token("niceNum69", TokenType::VariableName));

    WHEN("There are no whitespaces") {
      std::string expression("niceNum69");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression(" \n \n\n  niceNum69   \n \n \n ");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }
  }

  GIVEN("Many constants and operators") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("2", TokenType::ConstantValue))
        .Push(Token("-", TokenType::ExpressionOp))
        .Push(Token("3", TokenType::ConstantValue))
        .Push(Token("*", TokenType::ExpressionOp))
        .Push(Token("4", TokenType::ConstantValue))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("5", TokenType::ConstantValue))
        .Push(Token("%", TokenType::ExpressionOp))
        .Push(Token("6", TokenType::ConstantValue));

    WHEN("There are no whitespaces") {
      std::string expression("1+2-3*4/5%6");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression(" \n 1  \n+ \n\n2  \n- \n3*4 /\n5%  \n\n6");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }
  }

  GIVEN("Many variables and operators") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("a", TokenType::VariableName))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token("-", TokenType::ExpressionOp))
        .Push(Token("c", TokenType::VariableName))
        .Push(Token("*", TokenType::ExpressionOp))
        .Push(Token("d", TokenType::VariableName))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("e", TokenType::VariableName))
        .Push(Token("%", TokenType::ExpressionOp))
        .Push(Token("f", TokenType::VariableName));

    WHEN("There are no whitespaces") {
      std::string expression("a+b-c*d/e%f");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression(" \n a  \n+ \n\nb  \n- \nc*d /\ne%  \n\nf");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }
  }

  GIVEN("Mix of constants/variables/parentheses and operators") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("a1", TokenType::VariableName))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("69", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("420", TokenType::ConstantValue))
        .Push(Token("-", TokenType::ExpressionOp))
        .Push(Token("b12", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("%", TokenType::ExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("88", TokenType::ConstantValue))
        .Push(Token("*", TokenType::ExpressionOp))
        .Push(Token("cs3203", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string expression("((a1+69)/(420-b12))%(88*cs3203)");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression(" (\n\n ( \n a1 \n\n+ \n69 ) /( \n\n420 \n- \n\nb12)) \n\n%( \n 88  * \ncs3203 \n\n)\n \n");
      TokenList lexer_tokens = ParserUtils::TokeniseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }
  }
}

SCENARIO("Lexer will throw on invalid tokens in an expression") {
  WHEN("there are no whitespaces") {
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("="));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression(">"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression(">="));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("<"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("<="));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("!="));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("=="));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("!"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("&&"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("||"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("["));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("]"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression(":"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression(";"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("'"));
    REQUIRE_THROWS(ParserUtils::TokeniseExpression("\""));
  }
}

SCENARIO("Lexer can tokenise a valid conditional expression") {
  GIVEN("All valid reserved keywords") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("!", TokenType::ConditionalExpressionOp))
        .Push(Token("&&", TokenType::ConditionalExpressionOp))
        .Push(Token("||", TokenType::ConditionalExpressionOp))
        .Push(Token(">", TokenType::RelativeExpressionOp))
        .Push(Token(">=", TokenType::RelativeExpressionOp))
        .Push(Token("<", TokenType::RelativeExpressionOp))
        .Push(Token("<=", TokenType::RelativeExpressionOp))
        .Push(Token("==", TokenType::RelativeExpressionOp))
        .Push(Token("!=", TokenType::RelativeExpressionOp))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("-", TokenType::ExpressionOp))
        .Push(Token("*", TokenType::ExpressionOp))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("%", TokenType::ExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token(")", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string expression("!&&||>>=<<===!=+-*/%()");
      TokenList lexer_tokens = ParserUtils::TokeniseConditionalExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression("!  \n\n && \n\n || \n> \n  >= \n<  \n<=   == != \n+\n - \n*\n  / \n%  \n( \n)  ");
      TokenList lexer_tokens = ParserUtils::TokeniseConditionalExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }
  }

  GIVEN("A complex valid conditional expression") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("a", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(">", TokenType::RelativeExpressionOp))
        .Push(Token("3", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("||", TokenType::ConditionalExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("g", TokenType::VariableName))
        .Push(Token("<", TokenType::RelativeExpressionOp))
        .Push(Token("5", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("&&", TokenType::ConditionalExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("!", TokenType::ConditionalExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("z", TokenType::VariableName))
        .Push(Token(">=", TokenType::RelativeExpressionOp))
        .Push(Token("7", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(")", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string expression("((((1/a)>3)||(g<5))&&(!(z>=7)))");
      TokenList lexer_tokens = ParserUtils::TokeniseConditionalExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression("(\n\n( \n(  \n(1   /\n a) >\n\n 3 \n)  || (\ng < 5  )\n ) \n&& (\n! \n(  \nz >= 7  )\n) \n) \n");
      TokenList lexer_tokens = ParserUtils::TokeniseConditionalExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, lexer_tokens));
    }
  }
}

SCENARIO("Lexer will throw on invalid tokens in a conditional expression") {
  WHEN("there are no whitespaces") {
    REQUIRE_THROWS(ParserUtils::TokeniseConditionalExpression("["));
    REQUIRE_THROWS(ParserUtils::TokeniseConditionalExpression("]"));
    REQUIRE_THROWS(ParserUtils::TokeniseConditionalExpression(":"));
    REQUIRE_THROWS(ParserUtils::TokeniseConditionalExpression(";"));
    REQUIRE_THROWS(ParserUtils::TokeniseConditionalExpression("'"));
    REQUIRE_THROWS(ParserUtils::TokeniseConditionalExpression("\""));
  }
}