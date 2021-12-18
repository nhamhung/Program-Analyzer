#include <string>

#include "catch.hpp"
#include "source_processor/ast/TNodeType.h"
#include "source_processor/token/TokenType.h"
#include "source_processor/utils/TypeUtils.h"

using namespace source_processor;

SCENARIO("TNode type is converted correctly") {
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::Program) == "Program");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::Procedure) == "Procedure");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::StatementList) == "StatementList");

  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::Call) == "Call");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::Read) == "Read");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::Print) == "Print");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::Assign) == "Assign");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::While) == "While");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::If) == "If");

  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::Variable) == "Variable");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::Constant) == "Constant");

  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::ExpressionOp) == "ExpressionOp");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::ConditionalExpressionOp) == "ConditionalExpressionOp");
  REQUIRE(TypeUtils::TNodeTypeToName(TNodeType::RelativeExpressionOp) == "RelativeExpressionOp");
}

SCENARIO("Token type is converted correctly") {
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::ProcedureName) == "ProcedureName");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::VariableName) == "VariableName");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::ConstantValue) == "ConstantValue");

  REQUIRE(TypeUtils::TokenTypeToName(TokenType::Semicolon) == "Semicolon");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::Parenthesis) == "Parenthesis");

  REQUIRE(TypeUtils::TokenTypeToName(TokenType::AssignmentOp) == "AssignmentOp");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::ExpressionOp) == "ExpressionOp");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::ConditionalExpressionOp) == "ConditionalExpressionOp");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::RelativeExpressionOp) == "RelativeExpressionOp");

  REQUIRE(TypeUtils::TokenTypeToName(TokenType::Procedure) == "Procedure");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::Read) == "Read");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::Print) == "Print");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::Call) == "Call");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::While) == "While");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::If) == "If");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::Then) == "Then");
  REQUIRE(TypeUtils::TokenTypeToName(TokenType::Else) == "Else");
}