#include <string>

#include "TestUtils.h"
#include "catch.hpp"
#include "source_processor/token/Token.h"
#include "source_processor/token/TokenList.h"
#include "source_processor/token/TokenType.h"

using namespace source_processor;

SCENARIO("Overloaded operators work as expected") {
  GIVEN("Two empty lists") {
    TokenList xs;
    TokenList ys;

    REQUIRE(xs == ys);
    REQUIRE((xs != ys) == false);
  }

  GIVEN("Two similar lists") {
    TokenList xs;
    TokenList ys;
    Token t1(std::string("test"), TokenType::VariableName);
    Token t2(std::string("proc"), TokenType::ProcedureName);
    xs.Push(t1).Push(t2);
    ys.Push(t1).Push(t2);

    REQUIRE(xs == ys);
    REQUIRE((xs != ys) == false);
  }

  GIVEN("Two lists of different lengths") {
    TokenList xs;
    TokenList ys;
    Token t1(std::string("test"), TokenType::VariableName);
    Token t2(std::string("proc"), TokenType::ProcedureName);
    xs.Push(t1).Push(t2);

    REQUIRE((xs == ys) == false);
    REQUIRE(xs != ys);
  }

  GIVEN("Two lists of same lengths but different tokens") {
    TokenList xs;
    TokenList ys;
    Token t1(std::string("test"), TokenType::VariableName);
    Token t2(std::string("proc"), TokenType::ProcedureName);
    Token t3(std::string("read"), TokenType::Read);
    xs.Push(t1).Push(t2);
    ys.Push(t1).Push(t3);

    REQUIRE((xs == ys) == false);
    REQUIRE(xs != ys);
  }
}

SCENARIO("Sublists can be correctly determined") {
  TokenList main_list;
  main_list
      .Push(Token("v1", TokenType::VariableName))
      .Push(Token("v2", TokenType::VariableName))
      .Push(Token("v3", TokenType::VariableName))
      .Push(Token("v4", TokenType::VariableName))
      .Push(Token("v5", TokenType::VariableName))
      .Push(Token("v6", TokenType::VariableName));

  GIVEN("A valid sublist without any tokens") {
    TokenList sub_list;
    REQUIRE(main_list.HasSublist(sub_list));
  }

  GIVEN("A valid sublist with one token") {
    TokenList sub_list;
    sub_list.Push(Token("v3", TokenType::VariableName));
    REQUIRE(main_list.HasSublist(sub_list));
  }

  GIVEN("A valid sublist with multiple tokens") {
    TokenList sub_list;
    sub_list
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v4", TokenType::VariableName));
    REQUIRE(main_list.HasSublist(sub_list));
  }

  GIVEN("A valid sublist with the exact same tokens") {
    TokenList sub_list;
    sub_list
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v4", TokenType::VariableName))
        .Push(Token("v5", TokenType::VariableName))
        .Push(Token("v6", TokenType::VariableName));
    REQUIRE(main_list == sub_list);
    REQUIRE(main_list.HasSublist(sub_list));
  }

  GIVEN("A valid sublist after initial non-matching pattern") {
    TokenList main_list;
    main_list
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v4", TokenType::VariableName))
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v4", TokenType::VariableName))
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v5", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName));

    TokenList sub_list;
    sub_list
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v4", TokenType::VariableName))
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v5", TokenType::VariableName));
    REQUIRE(main_list.HasSublist(sub_list));

    sub_list.Clear();
    sub_list
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName));
    REQUIRE(main_list.HasSublist(sub_list));
  }

  GIVEN("An invalid sublist with more tokens than main list") {
    TokenList sub_list;
    sub_list
        .Push(Token("v1", TokenType::VariableName))
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v4", TokenType::VariableName))
        .Push(Token("v5", TokenType::VariableName))
        .Push(Token("v6", TokenType::VariableName))
        .Push(Token("v7", TokenType::VariableName));
    REQUIRE_FALSE(main_list.HasSublist(sub_list));
  }

  GIVEN("An invalid sublist with missing consecutive tokens") {
    TokenList sub_list;
    sub_list
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v4", TokenType::VariableName));
    REQUIRE_FALSE(main_list.HasSublist(sub_list));

    sub_list.Clear();
    sub_list
        .Push(Token("v2", TokenType::VariableName))
        .Push(Token("v3", TokenType::VariableName))
        .Push(Token("v6", TokenType::VariableName));
    REQUIRE_FALSE(main_list.HasSublist(sub_list));
  }
}

