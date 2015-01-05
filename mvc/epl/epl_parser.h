/**
 * @file epl_parser.h
 * @brief Server class for EPL parsing.
 *
 * @author cjeong
 */
#ifndef MV_EPL_PARSER_H
#define MV_EPL_PARSER_H

#include <iostream>
#include <string>
#include "mv/epl.h"
#include "mv/epl/epl_module.h"

namespace mv { namespace epl {

/**
 * @class Parser
 */
class Parser {
public:
  static Module *parse(SourceTag tag, const std::string& str);

private:
  Parser();
  ~Parser();
};

}} /* mv::epl */


#endif /* MV_EPL_PARSER_H */
