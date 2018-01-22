#ifndef __DYAF_H__
#define __DYAF_H__

#include <string>
#include <vector>
#include <map>
#include <stdint.h>

namespace dyaf {
  
  struct {
    int32_t start;
    int32_t end;
  }typedef range;
  
  class Extension {
  public:
    Extension(std::string name, int32_t priority);
    std::string name();
    int32_t priority();
    
    virtual range match(std::string)=0;
    virtual std::string inner_code(std::string)=0;
    virtual std::map<std::string, std::string> arguments(std::string)=0;
    
    
  private:
    std::string name_;
    int32_t priority_;
  };
  
  class NoExtension : public Extension{
  public:
    NoExtension();
    virtual range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::map<std::string, std::string> arguments(std::string);
  };
  
  class ASTNode {
  public:
    ASTNode(std::string code, Extension * generator, ASTNode * parent);
    virtual ~ASTNode();
    
    void leafs(std::vector<ASTNode*>);
    std::vector<ASTNode*> leafs();
    bool is_leaf();
    
    std::string type();

    std::string inner_code();
    std::string code();
    std::map<std::string, std::string> arguments();
    Extension * generator();
    
    std::string to_str(std::string spacer="");
  protected:
    ASTNode * parent;
    Extension * generator_;
    std::string code_;
    std::string inner_code_;
    std::map<std::string, std::string> args_;
    std::vector<ASTNode*> leafs_;
  };
  
  class ASTRoot: public ASTNode{
  public:
    ASTRoot(std::string code);
  };
  
  
  class Parser{
  public:
    Parser(std::vector<Extension*> exts = std::vector<Extension*>());
    ASTNode * parse(std::string code);
  private:
    Extension * no_symobl_;
    std::vector<Extension*> extensions_;
    
    std::vector<ASTNode*> parse_code(std::string code, ASTNode * parent);
  };
  
  class CompilerException: public std::exception
  {
  public:
    CompilerException(std::string message);
    virtual const char* what() const throw();
  private:
    std::string msg_;
  };
  
  
};
#endif 