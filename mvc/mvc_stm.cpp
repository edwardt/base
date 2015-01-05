/**
 * @file mvc_stm.cpp
 */
#include "mvc_stm.h"

namespace mvc {

/**
 * @class StmFactoryImpl
 */
class StmFactoryImpl : protected StmFactory {
public:
  StmFactoryImpl();
  ~StmFactoryImpl();

public:
  /* definitions */
  EventdefStm *createEventdef(SymbolExp *exp);
  VardefStm *createVardef(SymbolExp *exp);
  ProcdefStm *createProcdef(SymbolExp *s, std::list<Exp *> *events, Stm *stm);
  FuncdefStm *createFuncdef(SymbolExp *s, std::list<Exp *> *params, Stm *stm);

  /* statements */
  BlockStm *createBlock();
  IfStm *createIf(Exp *cond, Stm *stm);
  IfElseStm *createIfElse(Exp *cond, Stm *then, Stm *elsee);
  WhileStm *createWhile(Exp *cond, Stm *body);
  ForStm *createFor(Stm *init, Exp *cond, Stm *step, Stm *body);
  AssignStm *createAssign(Exp *lhs, Exp *rhs);
  TriggerStm *createTrigger();
  ReturnStm *createReturn(Exp *exp);
  ContinueStm *createContinue();
  BreakStm *createBreak();
};

EventdefStm *StmFactoryImpl::createEventdef(SymbolExp *exp)
{
  EventdefStm *evdef = new EventdefStm(exp);
  
  return evdef;
}

VardefStm *StmFactoryImpl::createVardef(SymbolExp *exp)
{
  VardefStm *vardef = new VardefStm(exp);
  
  return vardef;
}

ProcdefStm *StmFactoryImpl::createProcdef(SymbolExp *name, 
                                          std::list<Exp *> *events, 
                                          Stm *body)
{
  ProcdefStm *proc = new ProcdefStm(name, events, body);
  return proc;
}

FuncdefStm *StmFactoryImpl::createFuncdef(SymbolExp *name,
                                          std::list<Exp *> *params, 
                                          Stm *body)
{
  FuncdefStm *proc = new FuncdefStm(name, params, body);
  return proc;
}

BlockStm *StmFactoryImpl::createBlock()
{
  BlockStm *block = new BlockStm();
  return block;
}

IfStm *StmFactoryImpl::createIf(Exp *cond, Stm *stm)
{
  IfStm *ifstm = new IfStm(cond, stm);
  return ifstm;
}

IfElseStm *StmFactoryImpl::createIfElse(Exp *cond, Stm *then, Stm *elsee)
{
  IfElseStm *ifelse = new IfElseStm(cond, then, elsee);
  return ifelse;
}

WhileStm *StmFactoryImpl::createWhile(Exp *cond, Stm *body)
{
  WhileStm *whiles = new WhileStm(cond, body);
  return whiles;
}

ForStm *StmFactoryImpl::createFor(Stm *init, Exp *cond, Stm *step, Stm *body)
{
  ForStm *fors = new ForStm(init, cond, step, body);
  return fors;
}

AssignStm *StmFactoryImpl::createAssign(Exp *lhs, Exp *rhs)
{
  AssignStm *assign = new AssignStm(lhs, rhs);
  return assign;
}

TriggerStm *StmFactoryImpl::createTrigger()
{
  TriggerStm *trigger = new TriggerStm();
  return trigger;
}

ReturnStm *StmFactoryImpl::createReturn(Exp *exp)
{
  ReturnStm *returns = new ReturnStm(exp);
  return returns;
}

ContinueStm *StmFactoryImpl::createContinue()
{
  ContinueStm *continues = new ContinueStm();
  return continues;
}

BreakStm *StmFactoryImpl::createBreak()
{
  BreakStm *breaks = new BreakStm();
  return breaks;
}

/*
 * StmFactory
 */
StmFactory *StmFactory::getInstance() 
{
  if (!_instance) {
    _instance = new StmFactory();
  }
  return _instance;
}

void StmFactory::shutdown() 
{
  delete _instance;
  _instance = NULL;
}

StmFactory *StmFactory::_instance = NULL;
StmFactory::StmFactory()
{
}

StmFactory::~StmFactory()
{
}

} /* mvc */
