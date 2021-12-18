#include "BuildPKBUtils.h"
#include "TestUtils.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "query_processor/QueryProcessor.h"
#include "query_processor/commons/query/Query.h"

using namespace std;
using namespace query_processor;

SCENARIO("Testing selection statements without clauses") {
  GIVEN("PKB built from Sample Code 4 from Basic SPA Requirements") {
    PKB pkb = BuildPKBSampleProgram();
    WHEN("Valid select variables query without clauses") {
      string VALID_SELECT_VARIABLE_QUERY = "variable v, k; Select v";
      list<string> variable_result = QueryProcessor::ProcessQuery(VALID_SELECT_VARIABLE_QUERY, pkb);
      THEN("Returned list of strings must match variable names") {
        REQUIRE(variable_result.size() == 7);
        REQUIRE(Contains(variable_result, "cenX"));
        REQUIRE(Contains(variable_result, "cenY"));
        REQUIRE(Contains(variable_result, "x"));
        REQUIRE(Contains(variable_result, "y"));
        REQUIRE(Contains(variable_result, "count"));
        REQUIRE(Contains(variable_result, "flag"));
        REQUIRE(Contains(variable_result, "normSq"));
      }
    }

    WHEN("Valid select call statement query with annoying synonyms") {
      string VALID_SYNONYM_SAMEAS_KEYWORD_QUERY = "read Select; Select Select";
      list<string> call_result = QueryProcessor::ProcessQuery(VALID_SYNONYM_SAMEAS_KEYWORD_QUERY, pkb);
      THEN("Return all read statements") {
        REQUIRE(call_result.size() == 2);
        REQUIRE(Contains(call_result, "4"));
        REQUIRE(Contains(call_result, "5"));
      }
    }

    WHEN("Invalid select statement query without proper declaration") {
      string INVALID_QUERY_BAD_ENTITY_DECLARATION = "random r; Select r";
      THEN("Throws error due to invalid design entity") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_QUERY_BAD_ENTITY_DECLARATION, pkb));
      }

      string INVALID_QUERY_BAD_SYNONYM_DECLARATION = "while w; Select stmt";
      THEN("Throws error due to undeclared synonym") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_QUERY_BAD_SYNONYM_DECLARATION, pkb) == list<string>{});
      }

      string INVALID_QUERY_BAD_SYNONYM_SYNTAX = "while w_2; Select w_2";
      THEN("Throws error due to invalid synonym syntax") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_QUERY_BAD_SYNONYM_SYNTAX, pkb) == list<string>{});
      }
    }
  }
}

