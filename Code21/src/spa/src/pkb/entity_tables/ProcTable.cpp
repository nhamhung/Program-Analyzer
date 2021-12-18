#include "ProcTable.h"

bool ProcTable::InsertProc(const std::string& proc_name, const std::pair<int, int>& start_to_end_indexes) {  //todo:check overlap interval
  std::unordered_map<std::string, std::pair<int, int>>& proc_map = proc_table.GetTable();
  for (auto& it : proc_map) {
    if (it.first == proc_name) {
      return false;
    }
    std::pair<int, int> current_proc_index_pair = it.second;
    if (current_proc_index_pair.first <= start_to_end_indexes.second  // overlapping range
        && start_to_end_indexes.first <= current_proc_index_pair.second) {
      return false;
    }
  }

  return proc_table.Insert(proc_name, start_to_end_indexes);
}

std::unordered_set<std::string> ProcTable::GetAllProcedures() {
  return proc_table.GetAllKeys();
}

std::pair<int, int> ProcTable::GetProcRange(const std::string& proc_name) {
  return proc_table.Get(proc_name);
}

void ProcTable::ClearProcTable() {
  proc_table.ClearTable();
}

TableSingle<std::string, std::pair<int, int>> ProcTable::GetProcTable() {
  return proc_table;
}