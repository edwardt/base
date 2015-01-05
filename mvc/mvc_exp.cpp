/**
 * @file mvc_exp.cpp
 */
#include "mvc_exp.h"

namespace mvc {

/**
 * @class ExpFactoryImpl
 */
class ExpFactoryImpl : public ExpFactory {
public:
  ExpFactoryImpl();
  ~ExpFactoryImpl();

public:
  SymbolExp *createSymbol(const std::string& name);
  FieldrefExp *createFieldref(Exp *varref, const std::string& field);
  ArrayrefExp *createArrayref(Exp *varref, Exp *index);

  IntegerExp *createInteger(int v);
  FloatExp *createFloat(float v);
  StringExp *createString(const std::string& v);
  UnaryExp *createUnary(UnaryTag utag);
  BinaryExp *createBinary(BinaryTag btag, Exp *lexp, Exp *rexp);
  
  FuncallExp *createFuncall(SymbolExp *name, std::list<Exp *> *args);
  TimeExp *createTime(size_t d, size_t h, size_t m, size_t s, size_t ms);
};

ExpFactoryImpl::ExpFactoryImpl()
{
}

ExpFactoryImpl::~ExpFactoryImpl()
{
}

SymbolExp *ExpFactoryImpl::createSymbol(const std::string& name)
{
  SymbolExp *symbol = new SymbolExp(name);
  
  return symbol;
}

FieldrefExp *ExpFactoryImpl::createFieldref(Exp *varref, const std::string& field) 
{
  FieldrefExp *fieldref = new FieldrefExp(varref, field);
  
  return fieldref;
}

ArrayrefExp *ExpFactory::createArrayref(Exp *varref, Exp *index) 
{
  ArrayrefExp *arrayref = new ArrayrefExp(varref, index);
  
  return arrayref;
}


/*
 * ExpFactory
 */
ExpFactory *ExpFactory::_instance = NULL;
ExpFactory *ExpFactory::getInstance()
{
  if (!_instance)
    _instance = new ExpFactoryImpl();

  return _instance;
}

ExpFactory::ExpFactory()
{
}

ExpFactory::~ExpFactory()
{
}


} /* mvc */
