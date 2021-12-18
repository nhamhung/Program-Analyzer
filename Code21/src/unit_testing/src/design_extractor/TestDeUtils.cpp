#include <string>

#include "TestUtils.h"
#include "catch.hpp"
#include "design_extractor/utils/DeUtils.h"

typedef std::unordered_map<std::string, std::unordered_set<std::string>> graph;

using namespace design_extractor;
using namespace std;

SCENARIO("DeUtils SetAddAll tests") {
  GIVEN("Two non-empty sets") {
    unordered_set<string> non_empty1{"a", "b", "c"};
    unordered_set<string> non_empty2{"d", "e", "f"};

    WHEN("Adding one to the other") {
      DeUtils::SetAddAll(non_empty1, non_empty2);

      THEN("One should contain all elements of both") {
        REQUIRE(Contains(non_empty1, "a"));
        REQUIRE(Contains(non_empty1, "b"));
        REQUIRE(Contains(non_empty1, "c"));
        REQUIRE(Contains(non_empty1, "d"));
        REQUIRE(Contains(non_empty1, "e"));
        REQUIRE(Contains(non_empty1, "f"));
        REQUIRE(non_empty1.size() == 6);
      }
    }
  }

  GIVEN("One non-empty and one empty set") {
    unordered_set<string> non_empty{"a", "b", "c", "d", "e", "f"};
    unordered_set<string> empty;

    WHEN("Adding empty to non-empty set") {
      DeUtils::SetAddAll(non_empty, empty);

      THEN("One should contain all elements of both") {
        REQUIRE(Contains(non_empty, "a"));
        REQUIRE(Contains(non_empty, "b"));
        REQUIRE(Contains(non_empty, "c"));
        REQUIRE(Contains(non_empty, "d"));
        REQUIRE(Contains(non_empty, "e"));
        REQUIRE(Contains(non_empty, "f"));
        REQUIRE(non_empty.size() == 6);
      }
    }

    WHEN("Adding non-empty to empty set") {
      DeUtils::SetAddAll(empty, non_empty);

      THEN("One should contain all elements of both") {
        REQUIRE(Contains(empty, "a"));
        REQUIRE(Contains(empty, "b"));
        REQUIRE(Contains(empty, "c"));
        REQUIRE(Contains(empty, "d"));
        REQUIRE(Contains(empty, "e"));
        REQUIRE(Contains(empty, "f"));
        REQUIRE(empty.size() == 6);
      }
    }
  }
}

SCENARIO("DeUtils Toposort tests") {
  GIVEN("Graph with unique toposort order; linear call graph") {
    /*
    "->" denotes calls
    A -> B -> C -> D
    Must use e.g. with unique topo ordering
    */
    graph g;
    g["A"] = unordered_set<string>{"B"};
    g["B"] = unordered_set<string>{"C"};
    g["C"] = unordered_set<string>{"D"};
    g["D"] = unordered_set<string>{};

    WHEN("toposort is called") {
      vector<string> result_order = DeUtils::Toposort(g);

      THEN("Correct toposort order is returned") {
        vector<string> topo_order{"A", "B", "C", "D"};

        REQUIRE(result_order.size() == topo_order.size());
        for (int i = 0; i < result_order.size(); ++i) {
          REQUIRE(result_order[i].compare(topo_order[i]) == 0);
        }
      }
    }
  }

  GIVEN("Graph with a cycle") {
    /*
    "->" denotes calls
    A -> B -> C -> D -> A
    Must use e.g. with unique topo ordering
    */
    graph g;
    g["A"] = unordered_set<string>{"B"};
    g["B"] = unordered_set<string>{"C"};
    g["C"] = unordered_set<string>{"D"};
    g["D"] = unordered_set<string>{"A"};

    WHEN("toposort is called") {
      THEN("Cycle is detected; error thrown") {
        REQUIRE_THROWS(DeUtils::Toposort(g));
      }
    }
  }
}