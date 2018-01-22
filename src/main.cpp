#include <iostream>
#include <string>

#include "dyaf.h"
#include "scidown.h"
#include <regex>

int main() {
  std::cout << "Do Yourself A Favor!\n";
  
  // Example of AST generation.
  std::string code = "# Title\nAnother line...\n## A sub-chapter\netc *etc* etc...\n# A second chapter\nBla bla bla...\n";

  auto parser = dyaf::Parser(scidown::load_extension());
  
  std::cout<<parser.parse(code)->to_str()<<"\n";

}
