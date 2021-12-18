#include "TestWrapper.h"

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "spa.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  this->pkb = PKB();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // read file to string:
  std::ifstream file(filename);
  std::stringstream sstream;
  sstream << file.rdbuf();

  try {
    SPA::ParseSourceCode(sstream.str(), pkb);
  } catch (const std::runtime_error& e) {
    std::cerr << "ERROR! SIMPLE source code is not valid\n"
              << "       " << e.what() << "\n"
              << "Exiting AutoTester without evaluating PQL queries or generating out.xml\n";
    exit(EXIT_FAILURE);
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  SPA::HandleQueries(query, results, pkb);
}
