#include "TableMultiple.h"

#include <algorithm>

template <class K, class V>
bool TableMultiple<K, V>::Insert(const K& k, const V& v) {
  if (table.find(k) == table.end()) {
    std::unordered_set<V> values = {v};
    table[k] = values;
    return true;
  } else {
    std::unordered_set<V>& existing_values = table[k];
    if (std::find(existing_values.begin(), existing_values.end(), v) != existing_values.end()) {
      return false;
    }
    existing_values.insert(v);
    return true;
  }
}

template <class K, class V>
int TableMultiple<K, V>::Size() {
  return table.size();
}

template <class K, class V>
std::unordered_set<V>& TableMultiple<K, V>::Get(const K& k) {
  return table[k];
}

template <class K, class V>
bool TableMultiple<K, V>::Contains(const K& k) {
  return table.find(k) != table.end();
}

template <class K, class V>
std::unordered_set<K> TableMultiple<K, V>::GetAllKeys() {
  std::unordered_set<K> all_keys;
  for (auto& it : table) {
    all_keys.insert(it.first);
  }

  return all_keys;
}

template <class K, class V>
bool TableMultiple<K, V>::TableExists() {
  return table.size() >= 0;
}

template <class K, class V>
bool TableMultiple<K, V>::IsEmpty() {
  return table.size() == 0;
}

template <class K, class V>
void TableMultiple<K, V>::ClearTable() {
  table.clear();
}

template class TableMultiple<std::string, std::string>;
template class TableMultiple<int, std::string>;
template class TableMultiple<std::string, int>;
template class TableMultiple<int, int>;
