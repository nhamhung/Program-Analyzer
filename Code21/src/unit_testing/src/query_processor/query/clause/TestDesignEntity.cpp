#include "catch.hpp"
#include "query_processor/commons/query/entities/DesignEntity.h"

using namespace std;
using namespace query_processor;

SCENARIO("Test DesignEntity constructor", "[query]") {
  WHEN("Valid DesignEntityType enum and valid synonym passed as constructor.") {
    DesignEntityType VALID_DESIGNENTITYTYPE_PARAMETER = DesignEntityType::STMT;
    string VALID_SYNONYM_PARAMETER = "s";
    DesignEntity test_designentity = DesignEntity(VALID_DESIGNENTITYTYPE_PARAMETER, VALID_SYNONYM_PARAMETER);

    THEN("GetDesignEntityType() returns correct DesignEntityType variable.") {
      REQUIRE(test_designentity.GetDesignEntityType() == VALID_DESIGNENTITYTYPE_PARAMETER);
      REQUIRE(test_designentity.GetSynonym() == "s");
    }
  }

  WHEN("Valid DesignEntityType and invalid synonym passed as constructor.") {
    DesignEntityType VALID_DESIGNENTITYTYPE_PARAMETER = DesignEntityType::STMT;
    string INVALID_SYNONYM_PARAMETER = "";

    THEN("Throws runtime error about empty synonym") {
      REQUIRE_THROWS(DesignEntity(VALID_DESIGNENTITYTYPE_PARAMETER, INVALID_SYNONYM_PARAMETER));
    }
  }
}
