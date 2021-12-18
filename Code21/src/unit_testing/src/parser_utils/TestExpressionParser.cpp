#include <string>

#include "TestUtils.h"
#include "catch.hpp"
#include "parser_utils/ExpressionParser.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;
using namespace parser_utils;

SCENARIO("Expression parser can detect invalid expression syntax") {
  GIVEN("Expressions starting or ending with an operator") {
    WHEN("There are no whitespaces") {
      std::string kInvalidExpression1("+");
      std::string kInvalidExpression2("1-");
      std::string kInvalidExpression3("-1");
      std::string kInvalidExpression4("-1%");
      std::string kInvalidExpression5("1+2/");
      std::string kInvalidExpression6("%1*2");
      std::string kInvalidExpression7("(+)");
      std::string kInvalidExpression8("((+))");
      std::string kInvalidExpression9("(1+2+)");
      std::string kInvalidExpression10("(1+2+)");
      std::string kInvalidExpression11("((1+2)-)");

      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression1));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression2));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression3));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression4));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression5));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression6));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression7));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression8));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression9));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression10));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression11));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string kInvalidExpression1(" + \n");
      std::string kInvalidExpression2("1 \n\n-");
      std::string kInvalidExpression3(" \n-  \n1");
      std::string kInvalidExpression4("  -1 \n%");
      std::string kInvalidExpression5("1 \n+  2  / ");
      std::string kInvalidExpression6("\n\n %\n 1 \n*  \n2  ");
      std::string kInvalidExpression7("( \n + \n)");
      std::string kInvalidExpression8(" \n( \n(  +  ) \n)");
      std::string kInvalidExpression9("( \n\n 1+ \n 2 \n\n + \n )");
      std::string kInvalidExpression10("( \n  1  \n+  \n2+ \n )");
      std::string kInvalidExpression11("( \n\n (1 \n+2 \n) \n -)");

      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression1));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression2));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression3));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression4));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression5));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression6));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression7));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression8));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression9));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression10));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression11));
    }
  }

  GIVEN("Expressions with invalid parentheses") {
    WHEN("There are no whitespaces") {
      std::string kInvalidExpression1("()");
      std::string kInvalidExpression2("(+");
      std::string kInvalidExpression3("1(");
      std::string kInvalidExpression4("a(");
      std::string kInvalidExpression5(")(");
      std::string kInvalidExpression6(")1");
      std::string kInvalidExpression7(")a");
      std::string kInvalidExpression8("+)");

      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression1));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression2));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression3));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression4));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression5));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression6));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression7));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression8));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string kInvalidExpression1(" (\n\n  )  ");
      std::string kInvalidExpression2(" \n\n ( \n\n + \n");
      std::string kInvalidExpression3(" 1  \n(  ");
      std::string kInvalidExpression4(" \na\n\n(");
      std::string kInvalidExpression5("  \n) \n(  \n");
      std::string kInvalidExpression6(" )\n\n 1\n\n");
      std::string kInvalidExpression7(")   a ");
      std::string kInvalidExpression8(" + \n\n)");

      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression1));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression2));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression3));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression4));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression5));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression6));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression7));
      REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression8));
    }
  }
}

SCENARIO("Expression parser can generate correct Reverse Polish Notation") {
  GIVEN("1 constant") {
    TokenList correct_tokens;
    correct_tokens.Push(Token("69", TokenType::ConstantValue));

    WHEN("There are no whitespaces") {
      std::string expression("69");
      TokenList rpn_tokens = ExpressionParser::ParseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, rpn_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression(" \n \n\n  69   \n \n \n ");
      TokenList rpn_tokens = ExpressionParser::ParseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, rpn_tokens));
    }
  }

  GIVEN("1 variable") {
    TokenList correct_tokens;
    correct_tokens.Push(Token("niceNum69", TokenType::VariableName));

    WHEN("There are no whitespaces") {
      std::string expression("niceNum69");
      TokenList rpn_tokens = ExpressionParser::ParseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, rpn_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression(" \n \n\n  niceNum69   \n \n \n ");
      TokenList rpn_tokens = ExpressionParser::ParseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, rpn_tokens));
    }
  }

  GIVEN("Mix of constants/variables and operators") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token("2", TokenType::ConstantValue))
        .Push(Token("3", TokenType::ConstantValue))
        .Push(Token("*", TokenType::ExpressionOp))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("a", TokenType::VariableName))
        .Push(Token("4", TokenType::ConstantValue))
        .Push(Token("%", TokenType::ExpressionOp))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("-", TokenType::ExpressionOp));

    WHEN("There are no whitespaces") {
      std::string expression("1+2*3-a%4/b");
      TokenList rpn_tokens = ExpressionParser::ParseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, rpn_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression("1 \n\n  + \n 2* \n\n 3- \n \n \na \n\n % \n4  /\n   \nb");
      TokenList rpn_tokens = ExpressionParser::ParseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, rpn_tokens));
    }
  }

  GIVEN("Mix of constants/variables/parentheses and operators") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("a1", TokenType::VariableName))
        .Push(Token("69", TokenType::ConstantValue))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("420", TokenType::ConstantValue))
        .Push(Token("b12", TokenType::VariableName))
        .Push(Token("-", TokenType::ExpressionOp))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("88", TokenType::ConstantValue))
        .Push(Token("cs3203", TokenType::VariableName))
        .Push(Token("*", TokenType::ExpressionOp))
        .Push(Token("%", TokenType::ExpressionOp));

    WHEN("There are no whitespaces") {
      std::string expression("((a1+69)/(420-b12))%(88*cs3203)");
      TokenList rpn_tokens = ExpressionParser::ParseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, rpn_tokens));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string expression(" (\n\n ( \n a1 \n\n+ \n69 ) /( \n\n420 \n- \n\nb12)) \n\n%( \n 88  * \ncs3203 \n\n)\n \n");
      TokenList rpn_tokens = ExpressionParser::ParseExpression(expression);
      REQUIRE(HasSimilarTokens(correct_tokens, rpn_tokens));
    }
  }
}

SCENARIO("Expression parser can detect non-balanced parenthesis") {
  WHEN("There are no whitespaces") {
    std::string kInvalidExpression1("(1+2))");
    std::string kInvalidExpression2("((1+2)");
    std::string kInvalidExpression3("(2))");
    std::string kInvalidExpression4("((a)");
    std::string kInvalidExpression5("(1+2+(a/2+(23))");
    std::string kInvalidExpression6("(1+2)+(a/2+(23)))");

    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression1));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression2));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression3));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression4));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression5));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression6));
  }

  WHEN("There are arbitrary whitespaces") {
    std::string kInvalidExpression1("( \n\n 1+ \n 2\n\n)  )");
    std::string kInvalidExpression2("( \n\n(1  + \n2  )");
    std::string kInvalidExpression3(" \n( 2 \n\n ) \n)");
    std::string kInvalidExpression4("(\n\n (  \na \n) ");
    std::string kInvalidExpression5("( \n\n1+ \n 2  + \n ( \na \n/ 2 \n\n+( \n23 \n)\n\n)  ");
    std::string kInvalidExpression6("(1 \n+  \n2)  \n+ (  \n\na/2+  \n\n(23 \n) ) \n\n)");

    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression1));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression2));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression3));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression4));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression5));
    REQUIRE_THROWS(ExpressionParser::ParseExpression(kInvalidExpression6));
  }
}