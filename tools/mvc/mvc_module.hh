/**
 * @file mvc_module.hh
 */
#ifndef MVC_MODULE_HH
#define MVC_MODULE_HH

#include <string>
#include <list>
#include <vector>
#include <mv/defs.h>
#include "mvc_exp.hh"
#include "mvc_stm.hh"

namespace mvc {

/**
 * @class Module
 */
class Module {
public:
  Module() : _name(NULL) { }
  ~Module() { }

  void setName(SymbolExp *name) { _name = name; }
  SymbolExp *getName() { return _name; }
  
  void addStm(Stm *stm) {
    _stms.push_front(stm);
  }
  std::list<Stm *>& getStms() { return _stms; }

private:
  SymbolExp *_name;
  std::list<Stm *> _stms;
};

/**
 * @class ModuleFactory
 */
class ModuleFactory {
public:
  static ModuleFactory *getInstance();

public:
  Module *createModule();

protected:
  ModuleFactory();
  ~ModuleFactory();
  
private:
  static ModuleFactory *_instance;
};

} /* mvc */

#endif /* MVC_MODULE_HH */
