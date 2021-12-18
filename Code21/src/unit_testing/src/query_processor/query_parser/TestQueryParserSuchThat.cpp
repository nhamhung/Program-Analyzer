#include <string>

#include "catch.hpp"
#include "query_processor/commons/query/Query.h"
#include "query_processor/query_parser/QueryParser.h"
#include "utils/Extension.h"

using namespace std;
using namespace query_processor;

// -----------------        GenerateQuery() general invalid test cases        --------------------

SCENARIO("Testing GenerateQuery() on invalid 'such that' clauses.", "[query_parser]") {
  WHEN("There is no space between 'such' and 'that'.") {
    string NO_SPACE_SUCH_THAT_STRING = "stmt s; Select s suchthat Follows(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_SUCH_THAT_STRING));
    }
  }

  WHEN("No clauses follow 'such that'.") {
    string NO_CLAUSE_SUCH_THAT_STRING = "stmt s; Select s such that ";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_CLAUSE_SUCH_THAT_STRING));
    }
  }

  WHEN("The 'Select' clause comes after the 'such that' clause.") {
    string WRONG_ORDER_SELECT_SUCH_THAT_CLAUSE = "stmt s; such that Follows(1, 2) Select s";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(WRONG_ORDER_SELECT_SUCH_THAT_CLAUSE));
    }
  }
}

// -----------------        GenerateQuery() Follows test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'Follows' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'Follows'.") {
    string INVALID_SPACE_FOLLOWS_STRING = "stmt s; Select s such that Foll ows(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_FOLLOWS_STRING));
    }
  }

  WHEN("There is no whitespace before 'Follows'.") {
    string NO_SPACE_FOLLOWS_STRING = "stmt s; Select s such thatFollows(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_FOLLOWS_STRING));
    }
  }

  WHEN("There are no brackets after 'Follows'.") {
    string NO_BRACKETS_FOLLOWS_STRING = "stmt s; Select s such that Follows 1, 2";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_BRACKETS_FOLLOWS_STRING));
    }
  }

  WHEN("There is no comma after 'Follows'.") {
    string NO_COMMA_FOLLOWS_STRING = "stmt s; Select s such that Follows(1 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_COMMA_FOLLOWS_STRING));
    }
  }

  WHEN("The parameter value is a float, not an integer.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Follows(1.0, 2.0)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is an integer containing leading zeroes.") {
    string LEADING_ZEROES_INTEGER_STRING = "stmt s; Select s such that Follows(1, 02)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LEADING_ZEROES_INTEGER_STRING));
    }
  }

  WHEN("The parameter value is a negative integer, with invalid hyphen.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Follows(-1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid underscores.") {
    string INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING = "stmt invalid_character; Select invalid_character such that Follows(invalid_character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid hyphens.") {
    string INVALID_PARAMETER_SYNONYM_HYPHENS_STRING = "stmt invalid-character; Select invalid-character such that Follows(invalid-character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_HYPHENS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid asterisks.") {
    string INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING = "stmt Follows*, Parent*; Select Follows* such that Follows(Follows*, Parent*)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on 'FollowsT' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace before the asterisk.") {
    string INVALID_SPACE_BEFORE_ASTERISK_STRING = "stmt s; Select s such that Follows *(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_BEFORE_ASTERISK_STRING));
    }
  }
}

// -----------------        GenerateQuery() Parent test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'Parent' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'Parent'.") {
    string INVALID_SPACE_FOLLOWS_STRING = "stmt s; Select s such that Par ent(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_FOLLOWS_STRING));
    }
  }

  WHEN("There is no whitespace before 'Parent'.") {
    string NO_SPACE_FOLLOWS_STRING = "stmt s; Select s such thatParent(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_FOLLOWS_STRING));
    }
  }

  WHEN("There are no brackets after 'Parent'.") {
    string NO_BRACKETS_FOLLOWS_STRING = "stmt s; Select s such that Parent 1, 2";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_BRACKETS_FOLLOWS_STRING));
    }
  }

  WHEN("There is no comma after 'Parent'.") {
    string NO_COMMA_FOLLOWS_STRING = "stmt s; Select s such that Parent(1 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_COMMA_FOLLOWS_STRING));
    }
  }

  WHEN("The parameter value is a float, not an integer.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Parent(1.0, 2.0)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is an integer containing leading zeroes.") {
    string LEADING_ZEROES_INTEGER_STRING = "stmt s; Select s such that Parent(1, 02)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LEADING_ZEROES_INTEGER_STRING));
    }
  }

  WHEN("The parameter value is a negative integer, with invalid hyphen.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Parent(-1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid underscores.") {
    string INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING = "stmt invalid_character; Select invalid_character such that Parent(invalid_character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid hyphens.") {
    string INVALID_PARAMETER_SYNONYM_HYPHENS_STRING = "stmt invalid-character; Select invalid-character such that Parent(invalid-character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_HYPHENS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid asterisks.") {
    string INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING = "stmt Parent*, Parent*; Select Parent* such that Parent(Parent*, Parent*)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on 'ParentT' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace before the asterisk.") {
    string INVALID_SPACE_BEFORE_ASTERISK_STRING = "stmt s; Select s such that Parent *(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_BEFORE_ASTERISK_STRING));
    }
  }
}

