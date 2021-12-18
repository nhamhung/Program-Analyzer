#include <string>

#include "catch.hpp"
#include "query_processor/commons/query/Query.h"
#include "query_processor/query_parser/QueryParser.h"

using namespace std;
using namespace query_processor;

// -----------------        GenerateQuery() invalid syntax test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'with' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'with'.") {
    string EXTRA_SPACE_WITH_STRING = "prog_line n; Select n wi th 1 = 1";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(EXTRA_SPACE_WITH_STRING));
    }
  }

  WHEN("No clauses follow 'with'.") {
    string NO_CLAUSE_WITH_STRING = "prog_line n; Select n with ";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_CLAUSE_WITH_STRING));
    }
  }

  WHEN("The 'Select' clause comes after the 'with' clause.") {
    string WRONG_ORDER_SELECT_WITH_CLAUSE = "prog_line n; with 1 = 1 Select n";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(WRONG_ORDER_SELECT_WITH_CLAUSE));
    }
  }

  WHEN("There is no whitespace after 'with'.") {
    string NO_SPACE_WITH_STRING = "prog_line n; Select n with1 = 1";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_WITH_STRING));
    }
  }

  WHEN("There is no '=' token.") {
    string NO_EQUALS_SIGN_WITH_STRING = "prog_line n; Select n with 1 1";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_EQUALS_SIGN_WITH_STRING));
    }
  }

  WHEN("There is an extra '=' token.") {
    string EXTRA_EQUALS_SIGN_WITH_STRING = "prog_line n; Select n with 1 == 1";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(EXTRA_EQUALS_SIGN_WITH_STRING));
    }
  }

  WHEN("The first parameter is not a valid reference.") {
    string FIRST_WRONG_REF_WITH_STRING = "prog_line n; Select n with 1n = 1";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(FIRST_WRONG_REF_WITH_STRING));
    }
  }

  WHEN("The second parameter is not a valid reference.") {
    string SECOND_WRONG_REF_WITH_STRING = "prog_line n; Select n with 1 = 1.stmt#";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SECOND_WRONG_REF_WITH_STRING));
    }
  }

  WHEN("The attribute synonym contains an invalid attribute.") {
    string INVALID_ATTRIBUTE_WITH_STRING = "prog_line n; Select n with n.stmt# = n.stmt";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_ATTRIBUTE_WITH_STRING));
    }
  }

  WHEN("The synonym attribute contains whitespace within the term.") {
    string SYNONYM_ATTRIBUTE_SPACE_WITH_STRING = "prog_line n; Select n with n.stmt# = n. stmt#";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(SYNONYM_ATTRIBUTE_SPACE_WITH_STRING));
    }
  }
}

// -----------------        GenerateQuery() invalid semantics test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'with' clauses with invalid semantics.", "[query_parser]") {
  WHEN("The synonym used is undeclared.") {
    string EXTRA_SPACE_WITH_STRING = "prog_line n; Select n with n = s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(EXTRA_SPACE_WITH_STRING));
    }
  }

  WHEN("The synonym used in the synonym attribute is undeclared.") {
    string NO_CLAUSE_WITH_STRING = "variable v; Select n with p.procName = v.varName";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_CLAUSE_WITH_STRING));
    }
  }
}

// -----------------        GenerateQuery() single valid clause test cases        --------------------

