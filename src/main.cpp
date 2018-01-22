#include <iostream>
#include <string>

#include "dyaf.h"
#include "extensions/scidown.h"
#include "extensions/math.h"
#include <regex>

int main() {
  std::cout << "Do Yourself A Favor!\n";
  
  // Example of AST generation.
  std::string code = "# Title\nAnother _line_...\n## A sub-chapter\netc *etc* etc...\n# A Second chapter\nthis is ~not~ a [link](url).\n Bla -bla- bla...\nThis @(ref1) is a reference to something not yet referenced.\nThis is an image:\n![the caption](image_url @ref size{30, 30})\n";

  auto parser = dyaf::Parser(scidown::load_extension());
  std::cout<<parser.parse(code)->to_str()<<"\n";
  
  code = "21.2*3+2";
  parser = dyaf::Parser(math::load_extension());
  dyaf::ASTNode * root = parser.parse(code);  
  std::cout<<root->to_str()<<std::endl;
  std::cout<<math::compile(root)<<std::endl;
}
