#ifndef __EXTENSIONS_H__
#define __EXTENSIONS_H__

#include "dyaf.h"

/***
 * SciDown Extensions
 ***/

namespace scidown {

  /***
   * Simple extension for syntax of the type:
   * ```/text/``` where / is the regex
   * */
  class BasicExt : public dyaf::Extension {
  public:
    BasicExt(std::string regx, size_t rel, std::string name, int32_t priority);
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);

  private:
    std::string regex;
    size_t r_length;
  };


  /***
   * Simple extension for line splitting
   * */
  class LineExt : public dyaf::Extension {
  public:
    LineExt();

    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
  };

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
  class BoldExt : public BasicExt {
  public:
    BoldExt();
  };

  /***
  * Italic Ext
  * Example: ```-Italic text-````
  * */
  class ItalicExt : public BasicExt {
  public:
    ItalicExt();
  };

  /***
  * Del Ext
  * Example: ```~Deleted text~````
  * */
  class DelExt : public BasicExt {
  public:
    DelExt();
  };

  /***
  * Underline Ext
  * Example: ```_Underlined text_````
  * */
  class UnderlineExt : public BasicExt {
  public:
    UnderlineExt();
  };

  /***
  * Link Ext
  * Example: ```[text](url)````
  * */
  class LinkExt : public dyaf::Extension {
  public:
    LinkExt();
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
  };

  /***
  * Reference Ext
  * Example: ```@(id)````
  * */
  class ReferenceExt : public dyaf::Extension {
  public:
    ReferenceExt();
    virtual dyaf::range match(std::string);
    virtual std::string inner_code(std::string);
    virtual std::vector<std::string> arguments(std::string);
  };

  std::vector<dyaf::Extension *> load_extension();
}; // namespace scidown

#endif