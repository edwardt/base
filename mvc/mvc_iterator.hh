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


enum IterTag {
  ITER_DFS,     /* depth-first traversal */
  ITER_BFS      /* breadth-first traversal */
};

class StmIterator {
public:
  StmIterator(Stm *stm);
  ~StmIterator();

  bool hasNext();
  Stm *getNext();

  StmIterator& next();

private:
  bool iterable(Stm *stm);

private:
  /* initial statement */
  Stm *_stm;

  /* pointer to the parent statement and the current statement */
  Stm *_pptr;
  Stm *_sptr;

  std::list<Stm *>::iterator _iter;
};

class ExpIterator {
public:
  ExpIterator(Exp *exp);
  ExpIterator(Stm *stm);
  ~ExpIterator();

  bool hasNext();
  Exp *getNext();

  ExpIterator& next();

private:
  /* initial expression */
  Stm *_stm;
  Exp *_exp;

  /* pointers to the parent and the current expression */
  Exp *_pptr;
  Exp *_sptr;
};


} /* mvc */

#endif /* MVC_ITERATOR_HH */
