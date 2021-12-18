#pragma once

namespace utils {

class Extension {
 public:
  static bool HasNextBip;
  static bool HasAffectsBip;

  // Extracts the environment variable and caches the result in the
  // static variables. Should be called only once within spa.cpp.
  static void ExtractEnvVar();
};

}  // namespace utils