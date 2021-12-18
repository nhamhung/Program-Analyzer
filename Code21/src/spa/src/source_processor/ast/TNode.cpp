#include "TNode.h"

#include <iomanip>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

#include "TNodeType.h"
#include "source_processor/token/TokenList.h"
#include "source_processor/utils/TypeUtils.h"

namespace source_processor {

// private constructor for use by overloaded constructors
void TNode::Init(TNodeType type, int statement_number,
                 std::string value, TokenList rpn_tokens) {
  this->type = type;
  this->statement_number = statement_number;
  this->value = value;
  this->rpn_tokens = rpn_tokens;
}

TNode::TNode(TNodeType type) {
  Init(type, -1, "", TokenList());
}

TNode::TNode(TNodeType type, std::string value) {
  Init(type, -1, value, TokenList());
}

TNode::TNode(TNodeType type, int statement_number) {
  Init(type, statement_number, "", TokenList());
}

TNode::TNode(TNodeType type, int statement_number, std::string value) {
  Init(type, statement_number, value, TokenList());
}

TNode::TNode(TNodeType type, int statement_number, TokenList rpn_tokens) {
  Init(type, statement_number, "", rpn_tokens);
}

//* methods below for building the AST *//

TNode& TNode::AddChild(TNode* child) {
  children.push_back(child);
  return *this;
}

TNode& TNode::SetConditionalExpressionOp(const std::string& expr_op) {
  if (!IsType(TNodeType::ConditionalExpressionOp)) {
    throw std::runtime_error("TNode::SetConditionalExpressionOp: called from invalid TNodeType");
  }
  if (expr_op != "&&" && expr_op != "||") {
    throw std::runtime_error("TNode::SetConditionalExpressionOp: arg can only be `&&` or `||`");
  }

  value = expr_op;
  return *this;
}

TNode& TNode::Clone(const TNode& root) {
  TNode* new_root = new TNode(root.GetType(), root.GetStatementNumber(), root.GetValue());
  for (const auto child : root.GetChildren()) {
    new_root->AddChild(&Clone(*child));
  }
  return *new_root;
}

//* methods below for traversing/accessing the AST *//

// Returns the child Variable TNode if called from a Read/Print TNode.
// Otherwise, throws an exception.
const TNode& TNode::GetVariableTNode() const {
  if (type != TNodeType::Print && type != TNodeType::Read) {
    throw std::runtime_error("TNode.GetVariableTNode: called from invalid TNodeType");
  }
  return *children[0];
}

// Returns the assignee Variable TNode if called from an Assign TNode.
// Otherwise, throws an exception.
const TokenList& TNode::GetRpnTokens() const {
  if (type != TNodeType::Assign) {
    throw std::runtime_error("TNode.GetRpnTokens: called from invalid TNodeType");
  }
  return rpn_tokens;
}

// Returns the assignee Variable TNode if called from an Assign TNode.
// Otherwise, throws an exception.
const TNode& TNode::GetAssigneeTNode() const {
  if (type != TNodeType::Assign) {
    throw std::runtime_error("TNode.GetAssigneeTNode: called from invalid TNodeType");
  }
  return *children[0];
}

// Returns the root ExpressionOp TNode if called from an Assign TNode.
// Otherwise, throws an exception.
const TNode& TNode::GetRootExpressionTNode() const {
  if (type != TNodeType::Assign) {
    throw std::runtime_error("TNode.GetRootExpressionTNode: called from invalid TNodeType");
  }
  return *children[1];
}

const TNode& TNode::GetLeftExpressionTNode() const {
  if (type != TNodeType::ExpressionOp) {
    throw std::runtime_error("TNode.GetLeftExpressionTNode: called from invalid TNodeType");
  }
  return *children[0];
}

const TNode& TNode::GetRightExpressionTNode() const {
  if (type != TNodeType::ExpressionOp) {
    throw std::runtime_error("TNode.GetRightExpressionTNode: called from invalid TNodeType");
  }
  return *children[1];
}

const TNode& TNode::GetRootConditionalExpressionTNode() const {
  if (type != TNodeType::While &&
      type != TNodeType::If) {
    throw std::runtime_error("TNode.GetRootConditionalExpressionTNode: called from invalid TNodeType");
  }
  return *children[0];
}

const TNode& TNode::GetProcedureStatementListTNode() const {
  if (type != TNodeType::Procedure) {
    throw std::runtime_error("TNode.GetProcedureStatementListTNode: called from invalid TNodeType");
  }
  return *children[0];
}

const TNode& TNode::GetWhileStatementListTNode() const {
  if (type != TNodeType::While) {
    throw std::runtime_error("TNode.GetWhileStatementListTNode: called from invalid TNodeType");
  }
  return *children[1];
}

const TNode& TNode::GetThenStatementListTNode() const {
  if (type != TNodeType::If) {
    throw std::runtime_error("TNode.GetThenStatementListTNode: called from invalid TNodeType");
  }
  return *children[1];
}

const TNode& TNode::GetElseStatementListTNode() const {
  if (type != TNodeType::If) {
    throw std::runtime_error("TNode.GetElseStatementListTNode: called from invalid TNodeType");
  }
  return *children[2];
}

bool TNode::IsType(TNodeType type) const {
  return this->type == type;
}

const TNode& TNode::GetProcedureStmtLst(const TNode& root, std::string pname) {
  auto procedure_nodes = root.GetChildren();

  for (auto proc : procedure_nodes) {
    if (proc->GetValue() == pname) {
      return proc->GetProcedureStatementListTNode();
    }
  }

  // Reaching here means that the pname is not found
  throw std::runtime_error("TNode::GetProcedureStmtLst: argument pname not found in given ast");
}

//* methods below for debugging purposes *//

TNode& TNode::PrintDebugInfo() {
  std::cout << "type: " << TypeUtils::TNodeTypeToName(GetType()) << ", "
            << "statement_number: " << statement_number << ", "
            << "children: " << children.size() << ", "
            << "value: " << value
            << "\n";

  return *this;
}

// DFS from root to pretty print with indentation
// format: TNODE_TYPE [VALUE] (STATEMENT_NUMBER)
void TNode::PrintDebugInfo(const TNode& node, int indent) {
  if (indent) {
    std::cout << std::setw(indent) << ' ';
  }
  std::cout << TypeUtils::TNodeTypeToName(node.GetType());
  if (node.GetValue().size() > 0) {
    std::cout << " [" << node.GetValue() << "]";
  }
  if (node.GetStatementNumber() > 0) {
    std::cout << " (" << node.GetStatementNumber() << ")";
  }
  std::cout << "\n";

  for (auto n : node.GetChildren()) {
    PrintDebugInfo(*n, indent + 2);
  }
}

}  // namespace source_processor