/**
 * @file mvc_parser.h
 *
 * @brief Interface to MVC parser.
 */
#ifndef MVC_PARSER_H
#define MVC_PARSER_H

#include <string>
#include "mvc_base.h"
#include "mvc_module.h"

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


#endif /* MVC_PARSER_H */
