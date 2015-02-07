/**
 * @file mvc_exp.cc
 */
#include <iostream>
#include "mvc_exp.hh"

namespace mvc {


SymbolExp::SymbolExp(const std::string& sym) : Exp(ET_SYMBOL)
{
  std::size_t pos = sym.find(":");
  if (pos != std::string::npos) {
    _dev = sym.substr(0, pos);
    _name = sym.substr(pos + 1);
  }
  else 
    _name = sym;
}


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

IntegerExp *ExpFactory::createInteger(int v)
{
  IntegerExp *intexp = new IntegerExp(v);
  
  return intexp;
}

FloatExp *ExpFactory::createFloat(float v)
{
  FloatExp *floatexp = new FloatExp(v);
  
  return floatexp;
}

StringExp *ExpFactory::createString(const std::string& v)
{
  StringExp *strexp = new StringExp(v);
  
  return strexp;
}

UnaryExp *ExpFactory::createUnary(UnaryTag utag, Exp *exp)
{
  UnaryExp *unary = new UnaryExp(utag, exp);
  
  return unary;
}

BinaryExp *ExpFactory::createBinary(BinaryTag btag, Exp *lexp, Exp *rexp)
{
  BinaryExp *binary = new BinaryExp(btag, lexp, rexp);
  
  return binary;
}

FuncallExp *ExpFactory::createFuncall(SymbolExp *name, std::list<Exp *> *args)
{
  FuncallExp *call = new FuncallExp(name, args);

  return call;
}

TimeExp *ExpFactory::createTime(size_t d, size_t h, size_t m, size_t s, 
                                size_t ms)
{
  TimeExp *timexp = new TimeExp(d, h, m, s, ms);

  return timexp;
}


} /* mvc */