// -----------------        GenerateQuery() Modifies test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'Modifies' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'Modifies'.") {
    string INVALID_SPACE_MODIFIES_STRING = "stmt s; Select s such that Modi fies(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_MODIFIES_STRING));
    }
  }

  WHEN("There is no whitespace before 'Modifies'.") {
    string NO_SPACE_MODIFIES_STRING = "stmt s; Select s such thatModifies(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_MODIFIES_STRING));
    }
  }

  WHEN("There are no brackets after 'Modifies'.") {
    string NO_BRACKETS_MODIFIES_STRING = "stmt s; Select s such that Modifies 1, 2";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_BRACKETS_MODIFIES_STRING));
    }
  }

  WHEN("There is no comma after 'Modifies'.") {
    string NO_COMMA_MODIFIES_STRING = "stmt s; Select s such that Modifies(1 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_COMMA_MODIFIES_STRING));
    }
  }

  WHEN("The synonym string starts with an integer.") {
    string INVALID_PARAMETER_STRING_FORMAT_STRING = "stmt s; Select s such that Modifies(1, \"2\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_FORMAT_STRING));
    }
  }

  WHEN("The parameter value is a float, not an integer.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Modifies(1.0, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is an integer containing leading zeroes.") {
    string LEADING_ZEROES_INTEGER_STRING = "stmt s; Select s such that Modifies(01, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LEADING_ZEROES_INTEGER_STRING));
    }
  }

  WHEN("The parameter value is a negative integer, with invalid hyphen.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Modifies(-1, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter string contains invalid underscores.") {
    string INVALID_PARAMETER_STRING_UNDERSCORES_STRING = "stmt s; Select s such that Modifies(1, \"invalid_character\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter string contains invalid hyphens.") {
    string INVALID_PARAMETER_STRING_HYPHENS_STRING = "stmt s; Select s such that Modifies(1, \"invalid-character\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_HYPHENS_STRING));
    }
  }

  WHEN("The parameter string contains invalid asterisks.") {
    string INVALID_PARAMETER_STRING_ASTERISKS_STRING = "stmt s; Select s such that Modifies(1, \"Modifies*\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_ASTERISKS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid underscores.") {
    string INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING = "stmt invalid_character; Select invalid_character such that Modifies(invalid_character, \"allowed\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid hyphens.") {
    string INVALID_PARAMETER_SYNONYM_HYPHENS_STRING = "stmt invalid-character; Select invalid-character such that Modifies(invalid-character, \"allowed\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_HYPHENS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid asterisks.") {
    string INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING = "stmt Modifies*, Modifies*; Select Modifies* such that Modifies(Modifies*, Modifies*)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING));
    }
  }
}

// -----------------        GenerateQuery() Uses test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'Uses' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'Uses'.") {
    string INVALID_SPACE_USES_STRING = "stmt s; Select s such that Modi fies(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_USES_STRING));
    }
  }

  WHEN("There is no whitespace before 'Uses'.") {
    string NO_SPACE_USES_STRING = "stmt s; Select s such thatUses(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_USES_STRING));
    }
  }

  WHEN("There are no brackets after 'Uses'.") {
    string NO_BRACKETS_USES_STRING = "stmt s; Select s such that Uses 1, 2";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_BRACKETS_USES_STRING));
    }
  }

  WHEN("There is no comma after 'Uses'.") {
    string NO_COMMA_USES_STRING = "stmt s; Select s such that Uses(1 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_COMMA_USES_STRING));
    }
  }

  WHEN("The synonym string starts with an integer.") {
    string INVALID_PARAMETER_STRING_FORMAT_STRING = "stmt s; Select s such that Uses(1, \"2\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_FORMAT_STRING));
    }
  }

  WHEN("The parameter value is a float, not an integer.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Uses(1.0, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is an integer containing leading zeroes.") {
    string LEADING_ZEROES_INTEGER_STRING = "stmt s; Select s such that Uses(01, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LEADING_ZEROES_INTEGER_STRING));
    }
  }

  WHEN("The parameter value is a negative integer, with invalid hyphen.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Uses(-1, s)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter string contains invalid underscores.") {
    string INVALID_PARAMETER_STRING_UNDERSCORES_STRING = "stmt s; Select s such that Uses(1, \"invalid_character\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter string contains invalid hyphens.") {
    string INVALID_PARAMETER_STRING_HYPHENS_STRING = "stmt s; Select s such that Uses(1, \"invalid-character\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_HYPHENS_STRING));
    }
  }

  WHEN("The parameter string contains invalid asterisks.") {
    string INVALID_PARAMETER_STRING_ASTERISKS_STRING = "stmt s; Select s such that Uses(1, \"Uses*\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_ASTERISKS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid underscores.") {
    string INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING = "stmt invalid_character; Select invalid_character such that Uses(invalid_character, \"allowed\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid hyphens.") {
    string INVALID_PARAMETER_SYNONYM_HYPHENS_STRING = "stmt invalid-character; Select invalid-character such that Uses(invalid-character, \"allowed\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_HYPHENS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid asterisks.") {
    string INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING = "stmt Uses*, Uses*; Select Uses* such that Uses(Uses*, Uses*)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING));
    }
  }
}

