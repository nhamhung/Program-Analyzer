#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/abstraction_tables/CallsTable.h"

SCENARIO("Construct a new calls_table.") {
  GIVEN("A calls_table has just been constructed.") {
    CallsTable calls_table;

    THEN("All underlying tables exist with size 0.") {
      REQUIRE(calls_table.GetCallsTable().IsEmpty());
      REQUIRE(calls_table.GetInverseCallsTable().IsEmpty());
      REQUIRE(calls_table.GetCallsStmtTable().IsEmpty());
      REQUIRE(calls_table.GetInverseCallsStmtTable().IsEmpty());
      REQUIRE(calls_table.GetCallsTTable().IsEmpty());
      REQUIRE(calls_table.GetInverseCallsTTable().IsEmpty());
    }
  }
}

SCENARIO("Underlying calls_stmt, calls and callsT tables created.") {
  CallsTable calls_table;

  REQUIRE(calls_table.InsertCalls(2, "computeCentroid"));
  REQUIRE(calls_table.InsertCalls(3, "printResults"));
  REQUIRE(calls_table.InsertCalls(13, "readPoint"));
  REQUIRE(calls_table.InsertCalls(18, "readPoint"));
  GIVEN("Test calls_stmt_table.") {
    WHEN("InsertCalls(stmt_no, proc_name) called with valid index and proc_name.") {
      THEN("Insertion returns true. Insertions are successful.") {
        REQUIRE(calls_table.GetCallsStmtTable().Size() == 4);
      }
    }

    WHEN("Inserts to calls_stmt_table with invalid index or invalid proc_name.") {
      THEN("Insertion returns false. Insertions are unsuccessful.") {
        REQUIRE_FALSE(calls_table.InsertCalls(-1, "funcName"));
        REQUIRE_FALSE(calls_table.InsertCalls(0, "funcName"));
        REQUIRE_FALSE(calls_table.InsertCalls(4, ""));
      }
    }

    WHEN("Inserts to calls_stmt_table with existing stmt_index.") {
      THEN("Insertion returns false. Each statement index can only calls one procedure.") {
        REQUIRE_FALSE(calls_table.InsertCalls(2, "otherFunc"));
        REQUIRE_FALSE(calls_table.InsertCalls(3, "otherFunc"));
      }
    }

    WHEN("GetCalledProcedure(stmt_index) called with existing index.") {
      THEN("Returns the procedure name of the statement index.") {
        REQUIRE(calls_table.GetCalledProcedure(2) == "computeCentroid");
        REQUIRE(calls_table.GetCalledProcedure(3) == "printResults");
        REQUIRE(calls_table.GetCalledProcedure(13) == "readPoint");
        REQUIRE(calls_table.GetCalledProcedure(18) == "readPoint");
      }
    }

    WHEN("GetCalledProcedure(stmt_index) called with invalid or non-existing index.") {
      THEN("Returns default empty string.") {
        REQUIRE(calls_table.GetCalledProcedure(0) == "");
        REQUIRE(calls_table.GetCalledProcedure(1) == "");
      }
    }

    WHEN("GetCallsStmtIndex(proc_name) called with existing proc_name.") {
      THEN("Returns a set of all statement indexes that call the given proc_name.") {
        std::unordered_set<int> all_stmt_indexes_that_calls_readPoint = calls_table.GetCallsStmtIndexes("readPoint");
        REQUIRE(all_stmt_indexes_that_calls_readPoint.size() == 2);
        REQUIRE(ContainsExactly(all_stmt_indexes_that_calls_readPoint, {13, 18}));
      }
    }

    WHEN("GetCallsStmtIndex(proc_name) called with invalid or non-existing proc_name.") {
      THEN("Returns default empty set.") {
        std::unordered_set<int> all_stmt_indexes_that_calls_unknown = calls_table.GetCallsStmtIndexes("unknown");
        std::unordered_set<int> all_stmt_indexes_that_calls_empty = calls_table.GetCallsStmtIndexes("");
        REQUIRE(all_stmt_indexes_that_calls_unknown == std::unordered_set<int>());
        REQUIRE(all_stmt_indexes_that_calls_empty == std::unordered_set<int>());
      }
    }

    WHEN("GetAllCallsStmts() called.") {
      THEN("Returns all calls statement indexes.") {
        std::unordered_set<int> all_calls_stmt_indexes = calls_table.GetAllCallsStmts();
        REQUIRE(ContainsExactly(all_calls_stmt_indexes, {2, 3, 13, 18}));
      }
    }
  }

  REQUIRE(calls_table.InsertCalls("main", "computeCentroid"));
  REQUIRE(calls_table.InsertCalls("main", "printResults"));
  REQUIRE(calls_table.InsertCalls("main", "readPoint"));
  REQUIRE(calls_table.InsertCalls("computeCentroid", "readPoint"));
  GIVEN("Test calls_table.") {
    WHEN("InsertCalls(caller, callee) called with caller and callee.") {
      THEN("Insertion returns true. Insertions are successful.") {
        REQUIRE(calls_table.GetCallsTable().Size() == 2);
      }
    }

    WHEN("Inserts to calls_table with invalid caller or callee.") {
      THEN("Insertion returns false. Insertions are unsuccessful.") {
        REQUIRE_FALSE(calls_table.InsertCalls("", "funcName"));
        REQUIRE_FALSE(calls_table.InsertCalls("main", ""));
      }
    }

    WHEN("Inserts to map an existing caller with an existing callee.") {
      THEN("Insertion returns false. The callee is already contained in the set at caller entry.") {
        REQUIRE_FALSE(calls_table.InsertCalls("main", "printResults"));
        REQUIRE_FALSE(calls_table.InsertCalls("computeCentroid", "readPoint"));
      }
    }

    WHEN("IsCalls(caller, callee) called with existing caller and callee.") {
      THEN("Returns true.") {
        REQUIRE(calls_table.IsCalls("main", "computeCentroid"));
        REQUIRE(calls_table.IsCalls("main", "printResults"));
        REQUIRE(calls_table.IsCalls("main", "readPoint"));
        REQUIRE(calls_table.IsCalls("computeCentroid", "readPoint"));
      }
    }

    WHEN("IsCalls(caller, callee) called with invalid or non-existing or false caller and callee.") {
      THEN("Returns false.") {
        REQUIRE_FALSE(calls_table.IsCalls("", "computeCentroid"));
        REQUIRE_FALSE(calls_table.IsCalls("main", ""));
        REQUIRE_FALSE(calls_table.IsCalls("readPoint", "computeCentroid"));
        REQUIRE_FALSE(calls_table.IsCalls("main", "unknown"));
      }
    }

    WHEN("GetProceduresThatCalls(callee) called with existing callee.") {
      THEN("Returns all procedures which calls the given callee.") {
        std::unordered_set<std::string> all_procedures_that_calls_readPoint = calls_table.GetProceduresThatCalls("readPoint");
        REQUIRE(ContainsExactly(all_procedures_that_calls_readPoint, {"main", "computeCentroid"}));
      }
    }

    WHEN("GetProceduresThatCalls(callee) called with invalid or non-existing callee.") {
      THEN("Returns all procedures which calls the given callee.") {
        std::unordered_set<std::string> all_procedures_that_calls_unknown = calls_table.GetProceduresThatCalls("unknown");
        std::unordered_set<std::string> all_procedures_that_calls_empty = calls_table.GetProceduresThatCalls("");
        REQUIRE(all_procedures_that_calls_unknown == std::unordered_set<std::string>());
        REQUIRE(all_procedures_that_calls_empty == std::unordered_set<std::string>());
      }
    }

    WHEN("GetProceduresCalled(caller) called with existing caller.") {
      THEN("Returns all procedures which are called by the given caller.") {
        std::unordered_set<std::string> all_procedures_called_by_main = calls_table.GetProceduresCalled("main");
        REQUIRE(ContainsExactly(all_procedures_called_by_main, {"computeCentroid", "printResults", "readPoint"}));
      }
    }

    WHEN("GetProceduresCalled(caller) called with invalid or non-existing caller.") {
      THEN("Returns all procedures which are called by the given caller.") {
        std::unordered_set<std::string> all_procedures_called_by_unknown = calls_table.GetProceduresCalled("unknown");
        std::unordered_set<std::string> all_procedures_called_by_empty = calls_table.GetProceduresCalled("");
        REQUIRE(all_procedures_called_by_unknown == std::unordered_set<std::string>());
        REQUIRE(all_procedures_called_by_empty == std::unordered_set<std::string>());
      }
    }

    WHEN("GetAllCallsProcedures() called.") {
      THEN("Returns all procedures that calls one or more other procedures.") {
        std::unordered_set<std::string> all_calls_procedures = calls_table.GetAllCallsProcedures();
        REQUIRE(ContainsExactly(all_calls_procedures, {"main", "computeCentroid"}));
      }
    }

    WHEN("GetAllCalledProcedures() called.") {
      THEN("Returns all procedures that are called by one or more other procedures.") {
        std::unordered_set<std::string> all_called_procedures = calls_table.GetAllCalledProcedures();
        REQUIRE(ContainsExactly(all_called_procedures, {"computeCentroid", "printResults", "readPoint"}));
      }
    }
  }

  REQUIRE(calls_table.InsertCallsT("main", "computeCentroid"));
  REQUIRE(calls_table.InsertCallsT("main", "printResults"));
  REQUIRE(calls_table.InsertCallsT("main", "readPoint"));
  REQUIRE(calls_table.InsertCallsT("computeCentroid", "readPoint"));
  GIVEN("Test calls_T_table.") {
    WHEN("InsertCallsT(caller, callee) called with caller and callee.") {
      THEN("Insertion returns true. Insertions are successful.") {
        REQUIRE(calls_table.GetCallsTTable().Size() == 2);
      }
    }

    WHEN("Inserts to calls_T_table with invalid caller or callee.") {
      THEN("Insertion returns false. Insertions are unsuccessful.") {
        REQUIRE_FALSE(calls_table.InsertCallsT("", "funcName"));
        REQUIRE_FALSE(calls_table.InsertCallsT("main", ""));
      }
    }

    WHEN("Inserts to map an existing caller with an existing callee.") {
      THEN("Insertion returns false. The callee is already contained in the set at caller entry.") {
        REQUIRE_FALSE(calls_table.InsertCallsT("main", "printResults"));
        REQUIRE_FALSE(calls_table.InsertCallsT("computeCentroid", "readPoint"));
      }
    }

    WHEN("IsCallsT(caller, callee) called with existing caller and callee.") {
      THEN("Returns true.") {
        REQUIRE(calls_table.IsCallsT("main", "computeCentroid"));
        REQUIRE(calls_table.IsCallsT("main", "printResults"));
        REQUIRE(calls_table.IsCallsT("main", "readPoint"));
        REQUIRE(calls_table.IsCallsT("computeCentroid", "readPoint"));
      }
    }

    WHEN("IsCallsT(caller, callee) called with invalid or non-existing or false caller and callee.") {
      THEN("Returns false.") {
        REQUIRE_FALSE(calls_table.IsCallsT("", "computeCentroid"));
        REQUIRE_FALSE(calls_table.IsCallsT("main", ""));
        REQUIRE_FALSE(calls_table.IsCallsT("readPoint", "computeCentroid"));
        REQUIRE_FALSE(calls_table.IsCallsT("main", "unknown"));
      }
    }

    WHEN("GetProceduresThatCallsT(callee) called with existing callee.") {
      THEN("Returns all procedures which callsT the given callee.") {
        std::unordered_set<std::string> all_procedures_that_callsT_readPoint = calls_table.GetProceduresThatCallsT("readPoint");
        REQUIRE(ContainsExactly(all_procedures_that_callsT_readPoint, {"main", "computeCentroid"}));
      }
    }

    WHEN("GetProceduresThatCallsT(callee) calledT with invalid or non-existing callee.") {
      THEN("Returns all procedures which calls the given callee.") {
        std::unordered_set<std::string> all_procedures_that_callsT_unknown = calls_table.GetProceduresThatCallsT("unknown");
        std::unordered_set<std::string> all_procedures_that_callsT_empty = calls_table.GetProceduresThatCallsT("");
        REQUIRE(all_procedures_that_callsT_unknown == std::unordered_set<std::string>());
        REQUIRE(all_procedures_that_callsT_empty == std::unordered_set<std::string>());
      }
    }

    WHEN("GetProceduresCalledT(caller) called with existing caller.") {
      THEN("Returns all procedures which are calledT by the given caller.") {
        std::unordered_set<std::string> all_procedures_calledT_by_main = calls_table.GetProceduresCalledT("main");
        REQUIRE(ContainsExactly(all_procedures_calledT_by_main, {"computeCentroid", "printResults", "readPoint"}));
      }
    }

    WHEN("GetProceduresCalledT(caller) called with invalid or non-existing caller.") {
      THEN("Returns all procedures which are called by the given caller.") {
        std::unordered_set<std::string> all_procedures_calledT_by_unknown = calls_table.GetProceduresCalledT("unknown");
        std::unordered_set<std::string> all_procedures_calledT_by_empty = calls_table.GetProceduresCalledT("");
        REQUIRE(all_procedures_calledT_by_unknown == std::unordered_set<std::string>());
        REQUIRE(all_procedures_calledT_by_empty == std::unordered_set<std::string>());
      }
    }

    WHEN("GetAllCallsTProcedures() called.") {
      THEN("Returns all procedures that calls one or more other procedures.") {
        std::unordered_set<std::string> all_callsT_procedures = calls_table.GetAllCallsTProcedures();
        REQUIRE(ContainsExactly(all_callsT_procedures, {"main", "computeCentroid"}));
      }
    }

    WHEN("GetAllCalledTProcedures() called.") {
      THEN("Returns all procedures that are called by one or more other procedures.") {
        std::unordered_set<std::string> all_calledT_procedures = calls_table.GetAllCalledTProcedures();
        REQUIRE(ContainsExactly(all_calledT_procedures, {"computeCentroid", "printResults", "readPoint"}));
      }
    }
  }

  GIVEN("ClearCallsTable() called.") {
    THEN("Clear content of calls table. Its size becomes 0.") {
      calls_table.ClearCallsTable();
      REQUIRE(calls_table.GetCallsStmtTable().IsEmpty());
      REQUIRE(calls_table.GetCallsTable().IsEmpty());
      REQUIRE(calls_table.GetCallsTTable().IsEmpty());
      REQUIRE(calls_table.GetInverseCallsStmtTable().IsEmpty());
      REQUIRE(calls_table.GetInverseCallsTable().IsEmpty());
      REQUIRE(calls_table.GetInverseCallsTTable().IsEmpty());
    }
  }
}