/**
 * @file mvc_stm.hh
 *
 * @brief Statement classes.
 */
#ifndef MVC_STM_HH
#define MVC_STM_HH

#include <list>
#include <vector>
#include "mvc_exp.hh"

namespace mvc {

enum StmTag {
  /* definitions */
  ST_EVENTDEF,   /* event definition */
  ST_VARDEF,     /* variable definition */
  ST_PROCDEF,    /* process definition */
  ST_FUNDEF,     /* function definition */
  ST_DEFINE,     /* define statement */

  /* statements */
  ST_BLOCK,      /* block, i.e. { } */
  ST_IF,         /* if (C) S */
  ST_IFELSE,     /* if (S) S1 else S2 */
  ST_WHILE,      /* while (C) S */
  ST_FOR,        /* for (S1; C; S2) S3 */
  ST_ASSIGN,     /* lhs = rhs */
  ST_FUNCALL,    /* foo(args) */
  ST_TRIGGER,    /* -> event(event_args) */
  ST_RETURN,     /* return E */
  ST_CONTINUE,   /* continue */
  ST_BREAK,      /* break */

  ST_NTAGS       /* number of tags */
};

/**
 * @class Stm
 */
class Stm {
public:
  Stm(StmTag t) : _tag(t) { }
  virtual ~Stm() { }

  StmTag getTag() { return _tag; }

  virtual void accept(StmVisitor& v) = 0;

private:
  Stm(const Stm& stm) = delete;
  Stm& operator=(const Stm& stm) = delete;
  
protected:
  StmTag _tag;
};

/**
 * @class EventdefStm
 */
class EventdefStm : public Stm {
public:
  EventdefStm(SymbolExp *name) 
    : Stm(ST_EVENTDEF), _name(name) { }
  ~EventdefStm() { }

  SymbolExp *getSym() { return _name; }

  void accept(StmVisitor& v) { v.visitEventdefStm(this); }

private:
  SymbolExp *_name;
};

/**
 * @class VardefStm
 */
class VardefStm : public Stm {
public:
  VardefStm(SymbolExp *name)
    : Stm(ST_VARDEF), _name(name) { }
  ~VardefStm() { }

  SymbolExp *getSym() { return _name; }

  void accept(StmVisitor& v) { v.visitVardefStm(this); }

private:
  SymbolExp *_name;
};

/**
 * @class ProcdefStm
 */
class ProcdefStm : public Stm {
public:
  ProcdefStm(SymbolExp *name, std::list<Exp *> *events, Stm *body) 
    : Stm(ST_PROCDEF), _name(name), _body(body) {
    _events.assign(events->begin(), events->end());
  }
  ~ProcdefStm() { }

  void accept(StmVisitor& v) { v.visitProcdefStm(this); }

  SymbolExp *getSym() { return _name; }
  std::list<Exp *>& getEvents() { return _events; }
  Stm *getBody() { return _body; }

private:
  SymbolExp *_name;
  std::list<Exp *> _events;
  Stm *_body;
};

/**
 * @class FundefStm
 */
class FundefStm : public Stm {
public:
  FundefStm(SymbolExp *name, std::list<Exp *> *params, Stm *body) 
    : Stm(ST_FUNDEF), _name(name), _params(params), _body(body) {
  }
  ~FundefStm() { 
    delete _params;
  }

  void accept(StmVisitor& v) { v.visitFundefStm(this); }

  SymbolExp *getSym() { return _name; }
  std::list<Exp *> *getParams() { return _params; }
  Stm *getBody() { return _body; }

private:
  SymbolExp *_name;
  std::list<Exp *> *_params;
  Stm *_body;
};

/**
 * @class BlockStm
 */
class BlockStm : public Stm {
public:
  BlockStm() : Stm(ST_BLOCK) { }
  ~BlockStm() { }

  void accept(StmVisitor& v) { v.visitBlockStm(this); }

  std::list<Stm *>& getBody() { return _body; }
  void addToBody(Stm *stm) { _body.push_back(stm); }

private:
  std::list<Stm *> _body;
};

/** 
 * @class IfStm
 */
class IfStm : public Stm {
public:
  IfStm(Exp *cond, Stm *stm) : Stm(ST_IF), _cond(cond), _stm(stm) { }
  ~IfStm() { }

  void accept(StmVisitor& v) { v.visitIfStm(this); }

  Exp *getCond() { return _cond; }
  Stm *getStm() { return _stm; }

private:
  Exp *_cond;
  Stm *_stm;
};

/**
 * @class IfElseStm
 */
class IfElseStm : public Stm {
public:
  IfElseStm(Exp *cond, Stm *then, Stm *elsee) 
    : Stm(ST_IFELSE), _cond(cond), _then(then), _else(elsee) { }
  ~IfElseStm() { }

  void accept(StmVisitor& v) { v.visitIfElseStm(this); }

