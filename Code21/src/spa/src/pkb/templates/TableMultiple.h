#pragma once

#include <string>
#include <unordered_map>  // Hash TableMultiple, O(1) average, O(n) worst case
//#include <map> // self-balancing BST, O(logn)
#include <unordered_set>

/* using class template, so TableMultiple class does not exist, only TableMultiple<K, V> exists*/
template <class K, class V>
class TableMultiple {
 private:
  /* using a hashmap of with keys and values of generic types for each TableMultiple */
  std::unordered_map<K, std::unordered_set<V>> table;

 public:
  TableMultiple(){};

  bool Insert(const K&, const V&);

  std::unordered_set<V>& Get(const K&);

  bool Contains(const K&);

  int Size();

  std::unordered_set<K> GetAllKeys();

  bool TableExists();

  bool IsEmpty();

  void ClearTable();
};