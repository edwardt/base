/**
 * @file epl_analyzer_driver.h
 * @brief Driver class for the EPL semantics analyzer.
 *
 * @author cjeong
 */
#ifndef MV_EPL_ANALYZER_DRIVER_H
#define MV_EPL_ANALYZER_DRIVER_H

#include <list>
#include "mv/epl.h"
#include "mv/epl/epl_module.h"
#include "mv/epl/epl_symtab.h"

namespace mv { namespace epl {

/**
 * @class AnalyzerDriver
 */
class AnalyzerDriver {
public:
  AnalyzerDriver();
  virtual ~AnalyzerDriver();
  
  int analyze(Module *module, SymbolTable *_symtab);

  void setModule(Module *module) { _module = module; }
  Module *getModule() { return _module; }

private:
  Module *_module;
  SymbolTable *_symtab;
};

}} /* mv::epl */

#endif /* MV_EPL_ANALYZER_DRIVER_H */
