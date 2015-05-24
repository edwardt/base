/**
 * @file ilist.h
 * @brief Interface to intrusive doubly linked list class.
 *
 * @note from LLVM ilist.h
 */
#ifndef MVC_ILIST_H
#define MVC_ILIST_H

namespace mvc {

#include <cassert>
#include <iterator>
#include "mv/common.h"


/* This file defines classes to implement an intrusive doubly linked list 
   class (i.e. each node of the list must contain a next and previous field 
   for the list.

   The ilist_traits trait class is used to gain access to the next and prev
   fields of the node type that the list is instantiated with.  If it is not
   specialized, the list defaults to using the getPrev(), getNext() method 
   calls to get the next and previous pointers.

   The ilist class itself, should be a plug in replacement for list, assuming
   that the nodes contain next/prev pointers.  This list replacement does not
   provide a constant time size() method, so be careful to use empty() when 
   you really want to know if it's empty.

   The ilist class is implemented by allocating a 'tail' node when the list 
   is created (using ilist_traits<>::createSentinel()).  This tail node is
   absolutely required because the user must be able to compute end()-1. 
   Because of this, users of the direct next/prev links will see an extra 
   link on the end of the list, which should be ignored.

   Requirements for a user of this list:

     * The user must provide {g|s}et{Next|Prev} methods, or specialize
       ilist_traits to provide an alternate way of getting and setting next 
       and prev links.
*/

template<typename NodeType, typename Traits> class iplist;
template<typename NodeType> class ilist_iterator;

/**
 * @trait ilist_next_prev_traits
 * @brief Template trait for ilist which provides default next/prev 
 * implementations for common operations. 
 */
template<typename T>
struct ilist_nextprev_traits {
  static T *getPrev(T *node) { return node->getPrev(); }
  static T *getNext(T *node) { return node->getNext(); }
  static const T *getPrev(const T *node) { return node->getPrev(); }
  static const T *getNext(const T *node) { return node->getNext(); }

  static void setPrev(T *node, T *prev) { node->setPrev(prev); }
  static void setNext(T *node, T *pext) { node->setNext(next); }
};

template<typename T>
struct ilist_traits;

/**
 * @trait ilist_sentinel_traits
 * @brief Template trait which provides default sentinal implementation. A
 * sentinel is a dummy node that goes at the front of a list which can keep 
 * track of the first and the last node of the list. By inserting after a 
 * sentinel node, a new node is prepended to the list. B inserting before a 
 * sentinel node, a new node is appended to the list. 
 * 
 * @note A lazy dynamic sentinel allocation stratey is implemented, where the 
 * sentinel is stored in the prev field of ilist's head. 
 */
template<typename T>
struct ilist_sentinel_traits {
  static T *createSentinel() { return new T(); }
  static void destroySentinel(T *node) { delete node; }

  static T *provideInitialHead() { return NULL; }
  static T *ensureHead(T *&head) {
    if (!head) {
      head = ilist_traits<T>::createSentinel();
      ilist_traits<T>::noteHead(head, head);
      ilist_traits<T>::setNext(head, NULL);
      return head;
    }
    return ilist_traits<T>::getPrev(head);
  }

  /* Stash the sentinel into its default location. */
  static void noteHead(T *newhead, T *sentinel) {
    ilist_traits<T>::setPrev(newhead, sentinel);
  }
};

/**
 * @trait ilist_node_traits
 * @brief Template traits for intrusive list that provides default node 
 * related operations. 
 */
template<typename T>
struct ilist_node_traits {
  static T *createNode(const T& v) { return new T(v); }
  static void deleteNode(T *V) { delete v; }

