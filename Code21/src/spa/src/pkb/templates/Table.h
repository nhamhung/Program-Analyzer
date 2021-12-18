#pragma once

#include <string>
#include <unordered_set>

template <class K>
class Table {
 private:
  std::unordered_set<K> table;

 public:
  Table(){};

  bool Insert(const K&);

  std::unordered_set<K> GetAll();

  void ClearTable();
};
