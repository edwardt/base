/**
 * @file mvc_stm.cc
 */
#include "mvc_stm.hh"

namespace mvc {

/*
 * StmFactory
 */
StmFactory *StmFactory::_instance = NULL;
StmFactory *StmFactory::getInstance() 
{
  if (!_instance) {
    _instance = new StmFactory();
  }
  return _instance;
}

StmFactory::StmFactory()
{
}

StmFactory::~StmFactory()
{
}

EventdefStm *StmFactory::createEventdef(SymbolExp *exp)
{
  EventdefStm *evdef = new EventdefStm(exp);
  
  return evdef;
}

VardefStm *StmFactory::createVardef(SymbolExp *exp)
{
  VardefStm *vardef = new VardefStm(exp);
  
  return vardef;
}

ProcdefStm *StmFactory::createProcdef(SymbolExp *name, 
                                          std::list<Exp *> *events, 
                                          Stm *body)
{
  ProcdefStm *proc = new ProcdefStm(name, events, body);
  return proc;
}

FundefStm *StmFactory::createFundef(SymbolExp *name,
                                    std::list<Exp *> *params, 
                                    Stm *body)
{
  FundefStm *proc = new FundefStm(name, params, body);
  return proc;
}

BlockStm *StmFactory::createBlock()
{
  BlockStm *block = new BlockStm();
  return block;
}

IfStm *StmFactory::createIf(Exp *cond, Stm *stm)
{
  IfStm *ifstm = new IfStm(cond, stm);
  return ifstm;
}

IfElseStm *StmFactory::createIfElse(Exp *cond, Stm *then, Stm *elsee)
{
  IfElseStm *ifelse = new IfElseStm(cond, then, elsee);
  return ifelse;
}

WhileStm *StmFactory::createWhile(Exp *cond, Stm *body)
{
  WhileStm *whiles = new WhileStm(cond, body);
  return whiles;
}

ForStm *StmFactory::createFor(Stm *init, Exp *cond, Stm *step, Stm *body)
{
  ForStm *fors = new ForStm(init, cond, step, body);
  return fors;
}

AssignStm *StmFactory::createAssign(Exp *lhs, Exp *rhs)
{
  AssignStm *assign = new AssignStm(lhs, rhs);
  return assign;
}

FuncallStm *StmFactory::createFuncall(FuncallExp *call)
{
  FuncallStm *callstm = new FuncallStm(call);
  return callstm;
}

TriggerStm *StmFactory::createTrigger()
{
  TriggerStm *trigger = new TriggerStm();
  return trigger;
}

ReturnStm *StmFactory::createReturn(Exp *exp)
{
  ReturnStm *returns = new ReturnStm(exp);
  return returns;
}

ContinueStm *StmFactory::createContinue()
{
  ContinueStm *continues = new ContinueStm();
  return continues;
}

BreakStm *StmFactory::createBreak()
{
  BreakStm *breaks = new BreakStm();
  return breaks;
}

} /* mvc */
