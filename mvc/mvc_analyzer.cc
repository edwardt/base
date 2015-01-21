/**
 * @file mvc_analyzer.cc
 */
#include <cassert>
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
  int buildSymtab();

  int analyzeProp(VardefStm *vardef);

private:
  Module *_mod;
  SymTab *_symtab;
};


int AnalyzerImpl::run()
{
  buildSymtab();
  
  return 0;
}

int AnalyzerImpl::buildSymtab()
{
  std::list<Stm *>& stms = _mod->getStms();
  std::list<Stm *>::iterator iter;
  for (iter = stms.begin(); iter != stms.end(); ++iter) {
    Stm *stm = *iter;

    switch (stm->getTag()) {
    case ST_VARDEF: 
      {
        VardefStm *vardef = (VardefStm *) stm;
        
      }
      break;
    case ST_FUNDEF:
      break;
    case ST_EVENTDEF:
      break;
    case ST_PROCDEF:
      break;
    default:
      assert(0 && "Invalid statement tag.");
    }
    if (stm->getTag() == ST_VARDEF)
      analyzeProp((VardefStm *) stm);
  }
}

int AnalyzerImpl::analyzeProp(VardefStm *vardef)
{
  SymbolExp *sym = vardef->getName();

  if (_symtab->add(sym->getName(), new Prop(sym->getName())) == -1) {
    std::cout << "Prop '" << sym->getName() << "' is already defined." 
              << std::endl;
  }
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
