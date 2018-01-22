#ifndef __CHARTER_H__
#define __CHARTER_H__

#include "../dyaf.h"

/***
 * Extension for Charter DSL
 * */
namespace charter{
  class PlotExt: public dyaf::Extension {
  public:
    PlotExt();
    
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
  };
  
  class ScatterExt: public dyaf::Extension {
  public:
    ScatterExt();
    
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
  };
  
  class BarExt: public dyaf::Extension {
  public:
    BarExt();
    
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
  };
  
  class AxisExt: public dyaf::Extension {
  public:
    AxisExt();
    
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
  };
  
  class LabelExt: public dyaf::Extension {};
  class RangeExt: public dyaf::Extension {};
  class ModeExt: public dyaf::Extension {};
  class ColorExt: public dyaf::Extension{};
  class LineStyleExt: public dyaf::Extension{};
  class MarkerStyleExt: public dyaf::Extension{};
  class LineWidthStyleExt: public dyaf::Extension{};
  class TitleExt: public dyaf::Extension{};
  class WidthExt: public dyaf::Extension{};
  class HeightExt: public dyaf::Extension{};
  class BarWidthExt: public dyaf::Extension{};
  class BarLineColorExt: public dyaf::Extension{};
  class DataExt: public dyaf::Extension{};
  class RawDataExt: public dyaf::Extension{};
  class RangeDataExt: public dyaf::Extension{};
  class CSVDataExt: public dyaf::Extension{};
  class MathDataExt: public dyaf::Extension{};
  
  std::vector<dyaf::Extension*> load_extension();
};

#endif