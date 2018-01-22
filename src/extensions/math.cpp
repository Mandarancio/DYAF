#include "math.h"

#include <regex>
#include <iostream>

using namespace math;

std::vector<dyaf::Extension*> math::load_extension()
{
  return {
    new NumberExt(),
    new OperatorExt("sum", "+", 3),
    new OperatorExt("sub", "-", 3),
    new OperatorExt("mul", "*", 2),
    new OperatorExt("div", "/", 2),
  };
}

math::NumberExt::NumberExt() :
  dyaf::Extension("number", 0)
{}

dyaf::range math::NumberExt::match(std::string code)
{
  dyaf::range r = {-1, -1};
  std::regex rx("[-+]?[0-9]*\\.?[0-9]+");
  std::smatch m;
  std::string str = code;
  std::regex_search(str, m, rx);
  r.start = m.position(0);
  r.end = r.start + m.str().length() ;
  return r;
}

std::string math::NumberExt::inner_code(std::string)
{
  return "";
}

std::map<std::string, std::string> math::NumberExt::arguments(std::string code)
{
  return {{"number", code}};
}

math::OperatorExt::OperatorExt(std::string name, std::string sym, int32_t priority) :
  dyaf::Extension(name, priority), symb(sym)
{}

dyaf::range math::OperatorExt::match(std::string code)
{
  dyaf::range r = {-1, -1};
  std::regex rx("["+symb+"][-+]?[0-9]*\\.?[0-9]+");
  std::smatch m;
  std::regex_search(code, m, rx);
  size_t pos = m.position(0);
  r.end = m.str().length()+pos;
  
  if (pos < code.length())
  {
    std::regex exp("[-+]?[0-9]*\\.?[0-9]+(\\+|-|/|\\*|\\(|\\)|/s+)");
    std::smatch m;
    std::regex_search(code, m, exp);
    if ((unsigned)m.position(0) < pos)
    {
      r.start = m.position(0);
    }
    else
    {
      r.start = 0;
    }

  }
  
  return r;
}

std::string math::OperatorExt::inner_code(std::string code)
{
  size_t pos = code.find(symb);
  code[pos] = ' ';
  return code;
}

std::map<std::string, std::string> math::OperatorExt::arguments(std::string)
{
  return {};
}

std::vector<dyaf::ASTNode*> clean_tree(std::vector<dyaf::ASTNode*> leafs)
{
  std::vector<dyaf::ASTNode*> nodes;
  for (auto node : leafs)
  {
    if (node->type() == "number" ||
        node->type() == "sum"    ||
        node->type() == "sub"    ||
        node->type() == "mul"    ||
        node->type() == "div")
    {
      nodes.push_back(node);
    } else if (node->type() != "NaN")
    {
      throw dyaf::CompilerException("Unknown type \""+node->type()+"\" found at: "+node->code());
    }
  }
  
  return nodes;
}

double math::compile(dyaf::ASTNode * root){
  if (root->type() == "NaN")
  {
    auto nodes = clean_tree(root->leafs());
    if (nodes.size() != 1)
    {
      throw dyaf::CompilerException("Malformed expression:\n"+root->to_str("\t"));
    }
    return compile(nodes[0]);
  }
  if (root->type() == "number")
  {
    return std::stod(root->arguments()["number"]);
  }
  auto nodes = clean_tree(root->leafs());
  if (nodes.size() !=2)
  {
    throw dyaf::CompilerException("Malformed expression:\n"+root->to_str("\t"));
  }
  double a = compile(nodes[0]);
  double b = compile(nodes[1]);
  if (root->type() == "sum")
    return a + b;
  if (root->type() == "sub")
    return a - b;
  if (root->type() == "div")
    return a / b;
  if (root->type() == "mul")
    return a * b;
  throw dyaf::CompilerException("Unknown operation \""+root->type()+"\" at:\n"+root->to_str("\t"));
}