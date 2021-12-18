#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "query_processor/commons/query/clause/ClauseParam.h"
#include "query_processor/commons/query_result/QueryResult.h"

namespace query_processor {
struct TableElement {
  int stmt;
  std::string name;
  QueryResultType type;

  TableElement() {}

  TableElement(int stmt) {
    this->stmt = stmt;
    this->type = QueryResultType::STMTS;
  }

  TableElement(std::string name) {
    this->name = name;
    this->type = QueryResultType::NAMES;
  }

  friend bool operator==(const TableElement& te1, const TableElement& te2) {
    if (te1.type != te2.type) {
      return false;
    }
    if (te1.type == QueryResultType::STMTS) {
      return te1.stmt == te2.stmt;
    }
    if (te1.type == QueryResultType::NAMES) {
      return te1.name == te2.name;
    }
    return false;
  }
};

typedef std::vector<TableElement> Column;
typedef std::unordered_map<std::string, TableElement> Row;

class ResultTable {
 protected:
  std::unordered_map<std::string, Column> columns;

 public:
  ResultTable();

  // Initialise table with synonyms (which are headers of the table)
  ResultTable(std::unordered_set<std::string>);
  Column& GetColumn(const std::string&);
  Column& GetColumn(ClauseParam&);
  bool IsEmpty();
  int GetSize();
  int GetHeight();
  Row GetRowAt(int);
  std::unordered_map<std::string, Column>& GetTable();
  std::unordered_set<std::string> GetHeaders();
  bool Contains(const std::string&);
  bool Contains(ClauseParam&);
  bool AddColumn(const std::string&, Column&);
  bool AddRow(const Row&);
  void Clear();
  ResultTable MergeTable(ResultTable& other);
  ResultTable CrossTable(ResultTable&);
  ResultTable MergeColumn(std::string, Column&);

 private:
  std::vector<std::string> FindIntersectingHeaders(ResultTable&);
  ResultTable InnerJoin(ResultTable&, std::string);
  ResultTable InnerJoin(ResultTable&, const std::vector<std::string>&);
};
}  // namespace query_processor