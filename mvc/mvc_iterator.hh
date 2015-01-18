/**
 * @file mvc_iterator.hh
 * 
 * @brief Iterators over Stm and Exp objects.
 */
#ifndef MVC_ITERATOR_HH
#define MVC_ITERATOR_HH

#include "mvc_stm.hh"
#include "mvc_exp.hh"

namespace mvc {

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


} /* mvc */

#endif /* MVC_ITERATOR_HH */
