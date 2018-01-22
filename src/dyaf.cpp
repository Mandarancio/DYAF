#include "dyaf.h"
using namespace dyaf;

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

dyaf::Extension::Extension(std::string name, int32_t priority):
  name_(name), priority_(priority)
{}

std::string dyaf::Extension::name()
{
  return name_;
}

int32_t dyaf::Extension::priority()
{
  return priority_;
}

dyaf::NoExtension::NoExtension():
  Extension("NaN", -1)
{}

range dyaf::NoExtension::match(std::string code)
{
  range r;
  r.start = 0;
  r.end = code.length();
  return r;
}

std::string dyaf::NoExtension::inner_code(std::string code)
{
  return code;
}

std::vector<std::string> dyaf::NoExtension::arguments(std::string)
{
  return std::vector<std::string>();
}
  
dyaf::ASTNode::ASTNode(std::string code, Extension * generator, ASTNode * parent):
  parent(parent), generator_(generator), code_(code)
{
  inner_code_ = generator->inner_code(code);
  args_ = generator->arguments(code);
}

dyaf::ASTNode::~ASTNode()
{
  for (auto *leaf: leafs_){
    delete leaf;
  }
  leafs_.clear();
}

void dyaf::ASTNode::leafs(std::vector<ASTNode*>ls)
{
  leafs_=ls;
}

std::string dyaf::ASTNode::inner_code()
{
  return inner_code_;
}

std::string dyaf::ASTNode::code()
{
  return code_;
}

std::vector<std::string> dyaf::ASTNode::arguments()
{
  return args_;
}

Extension * dyaf::ASTNode::generator()
{
  return generator_;
}

std::string dyaf::ASTNode::to_str(std::string spacer)
{
  
  std::string res = spacer;
  if (res.size())
  {
    if (ends_with(res, "│"))
    {
      res = res.substr(0, res.length()-3);
      res += "├";
    } else{
      res = res.substr(0, res.length()-1);
      res += "└";
    }
  }

  if (leafs_.size())
  {
    res += "┬─ ";
  } else {
    res += "── ";
  }
  res += generator_->name() + "(";
  for (auto arg : args_)
  {
    if (arg == args_[args_.size()-1])
      res += arg;
    else
      res += arg+", ";
  }
  res += ")";
  
  if (leafs_.size() > 0)
  {
    res += "\n";
    for (auto *leaf : leafs_)
    {
      if (leaf == leafs_[leafs_.size()-1]){
        res += leaf->to_str(spacer+" ");
      }
      else{
        res += leaf->to_str(spacer+"│");
      }
    }
  } else
  {
    res += ": [" + inner_code_ + "]\n";
  }
  
  return res;
}

dyaf::ASTRoot::ASTRoot(std::string code):
  ASTNode(code, new NoExtension(), nullptr)
{}
  
dyaf::Parser::Parser(std::vector<Extension*> exts):
  extensions_(exts)
{
  no_symobl_ = new NoExtension();
}

ASTNode* dyaf::Parser::parse(std::string code)
{
  ASTNode * root = new ASTRoot(code);
  root->leafs(this->parse_code(code, root));
  return root;
}


std::vector<ASTNode*> dyaf::Parser::parse_code(std::string code, ASTNode * parent)
{
  if (!code.length())
    return {};
  
  std::vector<ASTNode*> leafs;
  Extension * current_symbol = nullptr;
  range current_range;
  current_range.start = code.length()+1;
  current_range.end = -1;
  for (auto *ext : this->extensions_)
  {
    range r = ext->match(code);
    if (r.start >= 0 &&
        (r.start < current_range.start ||
         (r.start == current_range.start &&
          ext->priority() > current_symbol->priority())))
    {
      current_range = r;
      current_symbol = ext;
    }
  }
  
  if (current_symbol)
  {
    if (current_range.start > 0)
    {
      leafs.push_back(new ASTNode(code.substr(0, current_range.start), no_symobl_, parent));
    }
    
    ASTNode * node = new ASTNode(code.substr(current_range.start, current_range.end), current_symbol, parent);
    node->leafs(parse_code(node->inner_code(), node));
    leafs.push_back(node);
    
    if ((unsigned) current_range.end + current_range.start < code.length())
    {
      size_t start = (unsigned)current_range.end + current_range.start;
      std::vector<ASTNode*> rest = parse_code(code.substr(start, code.length()-start), parent);
      leafs.reserve(leafs.size() + distance(rest.begin(),rest.end()));
      leafs.insert(leafs.end(), rest.begin(), rest.end());
    }
  } else
  {
    leafs.push_back(new ASTNode(code, no_symobl_, parent));
  }
  return leafs;
}