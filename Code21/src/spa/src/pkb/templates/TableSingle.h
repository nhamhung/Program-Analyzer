#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "source_processor/token/TokenList.h"

template <class K, class V>
class TableSingle {
 private:
  std::unordered_map<K, V> table;

 public:
  TableSingle(){};

  bool Insert(const K&, const V&);

  V& Get(const K&);

  bool Contains(const K&);

  int Size();

  std::unordered_set<K> GetAllKeys();

  std::unordered_map<K, V>& GetTable();

  bool TableExists();

  bool IsEmpty();

  void ClearTable();
};