  void addNodeToList(T *) { }
  void removeNodeFromList(T *) { }
  void transferNodesFromList(ilist_node_traits&  /* source traits */,
                             ilist_iterator<T>   /* first */,
                             ilist_iterator<T>   /* last */) { }
};

/**
 * @trait ilist_default_traits
 * @brief Default template traits for intrusive list. By inheriting from this, 
 * you can easily use default implementations for all common operations. 
 */
template<typename T>
struct ilist_default_traits : public ilist_nextprev_traits<T>,
                              public ilist_sentinel_traits<T>,
                              public ilist_node_traits<T> { };

/**
 * @trait ilist_traits
 * @brief Template traits for intrusive list. By specializing this template 
 * class, you can change what next/prev fields are used to store the links.
 */
template<typename T>
struct ilist_traits : public ilist_default_traits<T> { };

template<typename T>
struct ilist_traits<const T> : public ilist_traits<T> { };

/**
 * @class ilist_iterator
 * @brief Iterator for intrusive list.
 */
template<typename T>
class ilist_iterator : public std::iterator<std::bidirectional_iterator_tag, 
                                            T, ptrdiff_t>  {
public:
  typedef ilist_traits<T> Traits;
  typedef std::iterator<std::bidirectional_iterator_tag, T, ptrdiff_t> super;

  typedef typename super::value_type value_type;
  typedef typename super::difference_type difference_type;
  typedef typename super::pointer pointer;
  typedef typename super::reference reference;

private:
  pointer nodeptr;

  /* ilist_iterator is not a random-access iterator, but it has an implicit 
     conversion to pointer-type, which is. Declare (but don't define) these 
     functions as private to help catch accidental misuse. */
  void operator[](difference_type) const;
  void operator+(difference_type) const;
  void operator-(difference_type) const;
  void operator+=(difference_type) const;
  void operator-=(difference_type) const;
  template<class T> void operator<(T) const;
  template<class T> void operator<=(T) const;
  template<class T> void operator>(T) const;
  template<class T> void operator>=(T) const;
  template<class T> void operator-(T) const;

public:
  ilist_iterator(pointer np) : nodeptr(np) { }
  ilist_iterator(reference nr) : nodeptr(&nr) { }
  ilist_iterator() : nodeptr(0) { }

  /* This is templated so that we can allow constructing a const iterator from
     a nonconst iterator. */
  template<class NodeType>
  ilist_iterator(const ilist_iterator<NodeType>& rhs)
    : nodeptr(rhs.getNodePtrUnchecked()) { }

  /* This is templated so that we can allow assigning to a const iterator from
     a nonconst iterator. */
  template<class NodeType>
  const ilist_iterator& operator=(const ilist_iterator<NodeType>& rhs) {
    nodeptr = rhs.getNodePtrUnchecked();
    return *this;
  }

  /* Accessors. */
  operator pointer() const { return nodeptr; }
  reference operator*() const { return *nodeptr; }
  pointer operator->() const { return &operator*(); }

  /* Comparison operators. */
  bool operator==(const ilist_iterator& rhs) const {
    return nodeptr == rhs.nodeptr;
  }
  bool operator!=(const ilist_iterator& rhs) const {
    return nodeptr != rhs.nodeptr;
  }

  /* Increment and decrement operators. */
  ilist_iterator& operator--() { 
    nodeptr = Traits::getPrev(nodeptr);
    assert(nodeptr && "--'d off the beginning of an ilist!");
    return *this;
  }
  ilist_iterator& operator++() {
    nodeptr = Traits::getNext(nodeptr);
    return *this;
  }
  ilist_iterator operator--(int) {
    ilist_iterator tmp = *this;
    --*this;
    return tmp;
  }
  ilist_iterator operator++(int) {
    ilist_iterator tmp = *this;
    ++*this;
    return tmp;
  }

  /* For internal use only. */
  pointer getNodePtrUnchecked() const { return nodeptr; }
};

/* These are to catch errors when people try to use them as random access
   iterators. */
template<typename T>
void operator-(int, ilist_iterator<T>) MV_DELETED_FUNCTION;
template<typename T>
void operator-(ilist_iterator<T>,int) MV_DELETED_FUNCTION;
template<typename T>
void operator+(int, ilist_iterator<T>) MV_DELETED_FUNCTION;
template<typename T>
void operator+(ilist_iterator<T>,int) MV_DELETED_FUNCTION;

/* operator!=/operator==: Allow mixed comparisons without dereferencing
   the iterator, which could very likely be pointing to end(). */
template<typename T>
bool operator!=(const T* lhs, const ilist_iterator<const T>& rhs) {
  return lhs != rhs.getNodePtrUnchecked();
}
template<typename T>
bool operator==(const T* lhs, const ilist_iterator<const T>& rhs) {
  return lhs == rhs.getNodePtrUnchecked();
}
template<typename T>
bool operator!=(T* lhs, const ilist_iterator<T>& rhs) {
  return lhs != rhs.getNodePtrUnchecked();
}
template<typename T>
bool operator==(T* lhs, const ilist_iterator<T>& rhs) {
  return lhs == rhs.getNodePtrUnchecked();
}

/* Allow ilist_iterators to convert into pointers to a node automatically when
   used by the dyn_cast, cast, isa mechanisms. */
template<typename From> struct simplify_type;

template<typename T> struct simplify_type<ilist_iterator<T> > {
  typedef T* SimpleType;

  static SimpleType getSimplifiedValue(ilist_iterator<T>& node) {
    return &*node;
  }
};
template<typename T> struct simplify_type<const ilist_iterator<T> > {
  typedef /*const*/ T* SimpleType;

  static T getSimplifiedValue(const ilist_iterator<T>& node) {
    return &*node;
  }
};


/**
 * @class iplist
 * @brief The subset of list functionality that can safely be used on nodes
 * of polymorphic types, i.e. a heterogeneous list with a common base class 
 * that holds the next/prev pointers.  The only state of the list itself is a 
 * single pointer to the head of the list.
 *
 * This list can be in one of three interesting states:
 * 
 * 1. The list may be completely unconstructed.  In this case, the head
 *    pointer is null.  When in this form, any query for an iterator (e.g.
 *    begin() or end()) causes the list to transparently change to state #2.
 *
 * 2. The list may be empty, but contain a sentinel for the end iterator. This
 *    sentinel is created by the Traits::createSentinel method and is a link
 *    in the list.  When the list is empty, the pointer in the iplist points
 *    to the sentinel.  Once the sentinel is constructed, it
 *    is not destroyed until the list is.
 *
 * 3. The list may contain actual objects in it, which are stored as a doubly
 *    linked list of nodes.  One invariant of the list is that the predecessor
 *    of the first node in the list always points to the last node in the list,
 *    and the successor pointer for the sentinel (which always stays at the
 *    end of the list) is always null.
 */
template<typename T, typename Traits=ilist_traits<T> >
class iplist : public Traits {
  mutable T *head;