// -----------------        GenerateQuery() Next test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'Next' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'Next'.") {
    string INVALID_SPACE_NEXT_STRING = "stmt s; Select s such that Ne xt(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_NEXT_STRING));
    }
  }

  WHEN("There is no whitespace before 'Next'.") {
    string NO_SPACE_NEXT_STRING = "stmt s; Select s such thatNext(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_NEXT_STRING));
    }
  }

  WHEN("There are no brackets after 'Next'.") {
    string NO_BRACKETS_NEXT_STRING = "stmt s; Select s such that Next 1, 2";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_BRACKETS_NEXT_STRING));
    }
  }

  WHEN("There is no comma after 'Next'.") {
    string NO_COMMA_NEXT_STRING = "stmt s; Select s such that Next(1 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_COMMA_NEXT_STRING));
    }
  }

  WHEN("The parameter value is a float, not an integer.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Next(1.0, 2.0)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is an integer containing leading zeroes.") {
    string LEADING_ZEROES_INTEGER_STRING = "stmt s; Select s such that Next(1, 02)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LEADING_ZEROES_INTEGER_STRING));
    }
  }

  WHEN("The parameter value is a negative integer, with invalid hyphen.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Next(-1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid underscores.") {
    string INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING = "stmt invalid_character; Select invalid_character such that Next(invalid_character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid hyphens.") {
    string INVALID_PARAMETER_SYNONYM_HYPHENS_STRING = "stmt invalid-character; Select invalid-character such that Next(invalid-character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_HYPHENS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid asterisks.") {
    string INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING = "stmt Next*, Parent*; Select Next* such that Next(Next*, Parent*)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on 'NextT' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace before the asterisk.") {
    string INVALID_SPACE_BEFORE_ASTERISK_STRING = "stmt s; Select s such that Next *(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_BEFORE_ASTERISK_STRING));
    }
  }
}

// -----------------        GenerateQuery() Affects test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'Affects' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'Affects'.") {
    string INVALID_SPACE_AFFECTS_STRING = "stmt s; Select s such that Affe cts(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_AFFECTS_STRING));
    }
  }

  WHEN("There is no whitespace before 'Affects'.") {
    string NO_SPACE_AFFECTS_STRING = "stmt s; Select s such thatAffects(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_AFFECTS_STRING));
    }
  }

  WHEN("There are no brackets after 'Affects'.") {
    string NO_BRACKETS_AFFECTS_STRING = "stmt s; Select s such that Affects 1, 2";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_BRACKETS_AFFECTS_STRING));
    }
  }

  WHEN("There is no comma after 'Affects'.") {
    string NO_COMMA_AFFECTS_STRING = "stmt s; Select s such that Affects(1 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_COMMA_AFFECTS_STRING));
    }
  }

  WHEN("The parameter value is a float, not an integer.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Affects(1.0, 2.0)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is an integer containing leading zeroes.") {
    string LEADING_ZEROES_INTEGER_STRING = "stmt s; Select s such that Affects(1, 02)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LEADING_ZEROES_INTEGER_STRING));
    }
  }

  WHEN("The parameter value is a negative integer, with invalid hyphen.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that Affects(-1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid underscores.") {
    string INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING = "stmt invalid_character; Select invalid_character such that Affects(invalid_character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid hyphens.") {
    string INVALID_PARAMETER_SYNONYM_HYPHENS_STRING = "stmt invalid-character; Select invalid-character such that Affects(invalid-character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_HYPHENS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid asterisks.") {
    string INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING = "stmt Affects*, Parent*; Select Affects* such that Affects(Affects*, Parent*)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on 'AffectsT' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace before the asterisk.") {
    string INVALID_SPACE_BEFORE_ASTERISK_STRING = "stmt s; Select s such that Affects *(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_BEFORE_ASTERISK_STRING));
    }
  }
}

