#include "spa.h"

#include <iostream>
#include <stdexcept>

#include "design_extractor/DesignExtractor.h"
#include "query_processor/QueryProcessor.h"
#include "query_processor/commons/BooleanSemanticError.h"
#include "source_processor/Parser.h"
#include "utils/Extension.h"

void SPA::ParseSourceCode(const std::string& source_code_string, PKB& pkb) {
  utils::Extension::ExtractEnvVar();
  std::cout << "Running SPA "
            << (utils::Extension::HasNextBip ? "with " : "without ")
            << "NextBip/NextBip* extension\n";
  std::cout << "Running SPA "
            << (utils::Extension::HasAffectsBip ? "with " : "without ")
            << "AffectsBip/AffectsBip* extension\n";

  const auto ast = source_processor::Parser::Parse(source_code_string);
  design_extractor::DesignExtractor::ExtractDesigns(pkb, ast);
}

void SPA::HandleQueries(const std::string& query, std::list<std::string>& results, PKB& pkb) {
  try {
    results = query_processor::QueryProcessor::ProcessQuery(query, pkb);
  }

  catch (BooleanSemanticError error) {
    std::cerr << "Boolean semantic error encountered: " << error.what() << std::endl;
    results.push_back("FALSE");
  } catch (std::runtime_error error) {
    std::cerr << "Error encountered: " << error.what() << std::endl;
  }
}
