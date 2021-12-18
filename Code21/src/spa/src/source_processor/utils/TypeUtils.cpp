#include "TypeUtils.h"

#include "source_processor/ast/TNodeType.h"
#include "source_processor/token/TokenType.h"

namespace source_processor {

// TODO: update this whenever TNodeType.h changes
// provides mapping from TNodeType enum to actual name
const std::string TypeUtils::tnode_type_map[] = {
    "Program",
    "Procedure",
    "StatementList",

    "Call",
    "Read",
    "Print",
    "Assign",
    "While",
    "If",

    "Variable",
    "Constant",

    "ExpressionOp",
    "ConditionalExpressionOp",
    "RelativeExpressionOp",
};

// TODO: update this whenever TokenType.h changes
// provides mapping from TokenType enum to actual name
const std::string TypeUtils::token_type_map[] = {
    "ProcedureName",
    "VariableName",
    "ConstantValue",

    "Semicolon",
    "Parenthesis",

    "AssignmentOp",
    "ExpressionOp",
    "ConditionalExpressionOp",
    "RelativeExpressionOp",

    "Procedure",
    "Read",
    "Print",
    "Call",
    "While",
    "If",
    "Then",
    "Else",
};

const std::string& TypeUtils::TNodeTypeToName(TNodeType type) {
  return tnode_type_map[static_cast<int>(type)];
}

const std::string& TypeUtils::TokenTypeToName(TokenType type) {
  return token_type_map[static_cast<int>(type)];
}

}  // namespace source_processor