SCENARIO("TokenList public methods work as expected") {
  GIVEN("A new list without any tokens") {
    TokenList tokens;
    REQUIRE(tokens.IsEmpty());

    WHEN("Peek and Pop are called") {
      THEN("They should throw") {
        REQUIRE_THROWS(tokens.Peek());
        REQUIRE_THROWS(tokens.Pop());
      }
    }

    WHEN("A single token is added to the list") {
      Token token_to_push(std::string("test"), TokenType::VariableName);
      tokens.Push(token_to_push);

      THEN("Peek should return that token without removing it from the list") {
        Token peeked_token = tokens.Peek();
        REQUIRE(IsSimilarToken(token_to_push, peeked_token));
        REQUIRE(tokens.GetSize() == 1);
      }

      THEN("Pop should return and remove that token from the list") {
        Token peeked_token = tokens.Pop();
        REQUIRE(IsSimilarToken(token_to_push, peeked_token));
        REQUIRE(tokens.IsEmpty());
      }
    }
  }

  GIVEN("A list with 3 added tokens t1, t2, t3") {
    TokenList tokens;
    Token t1(std::string("t1"), TokenType::VariableName);
    Token t2(std::string("t2"), TokenType::VariableName);
    Token t3(std::string("t3"), TokenType::VariableName);
    tokens.Push(t1).Push(t2).Push(t3);
    REQUIRE(tokens.IsEmpty() == false);
    REQUIRE(tokens.GetSize() == 3);

    WHEN("A single token is added to the list") {
      Token token_to_push(std::string("t4"), TokenType::VariableName);
      tokens.Push(token_to_push);
      REQUIRE(tokens.GetSize() == 4);

      THEN("Peek should return token t1 without removing it from the list") {
        Token peeked_token = tokens.Peek();
        REQUIRE(IsSimilarToken(t1, peeked_token));
        REQUIRE(tokens.GetSize() == 4);
      }

      THEN("Pop should return and remove token t1 from the list") {
        Token popped_token = tokens.Pop();
        REQUIRE(IsSimilarToken(t1, popped_token));
        REQUIRE(tokens.GetSize() == 3);
      }
    }
  }

  GIVEN("A list with 1 added token") {
    TokenList tokens;
    Token t1(std::string("t1"), TokenType::VariableName);
    tokens.Push(t1);
    REQUIRE(tokens.GetSize() == 1);

    WHEN("PeekMatch is called") {
      THEN("It should not remove any tokens from the list") {
        tokens.PeekMatch(TokenType::VariableName);
        REQUIRE(tokens.GetSize() == 1);
      }

      THEN("It should return true if the TokenType is the same") {
        REQUIRE(tokens.PeekMatch(TokenType::VariableName) == true);
      }

      THEN("It should return false if the TokenType is not the same") {
        REQUIRE(tokens.PeekMatch(TokenType::Semicolon) == false);
      }
    }

    WHEN("PopExpect is called") {
      THEN("It should throw if the TokenType is not the same") {
        REQUIRE_THROWS(tokens.PopExpect(TokenType::Semicolon));
      }

      THEN("It should return the token and remove it from the list if the TokenType is the same") {
        Token popped_token = tokens.PopExpect(TokenType::VariableName);
        REQUIRE(IsSimilarToken(t1, popped_token));
        REQUIRE(tokens.IsEmpty());
      }
    }
  }
}