SCENARIO("Test selection statements with one clause") {
  GIVEN("PKB built from Sample Code 4 from Basic SPA Requirements") {
    PKB pkb = BuildPKBSampleProgram();
    WHEN("Valid Follows clause returns empty list") {
      string VALID_FOLLOWS_QUERY_NONE = "print p1; while w1; variable v1; Select v1 such that Follows(p1, w1)";
      list<string> empty_result = QueryProcessor::ProcessQuery(VALID_FOLLOWS_QUERY_NONE, pkb);
      THEN("Returns empty list") {
        REQUIRE(empty_result.empty());
      }
    }

    WHEN("Valid Follows* clause returns procedure names") {
      string VALID_FOLLOWS_SELECT_PROCEDURE = "read r; procedure p; Select p such that Follows* (r, _)";
      list<string> procedure_result = QueryProcessor::ProcessQuery(VALID_FOLLOWS_SELECT_PROCEDURE, pkb);
      THEN("Returns list of all procedures") {
        REQUIRE(procedure_result.size() == 4);
        REQUIRE(Contains(procedure_result, "main"));
        REQUIRE(Contains(procedure_result, "readPoint"));
        REQUIRE(Contains(procedure_result, "printResults"));
        REQUIRE(Contains(procedure_result, "computeCentroid"));
      }
    }

    WHEN("Valid Follows query with naughty synonym names") {
      string VALID_FOLLOWS_WILD_SYNONYMS = "stmt stmt, Follows; if if, assign, Select; Select Select such that Follows(Follows, Select)";
      list<string> if_result = QueryProcessor::ProcessQuery(VALID_FOLLOWS_WILD_SYNONYMS, pkb);
      THEN("Return all if statements that come after other statements") {
        REQUIRE(if_result.size() == 1);
        REQUIRE(Contains(if_result, "19"));
      }
    }

    WHEN("Invalid Follows clause with variable or procedure name") {
      string INVALID_FOLLOWS_CLAUSE_PARAMS = "stmt s; Select s such that Follows(s, \"varX\")";
      THEN("Throws error due to invalid Follows clause parameter") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_FOLLOWS_CLAUSE_PARAMS, pkb));
      }
    }

    WHEN("Invalid Follows clause with wrong design entity") {
      string INVALID_FOLLOWS_CLAUSE_DESIGN_ENTITY = "stmt s; procedure p; Select s such that Follows(s, p)";
      THEN("Throws error due to invalid procedure FollowsClause param") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_FOLLOWS_CLAUSE_DESIGN_ENTITY, pkb));
      }
    }

    WHEN("Valid Parent* clause returns nested assignment statements") {
      string VALID_PARENT_SELECT_VARIABLE = "assign a; Select a such that Parent*(14, a)";
      list<string> assign_result = QueryProcessor::ProcessQuery(VALID_PARENT_SELECT_VARIABLE, pkb);
      THEN("Returns all assignment statements inside statement 14") {
        REQUIRE(assign_result.size() == 3);
        REQUIRE(Contains(assign_result, "15"));
        REQUIRE(Contains(assign_result, "16"));
        REQUIRE(Contains(assign_result, "17"));
      }
    }

    WHEN("Invalid Parent clause with wrong design entity") {
      string INVALID_PARENT_CLAUSE_VARIABLE = "variable v; Select v such that Parent(5, v)";
      THEN("Throws error due to invalid Parent clause param variable") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_PARENT_CLAUSE_VARIABLE, pkb));
      }
    }

    WHEN("Invalid Uses clause with wildcard as first parameter") {
      string INVALID_USES_CLAUSE_VARIABLE = "variable v; Select v such that Uses(_, v)";
      THEN("Throws error due to invalid SuchThatClause parameter for design entity") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_USES_CLAUSE_VARIABLE, pkb));
      }
    }

    WHEN("Valid Uses clause returns while statements that use a specific variable") {
      string VALID_USES_CLAUSE_WHILE = "while w; Select w such that Uses(w, \"normSq\")";
      list<string> while_result = QueryProcessor::ProcessQuery(VALID_USES_CLAUSE_WHILE, pkb);
      THEN("Returns no values, since none fulfill the conditions") {
        REQUIRE(while_result.size() == 0);
      }
    }

    WHEN("Invalid Modifies clause with constant as second parameter") {
      string INVALID_MODIFIES_CLAUSE_CONSTANT = "assign a; constant c; Select a such that Modifies(a, c)";
      THEN("Throws error due to invalid constant parameter in Modifies clause") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_MODIFIES_CLAUSE_CONSTANT, pkb));
      }
    }

    WHEN("Valid Modifies clause returns statements that modify any variable") {
      string VALID_MODIFIES_CLAUSE_STMT = "stmt s; variable v; Select s such that Modifies(s, v)";
      list<string> statement_result = QueryProcessor::ProcessQuery(VALID_MODIFIES_CLAUSE_STMT, pkb);
      THEN("Returns all statements that modify some variable") {
        REQUIRE(statement_result.size() == 18);
        REQUIRE(Contains(statement_result, "1"));
        REQUIRE(Contains(statement_result, "2"));
        REQUIRE(Contains(statement_result, "4"));
        REQUIRE(Contains(statement_result, "5"));
        REQUIRE(Contains(statement_result, "10"));
        REQUIRE(Contains(statement_result, "11"));
        REQUIRE(Contains(statement_result, "12"));
        REQUIRE(Contains(statement_result, "13"));
        REQUIRE(Contains(statement_result, "14"));
        REQUIRE(Contains(statement_result, "15"));
        REQUIRE(Contains(statement_result, "16"));
        REQUIRE(Contains(statement_result, "17"));
        REQUIRE(Contains(statement_result, "18"));
        REQUIRE(Contains(statement_result, "19"));
        REQUIRE(Contains(statement_result, "20"));
        REQUIRE(Contains(statement_result, "21"));
        REQUIRE(Contains(statement_result, "22"));
        REQUIRE(Contains(statement_result, "23"));
      }
    }

    //WHEN("Valid Modifies clause returns procedures that modify any variable") {
    //    string VALID_MODIFIES_CLAUSE_PROCEDURE = "procedure p; variable v; Select p such that Modifies(p, v)";
    //    list<string> procedure_result = QueryProcessor::ProcessQuery(VALID_MODIFIES_CLAUSE_PROCEDURE, pkb);
    //    THEN("Returns all procedures that modify any variable") {
    //        REQUIRE(procedure_result.size() == 3);
    //        REQUIRE(Contains(procedure_result, "main"));
    //        REQUIRE(Contains(procedure_result, "readPoint"));
    //        REQUIRE(Contains(procedure_result, "computeCentroid"));
    //    }
    //}

    WHEN("Invalid pattern clause with variable as synonym") {
      string INVALID_PATTERN_CLAUSE_VARIABLE = "assign a; variable v; Select a pattern v(_, _\"x\"_)";
      THEN("Returns empty result due to invalid PatternClause parameter for design entity.") {
        REQUIRE_THROWS(QueryProcessor::ProcessQuery(INVALID_PATTERN_CLAUSE_VARIABLE, pkb));
      }
    }

    WHEN("Valid pattern clause returns assignment statements that use 'cenX'") {
      string VALID_PATTERN_CLAUSE_CENX = "assign a; Select a pattern a(_, _\"cenX\"_)";
      list<string> assign_result = QueryProcessor::ProcessQuery(VALID_PATTERN_CLAUSE_CENX, pkb);
      THEN("Returns all assignment statements that use 'cenX'") {
        REQUIRE(assign_result.size() == 3);
        REQUIRE(Contains(assign_result, "16"));
        REQUIRE(Contains(assign_result, "21"));
        REQUIRE(Contains(assign_result, "23"));
      }
    }
  }
}

