#include <string>

#include "TestUtils.h"
#include "catch.hpp"
#include "source_processor/Parser.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

using namespace source_processor;

SCENARIO("Parser can form the valid AST as expected") {
  GIVEN("A valid SIMPLE program with 1 procedure and read/print statements") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, std::string("main"));
    TNode stmt_list_node(TNodeType::StatementList);
    TNode read_node(TNodeType::Read, 1);
    TNode read_var_node(TNodeType::Variable, 1, std::string("x"));
    TNode print_node(TNodeType::Print, 2);
    TNode print_var_node(TNodeType::Variable, 2, std::string("y"));
    correct_ast.AddChild(
        &proc_node.AddChild(
            &stmt_list_node
                 .AddChild(&read_node.AddChild(&read_var_node))
                 .AddChild(&print_node.AddChild(&print_var_node))));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read x;print y;}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("\n\n procedure\n main{ \nread  \n\n\n x  \n; \n\nprint y   ;\n\n } ");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }
  }

  GIVEN("A valid SIMPLE program with 3 procedures and call statements") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_main_node(TNodeType::Procedure, std::string("main"));
    TNode stmt_list_node_1(TNodeType::StatementList);
    TNode read_node(TNodeType::Read, 1);
    TNode read_var_node(TNodeType::Variable, 1, std::string("x"));
    TNode proc_foo_node(TNodeType::Procedure, std::string("foo"));
    TNode stmt_list_node_2(TNodeType::StatementList);
    TNode call_main_node(TNodeType::Call, 2, std::string("main"));
    TNode proc_bar_node(TNodeType::Procedure, std::string("bar"));
    TNode stmt_list_node_3(TNodeType::StatementList);
    TNode call_foo_node(TNodeType::Call, 3, std::string("foo"));
    correct_ast
        .AddChild(
            &proc_main_node.AddChild(
                &stmt_list_node_1
                     .AddChild(&read_node.AddChild(&read_var_node))))
        .AddChild(
            &proc_foo_node.AddChild(
                &stmt_list_node_2
                     .AddChild(&call_main_node)))
        .AddChild(
            &proc_bar_node.AddChild(
                &stmt_list_node_3
                     .AddChild(&call_foo_node)));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read x;}procedure foo{call main;}procedure bar{call foo;}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main{read x;}procedure foo{call main;}procedure bar{call foo;}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }
  }

  GIVEN("A valid SIMPLE program with 3 procedures and read/print statements") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node_1(TNodeType::Procedure, std::string("main"));
    TNode stmt_list_node_1(TNodeType::StatementList);
    TNode read_node_x(TNodeType::Read, 1);
    TNode read_var_node_x(TNodeType::Variable, 1, std::string("x"));
    TNode proc_node_2(TNodeType::Procedure, std::string("foo"));
    TNode stmt_list_node_2(TNodeType::StatementList);
    TNode print_node_a(TNodeType::Print, 2);
    TNode print_var_node_a(TNodeType::Variable, 2, std::string("a"));
    TNode proc_node_3(TNodeType::Procedure, std::string("bar"));
    TNode stmt_list_node_3(TNodeType::StatementList);
    TNode print_node_y(TNodeType::Print, 3);
    TNode print_var_node_y(TNodeType::Variable, 3, std::string("y"));
    TNode read_node_z(TNodeType::Read, 4);
    TNode read_var_node_z(TNodeType::Variable, 4, std::string("z"));
    correct_ast.AddChild(
        &proc_node_1.AddChild(
            &stmt_list_node_1.AddChild(
                &read_node_x.AddChild(&read_var_node_x))));
    correct_ast.AddChild(
        &proc_node_2.AddChild(
            &stmt_list_node_2.AddChild(
                &print_node_a.AddChild(&print_var_node_a))));
    correct_ast.AddChild(
        &proc_node_3.AddChild(
            &stmt_list_node_3
                 .AddChild(&print_node_y.AddChild(&print_var_node_y))
                 .AddChild(&read_node_z.AddChild(&read_var_node_z))));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read x;}procedure foo {print a;}procedure bar{print y; read z;}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main\n\n {\nread   x\n ; }  \n\nprocedure foo {  print a\n;\n} \n\nprocedure bar  \n{print\n\n y\n;  \nread z\n   ;\n\n}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }
  }

  GIVEN("A valid SIMPLE program with 1 simple assign statement") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, std::string("main"));
    TNode stmt_list_node(TNodeType::StatementList);
    TNode assign_node(TNodeType::Assign, 1);
    TNode var_node(TNodeType::Variable, 1, "myGoodVar420");
    TNode const_node(TNodeType::Constant, 1, "69");
    correct_ast.AddChild(&proc_node.AddChild(&stmt_list_node.AddChild(
        &assign_node.AddChild(&var_node).AddChild(&const_node))));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{myGoodVar420=69;}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main \n\n  \n{  \nmyGoodVar420=  \n\n69  \n;  \n}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }
  }

  GIVEN("A valid SIMPLE program with 1 assign statement (left associative and precedence test)") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, std::string("main"));
    TNode stmt_list_node(TNodeType::StatementList);
    TNode assign_node(TNodeType::Assign, 1);
    TNode v_x_node(TNodeType::Variable, 1, "x");
    TNode c_1_node(TNodeType::Constant, 1, "1");
    TNode c_2_node(TNodeType::Constant, 1, "2");
    TNode c_3_node(TNodeType::Constant, 1, "3");
    TNode c_4_node(TNodeType::Constant, 1, "4");
    TNode c_5_node(TNodeType::Constant, 1, "5");
    TNode c_6_node(TNodeType::Constant, 1, "6");
    TNode plus_node(TNodeType::ExpressionOp, 1, "+");
    TNode minus_node(TNodeType::ExpressionOp, 1, "-");
    TNode times_node(TNodeType::ExpressionOp, 1, "*");
    TNode div_node(TNodeType::ExpressionOp, 1, "/");
    TNode mod_node(TNodeType::ExpressionOp, 1, "%");
    // AST for expression:
    minus_node
        .AddChild(
            &plus_node
                 .AddChild(&mod_node.AddChild(&c_1_node).AddChild(&c_2_node))
                 .AddChild(&times_node.AddChild(&c_3_node).AddChild(&c_4_node)))
        .AddChild(
            &div_node
                 .AddChild(&c_5_node)
                 .AddChild(&c_6_node));
    correct_ast.AddChild(&proc_node.AddChild(&stmt_list_node.AddChild(
        &assign_node.AddChild(&v_x_node).AddChild(&minus_node))));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{x=1%2+3*4-5/6;}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main   {\nx  \n\n= \n1 \n%  \n2+  \n3\n\n  *  4\n\n- \n 5 \n\n /6  \n;  }");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }
  }

  GIVEN("A valid SIMPLE program with 1 complex assign statement") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, std::string("main"));
    TNode stmt_list_node(TNodeType::StatementList);
    TNode assign_node(TNodeType::Assign, 1);
    TNode v_x_node(TNodeType::Variable, 1, "x");
    TNode v_a1_node(TNodeType::Variable, 1, "a1");
    TNode c_69_node(TNodeType::Constant, 1, "69");
    TNode c_420_node(TNodeType::Constant, 1, "420");
    TNode v_b12_node(TNodeType::Variable, 1, "b12");
    TNode c_88_node(TNodeType::Constant, 1, "88");
    TNode v_cs3203_node(TNodeType::Variable, 1, "cs3203");
    TNode plus_node(TNodeType::ExpressionOp, 1, "+");
    TNode minus_node(TNodeType::ExpressionOp, 1, "-");
    TNode times_node(TNodeType::ExpressionOp, 1, "*");
    TNode div_node(TNodeType::ExpressionOp, 1, "/");
    TNode mod_node(TNodeType::ExpressionOp, 1, "%");
    // AST for expression:
    mod_node
        .AddChild(
            &div_node
                 .AddChild(&plus_node.AddChild(&v_a1_node).AddChild(&c_69_node))
                 .AddChild(&minus_node.AddChild(&c_420_node).AddChild(&v_b12_node)))
        .AddChild(
            &times_node.AddChild(&c_88_node).AddChild(&v_cs3203_node));
    correct_ast.AddChild(&proc_node.AddChild(&stmt_list_node.AddChild(
        &assign_node.AddChild(&v_x_node).AddChild(&mod_node))));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{x=((a1+69)/(420-b12))%(88*cs3203);}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main{x  \n\n=  \n(   (a1+  \n\n 69) / \n\n(420  \n\n - \n\nb12)\n\n  )  %\n(88  \n*\n\n cs3203)  \n\n;  \n}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }
  }

  GIVEN("A valid SIMPLE program with read statements and read assign statements") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, "main");
    TNode stmt_list_node(TNodeType::StatementList);
    TNode read_node_1(TNodeType::Read, 1);
    TNode read_var_node_1(TNodeType::Variable, 1, "x");
    TNode assign_node_2(TNodeType::Assign, 2);
    TNode v_read_node_2(TNodeType::Variable, 2, "read");
    TNode v_print_node_2(TNodeType::Variable, 2, "print");
    TNode read_node_3(TNodeType::Read, 3);
    TNode read_var_node_3(TNodeType::Variable, 3, "y");
    TNode assign_node_4(TNodeType::Assign, 4);
    TNode times_node_4(TNodeType::ExpressionOp, 4, "*");
    TNode v_read_node_4(TNodeType::Variable, 4, "read");
    TNode v_print_node_4(TNodeType::Variable, 4, "print");
    correct_ast.AddChild(
        &proc_node.AddChild(
            &stmt_list_node
                 .AddChild(&read_node_1.AddChild(&read_var_node_1))
                 .AddChild(&assign_node_2.AddChild(&v_read_node_2).AddChild(&v_print_node_2))
                 .AddChild(&read_node_3.AddChild(&read_var_node_3))
                 .AddChild(&assign_node_4
                                .AddChild(&v_read_node_4)
                                .AddChild(&times_node_4
                                               .AddChild(&v_read_node_4)
                                               .AddChild(&v_print_node_4)))));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{read x;read=print;read y;read=read*print;}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("procedure main{\n\nread  \n x  ;  \n\n  read \n =  \n\n  print  \n; \n\n read y  ;\n\nread= \n\n read\n  *\n\nprint  ;\n\n }  ");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }
  }

  GIVEN("A valid SIMPLE program with print statements and print assign statements") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, "main");
    TNode stmt_list_node(TNodeType::StatementList);
    TNode print_node_1(TNodeType::Print, 1);
    TNode print_var_node_1(TNodeType::Variable, 1, "x");
    TNode assign_node_2(TNodeType::Assign, 2);
    TNode v_print_node_2(TNodeType::Variable, 2, "print");
    TNode v_read_node_2(TNodeType::Variable, 2, "read");
    TNode print_node_3(TNodeType::Print, 3);
    TNode print_var_node_3(TNodeType::Variable, 3, "y");
    TNode assign_node_4(TNodeType::Assign, 4);
    TNode times_node_4(TNodeType::ExpressionOp, 4, "*");
    TNode v_print_node_4(TNodeType::Variable, 4, "print");
    TNode v_read_node_4(TNodeType::Variable, 4, "read");
    correct_ast.AddChild(
        &proc_node.AddChild(
            &stmt_list_node
                 .AddChild(&print_node_1.AddChild(&print_var_node_1))
                 .AddChild(&assign_node_2.AddChild(&v_print_node_2).AddChild(&v_read_node_2))
                 .AddChild(&print_node_3.AddChild(&print_var_node_3))
                 .AddChild(&assign_node_4
                                .AddChild(&v_print_node_4)
                                .AddChild(&times_node_4
                                               .AddChild(&v_read_node_4)
                                               .AddChild(&v_print_node_4)))));

    WHEN("There are no whitespaces") {
      std::string program("procedure main{print x;print=read;print y;print=read*print;}");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }

    WHEN("There are arbitrary whitespaces") {
      std::string program("\n procedure  \nmain\n\n {   print x\n  ;\nprint\n\n=  \n\n read\n ;  print y;\n\nprint =\n read*\n \n print; \n } ");
      TNode parser_ast = Parser::Parse(program);
      REQUIRE(IsSimilarAST(correct_ast, parser_ast));
    }
  }

  GIVEN("A valid SIMPLE program with valid relative expressions") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, "main");
    TNode stmt_list_node(TNodeType::StatementList);
    TNode while_node(TNodeType::While, 1);
    TNode a_node(TNodeType::Variable, 1, "a");
    TNode eq_node(TNodeType::RelativeExpressionOp, 1, "==");
    TNode c_2_node(TNodeType::Constant, 1, "2");
    TNode while_stmt_list_node(TNodeType::StatementList);
    TNode read_node(TNodeType::Read, 2);
    TNode x_node(TNodeType::Variable, 2, "x");
    correct_ast.AddChild(
        &proc_node.AddChild(
            &stmt_list_node.AddChild(
                &while_node
                     .AddChild(&eq_node.AddChild(&a_node).AddChild(&c_2_node))
                     .AddChild(&while_stmt_list_node
                                    .AddChild(&read_node.AddChild(&x_node))))));

    WHEN("There are no whitespaces") {
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while(a==2){read x;}}")));
      // arbitrary parentheses around expressions within the conditional expression:
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while(a==(2)){read x;}}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while((a)==2){read x;}}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while((((a)))==2){read x;}}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while(a==(((2)))){read x;}}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while((((((a)))))==((2))){read x;}}")));
    }
  }

  GIVEN("A valid SIMPLE program with valid conditional expressions") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, "main");
    TNode stmt_list_node(TNodeType::StatementList);
    TNode while_node(TNodeType::While, 1);
    TNode and_node(TNodeType::ConditionalExpressionOp, 1, "&&");
    TNode gt_node(TNodeType::RelativeExpressionOp, 1, ">");
    TNode a_node(TNodeType::Variable, 1, "a");
    TNode b_node(TNodeType::Variable, 1, "b");
    TNode lt_node(TNodeType::RelativeExpressionOp, 1, "<");
    TNode c_node(TNodeType::Variable, 1, "c");
    TNode d_node(TNodeType::Variable, 1, "d");
    TNode while_stmt_list_node(TNodeType::StatementList);
    TNode read_node(TNodeType::Read, 2);
    TNode x_node(TNodeType::Variable, 2, "x");
    correct_ast.AddChild(
        &proc_node.AddChild(
            &stmt_list_node.AddChild(
                &while_node
                     .AddChild(&and_node
                                    .AddChild(&gt_node.AddChild(&a_node).AddChild(&b_node))
                                    .AddChild(&lt_node.AddChild(&c_node).AddChild(&d_node)))
                     .AddChild(&while_stmt_list_node.AddChild(&read_node.AddChild(&x_node))))));

    WHEN("There are no whitespaces") {
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while((a>b)&&(c<d)){read x;}}")));
      // arbitrary parentheses around expressions within the conditional expression:
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while((((a))>b)&&(c<d)){read x;}}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while((a>((b)))&&(c<d)){read x;}}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while((a>b)&&(((c))<d)){read x;}}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while((a>b)&&(c<((d)))){read x;}}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main{while(((((a)))>((b)))&&(((c))<(((d))))){read x;}}")));
    }

    WHEN("There are arbitrary whitespaces") {
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main \n\n   { \n while \n (  \n( \n a  \n >  \n b \n  )  && \n\n ( \n c \n\n < \n d \n)  \n) { read x \n\n ; } \n }")));
      // arbitrary parentheses around expressions within the conditional expression:
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main \n {  while( \n  (( \n (\n a)\n ) \n >b) \n  &&  \n( \nc  <\n d) \n ){ \n read x; \n }}  \n")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main  \n { \n while \n\n ( \n ( \n a \n > \n (  \n ( \n b) )  \n) \n&& \n (c  \n< \n d)  \n) {\n read x\n ; \n } \n  }")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main \n\n { \n while \n( \n ( \na \n> \nb)&&(\n\n ((c)\n) \n<d) \n){ \nread x \n;} \n}")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main\n  { \n  while((\n\n a \n>b \n)&&  \n\n(c<((d \n)  ) ) ) {read\n x; }}  ")));
      REQUIRE(IsSimilarAST(
          correct_ast,
          Parser::Parse("procedure main { \nwhile\n((\n\n( ( (a )) \n)>\n( \n(b))\n) &&(\n( \n(c)) \n<( ((d) ) )  ) ){read x;}}")));
    }
  }

  GIVEN("A complex iteration 1 source program") {
    TNode correct_ast(TNodeType::Program);
    TNode proc_node(TNodeType::Procedure, "main");
    TNode stmt_list_node_1(TNodeType::StatementList);
    TNode while_node_1(TNodeType::While, 1);
    TNode or_node_1(TNodeType::ConditionalExpressionOp, 1, "||");
    TNode neq_node_1(TNodeType::RelativeExpressionOp, 1, "!=");
    TNode plus_node_1(TNodeType::ExpressionOp, 1, "+");
    TNode v_while_node_1(TNodeType::Variable, 1, "while");
    TNode v_print_node_1(TNodeType::Variable, 1, "print");
    TNode v_read_node_1(TNodeType::Variable, 1, "read");
    TNode leq_node_1(TNodeType::RelativeExpressionOp, 1, "<=");
    TNode v_if_node_1(TNodeType::Variable, 1, "if");
    TNode v_else_node_1(TNodeType::Variable, 1, "else");
    TNode stmt_list_node_2(TNodeType::StatementList);
    TNode if_node_2(TNodeType::If, 2);
    TNode not_node_2(TNodeType::ConditionalExpressionOp, 2, "!");
    TNode and_node_2(TNodeType::ConditionalExpressionOp, 2, "&&");
    TNode gt_node_2(TNodeType::RelativeExpressionOp, 2, ">");
    TNode v_if_node_2(TNodeType::Variable, 2, "if");
    TNode v_then_node_2(TNodeType::Variable, 2, "then");
    TNode lt_node_2(TNodeType::RelativeExpressionOp, 2, "<");
    TNode v_print_node_2(TNodeType::Variable, 2, "print");
    TNode v_while_node_2(TNodeType::Variable, 2, "while");
    TNode stmt_list_node_3(TNodeType::StatementList);
    TNode read_node_3(TNodeType::Read, 3);
    TNode v_read_node_3(TNodeType::Variable, 3, "read");
    TNode stmt_list_node_4(TNodeType::StatementList);
    TNode while_node_4(TNodeType::While, 4);
    TNode gte_node_4(TNodeType::RelativeExpressionOp, 4, ">=");
    TNode v_print_node_4(TNodeType::Variable, 4, "print");
    TNode mod_node_4(TNodeType::ExpressionOp, 4, "%");
    TNode plus_node_4(TNodeType::ExpressionOp, 4, "+");
    TNode v_call_node_4(TNodeType::Variable, 4, "call");
    TNode v_read_node_4(TNodeType::Variable, 4, "read");
    TNode v_else_node_4(TNodeType::Variable, 4, "else");
    TNode stmt_list_node_5(TNodeType::StatementList);
    TNode assign_node_5(TNodeType::Assign, 5);
    TNode v_print_node_5(TNodeType::Variable, 5, "print");
    TNode plus_node_5(TNodeType::ExpressionOp, 5, "+");
    TNode v_call_node_5(TNodeType::Variable, 5, "call");
    TNode v_procedure_node_5(TNodeType::Variable, 5, "procedure");

    stmt_list_node_5.AddChild(
        &assign_node_5
             .AddChild(&v_print_node_5)
             .AddChild(&plus_node_5
                            .AddChild(&v_call_node_5)
                            .AddChild(&v_procedure_node_5)));
    stmt_list_node_4.AddChild(
        &while_node_4
             .AddChild(&gte_node_4
                            .AddChild(&v_print_node_4)
                            .AddChild(&mod_node_4
                                           .AddChild(&plus_node_4.AddChild(&v_call_node_4).AddChild(&v_read_node_4))
                                           .AddChild(&v_else_node_4)))
             .AddChild(&stmt_list_node_5));
    stmt_list_node_3.AddChild(&read_node_3.AddChild(&v_read_node_3));
    stmt_list_node_2.AddChild(
        &if_node_2
             .AddChild(&not_node_2.AddChild(
                 &and_node_2
                      .AddChild(&gt_node_2.AddChild(&v_if_node_2).AddChild(&v_then_node_2))
                      .AddChild(&lt_node_2.AddChild(&v_print_node_2).AddChild(&v_while_node_2))))
             .AddChild(&stmt_list_node_3)
             .AddChild(&stmt_list_node_4));
    stmt_list_node_1.AddChild(
        &while_node_1
             .AddChild(&or_node_1
                            .AddChild(&neq_node_1
                                           .AddChild(&plus_node_1
                                                          .AddChild(&v_while_node_1)
                                                          .AddChild(&v_print_node_1))
                                           .AddChild(&v_read_node_1))
                            .AddChild(&leq_node_1.AddChild(&v_if_node_1).AddChild(&v_else_node_1)))
             .AddChild(&stmt_list_node_2));
    correct_ast.AddChild(&proc_node.AddChild(&stmt_list_node_1));

    // note the arbitrary parentheses in expressions within conditional expressions
    // BUT NOT in conditional expressions itself
    std::string program(
        "procedure main {\
          while ( ((while + (print)) != read) || ((if) <= else) ) {\
            if (!((if > then) && (print < while))) then {\
              read read;\
            } else {\
              while (print >= (((call) + ((read))) % else)) {\
                print = call + procedure;\
              }\
            }\
          }\
        }\
    ");

    TNode parser_ast = Parser::Parse(program);
    REQUIRE(IsSimilarAST(correct_ast, parser_ast));
  }
}

