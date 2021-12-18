#include "QueryParserUtils.h"
#include "utils/Extension.h"


#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>

#include "QueryRegex.h"

namespace query_processor {


std::string& QueryParserUtils::StripWhitespace(std::string& input_string) {
  /*
		Strip all leading and trailing whitespace from an input string.
		Also remove all non-space whitespace between characters, and reduces redundant spaces to 1 space.
	*/

  // Replace all consecutive spaces with a single space
  input_string = std::regex_replace(input_string, QueryRegex::ONE_OR_MORE_WHITESPACE_REGEX, " ");

  // Strip all spaces from the front and back of the substrings
  input_string = std::regex_replace(input_string, QueryRegex::LEADING_OR_TRAILING_SPACE_REGEX, "");

  return input_string;
}

std::vector<std::string> QueryParserUtils::SplitStringAlongChar(std::string& input_string, char char_to_split) {
  std::string& stripped_substring = QueryParserUtils::StripWhitespace(input_string);

  std::vector<std::string> split_strings;
  std::stringstream string_stream(stripped_substring);

  // Split along chosen char and strip whitespace
  std::string substring;
  while (string_stream.good()) {
    getline(string_stream, substring, char_to_split);
    split_strings.push_back(substring);
  }

  return split_strings;
}

std::vector<std::string> QueryParserUtils::GetAllMatchesFromString(std::string& input_string, const std::regex& regex_to_match) {
  std::vector<std::string> all_matches = std::vector<std::string>{};

  std::smatch string_match;
  for (std::sregex_iterator iter = std::sregex_iterator(input_string.begin(), input_string.end(), regex_to_match);
       iter != std::sregex_iterator(); iter++) {
    string_match = *iter;
    all_matches.push_back(string_match.str(0));
  }

  return all_matches;
}

std::pair<std::string, std::string> QueryParserUtils::PopMatchFromFront(std::string& input_string, const std::regex& regex_to_match) {
  std::smatch string_match;

  std::regex_search(input_string, string_match, regex_to_match);
  std::string popped_match = string_match.str(0);
  input_string.erase(0, popped_match.length());

  return std::pair<std::string, std::string>(popped_match, input_string);
}

}  // namespace query_processor