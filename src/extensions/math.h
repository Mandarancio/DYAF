#ifndef __MATH_H__
#define __MATH_H__

#include "../dyaf.h"

namespace math {
  class NumberExt : public dyaf::Extension
  {
  public:
    NumberExt();
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::map<std::string, std::string> arguments(std::string);
  };
  
  class OperatorExt : public dyaf::Extension
  {
  public:
    OperatorExt(std::string name, std::string symb, int32_t priority);
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::map<std::string, std::string> arguments(std::string);
  private:
    std::string symb;
  };
  
  std::vector<dyaf::Extension*> load_extension();
};

#endif