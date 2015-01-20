/**
 * @file mvc_symtab.hh
 *
 * @brief Interface to symbol tables.
 */
#ifndef MVC_SYMTAB_HH
#define MVC_SYMTAB_HH

#include <string>
#include <map>
#include "mvc_exp.hh"
#include "mvc_value.hh"

namespace mvc {

/**
 * @class SymTab
 */
class SymTab {
public:
  SymTab() { }
  ~SymTab() { }

  Value *lookup(SymbolExp *symbol) {
    std::string& name = symbol->getName();
    std::map<std::string, Value *>::iterator iter = _map.find(name);
    if (iter == _map.end()) { 
      return NULL;
    }
    return iter->second;
  }
  
  void add(SymbolExp *symbol, Value *value) {
    std::string name = symbol->getName();
    _map[name] = value;
  }

private:
  std::map<std::string, Value *> _map;
};

} /* mvc */

#endif /* MVC_SYMTAB_HH */
