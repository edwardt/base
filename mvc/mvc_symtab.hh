/**
 * @file mvc_symtab.hh
 *
 * @brief Interface to symbol tables.
 */
#ifndef MVC_SYMTAB_HH
#define MVC_SYMTAB_HH

#include <string>
#include <map>
#include <mv/value.h>
#include "mvc_exp.hh"

namespace mvc {

/**
 * @class SymbolTable
 *
 * @todo For now, we just regard SymbolTable as a singleton class. Later,
 *       we need to extend this to be a "distributed symbol table".
 */
class SymbolTable {
public:
  static SymbolTable *getInstance();

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
  SymbolTable() { }
  ~SymbolTable() { }

  static SymbolTable *_instance;

private:
  std::map<std::string, Value *> _map;
};

} /* mvc */

#endif /* MVC_SYMTAB_HH */
