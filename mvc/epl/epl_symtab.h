/**
 * @file epl_symtab.h 
 * @brief Interface to symbol tables.
 *
 * @author cjeong
 */
#ifndef MV_EPL_SYMTAB_H
#define MV_EPL_SYMTAB_H

#include <string>
#include <map>
#include "mv/common.h"
#include "mv/value.h"
#include "mv/epl/epl_exp.h"

namespace mv { namespace epl {

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

}} /* mv::epl */

#endif /* MV_EPL_SYMTAB_H */
