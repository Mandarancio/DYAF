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