  /* Use the prev node pointer of 'head' as the tail pointer.  This is really 
     a circular linked list where we snip the 'next' link from the sentinel 
     node back to the first node in the list (to preserve assertions about 
     going off the end of the list. */
  T *getTail() { return this->ensureHead(head); }
  const T *getTail() const { return this->ensureHead(head); }
  void setTail(T *node) const { this->noteHead(head, node); }

  /* Verifies whether the sentinel for the list has been created and 
     lazily makes it if not. */
  void createLazySentinel() const {
    this->ensureHead(head);
  }

  static bool op_less(T& lhs, T& rhs) { return lhs < rhs; }
  static bool op_equal(T& lhs, T& rhs) { return lhs == rhs; }

  /* No fundamental reason why iplist can't be copyable, but the default
     copy/copy-assign won't do. */
  iplist(const iplist&) MV_DELETED_FUNCTION;
  void operator=(const iplist&) MV_DELETED_FUNCTION;

public:
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T value_type;
  typedef ilist_iterator<T> iterator;
  typedef ilist_iterator<const T> const_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
  typedef std::reverse_iterator<iterator>  reverse_iterator;

  iplist() : Head(this->provideInitialHead()) {}
  ~iplist() {
    if (!head) return;
    clear();
    Traits::destroySentinel(getTail());
  }

  /* Iterator creation methods. */
  iterator begin() {
    createLazySentinel();
    return iterator(head);
  }
  const_iterator begin() const {
    createLazySentinel();
    return const_iterator(head);
  }
  iterator end() {
    createLazySentinel();
    return iterator(getTail());
  }
  const_iterator end() const {
    createLazySentinel();
    return const_iterator(getTail());
  }

  /* Reverse iterator creation methods. */
  reverse_iterator rbegin() { 
    return reverse_iterator(end()); 
  }
  const_reverse_iterator rbegin() const{ 
    return const_reverse_iterator(end()); 
  }
  reverse_iterator rend() { 
    return reverse_iterator(begin()); 
  }
  const_reverse_iterator rend() const { 
    return const_reverse_iterator(begin());
  }

  /* Miscellaneous inspection routines. */
  size_type max_size() const { return size_type(-1); }
  bool empty() const { return head == 0 || head == getTail(); }

  /* Front and back accessor functions. */
  reference front() {
    assert(!empty() && "Called front() on empty list.");
    return *head;
  }
  const_reference front() const {
    assert(!empty() && "Called front() on empty list.");
    return *head;
  }
  reference back() {
    assert(!empty() && "Called back() on empty list.");
    return *this->getPrev(getTail());
  }
  const_reference back() const {
    assert(!empty() && "Called back() on empty list.");
    return *this->getPrev(getTail());
  }

  void swap(iplist &rhs) {
    assert(0 && "Swap does not use list traits callback correctly yet!");
    std::swap(head, rhs.head);
  }

