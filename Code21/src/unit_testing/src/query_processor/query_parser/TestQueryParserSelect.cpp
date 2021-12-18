#include <string>

#include "catch.hpp"
#include "query_processor/commons/query/Query.h"
#include "query_processor/query_parser/QueryParser.h"

using namespace std;
using namespace query_processor;

SCENARIO("Testing GenerateQuery() when query's declaration has invalid syntax.", "[query_parser]") {
  WHEN("The query contains no declarations.") {
    string NO_DECLARATION_STRING = "Select stmt";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_DECLARATION_STRING));
    }
  }

  WHEN("One of the query's declaration clauses contains fewer than 2 terms.") {
    string SHORT_DECLARATION_STRING = "stmt ; Select s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SHORT_DECLARATION_STRING));
    }
  }

  WHEN("One of the query's declaration clauses contains more than 2 terms without a comma.") {
    string LONG_DECLARATION_STRING = "stmt stmt s; Select s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LONG_DECLARATION_STRING));
    }
  }

  WHEN("The query's declaration has an invalid design entity name.") {
    string INVALID_DECLARATION_ENTITY_STRING = "Stmt s; Select s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_DECLARATION_ENTITY_STRING));
    }
  }

  WHEN("The query's declaration has an invalid synonym name.") {
    string INVALID_DECLARATION_SYNONYM_STRING = "stmt _s; Select _s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_DECLARATION_SYNONYM_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() when query's select clause has invalid syntax.", "[query_parser]") {
  WHEN("'Select' is absent from the select clause.") {
    string MISSING_SELECT_STRING = "stmt s; s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(MISSING_SELECT_STRING));
    }
  }

  WHEN("'Select' is written in lowercase in the select clause.") {
    string LOWERCASE_SELECT_STRING = "stmt s; select s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LOWERCASE_SELECT_STRING));
    }
  }

  WHEN("No space is given between 'Select' and the following synonym.") {
    string NO_SPACE_STRING = "stmt s; Selects";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_STRING));
    }
  }

  WHEN("The query has an additional semicolon.") {
    string MISSING_SEMICOLON_STRING = "stmt s; Select s;";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(MISSING_SEMICOLON_STRING));
    }
  }

  WHEN("The select clause occurs before some declaration clauses.") {
    string INCORRECT_CLAUSE_ORDER_STRING = "stmt s; Select s; while w";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INCORRECT_CLAUSE_ORDER_STRING));
    }
  }

  WHEN("The selected entity is neither a synonym nor a synonym attribute.") {
    string INCORRECT_SELECTION_TYPE_STRING = "stmt s; Select \"s\"";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INCORRECT_SELECTION_TYPE_STRING));
    }
  }

  WHEN("The BOOLEAN selection has a space within it.") {
    string SPACE_INSIDE_BOOLEAN_STRING = "stmt s; Select BOOL EAN";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SPACE_INSIDE_BOOLEAN_STRING));
    }
  }

  WHEN("The synonym attribute has an invalid attribute type.") {
    string INVALID_ATTRIBUTE_TYPE_STRING = "variable v; Select v.varname";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_ATTRIBUTE_TYPE_STRING));
    }
  }

  WHEN("The synonym attribute has no period.") {
    string MISSING_ATTRIBUTE_PERIOD_STRING = "variable v; Select v varName";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(MISSING_ATTRIBUTE_PERIOD_STRING));
    }
  }

  WHEN("The synonym attribute's synonym is not a synonym.") {
    string ATTRIBUTE_NO_SYNONYM_STRING = "variable v; Select 1.value";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(ATTRIBUTE_NO_SYNONYM_STRING));
    }
  }

  WHEN("The selected tuple has a missing brace.") {
    string MISSING_TUPLE_BRACE_STRING = "variable v, v1; Select <v, v1";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(MISSING_TUPLE_BRACE_STRING));
    }
  }

  WHEN("The selected tuple is empty.") {
    string EMPTY_TUPLE_STRING = "variable v, v1; Select < >";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(EMPTY_TUPLE_STRING));
    }
  }

  WHEN("The selected tuple has no comma.") {
    string MISSING_COMMA_TUPLE_STRING = "variable v, v1; Select <v v1>";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(MISSING_COMMA_TUPLE_STRING));
    }
  }

  WHEN("The selected tuple contains a non-synonym and non-attribute element.") {
    string INCORRECT_ELEMENT_TUPLE_STRING = "variable v, v1; Select <v, 1>";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INCORRECT_ELEMENT_TUPLE_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() when query's select clause has invalid semantics.") {
  WHEN("The declared synonym does not match the synonym used in the select clause.") {
    string INCORRECT_SYNONYM_STRING = "stmt s; Select w";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INCORRECT_SYNONYM_STRING));
    }
  }

  WHEN("Multiple declarations of the same synonym are made.") {
    string MULTIPLE_SAME_SYNONYM_NAME_STRING = "stmt s; while s; Select s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(MULTIPLE_SAME_SYNONYM_NAME_STRING));
    }
  }

  WHEN("The BOOLEAN selection is not entirely uppercase and is therefore treated as an undeclared synonym.") {
    string NON_UPPERCASE_BOOLEAN_STRING = "stmt s; Select BOoLEAN";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NON_UPPERCASE_BOOLEAN_STRING));
    }
  }

  WHEN("The selected synonym attribute's synonym is undeclared.") {
    string UNDECLARED_SYNONYM_ATTRIBUTE_STRING = "assign a; Select c.value";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_SYNONYM_ATTRIBUTE_STRING));
    }
  }

  WHEN("The selected synonym attribute's attribute type does not match the synonym's expected type.") {
    string INCONGRUENT_ATTRIBUTE_TYPE_STRING = "assign a; Select a.value";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INCONGRUENT_ATTRIBUTE_TYPE_STRING));
    }
  }

  WHEN("The selected tuple's synonym is undeclared.") {
    string UNDECLARED_SYNONYM_TUPLE_STRING = "assign a, a1; Select <a, n>";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_SYNONYM_TUPLE_STRING));
    }
  }

  WHEN("The selected tuple's synonym in synonym attribute is undeclared.") {
    string UNDECLARED_SYNONYM_ATTRIBUTE_TUPLE_STRING = "assign a, a1; Select <a.stmt#, c.value>";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_SYNONYM_ATTRIBUTE_TUPLE_STRING));
    }
  }

  WHEN("The selected tuple contains an attribute type does not match the synonym's expected type.") {
    string INCONGRUENT_ATTRIBUTE_TYPE_TUPLE_STRING = "assign a, a1; Select <a.stmt#, a1.value>";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INCONGRUENT_ATTRIBUTE_TYPE_TUPLE_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with only Select clause.", "[query_parser]") {
  WHEN("One valid declaration is given without trailing space.") {
    string ONE_VALID_DECLARATION_STRING = "stmt s;Select s";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(ONE_VALID_DECLARATION_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }

  WHEN("One valid declaration is given and is named 'Select'.") {
    string DECLARATION_NAMED_SELECT_STRING = "stmt Select; Select Select";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(DECLARATION_NAMED_SELECT_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "Select")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }

  WHEN("One valid declaration is given and is named 'boolean'.") {
    string DECLARATION_NAMED_BOOLEAN_STRING = "stmt boolean; Select boolean";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(DECLARATION_NAMED_BOOLEAN_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "boolean")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }

  WHEN("One valid declaration is given and is named 'stmt'.") {
    string DECLARATION_NAMED_STMT_STRING = "stmt stmt; Select stmt";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(DECLARATION_NAMED_STMT_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "stmt")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }

  WHEN("Multiple valid declarations of different types are given and some are not used.") {
    string MULTIPLE_VALID_DIFFERENT_TYPE_DECLARATIONS_STRING = "stmt s; while w; Select s";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_VALID_DIFFERENT_TYPE_DECLARATIONS_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }

  WHEN("Multiple valid declarations of the same type are given and some are not used.") {
    string MULTIPLE_VALID_SAME_TYPE_DECLARATIONS_STRING = "stmt s, s1, s2; Select s1";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_VALID_SAME_TYPE_DECLARATIONS_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }

  WHEN("The same design entity type is declared multiple times.") {
    string MULTIPLE_VALID_SAME_TYPE_DECLARATIONS_STRING = "stmt s; stmt s1; stmt s2; Select s1";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_VALID_SAME_TYPE_DECLARATIONS_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }

  WHEN("Extensive valid whitespace is used in the query.") {
    string VALID_WHITESPACE_STRING = "\t \r\n\n\n \v \rstmt\t \n \v \rs\t\n\v\r\n\n\n\r,\t\n \v\rs1\t\n\n\n\v\r;\t \n \v \rSelect\t\r\n\n\n\n\v\rs1\t \n \v \r";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHITESPACE_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }

  WHEN("A valid input query string with multiple synonyms, design entities, and excessive whitespace is passed as input.") {
    string VALID_STRING = "stmt s,\t\r\n\n\n\r\vs1, s2; if ifs, \t \n \r \vifs1\t\n \r\v, ifs2\t \n \r \v; while w, w1;\t \n \r \vSelect\t\r\n\n\n\r \v ifs1\t\n\r\n\n\r\v";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::IF, "ifs1")));

    THEN("GenerateQuery() generates the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query that selects BOOLEAN.", "[query_parser]") {
  WHEN("BOOLEAN does not appear in the synonyms.") {
    string BASIC_BOOLEAN_STRING = "stmt s; Select BOOLEAN";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(BASIC_BOOLEAN_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
    }
  }

  WHEN("BOOLEAN appears in the synonyms.") {
    string BOOLEAN_IN_SYNONYMS_STRING = "stmt BOOLEAN; Select BOOLEAN";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(BOOLEAN_IN_SYNONYMS_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
    }
  }

  WHEN("No synonym is declared but BOOLEAN is selected.") {
    string NO_DECLARATION_STRING = "Select BOOLEAN";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(NO_DECLARATION_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with only Select clause that selects an attribute.", "[query_parser]") {
  WHEN("The synonym used is declared and the attribute type is correct.") {
    string ONE_VALID_ATTRIBUTE_STRING = "read r; Select r.varName";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(ONE_VALID_ATTRIBUTE_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::READ, "r"), AttributeType::VAR_NAME)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::ATTRIBUTE);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().attribute == EXPECTED_QUERY.GetSelectedEntity().attribute);
    }
  }

  WHEN("The synonym used is declared and the attribute type is correct, and tricky names are used.") {
    string ONE_TRICKY_VALID_ATTRIBUTE_STRING = "constant value; Select value.value";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(ONE_TRICKY_VALID_ATTRIBUTE_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CONSTANT, "value"), AttributeType::VALUE)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::ATTRIBUTE);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().attribute == EXPECTED_QUERY.GetSelectedEntity().attribute);
    }
  }

  WHEN("The synonym used is declared and the attribute type is correct, and excessive valid whitespace exists.") {
    string ONE_VALID_ATTRIBUTE_EXCESSIVE_WHITESPACE_STRING = "call		procName  ; Select		\n\n procName.procName \n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(ONE_VALID_ATTRIBUTE_EXCESSIVE_WHITESPACE_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CALL, "procName"), AttributeType::PROC_NAME)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::ATTRIBUTE);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().attribute == EXPECTED_QUERY.GetSelectedEntity().attribute);
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with only Select clause that selects a tuple.", "[query_parser]") {
  WHEN("One declared synonym is selected in the tuple.") {
    string ONE_VALID_SYNONYM_STRING = "prog_line n; Select <n>";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(ONE_VALID_SYNONYM_STRING);
    Query EXPECTED_QUERY = Query(vector<SelectedEntity>{
        SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n"))});

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(0).entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(0).design_entity == EXPECTED_QUERY.GetSelectedEntities().at(0).design_entity);
    }
  }

  WHEN("One declared synonym attribute is selected in the tuple, and the attribute type is correct.") {
    string ONE_VALID_ATTRIBUTE_STRING = "while w; Select <w.stmt#>";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(ONE_VALID_ATTRIBUTE_STRING);
    Query EXPECTED_QUERY = Query(vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::WHILE, "w"), AttributeType::STMT_NO))});

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(0).entity_type == SelectedEntityType::ATTRIBUTE);
      REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(0).attribute == EXPECTED_QUERY.GetSelectedEntities().at(0).attribute);
    }
  }

  WHEN("Multiple declared synonym attributes are selected in the tuple.") {
    string MULTIPLE_VALID_ATTRIBUTES_STRING = "constant c; if ifs; print pn; procedure p; Select <ifs.stmt#, p.procName, pn.varName, c.value>";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_VALID_ATTRIBUTES_STRING);
    Query EXPECTED_QUERY = Query(vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::IF, "ifs"), AttributeType::STMT_NO)),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::PROCEDURE, "p"), AttributeType::PROC_NAME)),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::PRINT, "pn"), AttributeType::VAR_NAME)),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CONSTANT, "c"), AttributeType::VALUE))});

    THEN("GenerateQuery() returns the expected Query.") {
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetSelectedEntities().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::ATTRIBUTE);
        REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).attribute == EXPECTED_QUERY.GetSelectedEntities().at(i).attribute);
      }
    }
  }

  WHEN("Multiple declared synonyms and synonym attributes are selected in the tuple.") {
    string MULTIPLE_VALID_SYNONYMS_AND_ATTRIBUTES_STRING = "constant c; if ifs; print pn; procedure p; Select <ifs.stmt#, p, pn, c.value>";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_VALID_SYNONYMS_AND_ATTRIBUTES_STRING);
    Query EXPECTED_QUERY = Query(vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::IF, "ifs"), AttributeType::STMT_NO)),
        SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")),
        SelectedEntity(DesignEntity(DesignEntityType::PRINT, "pn")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CONSTANT, "c"), AttributeType::VALUE))});

    THEN("GenerateQuery() returns the expected Query.") {
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetSelectedEntities().size(); i++) {
        if (i == 0 || i == 3) {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::ATTRIBUTE);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).attribute == EXPECTED_QUERY.GetSelectedEntities().at(i).attribute);
        } else {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::DESIGN_ENTITY);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).design_entity == EXPECTED_QUERY.GetSelectedEntities().at(i).design_entity);
        }
      }
    }
  }

  WHEN("Multiple declared synonyms and synonym attributes are selected in the tuple, which also contains duplicated synonyms and synonym attributes.") {
    string MULTIPLE_VALID_SYNONYMS_AND_ATTRIBUTES_STRING = "constant c; if ifs; read r; procedure p; Select <ifs.stmt#, ifs.stmt#, p, r.varName, c.value, c, r.stmt#, p>";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_VALID_SYNONYMS_AND_ATTRIBUTES_STRING);
    Query EXPECTED_QUERY = Query(vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::IF, "ifs"), AttributeType::STMT_NO)),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::IF, "ifs"), AttributeType::STMT_NO)),
        SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::READ, "r"), AttributeType::VAR_NAME)),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CONSTANT, "c"), AttributeType::VALUE)),
        SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "c")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::READ, "r"), AttributeType::STMT_NO)),
        SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p"))});

    THEN("GenerateQuery() returns the expected Query.") {
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetSelectedEntities().size(); i++) {
        if (i == 0 || i == 1 || i == 3 || i == 4 || i == 6) {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::ATTRIBUTE);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).attribute == EXPECTED_QUERY.GetSelectedEntities().at(i).attribute);
        } else {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::DESIGN_ENTITY);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).design_entity == EXPECTED_QUERY.GetSelectedEntities().at(i).design_entity);
        }
      }
    }
  }

  WHEN("Multiple declared synonyms and synonym attributes are selected in the tuple, which uses tricky names.") {
    string MULTIPLE_VALID_SYNONYMS_AND_ATTRIBUTES_STRING = "constant c; if Select; print value; Select <Select.stmt#, c,value, c.value>";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_VALID_SYNONYMS_AND_ATTRIBUTES_STRING);
    Query EXPECTED_QUERY = Query(vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::IF, "Select"), AttributeType::STMT_NO)),
        SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "c")),
        SelectedEntity(DesignEntity(DesignEntityType::PRINT, "value")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CONSTANT, "c"), AttributeType::VALUE))});

    THEN("GenerateQuery() returns the expected Query.") {
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetSelectedEntities().size(); i++) {
        if (i == 0 || i == 3) {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::ATTRIBUTE);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).attribute == EXPECTED_QUERY.GetSelectedEntities().at(i).attribute);
        } else {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::DESIGN_ENTITY);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).design_entity == EXPECTED_QUERY.GetSelectedEntities().at(i).design_entity);
        }
      }
    }
  }

  WHEN("Multiple declared synonyms and synonym attributes are selected in the tuple, which also contains duplicated synonyms and synonym attributes, and contains excessive whitespace.") {
    string MULTIPLE_VALID_SYNONYMS_AND_ATTRIBUTES_STRING = "constant c; if ifs; read r; procedure p;  \n\n\n	Select   <		\t\r\n	ifs.stmt#	,	ifs.stmt#  \t\r\n,		p , r.varName	,		 c.value, \n\n c,  r.stmt#,			p   >		";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_VALID_SYNONYMS_AND_ATTRIBUTES_STRING);
    Query EXPECTED_QUERY = Query(vector<SelectedEntity>{
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::IF, "ifs"), AttributeType::STMT_NO)),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::IF, "ifs"), AttributeType::STMT_NO)),
        SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::READ, "r"), AttributeType::VAR_NAME)),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::CONSTANT, "c"), AttributeType::VALUE)),
        SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "c")),
        SelectedEntity(pair<DesignEntity, AttributeType>(DesignEntity(DesignEntityType::READ, "r"), AttributeType::STMT_NO)),
        SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p"))});

    THEN("GenerateQuery() returns the expected Query.") {
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetSelectedEntities().size(); i++) {
        if (i == 0 || i == 1 || i == 3 || i == 4 || i == 6) {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::ATTRIBUTE);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).attribute == EXPECTED_QUERY.GetSelectedEntities().at(i).attribute);
        } else {
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).entity_type == SelectedEntityType::DESIGN_ENTITY);
          REQUIRE(GENERATED_QUERY.GetSelectedEntities().at(i).design_entity == EXPECTED_QUERY.GetSelectedEntities().at(i).design_entity);
        }
      }
    }
  }
}
