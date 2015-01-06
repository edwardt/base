/**
 * @file mvc_parser.hh
 *
 * @brief Interface to MVC parser.
 */
#ifndef MVC_PARSER_HH
#define MVC_PARSER_HH

#include <string>
#include "mvc_base.hh"
#include "mvc_module.hh"

namespace mvc {

/**
 * @class Parser
 */
class Parser {
public:
  /* Parse MVC statements. If source is SCT_FILE, str must be the name 
     of the file. If source is SCT_STR, str itself should contain an MVC
     code. If source is STDIN, str is not used. Returns 0 on success and 
     returns -1 on error. */
  static Module *parse(SourceTag tag, const std::string& str);

private:
  Parser();
  ~Parser();
};

} /* mvc */


#endif /* MVC_PARSER_HH */
