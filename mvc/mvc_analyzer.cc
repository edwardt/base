/**
 * @file mvc_analyzer.cc
 *
 * @brief Performs semantics checking.
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

    Value *value = ValueFactory::createValue(stm);
    if (_symtab->add(value->getName(), value) == -1) {
      std::string s = Util::sformat("Name \"%s\" for is already defined.",
                                    value->getName().c_str()); 
      std::cout << s << " :" << value->getTagstr() << std::endl;
      delete value;
    }
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
