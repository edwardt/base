/**
 * @file epl_parser.h
 * @brief Server class for EPL semantics analysis.
 *
 * @author cjeong
 */
#ifndef MV_EPL_ANALYZER_H
#define MV_EPL_ANALYZER_H

#include <iostream>
#include <string>
#include "mv/epl.h"
#include "mv/epl/epl_module.h"
#include "mv/epl/epl_symtab.h"

namespace mv { namespace epl {

/**
 * @class Analyzer
 */
class Analyzer {
public:
  static int analyze(Module *module, SymbolTable *symtab);

private:
  Analyzer();
  ~Analyzer();
};

}} /* mv::epl */


#endif /* MV_EPL_ANALYZER_H */