  iterator insert(iterator where, T *newnode) {
    T *currnode = where.getNodePtrUnchecked();
    T *prevnode = this->getPrev(currnode);
    this->setNext(newnode, currnode);
    this->setPrev(newnode, prevnode);

    // is prevnode off the beginning of the list?
    if (currnode != head)
      this->setNext(prevnode, newnode);
    else
      head = newnode;
    this->setPrev(currnode, newnode);

    // notify traits that we added a node
    this->addNodeToList(newnode);
    return newnode;
  }

  iterator insertAfter(iterator where, T *newnode) {
    if (empty())
      return insert(begin(), newnode);
    else
      return insert(++where, newnode);
  }

  T *remove(iterator& iter) {
    assert(iter != end() && "Cannot remove end of list.");
    T *node = &*iter;
    T *nextnode = this->getNext(node);
    T *prevnode = this->getPrev(node);

    // is PrevNode off the beginning of the list?
    if (node != head)  
      this->setNext(PrevNode, NextNode);
    else
      Head = NextNode;
    this->setPrev(NextNode, PrevNode);
    IT = NextNode;

    // notify traits that we removed a node
    this->removeNodeFromList(Node);

    // Set the next/prev pointers of the current node to NULL.  This isn't
    // strictly required, but this catches errors where a node is removed from
    // an ilist (and potentially deleted) with iterators still pointing at it.
    // When those iterators are incremented or decremented, they will assert on
    // the null next/prev pointer instead of "usually working".
    this->setNext(node, 0);
    this->setPrev(node, 0);
    return Node;
  }

  T *remove(const iterator& iter) {
    iterator mutable_iter = iter;
    return remove(mutable_iter);
  }

  /* remove a node from the controlled sequence and delete it */
  iterator erase(iterator where) {
    this->deleteNode(remove(where));
    return where;
  }

  /* Remove all nodes from the list like clear(), but do not call
     removeNodeFromList() or deleteNode(). This should only be used
     immediately before freeing nodes in bulk to avoid traversing the list
     and bringing all the nodes into cache. */
  void clearAndLeakNodesUnsafely() {
    if (head) {
      head = getTail();
      this->setPrev(head, head);
    }
  }

private:
  /* The heart of the splice function.  Move linked list nodes from 
     [first, last) into position. */
  void transfer(iterator position, iplist &L2, iterator first, iterator last) {
    assert(first != last && "Should be checked by callers");
    // position cannot be contained in the range to be transferred; check 
    // for the most common mistake
    assert(position != first &&
           "Insertion point can't be one of the transferred nodes");

    if (position != last) {
      // Note: we have to be careful about the case when we move the first node
      // in the list.  This node is the list sentinel node and we can't move it.
      T *ThisSentinel = getTail();
      setTail(0);
      T *L2Sentinel = L2.getTail();
      L2.setTail(0);

      // Remove [first, last) from its old position.
      T *First = &*first, *Prev = this->getPrev(First);
      T *Next = last.getNodePtrUnchecked(), *Last = this->getPrev(Next);
      if (Prev)
        this->setNext(Prev, Next);
      else
        L2.head = Next;
      this->setPrev(Next, Prev);

      // splice [first, last) into its new position
      T *PosNext = position.getNodePtrUnchecked();
      T *PosPrev = this->getPrev(PosNext);

      // fix head of list
      if (PosPrev)
        this->setNext(PosPrev, First);
      else
        head = First;
      this->setPrev(First, PosPrev);

      // fix end of list
      this->setNext(Last, PosNext);
      this->setPrev(PosNext, Last);

      this->transferNodesFromList(L2, First, PosNext);

      // now that everything is set, restore the pointers to list sentinels
      L2.setTail(L2Sentinel);
      setTail(ThisSentinel);
    }
  }

public:

  //===----------------------------------------------------------------------===
  // Functionality derived from other functions defined above...
  //

  size_type size() const {
    if (head == 0) return 0; // Don't require construction of sentinel if empty.
    return std::distance(begin(), end());
  }

  iterator erase(iterator first, iterator last) {
    while (first != last)
      first = erase(first);
    return last;
  }

  void clear() { if (head) erase(begin(), end()); }

  // Front and back inserters...
  void push_front(T *val) { insert(begin(), val); }
  void push_back(T *val) { insert(end(), val); }
  void pop_front() {
    assert(!empty() && "pop_front() on empty list!");
    erase(begin());
  }
  void pop_back() {
    assert(!empty() && "pop_back() on empty list!");
    iterator t = end(); erase(--t);
  }

  // Special forms of insert...
  template<class InIt> void insert(iterator where, InIt first, InIt last) {
    for (; first != last; ++first) insert(where, *first);
  }

