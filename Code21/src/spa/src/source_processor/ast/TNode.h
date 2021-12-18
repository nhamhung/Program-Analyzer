#pragma once

#include <string>
#include <vector>

#include "TNodeType.h"
#include "source_processor/token/TokenList.h"

namespace source_processor {

class TNode {
 private:
  void Init(TNodeType, int, std::string, TokenList);

 protected:
  TNodeType type;
  int statement_number;
  std::vector<TNode*> children;
  std::string value;     // var_name, proc_name, or const_value
  TokenList rpn_tokens;  // to store the RPN for an expression

 public:
  TNodeType GetType() const { return type; }
  int GetStatementNumber() const { return statement_number; }
  const std::vector<TNode*>& GetChildren() const { return children; }
  const std::string& GetValue() const { return value; }

  TNode(TNodeType);
  TNode(TNodeType, int);
  TNode(TNodeType, std::string);
  TNode(TNodeType, int, std::string);
  TNode(TNodeType, int, TokenList);

  //* methods below for building the AST *//

  TNode& AddChild(TNode*);
  TNode& SetConditionalExpressionOp(const std::string&);
  static TNode& Clone(const TNode&);

  //* methods below for traversing/accessing the AST *//

  bool IsType(TNodeType) const;
  const TNode& GetVariableTNode() const;  // used by Read/Print node

  const TokenList& GetRpnTokens() const;                   // used by Assign node
  const TNode& GetAssigneeTNode() const;                   // used by Assign node
  const TNode& GetRootExpressionTNode() const;             // used by Assign node
  const TNode& GetLeftExpressionTNode() const;             // used by ExpressionOp node
  const TNode& GetRightExpressionTNode() const;            // used by ExpressionOp node
  const TNode& GetRootConditionalExpressionTNode() const;  // used by While and If nodes
  const TNode& GetProcedureStatementListTNode() const;     // used by Procedure node
  const TNode& GetWhileStatementListTNode() const;         // used by While node
  const TNode& GetThenStatementListTNode() const;          // used by If node
  const TNode& GetElseStatementListTNode() const;          // used by If node

  // Returns the StatementList node given the procedure name.
  // Throws runtime_error if no such procedure is found.
  static const TNode& GetProcedureStmtLst(const TNode&, std::string);

  //* methods below for debugging purposes *//

  TNode& PrintDebugInfo();
  static void PrintDebugInfo(const TNode&, int indent = 0);
};

}  // namespace source_processor
