#include "Extension.h"

#include <cstdlib>
#include <string>

namespace utils {

// Defaults to false
bool Extension::HasNextBip = false;
bool Extension::HasAffectsBip = false;

void Extension::ExtractEnvVar() {
  const char* env_char = std::getenv("EXTENSION");
  if (env_char == NULL) {
    return;
  }

  std::string env_str = std::string(env_char);
  if (env_str.find("NB") != std::string::npos) {
    HasNextBip = true;
  }
  if (env_str.find("AB") != std::string::npos) {
    HasAffectsBip = true;
  }
}

}  // namespace utils