// -----------------        GenerateQuery() NextBip test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'NextBip' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'NextBip'.") {
    string INVALID_SPACE_NEXT_BIP_STRING = "stmt s; Select s such that Next Bip(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_NEXT_BIP_STRING));
    }
  }

  WHEN("There is no whitespace before 'NextBip'.") {
    string NO_SPACE_NEXT_BIP_STRING = "stmt s; Select s such thatNextBip(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_NEXT_BIP_STRING));
    }
  }

  WHEN("There are no brackets after 'NextBip'.") {
    string NO_BRACKETS_NEXT_BIP_STRING = "stmt s; Select s such that NextBip 1, 2";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_BRACKETS_NEXT_BIP_STRING));
    }
  }

  WHEN("There is no comma after 'NextBip'.") {
    string NO_COMMA_NEXT_BIP_STRING = "stmt s; Select s such that NextBip(1 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_COMMA_NEXT_BIP_STRING));
    }
  }

  WHEN("The parameter value is a float, not an integer.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that NextBip(1.0, 2.0)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is an integer containing leading zeroes.") {
    string LEADING_ZEROES_INTEGER_STRING = "stmt s; Select s such that NextBip(1, 02)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LEADING_ZEROES_INTEGER_STRING));
    }
  }

  WHEN("The parameter value is a negative integer, with invalid hyphen.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that NextBip(-1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is a string.") {
    string INVALID_PARAMETER_STRING_STRING = "stmt s; Select s such that NextBip(\"1\", 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_STRING_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid underscores.") {
    string INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING = "stmt invalid_character; Select invalid_character such that NextBip(invalid_character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid hyphens.") {
    string INVALID_PARAMETER_SYNONYM_HYPHENS_STRING = "stmt invalid-character; Select invalid-character such that NextBip(invalid-character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_HYPHENS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid asterisks.") {
    string INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING = "stmt NextBip*, Parent*; Select NextBip* such that NextBip(NextBip*, Parent*)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING));
    }
  }

  WHEN("The parameter synonym contains extra spaces.") {
    string INVALID_PARAMETER_SYNONYM_SPACES_STRING = "stmt a, thing, space; Select NextBip* such that NextBip(a space, thing)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_SPACES_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on 'NextBipT' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace before the asterisk.") {
    string INVALID_SPACE_BEFORE_ASTERISK_STRING = "stmt s; Select s such that NextBip *(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_BEFORE_ASTERISK_STRING));
    }
  }
}

// -----------------        GenerateQuery() AffectsBip test cases        --------------------

SCENARIO("Testing GenerateQuery() on 'AffectsBip' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace within 'AffectsBip'.") {
    string INVALID_SPACE_AFFECTS_BIP_STRING = "stmt s; Select s such that Affects Bip(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_AFFECTS_BIP_STRING));
    }
  }

  WHEN("There is no whitespace before 'AffectsBip'.") {
    string NO_SPACE_AFFECTS_BIP_STRING = "stmt s; Select s such thatAffectsBip(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_SPACE_AFFECTS_BIP_STRING));
    }
  }

  WHEN("There are no brackets after 'AffectsBip'.") {
    string NO_BRACKETS_AFFECTS_BIP_STRING = "stmt s; Select s such that AffectsBip 1, 2";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_BRACKETS_AFFECTS_BIP_STRING));
    }
  }

  WHEN("There is no comma after 'AffectsBip'.") {
    string NO_COMMA_AFFECTS_BIP_STRING = "stmt s; Select s such that AffectsBip(1 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(NO_COMMA_AFFECTS_BIP_STRING));
    }
  }

  WHEN("The parameter value is a float, not an integer.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that AffectsBip(1.0, 2.0)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter value is an integer containing leading zeroes.") {
    string LEADING_ZEROES_INTEGER_STRING = "stmt s; Select s such that AffectsBip(1, 02)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(LEADING_ZEROES_INTEGER_STRING));
    }
  }

  WHEN("The parameter value is a negative integer, with invalid hyphen.") {
    string INVALID_PARAMETER_FLOAT_STRING = "stmt s; Select s such that AffectsBip(-1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_FLOAT_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid underscores.") {
    string INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING = "stmt invalid_character; Select invalid_character such that AffectsBip(invalid_character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_UNDERSCORES_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid hyphens.") {
    string INVALID_PARAMETER_SYNONYM_HYPHENS_STRING = "stmt invalid-character; Select invalid-character such that AffectsBip(invalid-character, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_HYPHENS_STRING));
    }
  }

  WHEN("The parameter synonym contains invalid asterisks.") {
    string INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING = "stmt AffectsBip*, Parent*; Select AffectsBip* such that AffectsBip(AffectsBip*, Parent*)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_PARAMETER_SYNONYM_ASTERISKS_STRING));
    }
  }
}

SCENARIO("Testing GenerateQuery() on 'AffectsBipT' clauses with invalid syntax.", "[query_parser]") {
  WHEN("There is whitespace before the asterisk.") {
    string INVALID_SPACE_BEFORE_ASTERISK_STRING = "stmt s; Select s such that AffectsBip *(1, 2)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(INVALID_SPACE_BEFORE_ASTERISK_STRING));
    }
  }
}

// -----------------        GenerateQuery() invalid semantics test cases        --------------------

