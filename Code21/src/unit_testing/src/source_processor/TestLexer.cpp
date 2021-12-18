#include <string>

#include "TestUtils.h"
#include "catch.hpp"
#include "source_processor/Lexer.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;

SCENARIO("Lexer can tokenise as expected") {
  GIVEN("A valid SIMPLE program with 1 empty procedure") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("  \n\n  procedure   \n\n\n   main \n { \n\n\n} \n\n  ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 'procedure' as procedure name") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("procedure", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure procedure{}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("  \n\n  procedure   \n\n\n   procedure \n { \n\n\n} \n\n  ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 3 empty procedures") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("foo", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("bar", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{}procedure foo{}procedure bar{}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program(" \n procedure \nmain\n { \n\n} procedure   foo{\n\n}\nprocedure\nbar {\n }  ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 1 procedure and read/print statements") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("read", TokenType::Read))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("y", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read x;print y;}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("\n\n procedure\n main{ \nread  \n\n\n x  \n; \n\nprint y   ;\n\n } ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A SIMPLE program with 1 procedure and read/print/call statements") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("read", TokenType::Read))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("y", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("call", TokenType::Call))
        .Push(Token("z", TokenType::ProcedureName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read x;print y;call z;}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("\n\n procedure\n main{ \nread  \n\n\n x  \n; \n\nprint y   ;\n\ncall  z ;\n } ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A SIMPLE program with 3 procedures and read/print statements") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("read", TokenType::Read))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("foo", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("bar", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("print", TokenType::Print))
        .Push(Token("y", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("z", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read x;}procedure foo {}procedure bar{print y; read z;}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main\n\n {\nread   x\n ; }  \n\nprocedure foo {\n} \n\nprocedure bar  \n{print\n\n y\n;  \nread z\n   ;\n\n}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 1 procedure and read/print/assign statements") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("read", TokenType::Read))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("y", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("0", TokenType::ConstantValue))
        .Push(Token("-", TokenType::ExpressionOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("2", TokenType::ConstantValue))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("123", TokenType::ConstantValue))
        .Push(Token("*", TokenType::ExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("x1", TokenType::VariableName))
        .Push(Token("%", TokenType::ExpressionOp))
        .Push(Token("y2", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read x;print y;print=((0-1));read=(2+x)/123*(x1%y2);}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main\n\n{ read x ; print y; \nprint=( ( 0\n-\n1 ) );\n\n read  \n=(\n 2+\n x)  \n\n/ \n\n123 \n*  \n\n (x1  \n\n\n%y2 \n) \n\n;}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with reserved keywords as variables") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("call", TokenType::Call))
        .Push(Token("call", TokenType::ProcedureName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("read", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("print", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("if", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("else", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("while", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("procedure", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("call", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("then", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("call", TokenType::Call))
        .Push(Token("read", TokenType::ProcedureName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("call", TokenType::Call))
        .Push(Token("print", TokenType::ProcedureName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{call call;read read;print print;read if;print else;read while;print procedure;read call;print then;call read;call print;}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main  { call\n\n call  ; \n read read \n ;  print print ; \nread if \n\n;print    \nelse \n ; read while  ;print\n  procedure\n  ;read\ncall\n  ;\n print then\n\n ; call  read\n\n;  \ncall\n print\n;  } ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 'while' in read statement before while statement") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("read", TokenType::Read))
        .Push(Token("while", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("while", TokenType::While))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token("==", TokenType::RelativeExpressionOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read while;while(1==1){x=1;}}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program(" \n procedure\n\n  main \n  { \n\n  read  \nwhile  ; \nwhile ( \n 1 \n== 1  ) \n\n{ \n x = 1;\n }  }   ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 'while' in print statement before while statement") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("print", TokenType::Print))
        .Push(Token("while", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("while", TokenType::While))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token("==", TokenType::RelativeExpressionOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{print while;while(1==1){x=1;}}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program(" \n procedure\n\n  main \n  { \n\n  print  \nwhile  ; \nwhile ( \n 1 \n== 1  ) \n\n{ \n x = 1;\n }  }   ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 'if' in read statement before if statement") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("read", TokenType::Read))
        .Push(Token("if", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("if", TokenType::If))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token("==", TokenType::RelativeExpressionOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("then", TokenType::Then))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("else", TokenType::Else))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read if;if(1==1)then{x=1;}else{x=1;}}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program(" \n procedure\n\n  main \n  { \n\n  read  \nif  ; \nif ( \n 1 \n== 1  ) then \n\n{ \n x = 1;\n } else \n{ \n x = 1;\n }  }  ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 'if' in print statement before if statement") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("print", TokenType::Print))
        .Push(Token("if", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("if", TokenType::If))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token("==", TokenType::RelativeExpressionOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("then", TokenType::Then))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("else", TokenType::Else))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("x", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{print if;if(1==1)then{x=1;}else{x=1;}}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program(" \n procedure\n\n  main \n  { \n\n  print  \nif  ; \nif ( \n 1 \n== 1  ) then \n\n{ \n x = 1;\n } else \n{ \n x = 1;\n }  }  ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with reserved keywords in read statements") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("read", TokenType::Read))
        .Push(Token("procedure", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("read", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("print", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("while", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("if", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("then", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("read", TokenType::Read))
        .Push(Token("else", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read procedure;read read;read print;read while;read if;read then;read else;}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main { read procedure ; read read ; read print ; read while ; read if ; read then ; read else ; }");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with reserved keywords in print statements") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("print", TokenType::Print))
        .Push(Token("procedure", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("read", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("print", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("while", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("if", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("then", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("print", TokenType::Print))
        .Push(Token("else", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{print procedure;print read;print print;print while;print if;print then;print else;}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main { print procedure ; print read ; print print ; print while ; print if ; print then ; print else ; }");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with reserved keywords in expression") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("read", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("procedure", TokenType::VariableName))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("print", TokenType::VariableName))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("if", TokenType::VariableName))
        .Push(Token("*", TokenType::ExpressionOp))
        .Push(Token("else", TokenType::VariableName))
        .Push(Token("%", TokenType::ExpressionOp))
        .Push(Token("then", TokenType::VariableName))
        .Push(Token("-", TokenType::ExpressionOp))
        .Push(Token("read", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read=procedure/print+if*else%then-read;}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main{  \n read \n  = \n procedure \n\n / \n print  + \n if \n\n*  \nelse%\n  then - \nread  \n; \n}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 1 while statement") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("while", TokenType::While))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("!", TokenType::ConditionalExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(">=", TokenType::RelativeExpressionOp))
        .Push(Token("2", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("||", TokenType::ConditionalExpressionOp))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("x1", TokenType::VariableName))
        .Push(Token("!=", TokenType::RelativeExpressionOp))
        .Push(Token("x2", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("x3", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("42", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{while((!(1>=2))||(x1!=x2)){x3=42;}}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main \n\n  { \n  while\n\n  (\n ( \n\n !  \n (  \n1  >=  \n2) )  ||  (  \nx1  \n!=  \nx2  \n)) \n{x3\n\n=42 \n;\n} \n}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    correct_tokens.Clear();
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("while", TokenType::While))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("read", TokenType::VariableName))
        .Push(Token("==", TokenType::RelativeExpressionOp))
        .Push(Token("2", TokenType::ConstantValue))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("print", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("3", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{while(read==2){print=3;}}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main \n{ \n\n while \n  ( \n read \n ==  \n2 )   \n{  \n\nprint \n=  \n3  ; \n} \n\n}  ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid SIMPLE program with 1 if statement") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("if", TokenType::If))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("if", TokenType::VariableName))
        .Push(Token(">=", TokenType::RelativeExpressionOp))
        .Push(Token("then", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("then", TokenType::Then))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("then", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("else", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("else", TokenType::Else))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("else", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("if", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{if(if>=then)then{then=else;}else{else=if;}}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main  \n\n { \n if  \n\n( \nif  \n >= \n then \n)  \n\nthen{  \nthen = \n\nelse ;  \n}  \nelse \n{ \n else\n\n= if\n\n; }\n}  ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }

  GIVEN("A valid simple program with nested while and if statements") {
    TokenList correct_tokens;
    correct_tokens
        .Push(Token("procedure", TokenType::Procedure))
        .Push(Token("main", TokenType::ProcedureName))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("if", TokenType::If))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("a", TokenType::VariableName))
        .Push(Token("<=", TokenType::RelativeExpressionOp))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("then", TokenType::Then))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("a", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("a", TokenType::VariableName))
        .Push(Token("+", TokenType::ExpressionOp))
        .Push(Token("1", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("while", TokenType::While))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("a", TokenType::VariableName))
        .Push(Token("!=", TokenType::RelativeExpressionOp))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token("/", TokenType::ExpressionOp))
        .Push(Token("2", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("if", TokenType::If))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("a", TokenType::VariableName))
        .Push(Token("==", TokenType::RelativeExpressionOp))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("then", TokenType::Then))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("a", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("else", TokenType::Else))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("c", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("d", TokenType::VariableName))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("else", TokenType::Else))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("if", TokenType::If))
        .Push(Token("(", TokenType::Parenthesis))
        .Push(Token("b", TokenType::VariableName))
        .Push(Token(">", TokenType::RelativeExpressionOp))
        .Push(Token("c", TokenType::VariableName))
        .Push(Token(")", TokenType::Parenthesis))
        .Push(Token("then", TokenType::Then))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("z", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("2", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("else", TokenType::Else))
        .Push(Token("{", TokenType::Parenthesis))
        .Push(Token("z", TokenType::VariableName))
        .Push(Token("=", TokenType::AssignmentOp))
        .Push(Token("4", TokenType::ConstantValue))
        .Push(Token(";", TokenType::Semicolon))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis))
        .Push(Token("}", TokenType::Parenthesis));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{if(a<=b)then{a=a+1;while(a!=b){b=b/2;if(a==b)then{a=b;}else{c=d;}}}else{if(b>c)then{z=2;}else{z=4;}}}");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main  \n{ \n if  \n(a \n<= \nb ) then {\n\n a= \na + 1 ; while \n(a\n !=  b ) \n{\nb = \nb/  2 ; \nif \n(a == b) \n\nthen \n{ \n\na=b \n;} \nelse{c\n=d \n;} \n} \n} \nelse\n { \nif( \nb \n>  c ) \nthen \n{ \nz= \n2; } \nelse{z= 4\n\n;}\n} \n}\n  ");
      TokenList lexer_tokens = Lexer::Tokenise(program);
      REQUIRE(correct_tokens == lexer_tokens);
    }
  }
}
