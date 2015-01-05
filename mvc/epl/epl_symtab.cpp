/**
 * @file epl_symtab.cpp
 *
 * @author cjeong
 */
#include "mv/epl/epl_symtab.h"

namespace mv { namespace epl {

/*
 * SymbolTable
 */
SymbolTable *SymbolTable::_instance = NULL;
SymbolTable *SymbolTable::getInstance() {
  if (_instance == NULL) {
    _instance = new SymbolTable();
  }
  return _instance;
}

}} /* mv::epl */
