#include "TestUtils.h"

#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "query_processor/query_evaluator/utils/QueryEvaluatorUtils.h"
#include "source_processor/ast/TNode.h"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;
using namespace query_processor;
using namespace std;

bool IsSimilarToken(const Token& t1, const Token& t2) {
  return t1.IsType(t2.GetType()) &&                  // same type
         t1.GetValue().compare(t2.GetValue()) == 0;  // same value
}

// Intentionally pass by value since the lists are manipulated
bool HasSimilarTokens(TokenList xs, TokenList ys) {
  if (xs.GetSize() != ys.GetSize()) {
    return false;
  }

  while (!xs.IsEmpty()) {
    if (!IsSimilarToken(xs.Pop(), ys.Pop())) {
      return false;
    }
  }

  return true;
}
// Checks for equality of member variables without checking children
bool IsSimilarTNode(const TNode& t1, const TNode& t2) {
  return t1.GetStatementNumber() == t2.GetStatementNumber() &&
         t1.IsType(t2.GetType()) &&
         t1.GetValue().compare(t2.GetValue()) == 0;
}

// Uses BFS to check for equality of all member variables including children
bool IsSimilarAST(const TNode& t1, const TNode& t2) {
  std::list<TNode> q1;
  q1.push_back(t1);
  std::list<TNode> q2;
  q2.push_back(t2);

  while (!q1.empty() && !q2.empty()) {
    auto n1 = q1.front();
    q1.pop_front();
    auto n2 = q2.front();
    q2.pop_front();

    if (!IsSimilarTNode(n1, n2)) {
      return false;
    }

    for (auto child : n1.GetChildren()) {
      q1.push_back(*child);
    }
    for (auto child : n2.GetChildren()) {
      q2.push_back(*child);
    }
  }

  return q1.empty() && q2.empty();
}

bool IsSimilarColumn(Column c1, Column c2) {
  if (c1.size() != c2.size()) {
    return false;
  }

  for (auto elem : c1) {
    auto iter = find(c2.begin(), c2.end(), elem);
    if (iter == c2.end()) {
      return false;
    } else {
      c2.erase(iter);
    }
  }
  return true;
}

template <typename T>
bool ContainsExactly(std::unordered_set<T> set, std::vector<T> expected_vals) {
  if (set.size() != expected_vals.size()) {
    return false;
  }
  for (const auto& value : expected_vals) {
    if (!Contains(set, value)) {
      return false;
    }
  }
  return true;
}

bool Contains(unordered_set<string> v, string s) {
  return v.find(s) != v.end();
}

bool Contains(unordered_set<int> v, int i) {
  return v.find(i) != v.end();
}

bool Contains(list<string> l, string s) {
  return find(l.begin(), l.end(), s) != l.end();
}

bool Contains(vector<string> l, string s) {
  return find(l.begin(), l.end(), s) != l.end();
}

bool Contains(vector<int> v, int s) {
  return find(v.begin(), v.end(), s) != v.end();
}

bool Contains(vector<TableElement> column, int i) {
  return find(column.begin(), column.end(), TableElement(i)) != column.end();
}

bool Contains(std::vector<std::vector<std::string>> tuple_list, std::vector<std::string> tuple_elem) {
  return find(tuple_list.begin(), tuple_list.end(), tuple_elem) != tuple_list.end();
}

ResultTable CreateSingleColumnResultTable(std::string synonym, std::unordered_set<int> values) {
  ResultTable result_table = ResultTable();
  Column column = QueryEvaluatorUtils::ConvertSetToColumn(values);
  result_table.AddColumn(synonym, column);
  return result_table;
}

ResultTable CreateSingleColumnResultTable(std::string synonym, std::unordered_set<std::string> values) {
  ResultTable result_table = ResultTable();
  Column column = QueryEvaluatorUtils::ConvertSetToColumn(values);
  result_table.AddColumn(synonym, column);
  return result_table;
}

template bool ContainsExactly<int>(std::unordered_set<int> set, std::vector<int> expected_vals);
template bool ContainsExactly<std::string>(std::unordered_set<std::string> set, std::vector<std::string> expected_vals);

bool IsSimilarCFG(const design_extractor::CFG& g1, const design_extractor::CFG& g2) {
  if (g1.size() != g2.size()) {
    return false;
  }

  for (int i = 0; i < g1.size(); i++) {
    if (g1[i].size() != g2[i].size()) {
      return false;
    }
    for (int stmt_num : g1[i]) {
      if (!Contains(g2[i], stmt_num)) {
        return false;
      }
    }
  }

  return true;
}