SCENARIO("Testing GenerateQuery() given a valid query with 'with' clauses.", "[query_parser]") {
  WHEN("'with' clause compares two integers.") {
    string TWO_INTS_WITH_STRING = "prog_line n; Select n with 1 = 1";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_INTS_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("'with' clause compares two synonyms.") {
    string TWO_SYNONYMS_WITH_STRING = "prog_line n, n1; Select n with n = n1";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n1")), AttributeType::NONE))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("'with' clause compares a trickily-named synonym string with a string-based synonym attribute.") {
    string SYNONYM_STRING_ATTRIBUTE_WITH_STRING = "variable varName; Select varName with \"test\" = varName.varName";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_STRING_ATTRIBUTE_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::VARIABLE, "varName")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam("test"), AttributeType::NAME),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "varName")), AttributeType::VAR_NAME))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("'with' clause compares an integer with a trickily-named integer-based synonym attribute.") {
    string INTEGER_ATTRIBUTE_WITH_STRING = "constant with; Select with with with.value = 5";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INTEGER_ATTRIBUTE_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::CONSTANT, "with")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "with")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(5), AttributeType::INTEGER))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("'with' clause compares an integer with a prog_line synonym.") {
    string INTEGER_SYNONYM_WITH_STRING = "prog_line n; Select n with 10 = n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INTEGER_SYNONYM_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(10), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("'with' clause compares an integer with a prog_line synonym, with excessive valid whitespace.") {
    string INTEGER_SYNONYM_WITH_STRING = " \n\n\nprog_line		n ;		Select n		 with	 10	 \n\n\n	=	\t\r\t\r\n\n\n n  ";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INTEGER_SYNONYM_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(10), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with 'with' clauses, where BOOLEAN is selected.", "[query_parser]") {
  WHEN("'with' clause compares a synonym string with a string-based synonym attribute.") {
    string SYNONYM_STRING_ATTRIBUTE_WITH_STRING = "call BOOLEAN; Select BOOLEAN with BOOLEAN.procName = \"BOOLEAN\"";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_STRING_ATTRIBUTE_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::CALL, "BOOLEAN")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CALL, "BOOLEAN")), AttributeType::PROC_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("'with' clause compares an integer with an integer-based synonym attribute.") {
    string INTEGER_ATTRIBUTE_WITH_STRING = "print BOOLEAN; Select BOOLEAN with 100 = BOOLEAN.stmt#";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INTEGER_ATTRIBUTE_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PRINT, "BOOLEAN")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(100), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PRINT, "BOOLEAN")), AttributeType::STMT_NO))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

// -----------------        GenerateQuery() multiple valid clause test cases        --------------------

SCENARIO("Testing GenerateQuery() given a valid query with multiple 'with' clauses only.", "[query_parser]") {
  WHEN("The query contains only two 'with' clauses.") {
    string TWO_WITH_CLAUSES_STRING = "prog_line n, n1; Select n with 1 = 1 with n = n1";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WITH_CLAUSES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROG_LINE, "n")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n1")), AttributeType::NONE))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query selects BOOLEAN and contains only two 'with' clauses.") {
    string TWO_SYNONYM_BOOLEAN_WITH_STRING = "assign a; read BOOLEAN; Select BOOLEAN with BOOLEAN.varName = \"BOOLEAN\" with 20 = a.stmt#";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYM_BOOLEAN_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::READ, "BOOLEAN")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query contains only multiple 'with' clauses.") {
    string MULTIPLE_SYNONYMS_WITH_STRING = "assign a; constant c; stmt s; prog_line n; variable BOOLEAN; Select s with n = 3 with 2 = 1 with BOOLEAN.varName = \"BOOLEAN\" with 20 = a.stmt# with c.value = 2";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_SYNONYMS_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("The query contains only multiple 'with' clauses, and has excessive valid whitespace.") {
    string MULTIPLE_SYNONYMS_WHITESPACE_WITH_STRING = "assign a; constant c; stmt s; prog_line n; variable BOOLEAN; Select s with n =3		with 2= 1	with	\n\n BOOLEAN.varName	= \"BOOLEAN\" with\n\n20	\t=		a.stmt# with c.value = 2";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_SYNONYMS_WHITESPACE_WITH_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n")), AttributeType::NONE),
        pair<ClauseParam, AttributeType>(ClauseParam(3), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(1), AttributeType::INTEGER))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN")), AttributeType::VAR_NAME),
        pair<ClauseParam, AttributeType>(ClauseParam("BOOLEAN"), AttributeType::NAME))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(20), AttributeType::INTEGER),
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a")), AttributeType::STMT_NO))));
    EXPECTED_QUERY.AddClause(Clause(WithClause(
        pair<ClauseParam, AttributeType>(ClauseParam(DesignEntity(DesignEntityType::CONSTANT, "c")), AttributeType::VALUE),
        pair<ClauseParam, AttributeType>(ClauseParam(2), AttributeType::INTEGER))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }
}