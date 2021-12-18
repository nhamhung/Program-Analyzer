#include "EntityHandler.h"

#include <string>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/ast/TNode.h"

namespace design_extractor {

const std::string EntityHandler::kprint_string = "print";
const std::string EntityHandler::kread_string = "read";
const std::string EntityHandler::kwhile_string = "while";
const std::string EntityHandler::kif_string = "if";
const std::string EntityHandler::kassign_string = "assign";
const std::string EntityHandler::kcall_string = "call";

void EntityHandler::ExtractStmtEntityType(PKB& pkb, const source_processor::TNode& node) {
  int stmt_num = node.GetStatementNumber();
  if (node.IsType(source_processor::TNodeType::Print)) {
    pkb.InsertEntity(stmt_num, EntityHandler::kprint_string);
  } else if (node.IsType(source_processor::TNodeType::Read)) {
    pkb.InsertEntity(stmt_num, EntityHandler::kread_string);
  } else if (node.IsType(source_processor::TNodeType::While)) {
    pkb.InsertEntity(stmt_num, EntityHandler::kwhile_string);
  } else if (node.IsType(source_processor::TNodeType::If)) {
    pkb.InsertEntity(stmt_num, EntityHandler::kif_string);
  } else if (node.IsType(source_processor::TNodeType::Assign)) {
    pkb.InsertEntity(stmt_num, EntityHandler::kassign_string);
  } else if (node.IsType(source_processor::TNodeType::Call)) {
    pkb.InsertEntity(stmt_num, EntityHandler::kcall_string);
  }
}

}  // namespace design_extractor