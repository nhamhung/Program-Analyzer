#include <string>

#include "catch.hpp"
#include "commons.cpp"
#include "parser_utils/ExpressionParser.h"
#include "query_processor/commons/query/Query.h"
#include "query_processor/query_parser/QueryParser.h"

using namespace std;
using namespace query_processor;

// NOTE: To toggle whether this scenario will be run, look at the 'GetTag()' method in commons.cpp

SCENARIO("Testing GenerateQuery() given a query with a huge number of conditional clauses.", GetTag()) {
  WHEN("There are a huge number of 'such that' clauses.") {
    string declaration_clauses = "stmt s1";
    string such_that_clauses = "Select s1 such that Follows(s1, s2)";
    int number_of_clauses = 500;

    for (int i = 2; i < number_of_clauses + 1; i++) {
      declaration_clauses = declaration_clauses + ", s" + to_string(i);
      such_that_clauses = such_that_clauses + " such that Follows(s" + to_string(i) + ", s" + to_string(i + 1) + ")";
    }
    string VERY_LONG_SUCH_THAT_QUERY = declaration_clauses + ", s" + to_string(number_of_clauses + 1) + "; " + such_that_clauses;

    Query GENERATED_QUERY = QueryParser::GenerateQuery(VERY_LONG_SUCH_THAT_QUERY);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")));
    for (int i = 1; i < number_of_clauses + 1; i++) {
      EXPECTED_QUERY.AddClause(Clause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                                     ClauseParam(
                                                         DesignEntity(DesignEntityType::STMT, "s" + to_string(i))),
                                                     ClauseParam(DesignEntity(DesignEntityType::STMT,
                                                                              "s" + to_string(i + 1))))));
    }

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < GENERATED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}
