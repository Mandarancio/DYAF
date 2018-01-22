#include "scidown.h"

#include <iostream>
#include <regex>

using namespace dyaf;

std::vector<Extension*> scidown::load_extension()
{
  return {
    new scidown::ChapterExt(),
    new scidown::SubChapterExt(),
    new scidown::LineExt(),
    new scidown::BoldExt()
  };
}



scidown::ChapterExt::ChapterExt() :
  Extension("Chapter", 0)
{}

range scidown::ChapterExt::match(std::string code)
{
  range r;
  r.start = -1;
  r.end = -1;
  
  std::regex re("(^# |\n# )(.*)");
  std::sregex_iterator next(code.begin(), code.end(), re);
  std::sregex_iterator end;
  
  if (next == end)
  {
    return r;
  }
  std::smatch match = *next;

  r.start = match.position();
  next++;
  if (next != end)
  {
    match = *next;
    r.end = match.position()+1;
  }

  return r;
}


std::string scidown::ChapterExt::inner_code(std::string code)
{
  size_t line = code.find("\n") + 1;
  if (line < code.length())
  {
    return code.substr(line,code.length()-line);
  }
  return "";
}

std::vector<std::string> scidown::ChapterExt::arguments(std::string code)
{
  std::vector<std::string> args;
  size_t line = code.find("\n");
  if (line < code.length())
  {
    args.push_back(code.substr(2, line-2));
  }
  return args;
}


scidown::LineExt::LineExt() :
  Extension("Line", -1)
{}

range scidown::LineExt::match(std::string code)
{
  range r = {-1, -1};
  size_t l = code.find("\n");
  if (l < code.length())
  {
    r.start = 0;
    r.end = l+1;
  }
  return r;
}

std::string scidown::LineExt::inner_code(std::string code)
{
  return code.substr(0, code.length()-1);
}

std::vector<std::string> scidown::LineExt::arguments(std::string)
{
  return std::vector<std::string>();
}


scidown::SubChapterExt::SubChapterExt() :
  Extension("SubChapter", 1)
{}

range scidown::SubChapterExt::match(std::string code)
{
  range r;
  r.start = -1;
  r.end = -1;
  
  std::regex re("(^## |\n## )(.*)");
  std::sregex_iterator next(code.begin(), code.end(), re);
  std::sregex_iterator end;
  
  if (next == end)
  {
    return r;
  }
  std::smatch match = *next;

  r.start = match.position();
  
  re = std::regex("(\n## |\n# )(.*)");
  next = std::sregex_iterator(code.begin() + r.start, code.end(), re);
  
  
  if (next != end)
  {
    match = *next;
    r.end = match.position()+1;
  }

  return r;
}

std::string scidown::SubChapterExt::inner_code(std::string code)
{
  size_t line = code.find("\n") + 1;
  if (line < code.length())
  {
    return code.substr(line,code.length()-line);
  }
  return "";
}

std::vector<std::string> scidown::SubChapterExt::arguments(std::string code)
{
  std::vector<std::string> args;
  size_t line = code.find("\n");
  if (line < code.length())
  {
    args.push_back(code.substr(2, line-2));
  }
  return args;
}


scidown::BoldExt::BoldExt():
  Extension("bold", -2)
{}

range scidown::BoldExt::match(std::string code)
{
  range r = {-1, -1};
  
  std::regex re("\\*(.*?)\\*");
  std::sregex_iterator next(code.begin(), code.end(), re);
  std::sregex_iterator end;
  
  if (next == end)
  {
    return r;
  }
  
  std::smatch match = *next;
  
  r.start = match.position();
  r.end = match.str().length();
  return r;
}

std::string scidown::BoldExt::inner_code(std::string code)
{
  return code.substr(1, code.length()-2);
}

std::vector<std::string> scidown::BoldExt::arguments(std::string)
{
  return {};
}