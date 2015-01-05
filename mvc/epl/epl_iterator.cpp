/**
 * @file epl_iterator.cpp
 *
 * @author cjeong
 */
#include "mv/epl/epl_iterator.h"

namespace mv { namespace epl {

/*
 * StmIterator
 */
StmIterator::StmIterator(Stm *stm) : _stm(stm), _pptr(stm), _sptr(NULL)
{
  if (!iterable(stm)) {
    return;
  }

  /* _sptr always points to the next Stm */
  switch (stm->getTag()) {
  case ST_BLOCK:
    {
      BlockStm *block = static_cast<BlockStm *>(stm);
      std::list<Stm *>& _stms = block->getBody();
      std::list<Stm *>::iterator iter = _stms.begin();
    }
    break;
  case ST_IF:
    {
      IfStm *ifs = static_cast<IfStm *>(stm);
      _sptr = ifs->getStm();
    }
    break;
  case ST_IFELSE:
    {
      IfElseStm *ifelse = static_cast<IfElseStm *>(stm);
      _sptr = ifelse->getThenStm();
    }
    break;
  case ST_WHILE:
    {
      BlockStm *block = static_cast<BlockStm *>(stm);
      std::list<Stm *>& _stms = block->getBody();
      _iter = _stms.begin();
    }
    break;
  case ST_FOR:
    {
      ForStm *fors = static_cast<ForStm *>(stm);
    }
    break;
  case ST_ASSIGN:
  case ST_TRIGGER:
  case ST_RETURN:
  case ST_CONTINUE:
  case ST_BREAK:
  default:
    break;
  }
  
}

StmIterator::~StmIterator()
{
}


bool StmIterator::iterable(Stm *stm)
{
  switch (stm->getTag()) {
  case ST_ASSIGN:
  case ST_TRIGGER:
  case ST_RETURN:
  case ST_CONTINUE:
  case ST_BREAK:
    return false;
  default:
    break;
  }
  return true;
}

bool StmIterator::hasNext()
{
  if (!_sptr)
    return false;
  return true;
}

void StmIterator::operator++()
{
}


Stm *StmIterator::getNext()
{
  switch (_sptr->getTag()) {
  case ST_BLOCK:
    break;
  case ST_IF:
    break;
  case ST_IFELSE:
    break;
  case ST_WHILE:
    break;
  case ST_FOR:
    break;
  default:
    break;
  }
  return NULL;
}

}} /* mv::epl */
