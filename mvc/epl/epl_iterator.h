/**
 * @file epl_iterator.h
 * @brief iterators over Stm and Exp objects
 *
 * @author cjeong
 */
#ifndef MV_EPL_ITERATOR_H
#define MV_EPL_ITERATOR_H

#include "mv/epl/epl_stm.h"
#include "mv/epl/epl_exp.h"
#include "mv/epl/epl_datatype.h"

namespace mv { namespace epl {

class StmIterator {
public:
  StmIterator(Stm *stm);
  ~StmIterator();

  bool hasNext();
  Stm *getNext();

  void operator++();

private:
  bool iterable(Stm *stm);

private:
  /* original statement */
  Stm *_stm;

  /* pointer to the parent statement and the current statement */
  Stm *_pptr;
  Stm *_sptr;

  std::list<Stm *>::iterator _iter;
};

class ExpIterator {
  ExpIterator(Exp *exp);
  ~ExpIterator();
};


}} /* mv::epl */

#endif /* MV_EPL_ITERATOR_H */
