#include <string>

using namespace std;

string GetTag() {
  // Toggle this boolean to determine whether time complexity tests will be run.
  //		All time complexity tests should reference this method to centralize test runs.

  bool RUN_TIME_COMPLEXITY_TESTS = false;

  if (RUN_TIME_COMPLEXITY_TESTS) {
    return "[run]";
  } else {
    return "[!hide]";
  }
}
