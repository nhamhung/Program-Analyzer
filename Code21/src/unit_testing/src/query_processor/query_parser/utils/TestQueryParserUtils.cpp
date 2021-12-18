#include <string>

#include "catch.hpp"
#include "query_processor/query_parser/utils/QueryParserUtils.h"

using namespace std;
using namespace query_processor;

SCENARIO("Testing StripWhitespace() on various input strings.", "[query_parser]") {
  GIVEN("A non-text string.") {
    WHEN("Input string is empty.") {
      string EMPTY_STRING = "";

      THEN("StripWhitespace() returns an empty string.") {
        REQUIRE(QueryParserUtils::StripWhitespace(EMPTY_STRING) == EMPTY_STRING);
      }
    }

    WHEN("Input string contains only non-space whitespace.") {
      string NON_SPACE_WHITESPACE_STRING = "\t\n\r\v";
      string EMPTY_STRING = "";

      THEN("StripWhitespace() returns an empty string.") {
        REQUIRE(QueryParserUtils::StripWhitespace(NON_SPACE_WHITESPACE_STRING) == EMPTY_STRING);
      }
    }

    WHEN("Input string contains only whitespace, including a space.") {
      string WHITESPACE_STRING = "\t\n \r\v";
      string EMPTY_STRING = "";

      THEN("StripWhitespace() returns an empty string.") {
        REQUIRE(QueryParserUtils::StripWhitespace(WHITESPACE_STRING) == EMPTY_STRING);
      }
    }
  }

  GIVEN("A string with no excessive whitespace.") {
    WHEN("No whitespace is present in the string.") {
      string NO_WHITESPACE_STRING = "nowhitespace";

      THEN("StripWhitespace() returns the same string.") {
        REQUIRE(QueryParserUtils::StripWhitespace(NO_WHITESPACE_STRING) == NO_WHITESPACE_STRING);
      }
    }

    WHEN("Only single spaces are present in the string.") {
      string SINGLE_SPACE_STRING = "white space";

      THEN("StripWhitespace() returns the same string.") {
        REQUIRE(QueryParserUtils::StripWhitespace(SINGLE_SPACE_STRING) == SINGLE_SPACE_STRING);
      }
    }
  }

  GIVEN("A string with excessive whitespace.") {
    WHEN("Leading whitespace is present in the string.") {
      string LEADING_WHITESPACE_STRING = "\t \n \r \vwhitespace";
      string STRIPPED_STRING = "whitespace";

      THEN("StripWhitespace() returns the string with leading whitespace stripped.") {
        REQUIRE(QueryParserUtils::StripWhitespace(LEADING_WHITESPACE_STRING) == STRIPPED_STRING);
      }
    }

    WHEN("Trailing whitespace is present in the string.") {
      string TRAILING_WHITESPACE_STRING = "whitespace\t \n \r \v";
      string STRIPPED_STRING = "whitespace";

      THEN("StripWhitespace() returns the string with trailing whitespace stripped.") {
        REQUIRE(QueryParserUtils::StripWhitespace(TRAILING_WHITESPACE_STRING) == STRIPPED_STRING);
      }
    }

    WHEN("Excessive whitespace is present in the string.") {
      string EXCESSIVE_WHITESPACE_STRING = "w\t\n\r\vhi\t \n \r \vte sp     ace";
      string STRIPPED_STRING = "w hi te sp ace";

      THEN("StripWhitespace() returns the string by stripping all excessive whitespace to a single space.") {
        REQUIRE(QueryParserUtils::StripWhitespace(EXCESSIVE_WHITESPACE_STRING) == STRIPPED_STRING);
      }
    }
  }
}

