#include "Table.h"

template <class K>
bool Table<K>::Insert(const K& value) {
  if (table.find(value) != table.end()) {
    return false;
  }
  table.insert(value);
  return true;
}

template <class K>
std::unordered_set<K> Table<K>::GetAll() {
  return table;
}

template <class K>
void Table<K>::ClearTable() {
  table.clear();
}

template class Table<int>;
template class Table<std::string>;