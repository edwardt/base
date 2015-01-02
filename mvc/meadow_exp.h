/**
 * @file meadow_exp.h 
 */
#ifndef MEADOW_EXP_H
#define MEADOW_EXP_H

#include <string>
#include <list>
#include <vector>
#include "mv/common.h"
#include "mv/epl/epl_visitor.h"

typedef enum {
  /* literals */
  ET_INTEGER,    /* integer literal */
  ET_FLOAT,      /* float literal */
  ET_STRING,     /* string literal */
  ET_CHAR,       /* char literal */

  /* reference expressions */
  ET_SYMBOL,     /* symbol */
  ET_FIELDREF,   /* structure field reference */
  ET_ARRAYREF,   /* array element reference */

  /* complex expressions */
  ET_UNARY,      /* expression with unary operator */
  ET_BINARY,     /* expression with binary operator */

  /* related to functions */
  ET_FUNCALL,    /* function call */

  ET_NTAGS       /* number of tags */
} Meadow_ExpTag;

typedef enum {
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
} Meadow_BinaryOpTag;

typedef int Meadow_Int;
typedef float Meadow_Float;
typedef char *Meadow_String;
typedef char Meadow_Char;
typedef char *Meadow_Symbol;

typedef struct {
  Meadow_Oper op; 
  struct Meadow_Exp *exp;
} Meadow_Unary;

typedef struct {
  Meadow_Oper op;
  struct Meadow_Exp *lexp;
  struct Meadow_Exp *rexp;
} Meadow_Binary;

typedef struct {
  Meadow_ExpTag tag;
  union {
    Meadow_Int ival;
    Meadow_Real fval;
    Meadow_String sval;
    Meadow_Char *cval;
    Meadow_Symbol *sym;
    Meadow_FuncCall *sym;
    Meadow_Unary *unary;
    Meadow_Binary *binary;
  } u;
} Meadow_Exp;

/**
 * @class Exp
 */
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
  SymbolExp(const std::string& name) : Exp(ET_SYMBOL), _name(name) { }
  ~SymbolExp() { }

  std::string& getName() { return _name; }
  
  void accept(ExpVisitor& v) { v.visitSymbolExp(this); }

private:
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
 * @class NumberExp
 */
class NumberExp : public Exp {
public:
  NumberExp(int v) : Exp(ET_NUMBER), _value(v) { }
  ~NumberExp() { }

  const int getValue() { return _value; }

  void accept(ExpVisitor& v) { v.visitNumberExp(this); }

private:
  int _value;
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
 * @class AliasExp
 */
class AliasExp : public Exp {
public:
  AliasExp(Exp *e, Exp *alias) : Exp(ET_ALIAS), _exp(e), _alias(alias) { }
  ~AliasExp() { }

  Exp *getExp() { return _exp; }
  Exp *getAlias() { return _alias; }

  void accept(ExpVisitor& v) { v.visitAliasExp(this); }

private:
  Exp *_exp;
  Exp *_alias;
};

/**
 * @class UnaryExp
 */
class UnaryExp : public Exp {
public:
  enum UnaryTag {
    UOT_MINUS,   /* - */
    UOT_NTAGS
  };

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
 * @class ParamExp
 * @brief Typed parameters for function definition.
 */
class Datatype;
class ParamExp : public Exp {
public:
  ParamExp(Datatype *type, SymbolExp *name) 
    : Exp(ET_PARAM), _type(type), _name(name) { }
  ~ParamExp() { }

  Datatype *getType() { return _type; }
  SymbolExp *getName() { return _name; }

  void accept(ExpVisitor& v) { v.visitParamExp(this); }

private:
  Datatype *_type;
  SymbolExp *_name;
};

/**
 * @class FuncallExp
 */
class FuncallExp : public Exp {
public:
  FuncallExp(SymbolExp *name, std::list<Exp *> *args) 
    : Exp(ET_FUNCALL), _name(name), _args(args) { }
  ~FuncallExp() { }

  SymbolExp *getName();
  
  void accept(ExpVisitor& v) { v.visitFuncallExp(this); }

private:
  SymbolExp *_name;
  std::list<Exp *> *_args;
};


/**
 * @class StreamExp
 */
class TimeExp;
class StreamExp : public Exp {
public:
  StreamExp(SymbolExp *s, uint32_t n, TimeExp *time) 
    : Exp(ET_STREAM), _event(s), _evcount(n), _time(time) { }
  ~StreamExp() { }

  SymbolExp *getEvent() { return _event; }
  uint32_t getEventCount() { return _evcount; }
  TimeExp *getTime() { return _time; }

  void accept(ExpVisitor& v) { v.visitStreamExp(this); }

private:
  SymbolExp *_event;
  uint32_t _evcount;
  TimeExp *_time;
};

/**
 * @class TimeExp
 */
class TimeExp : public Exp {
public:
  TimeExp(uint32_t day, uint32_t hr, uint32_t min, uint32_t sec, uint32_t ms) 
    : Exp(ET_TIME) {
    _msec = ms;
    _msec += (sec * 1000);
    _msec += (min * 60 * 1000);
    _msec += (hr * 60 * 60 * 1000);
    _msec += (day * 24 * 60 * 60 * 1000);
  }
  ~TimeExp() { }

  uint64_t getMilliseconds() { return _msec; }

  void accept(ExpVisitor& v) { v.visitTimeExp(this); }


private:
  uint64_t _msec;
};


/**
 * @class ExpMgr
 */
class ExpMgr {
public:
  static ExpMgr *getInstance();
  static void shutdown();

  SymbolExp *createSymbol(const std::string& name);
  FieldrefExp *createFieldref(Exp *varref, const std::string& field);
  ArrayrefExp *createArrayref(Exp *varref, Exp *index);

  NumberExp *createNumber(int v);
  StringExp *createString(const std::string& v);
  AliasExp *createAlias(Exp *e, Exp *alias);
  UnaryExp *createUnary(UnaryExp::UnaryTag utag);
  BinaryExp *createBinary(BinaryExp::BinaryTag btag, Exp *lexp, Exp *rexp);

  ParamExp *createParam(Datatype *type, SymbolExp *name);
  FuncallExp *createFuncall(SymbolExp *name, std::list<Exp *> *args);
  StreamExp *createStream(SymbolExp *name, uint32_t n, TimeExp *time);
  TimeExp *createTime(uint32_t d, uint32_t h, uint32_t m, uint32_t s, 
                      uint32_t ms);

private:
  ExpMgr();
  ~ExpMgr();

private:
  static ExpMgr *_instance;

private:
  std::list<Exp *> _exps;
};

#endif /* MEADOW_EXP_H */
