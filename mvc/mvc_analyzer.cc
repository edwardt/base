/**
 * @file mvc_analyzer.cc
 *
 * @brief Performs semantics checking.
 */
#include <cassert>
#include "mvc_analyzer.hh" 
#include "mvc_exp.hh" 
#include "mvc_stm.hh" 
#include "mvc_iterator.hh" 
#include "mvc_util.hh"

namespace mvc {

/**
 * @class AnalyzerImpl
 */
class AnalyzerImpl
{
public:
  AnalyzerImpl(Module *mod, SymTab& symtab) : _mod(mod), _symtab(symtab) { }
  ~AnalyzerImpl() { }

  int run();
  
private:
  int buildSymtab();

  int analyze(Stm *stm);
  int analyzeVardef(VardefStm *vardef);
  int analyzeFundef(FundefStm *fundef);
  int analyzeProcdef(ProcdefStm *procdef);
  int analyzeEventdef(EventdefStm *eventdef);
  int analyzeStm(Stm *stm);
  int analyzeExp(Exp *exp);

private:
  Module *_mod;
  SymTab& _symtab;
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
    
    Util::print(std::cout, stm);
    if (analyze(stm) == -1)
      continue;
  }
}


int AnalyzerImpl::analyze(Stm *stm)
{
  switch (stm->getTag()) {
  case ST_VARDEF: 
    return analyzeVardef(static_cast<VardefStm *>(stm));
  case ST_EVENTDEF:
    return analyzeEventdef(static_cast<EventdefStm *>(stm));
  case ST_PROCDEF:
    return analyzeProcdef(static_cast<ProcdefStm *>(stm));
  case ST_FUNDEF:
    return analyzeFundef(static_cast<FundefStm *>(stm));
  default:
    assert(0 && "analyze: Invalid statement tag");
    return -1;
  }

  return 0;
}

int AnalyzerImpl::analyzeVardef(VardefStm *vardef)
{
  const std::string& name = vardef->getSym()->getName();
  if (_symtab.lookup(name) != NULL) {
    std::string s = Util::sformat("Property \"%s\" already defined.",
                                  name.c_str()); 
    std::cout << s << std::endl;
    return -1;
  }

  Prop *prop = ValueFactory::createProp(name);
  if (_symtab.add(name, prop) == -1) {
    std::string s = Util::sformat("Failed to add: \"%s\".", name.c_str());
    std::cout << s << std::endl;
    return -1;
  }
  
  return 0;
}

int AnalyzerImpl::analyzeEventdef(EventdefStm *eventdef)
{
  const std::string& name = eventdef->getSym()->getName();
  if (_symtab.lookup(name) != NULL) {
    std::string s = Util::sformat("Event \"%s\" already defined.",
                                  name.c_str()); 
    std::cout << s << std::endl;
    return -1;
  }

  Event *event = ValueFactory::createEvent(name);
  if (_symtab.add(name, event) == -1) {
    std::string s = Util::sformat("Failed to add: \"%s\".", name.c_str());
    std::cout << s << std::endl;
    return -1;
  }
  
  return 0;
}

int AnalyzerImpl::analyzeProcdef(ProcdefStm *procdef)
{
  const std::string& name = procdef->getSym()->getName();
  if (_symtab.lookup(name) != NULL) {
    std::string s = Util::sformat("Reactor \"%s\" already defined.",
                                  name.c_str()); 
    std::cout << s << std::endl;
    return -1;
  }

  Reactor *reactor = ValueFactory::createReactor(name);
  if (_symtab.add(name, reactor) == -1) {
    std::string s = Util::sformat("Failed to add: \"%s\".", name.c_str());
    std::cout << s << std::endl;
    return -1;
  }
  
  Util::print(std::cout, procdef);
  analyzeStm(procdef->getBody());
  return 0;
}

int AnalyzerImpl::analyzeFundef(FundefStm *fundef)
{
  const std::string& name = fundef->getSym()->getName();
  if (_symtab.lookup(name) != NULL) {
    std::string s = Util::sformat("Function \"%s\" already defined.",
                                  name.c_str()); 
    std::cout << s << std::endl;
    return -1;
  }

  Function *fun = ValueFactory::createFunction(name);
  if (_symtab.add(name, fun) == -1) {
    std::string s = Util::sformat("Failed to add: \"%s\".", name.c_str());
    std::cout << s << std::endl;
    return -1;
  }
  
  Util::print(std::cout, fundef);
  analyzeStm(fundef->getBody());
  return 0;
}


int AnalyzerImpl::analyzeStm(Stm *stm)
{
  std::cout << "ANALYZE_STM: ";
  Util::print(std::cout, stm);

  ExpIterator eiter(stm);
  while (eiter.hasNext()) {
    Exp *exp = eiter.getNext();
    if (analyzeExp(exp) == -1)
      return -1;
    eiter.next();
  }
  
  StmIterator siter(stm);
  while (siter.hasNext()) {
    Stm *stm = siter.getNext();
    analyzeStm(stm);
    siter.next();
  }
}

int AnalyzerImpl::analyzeExp(Exp *exp)
{
  std::cout << "ANALYZE_EXP: ";
  Util::print(std::cout, exp);
  std::cout << std::endl;

  switch (exp->getTag()) {
  case ET_SYMBOL: {
    SymbolExp *sym = static_cast<SymbolExp *>(exp);
    const std::string& name = sym->getName();
    if (_symtab.lookup(name) == NULL) {
      std::string s = Util::sformat("No symbol found: \"%s\".", name.c_str());
      std::cout << s << std::endl;
      return -1;
    }
    break;
  }
  default:
    //assert(0 && "analyzeExp: Invalid expression tag");
    break;
  }

  ExpIterator eiter(exp);
  while (eiter.hasNext()) {
    Exp *subexp = eiter.getNext();
    if (analyzeExp(subexp) == -1)
      return -1;
    eiter.next();
  }

  return 0;
}


/*
 * Analyzer
 */
Analyzer::Analyzer(Module *mod, SymTab& symtab) :
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
