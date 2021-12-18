#include "TableSingle.h"

template <class K, class V>
bool TableSingle<K, V>::Insert(const K& k, const V& v) {
  if (table.find(k) != table.end()) {
    return false;
  } else {
    table[k] = v;
    return true;
  }
}

template <class K, class V>
V& TableSingle<K, V>::Get(const K& k) {
  return table[k];
}

template <class K, class V>
bool TableSingle<K, V>::Contains(const K& k) {
  return table.find(k) != table.end();
}

template <class K, class V>
int TableSingle<K, V>::Size() {
  return table.size();
}

template <class K, class V>
std::unordered_set<K> TableSingle<K, V>::GetAllKeys() {
  std::unordered_set<K> all_keys;
  for (auto& it : table) {
    all_keys.insert(it.first);
  }

  return all_keys;
}

template <class K, class V>
std::unordered_map<K, V>& TableSingle<K, V>::GetTable() {
  return table;
}

template <class K, class V>
bool TableSingle<K, V>::TableExists() {
  return !table.empty();
}

template <class K, class V>
bool TableSingle<K, V>::IsEmpty() {
  return table.empty();
}

template <class K, class V>
void TableSingle<K, V>::ClearTable() {
  table.clear();
}

template class TableSingle<int, std::string>;
template class TableSingle<std::string, int>;
template class TableSingle<std::string, std::string>;
template class TableSingle<int, int>;
template class TableSingle<int, source_processor::TokenList>;
template class TableSingle<std::string, std::pair<int, int>>;