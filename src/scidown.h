#ifndef __EXTENSIONS_H__
#define __EXTENSIONS_H__

#include "dyaf.h"

/***
 * SciDown Extensions
 ***/

namespace scidown{
  /***
   * Chapter extension:
   * A chapter start with a line of type: ``` # TITLE ``` and ends
   * with a new chapter or the end of the file.
   **/
  class ChapterExt : public dyaf::Extension {
  public:
    ChapterExt();
    
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
     
  };
  
  class LineExt : public dyaf::Extension {
  public:
    LineExt();
    
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
     
  };

  /***
   * SubChapter extension:
   * A sub chapter start with a line of type: ``` ## TITLE ``` and ends
   * with a new sub chapter or a new chapter or the end of the file.
   **/
  class SubChapterExt : public dyaf::Extension {
  public:
    SubChapterExt();
    
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
  };
  
  /***
   * Bold Ext
   * Example: ```*Bold text*````
   * */
   class BoldExt : public dyaf::Extension {
   public:
     BoldExt();
     
     virtual dyaf::range match(std::string);
     virtual std::string inner_code(std::string);
     virtual std::vector<std::string> arguments(std::string);
   };
   
   std::vector<dyaf::Extension*> load_extension();
};

#endif