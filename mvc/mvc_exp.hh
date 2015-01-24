/**
 * @file mvc_exp.hh
 */
#ifndef MVC_EXP_HH
#define MVC_EXP_HH

#include <string>
#include <list>
#include <vector>
#include <mv/defs.h>
#include "mvc_visitor.hh"

namespace mvc {

enum ExpTag {
  /* literals */
  ET_INTEGER,    /* integer literal */
  ET_FLOAT,      /* float literal */
  ET_STRING,     /* string literal */

  /* reference expressions */
  ET_SYMBOL,     /* symbol */
  ET_FIELDREF,   /* structure field reference */
  ET_ARRAYREF,   /* array element reference */

  /* complex expressions */
  ET_UNARY,      /* expression with unary operator */
  ET_BINARY,     /* expression with binary operator */

  /* related to functions */
  ET_FUNCALL,    /* function call */

  /* special expressions */
  ET_TIME,       /* time interval */

  ET_NTAGS       /* number of tags */
};

enum UnaryTag {
  UOT_MINUS,  /* - */
  UOT_NTAGS
};

enum BinaryTag {
  BOT_ADD,    /* + */
  BOT_SUB,    /* - */
  BOT_MUL,    /* * */
  BOT_DIV,    /* / */
  BOT_SHL,    /* << */
  BOT_SHR,    /* >> */
  BOT_OR,     /* || */
  BOT_AND,    /* && */
  BOT_EQ,     /* == */
  BOT_NE,     /* != */
  BOT_LT,     /* < */
  BOT_LE,     /* <= */
  BOT_GT,     /* > */
  BOT_GE,     /* >= */
  BOT_NTAGS
};

/**
 * @class Exp
 */
class ExpVisitor;
class Exp {
public:
  Exp(ExpTag t) : _tag(t) { }
  virtual ~Exp() { }

  ExpTag getTag() { return _tag; }

  virtual void accept(ExpVisitor& v) = 0;

protected:
  ExpTag _tag;
};

/**
 * @class SymbolExp
 */
class SymbolExp : public Exp {
public:
  SymbolExp(const std::string& sym);
  ~SymbolExp() { }

  std::string& getDev() { return _dev; }
  std::string& getName() { return _name; }
  
  void accept(ExpVisitor& v) { v.visitSymbolExp(this); }

  /** Returns true iff symbol denotes a local object. */
  bool isLocal() { return (_dev == "") ? true : false; }

private:
  std::string _dev;
  std::string _name;
};

/**
 * @class FieldrefExp
 */
class FieldrefExp : public Exp {
public:
  FieldrefExp(Exp *varref, const std::string& field) 
    : Exp(ET_FIELDREF), _varref(varref), _field(field) { }
  ~FieldrefExp() { }

  Exp *getVarref() { return _varref; }
  const std::string& getField() { return _field; }
  
  void accept(ExpVisitor& v) { v.visitFieldrefExp(this); }

private:
  Exp *_varref;
  std::string _field;
};

/**
 * @class ArrayrefExp
 */
class ArrayrefExp : public Exp {
public:
  ArrayrefExp(Exp *varref, Exp *index) 
    : Exp(ET_FIELDREF), _varref(varref), _index(index) { }
  ~ArrayrefExp() { }

  Exp *getVarref() { return _varref; }
  Exp *getIndex() { return _index; }
  
  void accept(ExpVisitor& v) { v.visitArrayrefExp(this); }

private:
  Exp *_varref;
  Exp *_index;
};

/**
 * @class IntegerExp
 */
class IntegerExp : public Exp {
public:
  IntegerExp(int v) : Exp(ET_INTEGER), _value(v) { }
  ~IntegerExp() { }

  const int getValue() { return _value; }

  void accept(ExpVisitor& v) { v.visitIntegerExp(this); }

private:
  int _value;
};

/**
 * @class FloatExp
 */
class FloatExp : public Exp {
public:
  FloatExp(float v) : Exp(ET_FLOAT), _value(v) { }
  ~FloatExp() { }

  const float getValue() { return _value; }

  void accept(ExpVisitor& v) { v.visitFloatExp(this); }

private:
  float _value;
};

/**
 * @class StringExp
 */
class StringExp : public Exp {
public:
  StringExp(const std::string& v) : Exp(ET_STRING), _value(v) { }
  ~StringExp() { }

  const std::string getValue() { return _value; }

  void accept(ExpVisitor& v) { v.visitStringExp(this); }

private:
  std::string _value;
};

/**
 * @class UnaryExp
 */
class UnaryExp : public Exp {
public:
  UnaryExp(UnaryTag utag, Exp *exp) : Exp(ET_UNARY), _utag(utag), _exp(exp) { }
  ~UnaryExp() { }

  UnaryTag getUnaryTag()  { return _utag; }
  
  Exp *getExp() { return _exp; }

  void accept(ExpVisitor& v) { v.visitUnaryExp(this); }

private:
  UnaryTag _utag;
  Exp *_exp;
};

/**
 * @class BinaryExp
 */
class BinaryExp : public Exp {
public:
  
public:
  BinaryExp(BinaryTag btag, Exp *lexp, Exp *rexp) 
    : Exp(ET_BINARY), _btag(btag), _lexp(lexp), _rexp(rexp) { }
  ~BinaryExp() { }

  Exp *getLexp() { return _lexp; }
  Exp *getRexp() { return _rexp; }

  BinaryTag getBinaryTag()  { return _btag; }

  void accept(ExpVisitor& v) { v.visitBinaryExp(this); }

private:
  BinaryTag _btag;
  Exp *_lexp;
  Exp *_rexp;
};

/**
 * @class FuncallExp
 */
class FuncallExp : public Exp {
public:
  FuncallExp(SymbolExp *name, std::list<Exp *> *args) 
    : Exp(ET_FUNCALL), _name(name), _args(args) { }
  ~FuncallExp() { }

  SymbolExp *getName() { return _name; }
  std::list<Exp *> *getArgs() { return _args; }
  
  void accept(ExpVisitor& v) { v.visitFuncallExp(this); }

private:
  SymbolExp *_name;
  std::list<Exp *> *_args;
};

/**
 * @class TimeExp
 */
class TimeExp : public Exp {
public:
  TimeExp(size_t day, size_t hr, size_t min, size_t sec, size_t ms) 
    : Exp(ET_TIME) {
    _msec = ms;
    _msec += (sec * 1000);
    _msec += (min * 60 * 1000);
    _msec += (hr * 60 * 60 * 1000);
    _msec += (day * 24 * 60 * 60 * 1000);
  }
  ~TimeExp() { }
  
  size_t getMilliseconds() { return _msec; }

  void accept(ExpVisitor& v) { v.visitTimeExp(this); }


private:
  size_t _msec;
};


/**
 * @class ExpFactory
 */
class ExpFactory {
public:
  static ExpFactory *getInstance();

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

protected:
  ExpFactory();
  virtual ~ExpFactory();

private:
  static ExpFactory *_instance;
};

} /* mv */

#endif /* MVC_EXP_HH */
