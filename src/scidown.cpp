#include "scidown.h"

#include <regex>

using namespace dyaf;

std::vector<Extension*> scidown::load_extension()
{
  return {
    new scidown::ChapterExt(),
    new scidown::SubChapterExt(),
    new scidown::LineExt(),
    new scidown::BoldExt(),
    new scidown::ItalicExt(),
    new scidown::DelExt(),
    new scidown::UnderlineExt(),
    new scidown::LinkExt(),
    new scidown::ReferenceExt()
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

scidown::BasicExt::BasicExt(std::string regex, size_t rel, std::string name, int32_t priority):
  Extension(name, priority), regex(regex), r_length(rel)
{}


range scidown::BasicExt::match(std::string code)
{
  range r = {-1, -1};
  
  std::regex re(regex+"(.*?)"+regex);
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

std::string scidown::BasicExt::inner_code(std::string code)
{
  return code.substr(r_length, code.length()-2*r_length);
}

std::vector<std::string> scidown::BasicExt::arguments(std::string)
{
  return {};
}

scidown::BoldExt::BoldExt():
  scidown::BasicExt("\\*", 1, "bold", -2)
{}

scidown::ItalicExt::ItalicExt():
  scidown::BasicExt("-", 1, "italic", -2)
{}

scidown::DelExt::DelExt():
  scidown::BasicExt("~", 1, "deleted", -2)
{}

scidown::UnderlineExt::UnderlineExt():
  scidown::BasicExt("_", 1, "underline", -2)
{}


scidown::LinkExt::LinkExt() :
  Extension("link", 2)
{}

range scidown::LinkExt::match(std::string code)
{
  range r = {-1, -1};
  
  std::regex re("\\[([^\\[]+)\\]\\(([^\\)]+)");
  std::sregex_iterator next(code.begin(), code.end(), re);
  std::sregex_iterator end;
  
  if (next == end)
  {
    return r;
  }
  
  std::smatch match = *next;
  
  r.start = match.position();
  r.end = match.str().length()+1;
  return r;
}

std::string scidown::LinkExt::inner_code(std::string)
{
  return "";
}

std::vector<std::string> scidown::LinkExt::arguments(std::string code)
{
  std::vector<std::string> args;
  std::regex inner("\\[([^\\[]+)\\]");
  std::regex outter("\\(([^\\)]+)");
  std::sregex_iterator next(code.begin(), code.end(), inner);
  std::sregex_iterator end;
  
  if (next == end)
  {
    return args;
  }
  
  std::smatch match = *next;
  std::string text = match.str();
  args.push_back(text.substr(1, text.length()-2));
  next = std::sregex_iterator(code.begin(), code.end(), outter);
  if (next == end)
  {
    return args;
  }
  match = *next;
  std::string url = match.str();
  args.push_back(url.substr(1, url.length()-1));
  return args;
}

scidown::ReferenceExt::ReferenceExt():
  Extension("reference", 0)
{}

dyaf::range scidown::ReferenceExt::match(std::string code)
{
  range r = {-1, -1};

  std::regex re("@\\(([^\\)]+)");
  std::sregex_iterator next(code.begin(), code.end(), re);
  std::sregex_iterator end;
  
  if (next == end)
  {
    return r;
  }
  
  std::smatch match = *next;
  
  r.start = match.position();
  r.end = match.str().length()+1;
  return r;
}


std::string scidown::ReferenceExt::inner_code(std::string)
{
  return "";
}

std::vector<std::string> scidown::ReferenceExt::arguments(std::string code)
{
  return {code.substr(2, code.length()-3)};
}
 