#pragma once

namespace query_processor {
enum class DesignEntityType {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE,
  PROG_LINE,
  WILDCARD,
};

}