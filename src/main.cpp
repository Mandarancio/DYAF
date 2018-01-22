#include <iostream>
#include <string>

#include "dyaf.h"
#include "scidown.h"
#include <regex>

int main() {
  std::cout << "Do Yourself A Favor!\n";
  
  // Example of AST generation.
  std::string code = "# Title\nAnother _line_...\n## A sub-chapter\netc *etc* etc...\n# A Second chapter\nthis is a [link](url).\n Bla -bla- bla...\n";

  auto parser = dyaf::Parser(scidown::load_extension());
  
  std::cout<<parser.parse(code)->to_str()<<"\n";

}