SCENARIO("Testing SplitStringAlongChar() on various strings.", "[query_parser]") {
  //GIVEN("A string to split.") {
  //	string SAMPLE_STRING = "test";

  //	WHEN("Char to split on is a non-space whitespace.") {
  //		char INVALID_CHAR = '\n';

  //		THEN("An error is thrown.") {
  //			REQUIRE_THROWS(QueryParserUtils::SplitStringAlongChar(SAMPLE_STRING, INVALID_CHAR));
  //		}
  //	}

  //}

  GIVEN("A string without any matches with the provided char.") {
    WHEN("Input string is empty.") {
      string EMPTY_STRING = "";
      char SOME_CHAR = 'x';

      vector<string> EXPECTED_RESULT{EMPTY_STRING};

      THEN("SplitStringAlongChar() returns a vector<string> with one element.") {
        REQUIRE(QueryParserUtils::SplitStringAlongChar(EMPTY_STRING, SOME_CHAR) == EXPECTED_RESULT);
      }
    }

    WHEN("Input string is non-empty and has no matches.") {
      string NON_MATCHING_STRING = "no matches";
      char SOME_CHAR = 'x';

      vector<string> EXPECTED_RESULT{NON_MATCHING_STRING};

      THEN("SplitStringAlongChar() returns a vector<string> with one element.") {
        REQUIRE(QueryParserUtils::SplitStringAlongChar(NON_MATCHING_STRING, SOME_CHAR) == EXPECTED_RESULT);
      }
    }
  }

  GIVEN("A string with matches of the provided char.") {
    WHEN("Input string consists only of multiple copies of the char.") {
      string ONLY_CONTAINS_CHAR_STRING = "xxx";
      char SOME_CHAR = 'x';
      string EMPTY_STRING = "";

      vector<string> EXPECTED_RESULT{EMPTY_STRING, EMPTY_STRING, EMPTY_STRING, EMPTY_STRING};

      THEN("SplitStringAlongChar() returns a vector<string> with multiple empty strings.") {
        REQUIRE(QueryParserUtils::SplitStringAlongChar(ONLY_CONTAINS_CHAR_STRING, SOME_CHAR) == EXPECTED_RESULT);
      }
    }

    WHEN("Input string consists partially of multiple copies of the char.") {
      string MULTIPLE_MATCHES_STRING = "soxme mxatchesx";
      char SOME_CHAR = 'x';

      vector<string> EXPECTED_RESULT{"so", "me m", "atches", ""};

      THEN("SplitStringAlongChar() returns a vector<string> with one element.") {
        REQUIRE(QueryParserUtils::SplitStringAlongChar(MULTIPLE_MATCHES_STRING, SOME_CHAR) == EXPECTED_RESULT);
      }
    }
  }
}

SCENARIO("Testing GetAllMatchesFromString() on various strings.", "[query_parser]") {
  WHEN("String to search on is empty and regex is non-empty.") {
    string EMPTY_STRING = "";
    regex NON_EMPTY_REGEX("some match");

    vector<string> EXPECTED_VECTOR = vector<string>();

    THEN("An empty vector is returned.") {
      REQUIRE(QueryParserUtils::GetAllMatchesFromString(EMPTY_STRING, NON_EMPTY_REGEX) == EXPECTED_VECTOR);
    }
  }

  WHEN("String to search on has no matches of regex.") {
    string NON_EMPTY_STRING = "some match some match";
    regex NO_MATCH_REGEX("no match");

    vector<string> EXPECTED_VECTOR = vector<string>();

    THEN("An empty vector is returned.") {
      REQUIRE(QueryParserUtils::GetAllMatchesFromString(NON_EMPTY_STRING, NO_MATCH_REGEX) == EXPECTED_VECTOR);
    }
  }

  WHEN("String to search on has one match of regex.") {
    string NON_EMPTY_STRING = "There is one match only of the regex.";
    regex ONE_MATCH_REGEX("one\\s*\\w+\\s*only");

    vector<string> EXPECTED_VECTOR = vector<string>{"one match only"};

    THEN("A vector with the one match is returned.") {
      REQUIRE(QueryParserUtils::GetAllMatchesFromString(NON_EMPTY_STRING, ONE_MATCH_REGEX) == EXPECTED_VECTOR);
    }
  }

  WHEN("String to search on has multiple matches of regex.") {
    string NON_EMPTY_STRING = "There are multiple regexes that can suitably fit the same regex.";
    regex MULTIPLE_MATCH_REGEX("\\w{5,}");

    vector<string> EXPECTED_VECTOR = vector<string>{"There", "multiple", "regexes", "suitably", "regex"};

    THEN("A vector with multiple matches is returned.") {
      REQUIRE(QueryParserUtils::GetAllMatchesFromString(NON_EMPTY_STRING, MULTIPLE_MATCH_REGEX) == EXPECTED_VECTOR);
    }
  }
}

SCENARIO("Testing PopMatchFromFront() on various strings.", "[query_parser]") {
  WHEN("String to search on is empty and matching string is empty.") {
    string EMPTY_STRING = "";
    regex EMPTY_MATCH = regex("");

    pair<string, string> EXPECTED_RESULT = pair<string, string>("", "");

    THEN("The result is two empty strings.") {
      REQUIRE(QueryParserUtils::PopMatchFromFront(EMPTY_STRING, EMPTY_MATCH) == EXPECTED_RESULT);
    }
  }

  WHEN("String to search on is non-empty and matching string is non-empty, and the match occurs at the start.") {
    string NON_EMPTY_STRING = "not empty here and not empty here either.";
    regex START_MATCH = regex("n[a-z]+ [a-z]+");

    pair<string, string> EXPECTED_RESULT = pair<string, string>("not empty", " here and not empty here either.");

    THEN("The first match is popped, and subsequent matches are untouched.") {
      REQUIRE(QueryParserUtils::PopMatchFromFront(NON_EMPTY_STRING, START_MATCH) == EXPECTED_RESULT);
    }
  }
}
