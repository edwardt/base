/**
 * @file epl_analyzer_driver.cpp
 *
 * @author cjeong
 */
#include "mv/common.h"
#include "mv/epl/epl_module.h"
#include "mv/epl/epl_analyzer_driver.h"


namespace mv { namespace epl {

/*
 * AnalyzerDriver
 */
AnalyzerDriver::AnalyzerDriver() 
  : _module(NULL), _symtab(NULL)
{
}

AnalyzerDriver::~AnalyzerDriver()
{
}

int AnalyzerDriver::analyze(Module *module, SymbolTable *symtab)
{
  setModule(module);

  std::list<Stm *>& stms = module->getStms();
  std::list<Stm *>::iterator iter;
  for (iter = stms.begin(); iter != stms.end(); ++iter) {
    Stm *stm = *iter;
    
  }

  return 0;
}

}} /* mv::epl */