SCENARIO("Testing GenerateQuery() given an invalid query with a such that clause.", "[query_parser]") {
  WHEN("The parameter synonym is undeclared in the declarations, for a Follows clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that Follows(1, s1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }

  WHEN("The parameter synonym is undeclared in the declarations, for a Parent clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that Parent(s1, 1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }

  WHEN("The parameter synonym is undeclared in the declarations, for a Modifies clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that Modifies(s1, \"s\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }

  WHEN("The parameter synonym is undeclared in the declarations, for a Uses clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that Uses(s1, \"s\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }

  WHEN("The parameter synonym is undeclared in the declarations, for a Calls clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that Calls(s1, \"s\")";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }

  WHEN("The parameter synonym is undeclared in the declarations, for a Next clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that Next(_, s1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }

  WHEN("The parameter synonym is undeclared in the declarations, for an Affects clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that Affects(s1, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }

  WHEN("The parameter synonym is undeclared in the declarations, for a NextBip clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that NextBip(_, s1)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }

  WHEN("The parameter synonym is undeclared in the declarations, for an AffectsBip clause.") {
    string UNDECLARED_PARAMETER_STRING = "stmt s; Select s such that AffectsBip(s1, _)";

    THEN("GenerateQuery() throws an std::runtime_error.") {
      REQUIRE_THROWS(QueryParser::GenerateQuery(UNDECLARED_PARAMETER_STRING));
    }
  }
}

// -----------------        GenerateQuery() valid test cases        --------------------

SCENARIO("Testing GenerateQuery() given a valid query with Follows clauses.", "[query_parser]") {
  WHEN("Follows clause compares an int with a valid synonym.") {
    string INT_SYNONYM_FOLLOWS_STRING = "stmt s; Select s such that Follows(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Follows clause uses synonyms and synonym strings with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, Follows; while while, if, Select; Select Select such that Follows(if, Select)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "if")),
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Follows clause compares two valid synonyms.") {
    string TWO_SYNONYMS_FOLLOWS_STRING = "stmt s, s1; Select s such that Follows(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Follows clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_FOLLOWS_STRING = "stmt s; Select s such that Follows(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Follows clause compares two wildcards.") {
    string TWO_WILDCARDS_FOLLOWS_STRING = "stmt s; Select s such that Follows(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WILDCARDS_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Follows clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_FOLLOWS_STRING = "stmt s; Select s \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nFollows\n\n\t   \n( \t \n\n \r\n1\n\n\n,\r\n\n\n   2\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with FollowsT clauses.", "[query_parser]") {
  WHEN("FollowsT clause has excessive valid whitespace between the asterisk and parameters.") {
    string VALID_WHITESPACE_FOLLOWST_STRING = "stmt s; Select s such that Follows*   \t\t\n\n\n\n \r\n\v\t (1, 2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHITESPACE_FOLLOWST_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with Parent clauses.", "[query_parser]") {
  WHEN("Parent clause compares an int with a valid synonym.") {
    string INT_SYNONYM_PARENT_STRING = "stmt s; Select s such that Parent(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_PARENT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Parent clause uses synonyms and synonym strings with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, Parent; while while, if, Select; Select Select such that Parent(if, Select)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "if")),
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Parent clause compares two valid synonyms.") {
    string TWO_SYNONYMS_PARENT_STRING = "stmt s, s1; Select s such that Parent(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_PARENT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Parent clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_PARENT_STRING = "stmt s; Select s such that Parent(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_PARENT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Parent clause compares two wildcards.") {
    string TWO_WILDCARDS_PARENT_STRING = "stmt s; Select s such that Parent(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WILDCARDS_PARENT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Parent clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_PARENT_STRING = "stmt s; Select s \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nParent\n\n\t   \n( \t \n\n \r\n1\n\n\n,\r\n\n\n   2\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_PARENT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with ParentT clauses.", "[query_parser]") {
  WHEN("ParentT clause has excessive valid whitespace between the asterisk and parameters.") {
    string VALID_WHITESPACE_PARENTT_STRING = "stmt s; Select s such that Parent*   \t\t\n\n\n\n \r\n\v\t (1, 2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHITESPACE_PARENTT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT_T,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with Modifies clauses.", "[query_parser]") {
  WHEN("Modifies clause compares an int with a valid synonym.") {
    string INT_SYNONYM_MODIFIES_STRING = "stmt s; Select s such that Modifies(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_MODIFIES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Modifies clause compares a valid synonym with a valid string.") {
    string STRING_SYNONYM_FOLLOWS_STRING = "stmt s; Select s such that Modifies(  s,    \"somestring\"   )";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(STRING_SYNONYM_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam("somestring")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Modifies clause compares a string with a valid synonym, both named 'Modifies'.") {
    string STRING_SYNONYM_FOLLOWS_STRING = "stmt Modifies; Select Modifies such that Modifies(  Modifies,    \"Modifies\"   )";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(STRING_SYNONYM_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "Modifies")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "Modifies")),
                                            ClauseParam("Modifies")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Modifies clause uses synonyms with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, Modifies; while while, if, Select; Select Select such that Modifies(if, Select)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "if")),
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Modifies clause compares two valid synonyms.") {
    string TWO_SYNONYMS_MODIFIES_STRING = "stmt s, s1; Select s such that Modifies(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_MODIFIES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Modifies clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_MODIFIES_STRING = "stmt s; Select s such that Modifies(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_MODIFIES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Modifies clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_MODIFIES_STRING = "stmt s; Select s \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nModifies\n\n\t   \n( \t \n\n \r\ns\n\n\n,\r\n\n\n   _\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_MODIFIES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with Uses clauses.", "[query_parser]") {
  WHEN("Uses clause compares an int with a valid synonym.") {
    string INT_SYNONYM_USES_STRING = "stmt s; Select s such that Uses(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_USES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Uses clause compares a valid synonym with a valid string.") {
    string STRING_SYNONYM_FOLLOWS_STRING = "stmt s; Select s such that Uses(  s,    \"somestring\"   )";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(STRING_SYNONYM_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam("somestring")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Uses clause compares a string with a valid synonym, both named 'Uses'.") {
    string STRING_SYNONYM_FOLLOWS_STRING = "stmt Uses; Select Uses such that Uses(  Uses,    \"Uses\"   )";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(STRING_SYNONYM_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "Uses")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "Uses")),
                                            ClauseParam("Uses")));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Uses clause uses synonyms with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, Uses; while while, if, Select; Select Select such that Uses(if, Select)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "if")),
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Uses clause compares two valid synonyms.") {
    string TWO_SYNONYMS_USES_STRING = "stmt s, s1; Select s such that Uses(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_USES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Uses clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_USES_STRING = "stmt s; Select s such that Uses(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_USES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Uses clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_USES_STRING = "stmt s; Select s \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nUses\n\n\t   \n( \t \n\n \r\ns\n\n\n,\r\n\n\n   _\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_USES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with Calls clauses.", "[query_parser]") {
  WHEN("Calls clause compares a synonym string with a valid synonym.") {
    string INT_SYNONYM_CALLS_STRING = "procedure p; Select p such that Calls(\"p\", p)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_CALLS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS,
                                            ClauseParam("p"),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Calls clause uses synonyms and synonym strings with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "procedure procedure, Calls, Select; Select Select such that Calls(Calls, procedure)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "Calls")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "procedure"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Calls clause compares two valid synonyms.") {
    string TWO_SYNONYMS_CALLS_STRING = "procedure p, p1; Select p such that Calls(p,p1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_CALLS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Calls clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_CALLS_STRING = "procedure p; Select p such that Calls(p, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_CALLS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Calls clause compares two wildcards.") {
    string TWO_WILDCARDS_CALLS_STRING = "procedure p; Select p such that Calls(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WILDCARDS_CALLS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Calls clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_CALLS_STRING = "procedure p; Select p \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nCalls\n\n\t   \n( \t \n\n \r\n_\n\n\n,\r\n\n\n   p\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_CALLS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with CallsT clauses.", "[query_parser]") {
  WHEN("CallsT clause has excessive valid whitespace between the asterisk and parameters.") {
    string VALID_WHITESPACE_CALLST_STRING = "procedure p; Select p such that Calls*   \t\t\n\n\n\n \r\n\v\t (_, p)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHITESPACE_CALLST_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::PROCEDURE, "p")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with Next clauses.", "[query_parser]") {
  WHEN("Next clause compares an int with a valid synonym.") {
    string INT_SYNONYM_NEXT_STRING = "stmt s; Select s such that Next(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_NEXT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Next clause uses synonyms and synonym strings with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, Next; while while, if, Select; Select Select such that Next(if, Select)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "if")),
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Next clause compares two valid synonyms.") {
    string TWO_SYNONYMS_NEXT_STRING = "stmt s, s1; Select s such that Next(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_NEXT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Next clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_NEXT_STRING = "stmt s; Select s such that Next(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_NEXT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Next clause compares two wildcards.") {
    string TWO_WILDCARDS_NEXT_STRING = "stmt s; Select s such that Next(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WILDCARDS_NEXT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Next clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_NEXT_STRING = "stmt s; Select s \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nNext\n\n\t   \n( \t \n\n \r\n1\n\n\n,\r\n\n\n   2\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_NEXT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with NextT clauses.", "[query_parser]") {
  WHEN("NextT clause has excessive valid whitespace between the asterisk and parameters.") {
    string VALID_WHITESPACE_NEXTT_STRING = "stmt s; Select s such that Next*   \t\t\n\n\n\n \r\n\v\t (1, 2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHITESPACE_NEXTT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT_T,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with Affects clauses.", "[query_parser]") {
  WHEN("Affects clause compares an int with a valid synonym.") {
    string INT_SYNONYM_AFFECTS_STRING = "stmt s; Select s such that Affects(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_AFFECTS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Affects clause uses synonyms and synonym strings with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, Affects; while while, if, Select; Select Select such that Affects(if, Select)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "if")),
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Affects clause compares two valid synonyms.") {
    string TWO_SYNONYMS_AFFECTS_STRING = "stmt s, s1; Select s such that Affects(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_AFFECTS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Affects clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_AFFECTS_STRING = "stmt s; Select s such that Affects(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_AFFECTS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Affects clause compares two wildcards.") {
    string TWO_WILDCARDS_AFFECTS_STRING = "stmt s; Select s such that Affects(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WILDCARDS_AFFECTS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Affects clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_AFFECTS_STRING = "stmt s; Select s \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nAffects\n\n\t   \n( \t \n\n \r\n1\n\n\n,\r\n\n\n   2\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_AFFECTS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with AffectsT clauses.", "[query_parser]") {
  WHEN("AffectsT clause has excessive valid whitespace between the asterisk and parameters.") {
    string VALID_WHITESPACE_AFFECTST_STRING = "stmt s; Select s such that Affects*   \t\t\n\n\n\n \r\n\v\t (1, 2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHITESPACE_AFFECTST_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with NextBip clauses.", "[query_parser]") {
  utils::Extension::HasNextBip = true;
  WHEN("NextBip clause compares an int with a valid synonym.") {
    string INT_SYNONYM_NEXT_BIP_STRING = "stmt s; Select s such that NextBip(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_NEXT_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXTBIP,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("NextBip clause uses synonyms and synonym strings with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, NextBip; while while, if, Select; Select Select such that NextBip(if, Select)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXTBIP,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "if")),
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("NextBip clause compares two valid synonyms.") {
    string TWO_SYNONYMS_NEXT_BIP_STRING = "stmt s, s1; Select s such that NextBip(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_NEXT_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXTBIP,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("NextBip clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_NEXT_BIP_STRING = "stmt s; Select s such that NextBip(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_NEXT_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXTBIP,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("NextBip clause compares two wildcards.") {
    string TWO_WILDCARDS_NEXT_BIP_STRING = "stmt s; Select s such that NextBip(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WILDCARDS_NEXT_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXTBIP,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("NextBip clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_NEXT_BIP_STRING = "stmt s; Select s \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nNextBip\n\n\t   \n( \t \n\n \r\n1\n\n\n,\r\n\n\n   2\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_NEXT_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXTBIP,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with NextBipT clauses.", "[query_parser]") {
  utils::Extension::HasNextBip = true;
  WHEN("NextBipT clause has excessive valid whitespace between the asterisk and parameters.") {
    string VALID_WHITESPACE_NEXT_BIP_T_STRING = "stmt s; Select s such that NextBip*   \t\t\n\n\n\n \r\n\v\t (1, 2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHITESPACE_NEXT_BIP_T_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXTBIP_T,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with AffectsBip clauses.", "[query_parser]") {
  utils::Extension::HasAffectsBip = true;
  WHEN("AffectsBip clause compares an int with a valid synonym.") {
    string INT_SYNONYM_AFFECTS_BIP_STRING = "stmt s; Select s such that AffectsBip(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_AFFECTS_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("AffectsBip clause uses synonyms and synonym strings with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, AffectsBip; while while, if, Select; Select Select such that AffectsBip(if, Select)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "Select")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "if")),
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("AffectsBip clause compares two valid synonyms.") {
    string TWO_SYNONYMS_AFFECTS_BIP_STRING = "stmt s, s1; Select s such that AffectsBip(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_AFFECTS_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("AffectsBip clause compares valid synonym with wildcard.") {
    string SYNONYM_WILDCARD_AFFECTS_BIP_STRING = "stmt s; Select s such that AffectsBip(s, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(SYNONYM_WILDCARD_AFFECTS_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("AffectsBip clause compares two wildcards.") {
    string TWO_WILDCARDS_AFFECTS_BIP_STRING = "stmt s; Select s such that AffectsBip(_, _)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_WILDCARDS_AFFECTS_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("AffectsBip clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_AFFECTS_BIP_STRING = "stmt s; Select s \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nAffectsBip\n\n\t   \n( \t \n\n \r\n1\n\n\n,\r\n\n\n   2\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_AFFECTS_BIP_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTSBIP,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query with AffectsBipT clauses.", "[query_parser]") {
  utils::Extension::HasAffectsBip = true;
  WHEN("AffectsBipT clause has excessive valid whitespace between the asterisk and parameters.") {
    string VALID_WHITESPACE_AFFECTS_BIP_T_STRING = "stmt s; Select s such that AffectsBip*   \t\t\n\n\n\n \r\n\v\t (1, 2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(VALID_WHITESPACE_AFFECTS_BIP_T_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTSBIP_T,
                                            ClauseParam(1), ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query that selects BOOLEAN with a 'such that' clause.", "[query_parser]") {
  WHEN("Follows clause compares an int with a valid synonym.") {
    string INT_SYNONYM_FOLLOWS_STRING = "stmt s; Select BOOLEAN such that Follows(1, s)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(INT_SYNONYM_FOLLOWS_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(1), ClauseParam(DesignEntity(DesignEntityType::STMT, "s"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Parent clause compares two valid synonyms.") {
    string TWO_SYNONYMS_PARENT_STRING = "stmt s, s1; Select BOOLEAN such that Parent(s,s1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(TWO_SYNONYMS_PARENT_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Modifies clause uses synonyms with design entity type names.") {
    string CONFUSING_SYNONYM_NAMES_STRING = "stmt stmt, BOOLEAn; while while, if, Select; Select BOOLEAN such that Modifies(Select, BOOLEAn)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(CONFUSING_SYNONYM_NAMES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "Select")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "BOOLEAn"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("Uses clause contains valid excessive whitespace.") {
    string EXCESSIVE_WHITESPACE_USES_STRING = "stmt s; Select\n\n \t\tBOOLEAN \n\n\n\t\r\v \nsuch\n\n\nthat\t\t \r\n\n\nUses\n\n\t   \n( \t \n\n \r\ns\n\n\n,\r\n\n\n   _\n\n\n     )\n\n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(EXCESSIVE_WHITESPACE_USES_STRING);

    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::WILDCARD, "_"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }

  WHEN("BOOLEAN appears in the synonyms.") {
    string BOOLEAN_IN_SYNONYMS_STRING = "stmt BOOLEAN; Select BOOLEAN such that Follows(BOOLEAN, 2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(BOOLEAN_IN_SYNONYMS_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "BOOLEAN")),
                                            ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

SCENARIO("Testing GenerateQuery() given a valid query that with BOOLEAN as the synonym.", "[query_parser]") {
  WHEN("BOOLEAN appears in the synonyms.") {
    string BOOLEAN_IN_SYNONYMS_STRING = "stmt BOOLEAN, s; Select s such that Follows(BOOLEAN, 2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(BOOLEAN_IN_SYNONYMS_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "BOOLEAN")),
                                            ClauseParam(2)));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
    }
  }
}

// -----------------        GenerateQuery() multiple clauses -------------------

SCENARIO("Testing GenerateQuery() given a valid query with multiple 'such that' clauses.", "[query_parser]") {
  WHEN("The query selects a synonym with two 'such that' clauses.") {
    string MULTIPLE_SUCH_THAT_CLAUSE_STRING = "stmt s1, s2; assign a1, a2; Select s1 such that Parent(s1, a1) such that Follows(s2, a2)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_SUCH_THAT_CLAUSE_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::STMT, "s1")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a2"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query selects BOOLEAN with multiple 'such that' clauses.") {
    string MULTIPLE_SUCH_THAT_CLAUSES_BOOLEAN_STRING = "stmt s; variable v, boolean; if ifs; Select BOOLEAN such that Modifies(ifs, v) such that Uses(s, boolean)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_SUCH_THAT_CLAUSES_BOOLEAN_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(SelectedEntityType::BOOLEAN));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::IF, "ifs")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "boolean"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::BOOLEAN);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      REQUIRE(GENERATED_QUERY.GetClauseList().at(0) == EXPECTED_QUERY.GetClauseList().at(0));
      REQUIRE(GENERATED_QUERY.GetClauseList().at(1) == EXPECTED_QUERY.GetClauseList().at(1));
    }
  }

  WHEN("The query selects a synonym with multiple 'such that' clauses.") {
    string MULTIPLE_SUCH_THAT_CLAUSE_STRING = "stmt s1, s2, s3; assign a1, a2; procedure p1, p2; prog_line n1, n2; while w1; variable v1, BOOLEAN; Select w1 such that Parent(s1, a1) such that Calls*(p1, p2) such that Follows*(s2, a2) such that Modifies(p2, v1) such that Affects*(s3, s2) such that Uses(w1, BOOLEAN) such that Next(n2, n1)";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_SUCH_THAT_CLAUSE_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w1")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s3")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT,
                                            ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n2")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n1"))));

    THEN("GenerateQuery() returns the expected Query.") {
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().entity_type == SelectedEntityType::DESIGN_ENTITY);
      REQUIRE(GENERATED_QUERY.GetSelectedEntity().design_entity == EXPECTED_QUERY.GetSelectedEntity().design_entity);
      REQUIRE(GENERATED_QUERY.GetClauseList().size() == EXPECTED_QUERY.GetClauseList().size());
      for (unsigned int i = 0; i < EXPECTED_QUERY.GetClauseList().size(); i++) {
        REQUIRE(GENERATED_QUERY.GetClauseList().at(i) == EXPECTED_QUERY.GetClauseList().at(i));
      }
    }
  }

  WHEN("The query selects a synonym with multiple 'such that' clauses, with a lot of excessive whitespace.") {
    string MULTIPLE_SUCH_THAT_CLAUSE_STRING = "stmt \n\n\n s1   ,		 s2 \t\t,\r\n s3     ;		assign a1, a2; procedure p1, p2; prog_line n1, n2; while w1; variable v1, BOOLEAN; Select w1 \t\n\n		such that	  Parent(  \n\ns1 ,a1 \t\r\n\n\n) such that \n\n\r Calls*			(  p1   , p2) such that Follows*(s2, a2) such that \r\n\n\n		 Modifies(p2, v1) such that				Affects*(s3, s2)	such that Uses(w1, BOOLEAN)  such that Next(n2, n1)  \n\n";
    Query GENERATED_QUERY = QueryParser::GenerateQuery(MULTIPLE_SUCH_THAT_CLAUSE_STRING);
    Query EXPECTED_QUERY = Query(SelectedEntity(DesignEntity(DesignEntityType::WHILE, "w1")));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::PARENT,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s1")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a1"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::CALLS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p1")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::FOLLOWS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2")),
                                            ClauseParam(DesignEntity(DesignEntityType::ASSIGN, "a2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::MODIFIES,
                                            ClauseParam(DesignEntity(DesignEntityType::PROCEDURE, "p2")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "v1"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::AFFECTS_T,
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s3")),
                                            ClauseParam(DesignEntity(DesignEntityType::STMT, "s2"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::USES,
                                            ClauseParam(DesignEntity(DesignEntityType::WHILE, "w1")),
                                            ClauseParam(DesignEntity(DesignEntityType::VARIABLE, "BOOLEAN"))));
    EXPECTED_QUERY.AddClause(SuchThatClause(DesignAbstraction::NEXT,
                                            ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n2")),
                                            ClauseParam(DesignEntity(DesignEntityType::PROG_LINE, "n1"))));

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
