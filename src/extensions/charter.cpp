#include "charter.h"

#include <regex>

#define min(a,b) a < b ? a : b

#define PLOT_TOK "(^plot:|^plot\n|\nplot\n|\nplot:)"
#define BAR_TOK "(^bar *\n|\nbar *\n|^bar:)"
#define SCATTER_TOK "(^scatter:|\nscatter:)"
#define AXIS_X_TOK "(^x-axis:|^x-axis\n|\nx-axis:|\nx-axis\n)(.*)"
#define AXIS_Y_TOK "(^y-axis:|^y-axis\n|\ny-axis:|\ny-axis\n)(.*)"


using namespace charter;

std::vector<dyaf::Extension*> charter::load_extension()
 {
   return {
     new PlotExt(),
     new ScatterExt(),
     new BarExt(),
     new AxisExt(),
     // new LabelExt(),
     // new RangeExt(),
     // new ModeExt(),
     // new ColorExt(),
     // new LineStyleExt(),
     // new MarkerStyleExt(),
     // new LineWidthStyleExt(),
     // new TitleExt(),
     // new WidthExt(),
     // new HeightExt(),
     // new BarWidthExt(),
     // new BarLineColorExt(),
     // new DataExt(),
     // new RawDataExt(),
     // new RangeDataExt(),
     // new CSVDataExt(),
     // new MathDataExt()
   };
 }

size_t find(std::string source, std::string regex)
{
  std::regex re(regex);
  std::sregex_iterator next(source.begin(), source.end(), re);
  std::sregex_iterator end;
  
  if(next == end) {
    return source.length() + 1;
  }
  
  std::smatch match = *next;
  return match.position();
}

size_t find_block(std::string rest)
{
  size_t x = find(rest, BAR_TOK);  
  size_t y = find(rest, SCATTER_TOK);  
  if(y < x)
    x = y;
  y = find(rest, AXIS_X_TOK);  
  if(y < x)
    x = y;
  y = find(rest, AXIS_Y_TOK);  
  if(y < x)
    x = y;
  return x;
}

PlotExt::PlotExt() : dyaf::Extension("plot", 0) {}

dyaf::range PlotExt::match(std::string code) {
  dyaf::range r = {-1, -1};
  size_t x = find(code, PLOT_TOK);

  if(x >= code.length()) {
    return r;
  }
 
  
  r.start = x;
  std::string rest = code.substr(x  + 1, code.length() - x + 1);
  
  r.end = find_block(rest) + x + 2;
  return r;
}

std::string PlotExt::inner_code(std::string code)
{
  size_t line = code.find("\n") + 1;
  if (line < code.length())
  {
    return code.substr(line,code.length()-line);
  }
  return "";
}

std::vector<std::string> PlotExt::arguments(std::string)
{
  return {};
}


BarExt::BarExt() : dyaf::Extension("bar", 0) {}

dyaf::range BarExt::match(std::string code) {
  dyaf::range r = {-1, -1};
  size_t x = find(code, BAR_TOK);

  if(x >= code.length()) {
    return r;
  }
 
  
  r.start = x;
  std::string rest = code.substr(x+1, code.length()-x-1);
  
  r.end = find_block(rest) + x + 2;
  return r;
}

std::string BarExt::inner_code(std::string code)
{
  size_t line = code.find("\n") + 1;
  if (line < code.length())
  {
    return code.substr(line,code.length()-line);
  }
  return "";
}

std::vector<std::string> BarExt::arguments(std::string)
{
  return {};
}

ScatterExt::ScatterExt() : dyaf::Extension("scatter", 0) {}

dyaf::range ScatterExt::match(std::string code) {
  dyaf::range r = {-1, -1};
  size_t x = find(code, SCATTER_TOK);

  if (x >= code.length()) {
    return r;
  }
  
  r.start = x;
  std::string rest = code.substr(x+1, code.length()-x-1);
  
  r.end = find_block(rest) + x + 2;
  return r;
}

std::string ScatterExt::inner_code(std::string code)
{
  size_t line = code.find("\n") + 1;
  if (line < code.length())
  {
    return code.substr(line,code.length()-line);
  }
  return "";
}

std::vector<std::string> ScatterExt::arguments(std::string)
{
  return {};
}

AxisExt::AxisExt() : dyaf::Extension("axis", 0) {}

dyaf::range AxisExt::match(std::string code) {
  dyaf::range r = {-1, -1};
  size_t x = min(find(code, AXIS_X_TOK), find(code, AXIS_Y_TOK));

  if(x >= code.length()) {
    return r;
  }
  r.start = x;
  std::string rest = code.substr(x+1, code.length()-x-1);
  r.end = find_block(rest) + x + 2;
  return r;
}

std::string AxisExt::inner_code(std::string code)
{
  size_t line = code.find("\n") + 1;
  if (line < code.length())
  {
    return code.substr(line,code.length()-line);
  }
  return "";
}

std::vector<std::string> AxisExt::arguments(std::string code)
{
  size_t x = find(code, AXIS_X_TOK);
  size_t y = find(code, AXIS_Y_TOK);
  return {
    (x < y ?
     "x" : "y")
  };
}