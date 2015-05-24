/**
 * @file mvc_iterator.cc
 */
#include <cassert>
#include "mvc_iterator.hh"

namespace mvc {

/*
 * StmIterator
 */
StmIterator::StmIterator(Stm *stm) : _stm(stm), _sptr(NULL)
{
  if (!stm)
    return;

  /* _sptr always points to the next Stm */
  switch (stm->getTag()) {
  case ST_BLOCK: {
    BlockStm *block = static_cast<BlockStm *>(stm);
    std::list<Stm *>& _stms = block->getBody();
    _iter = _stms.begin();
    if (_iter == _stms.end())
      _sptr = NULL;
    else
      _sptr = *_iter;
    break;
  }
  case ST_IF: {
    IfStm *ifs = static_cast<IfStm *>(stm);
    _sptr = ifs->getStm();
    break;
  }
  case ST_IFELSE: {
    IfElseStm *ifelse = static_cast<IfElseStm *>(stm);
    _sptr = ifelse->getThenStm();
    break;
  }
  case ST_WHILE: {
    WhileStm *whiles = static_cast<WhileStm *>(stm);
    _sptr = whiles->getBody();
    break;
  }
  case ST_FOR: {
    ForStm *fors = static_cast<ForStm *>(stm);
    _sptr = fors->getInit();
    break;
  }
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


bool StmIterator::hasNext()
{
  if (_sptr)
    return true;
  return false;
}

Stm *StmIterator::getNext()
{
  return _sptr;
}

StmIterator& StmIterator::next()
{
  switch (_stm->getTag()) {
  case ST_BLOCK: {
    BlockStm *block = static_cast<BlockStm *>(_stm);
    std::list<Stm *>& _stms = block->getBody();
    ++_iter;
    if (_iter == _stms.end())
      _sptr = NULL;
    else
      _sptr = *_iter;
    break;
  }
  case ST_IF:
  case ST_WHILE:
    _sptr = NULL;
    break;
  case ST_IFELSE: {
    IfElseStm *ifelse = static_cast<IfElseStm *>(_stm);
    if (_sptr == ifelse->getThenStm())
      _sptr = ifelse->getElseStm();
    else
      assert(0);
    break;
  }
  case ST_FOR: {
    ForStm *fors = static_cast<ForStm *>(_stm);
    if (_sptr = fors->getInit())
      _sptr = fors->getBody();
    else if (_sptr = fors->getBody())
      _sptr = fors->getStep();
    else if (_sptr = fors->getStep())
      _sptr = NULL;
    else
      assert(0);
    break;
  }
  case ST_ASSIGN:
  case ST_TRIGGER:
  case ST_RETURN:
  case ST_CONTINUE:
  case ST_BREAK:
    _sptr = NULL;
  default:
    break;
  }

  return *this;
}

/*
 * ExpIterator
 */
ExpIterator::ExpIterator(Exp *exp) : _stm(NULL), _exp(exp), _eptr(NULL)
{
  if (!exp)
    return;

  switch (exp->getTag()) {
  case ET_INTEGER:
  case ET_FLOAT:
  case ET_STRING:
  case ET_SYMBOL:
    break;
  case ET_FIELDREF: {
    FieldrefExp *fieldref = static_cast<FieldrefExp *>(exp);
    _eptr = fieldref->getVarref();
    break;
  }
  case ET_ARRAYREF: {
    ArrayrefExp *arrayref = static_cast<ArrayrefExp *>(exp);
    _eptr = arrayref->getVarref();
    break;
  }
  case ET_UNARY: {
    UnaryExp *unary = static_cast<UnaryExp *>(exp);
    _eptr = unary->getExp();
    break;
  }
  case ET_BINARY: {
    BinaryExp *binary = static_cast<BinaryExp *>(exp);
    _eptr = binary->getLexp();
    break;
  }
  case ET_FUNCALL: {
    FuncallExp *funcall = static_cast<FuncallExp *>(exp);
    _eptr = funcall->getName();
    break;
  }
  case ET_TIME:
    assert(0 && "Not implemented");
  default:
    break;
  }
}

ExpIterator::ExpIterator(Stm *stm) :_stm(stm), _exp(NULL), _eptr(NULL)
{
  if (!stm)
    return;
  
  switch (stm->getTag()) {
  case ST_IF: {
    IfStm *ifs = static_cast<IfStm *>(stm);
    _eptr = ifs->getCond();
    break;
  }
  case ST_IFELSE: {
    IfElseStm *ifelse = static_cast<IfElseStm *>(stm);
    _eptr = ifelse->getCond();
    break;
  }
  case ST_WHILE: {
    WhileStm *whiles = static_cast<WhileStm *>(stm);
    _eptr = whiles->getCond();
    break;
  }
  case ST_FOR: {
    ForStm *fors = static_cast<ForStm *>(stm);
    _eptr = fors->getCond();
    break;
  }
  case ST_ASSIGN: {
    AssignStm *asgn = static_cast<AssignStm *>(stm);
    _eptr = asgn->getLhs();
    break;
  }
  case ST_TRIGGER: {
    assert(0 && "Not implemented");
    break;
  }
  case ST_RETURN: {
    ReturnStm *returns = static_cast<ReturnStm *>(stm);
    _eptr = returns->getExp();
  }
  case ST_BLOCK:
  case ST_CONTINUE:
  case ST_BREAK:
  default:
    break;
  }
}

ExpIterator::~ExpIterator()
{
}

bool ExpIterator::hasNext()
{
  if (_eptr)
    return true;
  return false;
}

Exp *ExpIterator::getNext()
{
  return _eptr;
}

ExpIterator& ExpIterator::next()
{
  if (_stm) {
    switch (_stm->getTag()) {
    case ST_IF:
    case ST_IFELSE:
    case ST_WHILE:
    case ST_FOR:
      _eptr = NULL;
      break;
    case ST_ASSIGN: {
      AssignStm *asgn = static_cast<AssignStm *>(_stm);
      if (_eptr == asgn->getLhs())
        _eptr = asgn->getRhs();
      else if (_eptr == asgn->getRhs())
        _eptr = NULL;
      else 
        assert(0);
      break;
    }
    case ST_TRIGGER: {
      assert(0 && "Not implemented");
      break;
    }
    case ST_RETURN: {
      _eptr = NULL;
    }
    case ST_BLOCK:
    case ST_CONTINUE:
    case ST_BREAK:
    default:
      break;
    }
  }
  else {
    switch (_exp->getTag()) {
    case ET_INTEGER:
    case ET_FLOAT:
    case ET_STRING:
    case ET_SYMBOL:
      break;
    case ET_FIELDREF:
      _eptr = NULL;
      break;
    case ET_ARRAYREF: {
      ArrayrefExp *arrayref = static_cast<ArrayrefExp *>(_exp);
      if (_eptr == arrayref->getVarref())
        _eptr = arrayref->getIndex();
      else if (_eptr == arrayref->getIndex())
        _eptr = NULL;
      else
        assert(0);
      break;
    }
    case ET_UNARY:
      _eptr = NULL;
      break;
    case ET_BINARY: {
      BinaryExp *binary = static_cast<BinaryExp *>(_exp);
      if (_eptr == binary->getLexp())
        _eptr = binary->getRexp();
      else if (_eptr == binary->getRexp())
        _eptr = NULL;
      else
        assert(0);
      break;
    }
    case ET_FUNCALL: {
      FuncallExp *funcall = static_cast<FuncallExp *>(_exp);
      std::list<Exp *> *args = funcall->getArgs();
      if (_eptr = funcall->getName()) {
        _iter = args->begin();
        if (_iter != args->end())
          _eptr = *_iter;
        else
          _eptr = NULL;
      }
      else {
        ++_iter;
        if (_iter != args->end())
          _eptr = *_iter;
        else
          _eptr = NULL;
      }
      break;
    }
    case ET_TIME:
      assert(0 && "Not implemented");
    default:
      break;
    }
  }

  return *this;
}


} /* mvc */