  // Splice members - defined in terms of transfer...
  void splice(iterator where, iplist &L2) {
    if (!L2.empty())
      transfer(where, L2, L2.begin(), L2.end());
  }
  void splice(iterator where, iplist &L2, iterator first) {
    iterator last = first; ++last;
    if (where == first || where == last) return; // No change
    transfer(where, L2, first, last);
  }
  void splice(iterator where, iplist &L2, iterator first, iterator last) {
    if (first != last) transfer(where, L2, first, last);
  }



  //===----------------------------------------------------------------------===
  // High-Level Functionality that shouldn't really be here, but is part of list
  //

  // These two functions are actually called remove/remove_if in list<>, but
  // they actually do the job of erase, rename them accordingly.
  //
  void erase(const T &val) {
    for (iterator I = begin(), E = end(); I != E; ) {
      iterator next = I; ++next;
      if (*I == val) erase(I);
      I = next;
    }
  }
  template<class Pr1> void erase_if(Pr1 pred) {
    for (iterator I = begin(), E = end(); I != E; ) {
      iterator next = I; ++next;
      if (pred(*I)) erase(I);
      I = next;
    }
  }

  template<class Pr2> void unique(Pr2 pred) {
    if (empty()) return;
    for (iterator I = begin(), E = end(), Next = begin(); ++Next != E;) {
      if (pred(*I))
        erase(Next);
      else
        I = Next;
      Next = I;
    }
  }
  void unique() { unique(op_equal); }

  template<class Pr3> void merge(iplist &right, Pr3 pred) {
    iterator first1 = begin(), last1 = end();
    iterator first2 = right.begin(), last2 = right.end();
    while (first1 != last1 && first2 != last2)
      if (pred(*first2, *first1)) {
        iterator next = first2;
        transfer(first1, right, first2, ++next);
        first2 = next;
      } else {
        ++first1;
      }
    if (first2 != last2) transfer(last1, right, first2, last2);
  }
  void merge(iplist &right) { return merge(right, op_less); }

  template<class Pr3> void sort(Pr3 pred);
  void sort() { sort(op_less); }
};


template<typename T>
struct ilist : public iplist<T> {
  typedef typename iplist<T>::size_type size_type;
  typedef typename iplist<T>::iterator iterator;

  ilist() {}
  ilist(const ilist &right) {
    insert(this->begin(), right.begin(), right.end());
  }
  explicit ilist(size_type count) {
    insert(this->begin(), count, T());
  }
  ilist(size_type count, const T &val) {
    insert(this->begin(), count, val);
  }
  template<class InIt> ilist(InIt first, InIt last) {
    insert(this->begin(), first, last);
  }

  // bring hidden functions into scope
  using iplist<T>::insert;
  using iplist<T>::push_front;
  using iplist<T>::push_back;

  // Main implementation here - Insert for a node passed by value...
  iterator insert(iterator where, const T &val) {
    return insert(where, this->createNode(val));
  }


  // Front and back inserters...
  void push_front(const T &val) { insert(this->begin(), val); }
  void push_back(const T &val) { insert(this->end(), val); }

  void insert(iterator where, size_type count, const T &val) {
    for (; count != 0; --count) insert(where, val);
  }

  // Assign special forms...
  void assign(size_type count, const T &val) {
    iterator I = this->begin();
    for (; I != this->end() && count != 0; ++I, --count)
      *I = val;
    if (count != 0)
      insert(this->end(), val, val);
    else
      erase(I, this->end());
  }
  template<class InIt> void assign(InIt first1, InIt last1) {
    iterator first2 = this->begin(), last2 = this->end();
    for ( ; first1 != last1 && first2 != last2; ++first1, ++first2)
      *first1 = *first2;
    if (first2 == last2)
      erase(first1, last1);
    else
      insert(last1, first2, last2);
  }


  // Resize members...
  void resize(size_type newsize, T val) {
    iterator i = this->begin();
    size_type len = 0;
    for ( ; i != this->end() && len < newsize; ++i, ++len) /* empty*/ ;

    if (len == newsize)
      erase(i, this->end());
    else                                          // i == end()
      insert(this->end(), newsize - len, val);
  }
  void resize(size_type newsize) { resize(newsize, T()); }
};

} /* mvc */

namespace std {
  /* Ensure that swap uses the fast list swap. */
  template<class T>
  void swap(llvm::iplist<T>& left, llvm::iplist<T>& right) {
    left.swap(right);
  }
}  /* std */

#endif /* MVC_ILIST_H */
