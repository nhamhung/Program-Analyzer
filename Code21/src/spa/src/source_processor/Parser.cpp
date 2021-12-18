#include "Parser.h"

#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "Lexer.h"
#include "parser_utils/ExpressionParser.h"
#include "parser_utils/ParserUtils.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"
#include "source_processor/token/TokenType.h"

using namespace parser_utils;

namespace source_processor {

//* member variables *//

// init static variables with default values:
int Parser::statement_number = 0;
TokenList Parser::tokens = TokenList();
std::string Parser::current_pname;
std::unordered_map<std::string, std::unordered_set<std::string>> Parser::pdeclared_to_pcalled;

//* utilities *//

// Returns a TokenList of expression when called during parsing of a
// rel_expr. The LHS will end right before a rel_op, and the RHS will
// end right before the last right bracket detected.
TokenList Parser::ExtractExpressionTokenList(bool is_lhs) {
  TokenList expr_tokens;
  // for RHS to match parenthesis algo to find the last `)`
  int num_left_paren = 0;
  while (!tokens.IsEmpty()) {
    if (!tokens.PeekMatch(TokenType::VariableName) &&
        !tokens.PeekMatch(TokenType::ConstantValue) &&
        !tokens.PeekMatch(TokenType::ExpressionOp) &&
        !tokens.PeekMatch(TokenType::RelativeExpressionOp) &&
        !tokens.PeekMatch(TokenType::Parenthesis)) {
      // invalid token found
      std::stringstream err_msg;
      err_msg << "Parser::ExtractExpressionTokenList: invalid token "
              << "[" << tokens.Peek().GetValue() << "] "
              << "in conditional expression";
      throw std::runtime_error(err_msg.str());
    }

    if (is_lhs) {
      // LHS expr ends right before a relative expression op
      if (tokens.PeekMatch(TokenType::RelativeExpressionOp)) {
        break;
      }
    } else {
      // RHS expr ends right before the last right parenthesis
      if (tokens.PeekMatch(TokenType::Parenthesis, ")")) {
        if (num_left_paren == 0) {
          break;
        }
        num_left_paren--;
      } else if (tokens.PeekMatch(TokenType::Parenthesis, "(")) {
        num_left_paren++;
      }
    }

    // pop the current token and add to expr_tokens
    expr_tokens.Push(tokens.Pop());
  }

  return expr_tokens;
}

// Returns true iff the current conditional expression is a relative expression
bool Parser::IsRelativeExpression() {
  // match parenthesis algo to find the last `)`
  int num_left_paren = 0;
  for (const auto token : tokens.GetUnderlyingList()) {
    if (!token.IsType(TokenType::VariableName) &&
        !token.IsType(TokenType::ConstantValue) &&
        !token.IsType(TokenType::ExpressionOp) &&
        !token.IsType(TokenType::ConditionalExpressionOp) &&
        !token.IsType(TokenType::RelativeExpressionOp) &&
        !token.IsType(TokenType::Parenthesis)) {
      // invalid token found
      std::stringstream err_msg;
      err_msg << "Parser::IsRelativeExpression: invalid token "
              << "[" << token.GetValue() << "] "
              << "in conditional expression";
      throw std::runtime_error(err_msg.str());
    }

    if (token.IsType(TokenType::ConditionalExpressionOp)) {
      // conditional expression op exists, this is NOT a relative expression
      return false;
    }

    if (token.IsType(TokenType::Parenthesis)) {
      if (token.IsValue(")")) {
        if (num_left_paren == 0) {
          return true;
        }
        num_left_paren--;
      } else if (token.IsValue("(")) {
        num_left_paren++;
      }
    }
  }

  // this is unreachable given a valid conditional expression
  throw std::runtime_error("Parser::IsRelativeExpression: invalid conditional expression detected");
}

//* parsing *//

void Parser::ParseCallStatement(TNode* stmt_list_node) {
  // expect the 'call' keyword
  tokens.PopExpect(TokenType::Call);
  // expect the procedure name token
  Token proc_name_token = tokens.PopExpect(TokenType::ProcedureName);
  // expect the ';' keyword
  tokens.PopExpect(TokenType::Semicolon);

  // save this procedure name in procedures called
  auto& procedures_called = pdeclared_to_pcalled.find(current_pname)->second;
  procedures_called.insert(proc_name_token.GetValue());

  // add proc_node as child of stmt_list_node
  TNode* proc_node = new TNode(TNodeType::Call, statement_number,
                               proc_name_token.GetValue());
  stmt_list_node->AddChild(proc_node);
}

void Parser::ParseReadStatement(TNode* stmt_list_node) {
  // expect the 'read' keyword
  tokens.PopExpect(TokenType::Read);
  // expect the var_name
  Token var_token = tokens.PopExpect(TokenType::VariableName);
  // expect the ';' keyword
  tokens.PopExpect(TokenType::Semicolon);

  // add read_node->var_node branch as child of stmt_list_node
  TNode* read_node = new TNode(TNodeType::Read, statement_number);
  TNode* var_node = new TNode(TNodeType::Variable, statement_number,
                              var_token.GetValue());
  stmt_list_node->AddChild(&read_node->AddChild(var_node));
}

void Parser::ParsePrintStatement(TNode* stmt_list_node) {
  // expect the 'print' keyword
  tokens.PopExpect(TokenType::Print);
  // expect the var_name
  Token var_token = tokens.PopExpect(TokenType::VariableName);
  // expect the ';' keyword
  tokens.PopExpect(TokenType::Semicolon);

  // add print_node->var_node branch as child of stmt_list_node
  TNode* print_node = new TNode(TNodeType::Print, statement_number);
  TNode* var_node = new TNode(TNodeType::Variable, statement_number,
                              var_token.GetValue());
  stmt_list_node->AddChild(&print_node->AddChild(var_node));
}

void Parser::ParseAssignStatement(TNode* stmt_list_node) {
  // expect the var_name
  Token var_token = tokens.PopExpect(TokenType::VariableName);
  // expect the '=' keyword
  tokens.PopExpect(TokenType::AssignmentOp);

  // fill up expr_tokens with all the expression tokens
  TokenList expr_tokens;
  while (!tokens.PeekMatch(TokenType::Semicolon)) {
    expr_tokens.Push(tokens.Pop());
  }
  // expect the ';' keyword
  tokens.PopExpect(TokenType::Semicolon);

  // get the parsed and validated RPN tokens
  TokenList rpn_tokens = ExpressionParser::ParseExpression(expr_tokens);

  // add assignee_node and expr_ast as left and right child of assign_node
  // add assign_node as child of stmt_list_node
  TNode* expr_ast = ParserUtils::ConstructExpressionAST(rpn_tokens, statement_number);
  TNode* assignee_node = new TNode(TNodeType::Variable, statement_number,
                                   var_token.GetValue());
  TNode* assign_node = new TNode(TNodeType::Assign, statement_number,
                                 rpn_tokens);
  assign_node->AddChild(assignee_node);
  assign_node->AddChild(expr_ast);
  stmt_list_node->AddChild(assign_node);
}

void Parser::ParseRelativeExpression(TNode* cond_expr_node) {
  // get the LHS expression
  TokenList lhs_expr_tokens = ExtractExpressionTokenList(true);
  TokenList lhs_rpn_tokens = ExpressionParser::ParseExpression(lhs_expr_tokens);

  // expect a relative expression op
  const auto rel_expr_op = tokens.PopExpect(TokenType::RelativeExpressionOp);

  // get the RHS expression
  TokenList rhs_expr_tokens = ExtractExpressionTokenList(false);
  TokenList rhs_rpn_tokens = ExpressionParser::ParseExpression(rhs_expr_tokens);

  // construct the AST
  TNode* lhs_ast = ParserUtils::ConstructExpressionAST(lhs_rpn_tokens, statement_number);
  TNode* rel_expr_node = new TNode(TNodeType::RelativeExpressionOp, statement_number,
                                   rel_expr_op.GetValue());
  TNode* rhs_ast = ParserUtils::ConstructExpressionAST(rhs_rpn_tokens, statement_number);
  rel_expr_node->AddChild(lhs_ast);
  rel_expr_node->AddChild(rhs_ast);
  cond_expr_node->AddChild(rel_expr_node);
}

void Parser::ParseConditionalExpression(TNode* current_node) {
  if (IsRelativeExpression()) {
    // if this cond_expr is really just a rel_expr, just parse it as a rel_expr
    ParseRelativeExpression(current_node);
  } else if (tokens.PeekMatch(TokenType::ConditionalExpressionOp, "!")) {
    // expect the '!' keyword
    tokens.PopExpect(TokenType::ConditionalExpressionOp, "!");
    // expect the '(' keyword
    tokens.PopExpect(TokenType::Parenthesis, "(");
    // create not node and add as child of current node
    TNode* not_node = new TNode(TNodeType::ConditionalExpressionOp, statement_number, "!");
    current_node->AddChild(not_node);
    ParseConditionalExpression(not_node);
    // expect the ')' keyword
    tokens.PopExpect(TokenType::Parenthesis, ")");
  } else if (tokens.PeekMatch(TokenType::Parenthesis, "(")) {
    // create cond_op node and add as child of current node (value will be set later)
    TNode* cond_node = new TNode(TNodeType::ConditionalExpressionOp, statement_number);
    current_node->AddChild(cond_node);

    // start of LHS; expect the '(' keyword
    tokens.PopExpect(TokenType::Parenthesis, "(");
    ParseConditionalExpression(cond_node);
    tokens.PopExpect(TokenType::Parenthesis, ")");

    // expect either the `&&` or `||` keyword; throw for `!`
    const auto cond_op = tokens.PopExpect(TokenType::ConditionalExpressionOp);
    if (cond_op.IsValue("!")) {
      throw std::runtime_error("Parser::ParseRelativeExpression: unexpected token [!] in conditional expression");
    }
    cond_node->SetConditionalExpressionOp(cond_op.GetValue());

    // start of RHS; expect the '(' keyword
    tokens.PopExpect(TokenType::Parenthesis, "(");
    ParseConditionalExpression(cond_node);
    tokens.PopExpect(TokenType::Parenthesis, ")");
  } else {
    // invalid token found
    std::stringstream err_msg;
    err_msg << "Parser::ParseStatement: invalid token "
            << "[" << tokens.Peek().GetValue() << "] "
            << "in conditional expression";
    throw std::runtime_error(err_msg.str());
  }
}

void Parser::ParseWhileStatement(TNode* stmt_list_node) {
  // expect the while keyword
  tokens.PopExpect(TokenType::While);

  // expect the '(' keyword
  tokens.PopExpect(TokenType::Parenthesis, "(");
  // create while node and add as child of stmt_list_node
  TNode* while_node = new TNode(TNodeType::While, statement_number);
  stmt_list_node->AddChild(while_node);
  ParseConditionalExpression(while_node);
  // expect the ')' keyword
  tokens.PopExpect(TokenType::Parenthesis, ")");

  // expect the '{' keyword
  tokens.PopExpect(TokenType::Parenthesis, "{");
  // parse the statement list within the while loop
  ParseStatementList(while_node);
  // expect the '}' keyword
  tokens.PopExpect(TokenType::Parenthesis, "}");
}

void Parser::ParseIfStatement(TNode* stmt_list_node) {
  // expect the if keyword
  tokens.PopExpect(TokenType::If);

  // expect the '(' keyword
  tokens.PopExpect(TokenType::Parenthesis, "(");
  // create if node and add as child of stmt_list_node
  TNode* if_node = new TNode(TNodeType::If, statement_number);
  stmt_list_node->AddChild(if_node);
  ParseConditionalExpression(if_node);
  // expect the ')' keyword
  tokens.PopExpect(TokenType::Parenthesis, ")");

  // expect the 'then' keyword
  tokens.PopExpect(TokenType::Then);
  // expect the '{' keyword
  tokens.PopExpect(TokenType::Parenthesis, "{");
  // parse statement list of 'then'
  ParseStatementList(if_node);
  // expect the '}' keyword
  tokens.PopExpect(TokenType::Parenthesis, "}");

  // expect the 'else' keyword
  tokens.PopExpect(TokenType::Else);
  // expect the '{' keyword
  tokens.PopExpect(TokenType::Parenthesis, "{");
  // parse statement list of 'else'
  ParseStatementList(if_node);
  // expect the '}' keyword
  tokens.PopExpect(TokenType::Parenthesis, "}");
}

void Parser::ParseStatement(TNode* stmt_list_node) {
  // increment the statement_number since a statement is encountered
  statement_number++;

  if (tokens.PeekMatch(TokenType::Call)) {
    ParseCallStatement(stmt_list_node);
  } else if (tokens.PeekMatch(TokenType::Read)) {
    ParseReadStatement(stmt_list_node);
  } else if (tokens.PeekMatch(TokenType::Print)) {
    ParsePrintStatement(stmt_list_node);
  } else if (tokens.PeekMatch(TokenType::VariableName)) {
    ParseAssignStatement(stmt_list_node);
  } else if (tokens.PeekMatch(TokenType::While)) {
    ParseWhileStatement(stmt_list_node);
  } else if (tokens.PeekMatch(TokenType::If)) {
    ParseIfStatement(stmt_list_node);
  } else {
    std::stringstream err_msg;
    err_msg << "Parser::ParseStatement: invalid token "
            << "[" << tokens.Peek().GetValue() << "] "
            << "at start of new statement";
    throw std::runtime_error(err_msg.str());
  }
}

void Parser::ParseStatementList(TNode* container_node) {
  if (tokens.PeekMatch(TokenType::Parenthesis, "}")) {
    // next token is immediately '}' which means this statement list is empty
    throw std::runtime_error("Parser::ParseStatementList: empty statement list detected");
  }

  // create statement list node and add as child of container_node
  TNode* stmt_list_node = new TNode(TNodeType::StatementList);
  container_node->AddChild(stmt_list_node);

  // call ParseStatement till we encounter the '}' keyword
  while (!tokens.PeekMatch(TokenType::Parenthesis, "}")) {
    ParseStatement(stmt_list_node);
  }
}

void Parser::ParseProcedure(TNode* program_node) {
  // expect the 'procedure' keyword
  tokens.PopExpect(TokenType::Procedure);
  // expect the proc_name
  Token proc_token = tokens.PopExpect(TokenType::ProcedureName);
  // mutate global pname
  current_pname = proc_token.GetValue();

  // throw if procedure name is not unique
  if (pdeclared_to_pcalled.find(current_pname) != pdeclared_to_pcalled.end()) {
    throw std::runtime_error("Parser::ParseProcedure: repeated procedure name detected");
  }
  // add current pname to pdeclared_to_pcalled
  std::unordered_set<std::string> empty_set;
  pdeclared_to_pcalled.insert(std::make_pair(current_pname, empty_set));

  // expect the '{' keyword
  tokens.PopExpect(TokenType::Parenthesis, "{");
  // create procedure node and add as child of root
  TNode* proc_node = new TNode(TNodeType::Procedure, current_pname);
  program_node->AddChild(proc_node);
  // parse the statement list
  ParseStatementList(proc_node);
  // expect the '}' keyword
  tokens.PopExpect(TokenType::Parenthesis, "}");
}

// Throws iff there exists a call statement which calls a non-existent procedure.
void Parser::ValidateExistentCalls() {
  // check for calls to non-exisiting procedures
  for (const auto& pname_pcalled_pair : pdeclared_to_pcalled) {
    const auto& procedures_called = pname_pcalled_pair.second;
    for (const auto& pcalled : procedures_called) {
      if (pdeclared_to_pcalled.find(pcalled) == pdeclared_to_pcalled.end()) {
        // throw if calling non-existent procedure
        throw std::runtime_error("Parser::ValidateExistentCalls: calling non-existent procedure");
      }
    }
  }
}

bool Parser::ValidateNonRecursiveCallsUtils(const std::string& proc_name,
                                            std::unordered_set<std::string>& visited,
                                            std::unordered_set<std::string>& visiting) {
  if (visited.find(proc_name) == visited.end()) {
    // not yet visited
    visited.insert(proc_name);
    visiting.insert(proc_name);
    const auto& children = pdeclared_to_pcalled.find(proc_name)->second;
    for (const auto& child : children) {
      if (visited.find(child) == visited.end() &&
          ValidateNonRecursiveCallsUtils(child, visited, visiting)) {
        return true;
      } else if (visiting.find(child) != visiting.end()) {
        return true;
      }
    }
  }
  visiting.erase(proc_name);
  return false;
}

// Throws error iff there exists a recursive procedure call.
// https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
void Parser::ValidateNonRecursiveCalls() {
  std::unordered_set<std::string> visited;
  std::unordered_set<std::string> visiting;
  for (const auto& pair : pdeclared_to_pcalled) {
    auto pdeclared = pair.first;
    if (ValidateNonRecursiveCallsUtils(pdeclared, visited, visiting)) {
      throw std::runtime_error("Parser::ValidateNonRecursiveCalls: recursive call statements detected");
    }
  }
}

TNode* Parser::ParseProgram() {
  // init root of the AST:
  TNode* program_node = new TNode(TNodeType::Program);

  while (!tokens.IsEmpty()) {
    ParseProcedure(program_node);
  }

  // validate all call statements:
  ValidateExistentCalls();
  ValidateNonRecursiveCalls();

  return program_node;
}

// Resets all static variables back to default values.
// Should be called when starting to parse new input.
void Parser::Reset() {
  // reset statement number to start from 0:
  statement_number = 0;
  // clear all tokens:
  tokens.Clear();
  // clear procedure names map:
  pdeclared_to_pcalled.clear();
  current_pname = "";
}

// Main entry point; calls the required parsing functions recursively
// and returns a const reference to the root of the AST formed.
const TNode& Parser::Parse(const std::string& raw_input) {
  Reset();
  // assign static TokenList from lexer:
  tokens = Lexer::Tokenise(raw_input);
  // start parsing using recursive descent:
  TNode* ast_root = ParseProgram();

  return *ast_root;
}

}  // namespace source_processor