  Exp *getCond() { return _cond; }
  Stm *getThenStm() { return _then; }
  Stm *getElseStm() { return _else; }

private:
  Exp *_cond;
  Stm *_then;
  Stm *_else;
};

/**
 * @class WhileStm
 */
class WhileStm : public Stm {
public:
  WhileStm(Exp *cond, Stm *body) 
    : Stm(ST_WHILE), _cond(cond), _body(body) { }
  ~WhileStm() { }

  void accept(StmVisitor& v) { v.visitWhileStm(this); }

  Exp *getCond() { return _cond; }
  Stm *getBody() { return _body; }

private:
  Exp *_cond;
  Stm *_body;
};

/**
 * @class ForStm
 */
class ForStm : public Stm {
public:
  ForStm(Stm *init, Exp *cond, Stm *step, Stm *body) 
    : Stm(ST_IFELSE), _init(init), _cond(cond), _step(step), _body(body) { }
  ~ForStm() { }

  void accept(StmVisitor& v) { v.visitForStm(this); }

  Stm *getInit() { return _init; }
  Exp *getCond() { return _cond; }
  Stm *getStep() { return _step; }
  Stm *getBody() { return _body; }

private:
  Stm *_init;
  Exp *_cond;
  Stm *_step;
  Stm *_body;
};

/**
 * @class AssignStm
 */
class AssignStm : public Stm {
public:
  AssignStm(Exp *lhs, Exp *rhs) : Stm(ST_ASSIGN), _lhs(lhs), _rhs(rhs) { }
  ~AssignStm() { }

  void accept(StmVisitor& v) { v.visitAssignStm(this); }
  
  Exp *getLhs() { return _lhs; }
  Exp *getRhs() { return _rhs; }

private:
  Exp *_lhs;
  Exp *_rhs;
};

/**
 * @class FuncallStm
 */
class FuncallStm : public Stm {
public:
  FuncallStm(FuncallExp *call) : Stm(ST_FUNCALL), _call(call) { }
  ~FuncallStm() { }

  void accept(StmVisitor& v) { v.visitFuncallStm(this); }
  
  FuncallExp *getCall() { return _call; }

private:
  FuncallExp *_call;
};

/**
 * @class TriggerStm
 */
class TriggerStm : public Stm {
public:
  TriggerStm() : Stm(ST_TRIGGER) { }
  ~TriggerStm() { }

  void accept(StmVisitor& v) { v.visitTriggerStm(this); }

  std::vector<Exp *>& getEvents() { return _events; }

private:
  std::vector<Exp *> _events;
};

/**
 * @class ReturnStm
 */
class ReturnStm : public Stm {
public:
  ReturnStm(Exp *exp) : Stm(ST_RETURN), _exp(exp) { }
  ~ReturnStm() { }

  void accept(StmVisitor& v) { v.visitReturnStm(this); }

  Exp *getExp() { return _exp; }

private:
  Exp *_exp;
};

/**
 * @class ContinueStm
 */
class ContinueStm : public Stm {
public:
  ContinueStm() : Stm(ST_CONTINUE) { }
  ~ContinueStm() { }

  void accept(StmVisitor& v) { v.visitContinueStm(this); }
};

/**
 * @class BreakStm
 */
class BreakStm : public Stm {
public:
  BreakStm() : Stm(ST_BREAK) { }
  ~BreakStm() { }

  void accept(StmVisitor& v) { v.visitBreakStm(this); }
};

/**
 * @class DefineStm
 */
class DefineStm : public Stm {
public:
  DefineStm(SymbolExp *name, Exp *def) 
    : Stm(ST_DEFINE), _name(name), _def(def) { }
  ~DefineStm() { }

  SymbolExp *getSym() { return _name; }
  Exp *getDef() { return _def; }

  void accept(StmVisitor& v) { v.visitDefineStm(this); }

private:
  SymbolExp *_name;
  Exp *_def;
};

/**
 * @class StmFactory
 */
class StmFactory {
public:
  static StmFactory *getInstance();
  static void shutdown();

  /* definitions */
  EventdefStm *createEventdef(SymbolExp *exp);
  VardefStm *createVardef(SymbolExp *exp);
  ProcdefStm *createProcdef(SymbolExp *s, std::list<Exp *> *events, Stm *stm);
  FundefStm *createFundef(SymbolExp *s, std::list<Exp *> *params, Stm *stm);

  /* statements */
  BlockStm *createBlock();
  IfStm *createIf(Exp *cond, Stm *stm);
  IfElseStm *createIfElse(Exp *cond, Stm *then, Stm *elsee);
  WhileStm *createWhile(Exp *cond, Stm *body);
  ForStm *createFor(Stm *init, Exp *cond, Stm *step, Stm *body);
  AssignStm *createAssign(Exp *lhs, Exp *rhs);
  FuncallStm *createFuncall(FuncallExp *fcall);
  TriggerStm *createTrigger();
  ReturnStm *createReturn(Exp *exp);
  ContinueStm *createContinue();
  BreakStm *createBreak();

protected:
  StmFactory();
  ~StmFactory();
  
private:
  static StmFactory *_instance;
};

} /* mvc */

#endif /* MVC_STM_HH */

