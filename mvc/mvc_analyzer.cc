/**
 * @file mvc_analyzer.cc
 */
#include "mvc_analyzer.hh" 
#include "mvc_exp.hh" 
#include "mvc_stm.hh" 
#include "mvc_util.hh"

namespace mvc {

/**
 * @class AnalyzerImpl
 */
class AnalyzerImpl
{
public:
  AnalyzerImpl(Module *mod, SymTab *symtab) : _mod(mod), _symtab(symtab) { }
  AnalyzerImpl() { }

  int run();
  
private:
  int analyzeProp(VardefStm *vardef);

private:
  Module *_mod;
  SymTab *_symtab;
};


int AnalyzerImpl::run()
{
  std::list<Stm *>& stms = _mod->getStms();
  std::list<Stm *>::iterator iter;
  for (iter = stms.begin(); iter != stms.end(); ++iter) {
    Stm *stm = *iter;
  }

  return 0;
}

int AnalyzerImpl::analyzeProp(VardefStm *vardef)
{
  SymbolExp *sym = vardef->getName();
  
  _symtab->add(sym, new Value());
}

/*
 * Analyzer
 */
Analyzer::Analyzer(Module *mod, SymTab *symtab) :
  Pass(PT_PASS, PT_ANALYZE, "semcheck")
{
  _impl = new AnalyzerImpl(mod, symtab);
}

Analyzer::~Analyzer()
{
  delete _impl;
}

int Analyzer::run()
{
  return _impl->run();
}

} /* mvc */
