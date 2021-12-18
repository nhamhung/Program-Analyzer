#include <string>
#include <utility>

#include "catch.hpp"
#include "query_processor/commons/query/Query.h"

using namespace std;
using namespace query_processor;

SCENARIO("Test Query object constructor", "[query]") {
  WHEN("Valid DesignEntity passed as constructor.") {
    DesignEntityType VALID_DESIGNENTITYTYPE_PARAMETER = DesignEntityType::STMT;
    string VALID_SYNONYM_PARAMETER = "s";
    DesignEntity VALID_DESIGNENTITY_PARAMETER = DesignEntity(VALID_DESIGNENTITYTYPE_PARAMETER, VALID_SYNONYM_PARAMETER);
    SelectedEntity VALID_SELECTEDENTITY_PARAMETER = SelectedEntity(VALID_DESIGNENTITY_PARAMETER);
    Query query = Query(VALID_SELECTEDENTITY_PARAMETER);

    THEN("GetDesignEntityType() returns correct DesignEntityType variable.") {
      REQUIRE(query.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(query.GetSelectedEntity().design_entity.GetDesignEntityType() == VALID_DESIGNENTITYTYPE_PARAMETER);
    }
  }
  WHEN("Valid SelectedEntity BOOLEAN passed as constructor") {
    Query query = Query(SelectedEntity(SelectedEntityType::BOOLEAN));

    THEN("Selected Entity should correspond to boolean") {
      REQUIRE(query.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
    }
  }
  WHEN("Invalid SelectedEntityType passed as constructor") {
    THEN("Throws invalid SelectedEntityType") {
      REQUIRE_THROWS(Query(SelectedEntity(SelectedEntityType::DESIGN_ENTITY)));
    }
  }

  WHEN("Valid SelectedEntity Tuple list passed as constructor") {
    std::vector<SelectedEntity> tuple_list{SelectedEntity(DesignEntity(DesignEntityType::ASSIGN, "a")),
                                           SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w")),
                                           SelectedEntity(DesignEntity(DesignEntityType::STMT, "s"))};

    Query query = Query(tuple_list);
    std::vector<SelectedEntity> result_tuple = query.GetSelectedEntities();
    THEN("SelectedEntity vector should contain selected synonyms in order") {
      REQUIRE(result_tuple.size() == 3);
      REQUIRE(result_tuple.at(0).design_entity == DesignEntity(DesignEntityType::ASSIGN, "a"));
      REQUIRE(result_tuple.at(1).design_entity == DesignEntity(DesignEntityType::WHILE, "w"));
      REQUIRE(result_tuple.at(2).design_entity == DesignEntity(DesignEntityType::STMT, "s"));
    }
  }

  WHEN("Valid SelectedEntity attribute type passed as constructor") {
    Query query = Query(SelectedEntity(make_pair(DesignEntity(DesignEntityType::ASSIGN, "a"), AttributeType::STMT_NO)));
    THEN("Selected design entity and attributes should be the same") {
      REQUIRE(query.GetSelectedEntity().entity_type == SelectedEntityType::ATTRIBUTE);
      REQUIRE(query.GetSelectedEntity().attribute.first == DesignEntity(DesignEntityType::ASSIGN, "a"));
      REQUIRE(query.GetSelectedEntity().attribute.second == AttributeType::STMT_NO);
    }
  }

  WHEN("Invalid SelectedEntity attribute type passed as constructor") {
    DesignEntity proc = DesignEntity(DesignEntityType::PROCEDURE, "p");
    THEN("Throws error upon construction of SelectedEntity") {
      REQUIRE_THROWS(Query(SelectedEntity(make_pair(proc, AttributeType::INTEGER))));
    }
  }
}

SCENARIO("Test adding SuchThatClause to Query") {
  GIVEN("Any clause, valid or invalid") {
    // Validation of the clause is done in QueryEvaluator
    DesignAbstraction param_design_abstraction = DesignAbstraction::FOLLOWS_T;
    ClauseParam param_lhs = ClauseParam(3);
    ClauseParam param_rhs = ClauseParam(4);
    SuchThatClause valid_clause = SuchThatClause(param_design_abstraction, param_lhs, param_rhs);

    WHEN("Adding clause to a Query") {
      Query test_query = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")));
      test_query.AddClause(Clause(valid_clause));
      THEN("Query list must be updated") {
        REQUIRE(test_query.GetClauseList().size() == 1);

        SuchThatClause first_clause = test_query.GetClauseList().front().GetSuchThatClause();
        REQUIRE(first_clause.GetDesignAbstraction() == DesignAbstraction::FOLLOWS_T);
        REQUIRE(first_clause.GetLHSParam().statement_index == 3);
        REQUIRE(first_clause.GetRHSParam().statement_index == 4);
      }
    }
  }
}
