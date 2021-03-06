#include "scidown.h"

#include <regex>
#include <iostream>

using namespace dyaf;

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline bool starts_with(std::string const & value, std::string const & starting)
{
    if (starting.size() > value.size()) return false;
    return std::equal(starting.begin(), starting.end(), starting.begin());
}


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
    new scidown::ReferenceExt(),
    new scidown::ImageExt()
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

std::map<std::string, std::string> scidown::ChapterExt::arguments(std::string code)
{
  std::map<std::string, std::string> args;
  size_t line = code.find("\n");
  if (line < code.length())
  {
    args["title"] = code.substr(2, line-2);
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

std::map<std::string, std::string> scidown::LineExt::arguments(std::string)
{
  return std::map<std::string, std::string>();
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

std::map<std::string, std::string> scidown::SubChapterExt::arguments(std::string code)
{
  std::map<std::string, std::string> args;
  size_t line = code.find("\n");
  if (line < code.length())
  {
    args["title"] = code.substr(3, line-3);
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

std::map<std::string, std::string> scidown::BasicExt::arguments(std::string)
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
  Extension("link", -2)
{}

range scidown::LinkExt::match(std::string code)
{
  range r = {-1, -1};
  
  std::regex re("\\[([^\\[]*)\\]\\(([^\\)]*)");
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

std::map<std::string, std::string> scidown::LinkExt::arguments(std::string code)
{
  std::map<std::string, std::string> args;
  std::regex inner("\\[([^\\[]*)\\]");
  std::regex outter("\\(([^\\)]*)");
  std::sregex_iterator next(code.begin(), code.end(), inner);
  std::sregex_iterator end;
  
  if (next == end)
  {
    return args;
  }
  
  std::smatch match = *next;
  std::string text = match.str();
  args["text"] = text.substr(1, text.length()-2);
  args["url"] = "";
  next = std::sregex_iterator(code.begin(), code.end(), outter);
  if (next == end)
  {
    return args;
  }
  match = *next;
  std::string url = match.str();
  args["url"] = (url.substr(1, url.length()-1));
  return args;
}

scidown::ReferenceExt::ReferenceExt():
  Extension("reference", -2)
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

std::map<std::string, std::string> scidown::ReferenceExt::arguments(std::string code)
{
  return {{"id",code.substr(2, code.length()-3)}};
}

scidown::ImageExt::ImageExt() :
  Extension("image", -2)
{}

dyaf::range scidown::ImageExt::match(std::string code)
{
  range r = {-1, -1};
  std::regex re("!\\[([^\\[]*)\\]\\(([^\\)]*)");
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

std::string scidown::ImageExt::inner_code(std::string)
{
  return "";
}

std::map<std::string, std::string> scidown::ImageExt::arguments(std::string code)
{
  std::map<std::string, std::string> args;
   std::regex inner("\\[([^\\[]*)\\]");
   std::regex outter("\\(([^\\)]*)");
   std::sregex_iterator next(code.begin(), code.end(), inner);
   std::sregex_iterator end;
   
   if (next == end)
   {
     return args;
   }
   
   std::smatch match = *next;
   std::string text = match.str();
   args["caption"] = (text.substr(1, text.length()-2));
   next = std::sregex_iterator(code.begin(), code.end(), outter);
   if (next == end)
   {
     return args;
   }
   match = *next;
   
   std::string image = match.str();
   image = image.substr(1, image.length()-1);
   
   size_t pos = image.find(" ");
   args["image"] = image.substr(0, pos);
   std::regex split("\\s+");
   next = std::sregex_iterator(image.begin()+pos+1, image.end(), split);
   for ( ; next != end; ++next)

   {
     match = *next;
     std::string arg = image.substr(pos+1, match.position());
     std::string key = this->get_key(arg);
     std::string value = this->get_value(arg, key);
     args[key] = value;
     pos += 1+match.position();
   }
   return args;
}

std::string scidown::ImageExt::get_key(std::string arg)
{
  if (arg[0] == '@')
  {
    return "reference";
  }
  if (starts_with(arg, "size{") && ends_with(arg, "}"))
  {
    return "size";
  }
  return "";
}

std::string scidown::ImageExt::get_value(std::string arg, std::string key)
{
  if (key == "reference")
  {
    return arg.substr(1, arg.length()-1);
  }
  if (key == "size")
  {
    arg = arg.substr(5, arg.length()-6);
    return arg;
  }
  return arg;
}
 