SCENARIO("Test selection of booleans") {
  GIVEN("PKB built from Sample Code 4 from Basic SPA Requirements") {
    PKB pkb = BuildPKBSampleProgram();
    WHEN("Query evaluates to true") {
      string VALID_QUERY_RETURN_TRUE = "assign a1, a2; while w; Select BOOLEAN such that Follows(a1, a2) such that Parent(w, a2)";
      list<string> boolean_result = QueryProcessor::ProcessQuery(VALID_QUERY_RETURN_TRUE, pkb);
      THEN("Return TRUE") {
        REQUIRE(boolean_result.front() == "TRUE");
      }
    }

    WHEN("Query evaluates to false") {
      string VALID_QUERY_RETURN_FALSE = "assign a1, a2; while w; Select BOOLEAN such that Follows*(a1, a2) pattern a2(\"count\", _)";
      list<string> boolean_result = QueryProcessor::ProcessQuery(VALID_QUERY_RETURN_FALSE, pkb);
      THEN("Return FALSE") {
        REQUIRE(boolean_result.front() == "FALSE");
      }
    }
  }
}

SCENARIO("Test one such that and one pattern clauase") {
  GIVEN("PKB built from Sample Code 4 from Basic SPA Requirements") {
    PKB pkb = BuildPKBSampleProgram();
    WHEN("Clauses evaluate independetly to true, selected design entity is dependent") {
      string query = "assign a; stmt s; Select s such that Uses(s, \"cenY\") pattern a(_,_)";
      list<string> result = QueryProcessor::ProcessQuery(query, pkb);
      THEN("Returns all statements that use cenY") {
        REQUIRE(result.size() == 8);
        REQUIRE(Contains(result, "2"));
        REQUIRE(Contains(result, "3"));
        REQUIRE(Contains(result, "8"));
        REQUIRE(Contains(result, "14"));
        REQUIRE(Contains(result, "17"));
        REQUIRE(Contains(result, "19"));
        REQUIRE(Contains(result, "22"));
        REQUIRE(Contains(result, "23"));
      }
    }
  }
}