SCENARIO("Parser can detect invalid SIMPLE programs") {
  GIVEN("An invalid SIMPLE program with empty statement lists") {
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure main{}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(1==1){}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{}else{}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{x=1;}else{}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{while(1==1){}}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{x=1;}else{while(1==1){}}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{read x;}procedure bar{}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{}procedure bar{read x}"));
    }

    WHEN("There are arbitrary whitespaces") {
      REQUIRE_THROWS(Parser::Parse(" \n procedure \n\n main  \n  {  \n   \n  }  \n  \r\t  "));
    }
  }

  GIVEN("An invalid SIMPLE program with multiple procedures with the same name") {
    REQUIRE_NOTHROW(Parser::Parse("procedure foo{read x;}procedure bar{read x;}"));  // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure foo{read x;}procedure foo{read x;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{read x;}procedure bar{read x;}foo{read x;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{read x;}procedure bar{read x;}bar{read x;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{read x;}procedure bar{read x;}goo{read x;}procedure foo{read x;}"));
    }
  }

  GIVEN("An invalid SIMPLE program calling non existent procedures") {
    REQUIRE_NOTHROW(Parser::Parse("procedure foo{read x;}procedure bar{call foo;}"));                          // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure foo{call koo;}procedure bar{call foo;}procedure koo{read y;}"));  // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure foo{read x;}procedure bar{call koo;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{call goo;}procedure bar{read x;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{call goo;}procedure bar{call koo;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{call goo;}procedure bar{call koo;}procedure koo{read y;}"));
    }
  }

  GIVEN("An invalid SIMPLE program with recursive procedure calls") {
    REQUIRE_NOTHROW(Parser::Parse("procedure foo{read x;}procedure bar{call foo;}"));                           // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure foo{call koo;}procedure bar{call foo;}procedure koo{read y;}"));   // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure main{call koo;}procedure bar{call koo;}procedure koo{read y;}"));  // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure main{call a;call b;call c;}procedure a{call b;call d;}procedure b{call c;}procedure c{read y;}procedure d{call b;}"));
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure foo{read x;}procedure bar{call bar;}"));  // self recursive
      REQUIRE_THROWS(Parser::Parse("procedure foo{call bar;}procedure bar{call foo;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{call bar;}procedure bar{call koo;}procedure koo{call foo;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{call bar;}procedure bar{call koo;}procedure koo{call bar;}"));
      REQUIRE_THROWS(Parser::Parse("procedure foo{call koo;}procedure bar{read x;}procedure koo{call foo;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{call a;call b;call c;}procedure a{call b;call d;}procedure b{call c;}procedure c{read y;}procedure d{call a;}"));
    }
  }

  GIVEN("Invalid procedure syntax") {
    REQUIRE_NOTHROW(Parser::Parse("procedure main{x=1;}"));  // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure{x=1;}"));
      REQUIRE_THROWS(Parser::Parse("procedur main{x=1;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{x=1;"));
      REQUIRE_THROWS(Parser::Parse("procedure 1main{x=1;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{x=1;}procedure"));
      REQUIRE_THROWS(Parser::Parse("procedure main{x=1;}read"));
    }
  }

  GIVEN("Invalid statement syntax") {
    REQUIRE_NOTHROW(Parser::Parse("procedure main{read x;}"));  // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure main{;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{2;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{read;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{print;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while;}"));
    }
  }

  GIVEN("Invalid assign statement syntax") {
    REQUIRE_NOTHROW(Parser::Parse("procedure main{a=2;}"));  // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure main{a=2;;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{a=;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{a=2;a;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{a b=2;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{a=2>3;}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{a==3;}"));
    }
  }

  GIVEN("Invalid while statement syntax") {
    REQUIRE_NOTHROW(Parser::Parse("procedure main{while(1==1){x=1;}}"));  // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure main{while(1==1){}}"));  // empty while statement list
      REQUIRE_THROWS(Parser::Parse("procedure main{while(1){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(1=1){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(1<!1){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(!1==1){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(!a<=b){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(!a!=b){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(1==1){x=1;};}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(1==1;){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(1==1){;}}"));
    }
  }

  GIVEN("Invalid if statement syntax") {
    REQUIRE_NOTHROW(Parser::Parse("procedure main{if(1==1)then{x=1;}else{x=1;}}"));  // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{}else{x=1;}}"));  // empty then statement list
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{x=1;}else{}}"));  // empty else statement list
      REQUIRE_THROWS(Parser::Parse("procedure main{if()then{x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1){x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1){x=1;}then{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1)then{x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1=1)then{x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1<!1)then{x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(!1==1)then{x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(!a<=b)then{x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(!a!=b)then{x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{x=1;}else{x=1;};}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1;)then{x=1;}else{x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{if(1==1)then{;}else{x=1;}}"));
    }
  }

  GIVEN("Invalid arbitrary parentheses surrounding a relative expression") {
    // arbitrary parentheses is ok in expressions within a relative expression
    REQUIRE_NOTHROW(Parser::Parse("procedure main{while(a==b){x=1;}}"));          // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure main{while(a==(b)){x=1;}}"));        // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure main{while((a)==(b)){x=1;}}"));      // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure main{while(((a))==((b))){x=1;}}"));  // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure main{while((a==b)){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(((a==b))){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while((a==)b){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(a(==)b){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while(a(==b)){x=1;}}"));
    }
  }

  GIVEN("Invalid arbitrary parentheses surrounding a conditional expression") {
    // arbitrary parentheses is ok in expressions within a relative expression
    REQUIRE_NOTHROW(Parser::Parse("procedure main{while((a==b)&&(1==2)){x=1;}}"));            // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure main{while(((a==b)&&(1==2))||(a>=3)){x=1;}}"));  // this is ok
    REQUIRE_NOTHROW(Parser::Parse("procedure main{while(!(a>4)){x=1;}}"));                    // this is ok
    WHEN("There are no whitespaces") {
      REQUIRE_THROWS(Parser::Parse("procedure main{while(((a==b)&&(1==2))){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while((((a==b)&&(1==2))||(a>=3))){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while((!(a>4))){x=1;}}"));
      REQUIRE_THROWS(Parser::Parse("procedure main{while((!((a>4)))){x=1;}}"));
    }
  }
}