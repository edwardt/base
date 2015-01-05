/**
 * @file mvc_exp.cpp
 */
#include "mvc_exp.h"

namespace mvc {

/*
 * ExpFactory
 */
ExpFactory *ExpFactory::_instance = NULL;
ExpFactory *ExpFactory::getInstance()
{
  if (!_instance)
    _instance = new ExpFactory();

  return _instance;
}

ExpFactory::ExpFactory()
{
}

ExpFactory::~ExpFactory()
{
}

SymbolExp *ExpFactory::createSymbol(const std::string& name)
{
  SymbolExp *symbol = new SymbolExp(name);
  
  return symbol;
}

FieldrefExp *ExpFactory::createFieldref(Exp *varref, const std::string& field) 
{
  FieldrefExp *fieldref = new FieldrefExp(varref, field);
  
  return fieldref;
}

ArrayrefExp *ExpFactory::createArrayref(Exp *varref, Exp *index) 
{
  ArrayrefExp *arrayref = new ArrayrefExp(varref, index);
  
  return arrayref;
}


} /* mvc */
