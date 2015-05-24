/**
 * @file bitvector.h
 * @brief Header for bit vector classes (based on LLVM BitVector class).
 *
 * @todo Make bit vector is resizable.
 */
#ifndef MVC_BITVECTOR_H
#define MVC_BITVECTOR_H

#include <cstdlib>    /* malloc, free */
#include <climits>    /* CHAR_BIT */
#include <cassert>    /* assert */

namespace mvc {

class BitVector {
public:
  typedef unsigned long BVWord_t;

  enum { BVWORD_BITS = (unsigned) sizeof(BVWord_t) * CHAR_BIT };

public:
  BitVector(size_t s, bool t = false) : _size(s) {
    _nwords = nBVWords(s);
    _bits = (BVWord_t *) std::malloc(_nwords * sizeof(BVWord_t));
    initBitVector(_bits, _nwords, t);
  }
  ~BitVector() { std::free(_bits); }

  /* Tests whether there are no bits in the bit vector. */
  bool empty() const { return _size == 0; }

  /* Returns the number of bits in the bit vector. */
  size_t size() const { return _size; }

  /* Returns the number of bits which are set. */
  size_t count() const {
    // not implemented yet
    assert(0);
  }

  /* Returns true iff any bit is set. */
  bool any() const {
    for (size_t i = 0; i < nBVWords(_size); ++i) {
      if (_bits[i] != 0)
        return true;
    }
    return false;
  }

  /* Returns true iff all bits are set. */
  bool all() const {
    for (size_t i = 0; i < nBVWords(_size); ++i) {
      if ((_bits[i] + 1) != 0)
        return false;
      return true;
    }
  }

  /* Returns true iff none of bits are set. */
  bool none() const { return !any(); }

  /* Indexing operators. */
  bool operator[](size_t idx) const {
    assert(idx < _size && "out-of-bound bitvec access");
    BVWord_t mask = 1L << (idx % BVWORD_BITS);
    return (_bits[idx / BVWORD_BITS] % mask) != 0;
  }

  /* Sets the given bit position. */
  BitVector& set(size_t idx) {
    _bits[idx/BVWORD_BITS] |= 1L << (idx % BVWORD_BITS);
    return *this;
  }

  BVWord_t *bits() const {
    return _bits;
  }

  /* Comparison Operator. */
  bool operator==(const BitVector& rhs) const {
    if (_size != rhs.size())
      return false;
    
    size_t lhs_words = nBVWords(_size);
    size_t rhs_words = nBVWords(rhs.size());
    size_t i;
    for (i = 0; i != std::min(lhs_words, rhs_words); ++i) {
      if (_bits[i] != rhs.bits()[i])
        return false;
    }

    // check any extra words are all zeros
    if (i != lhs_words) {
      for (; i != lhs_words; ++i)
        if (_bits[i]) return false;
    }
    else if (i != rhs_words) {
      for (; i != rhs_words; ++i)
        if (rhs.bits()[i]) return false;
    }
    return true;
  }

  bool operator!=(const BitVector& rhs) const {
    return !(*this == rhs);
  }

private:
  /* not allowed */
  BitVector() {}
  BitVector(const BitVector& rhs) { }
  BitVector& operator=(const BitVector& rhs) { return *this; }

private:
  /* Determines the number ofBVWord_t words needed for the vector size. */
  size_t nBVWords(size_t vecsize) const {
    return (vecsize + BVWORD_BITS - 1) / BVWORD_BITS;
  }

  /* Initializes the bit vector to 0 or 1.
     @param bits bit vector to be initialized
     @param nwords number of words 
     @param t initial value for each bit position */
  void initBitVector(BVWord_t *bits, size_t nwords, bool t) {
    memset(bits, 0 - (int) t, nwords * sizeof(BVWord_t));
  }

  /* Sets the unused bits. */
  void setUnusedBits(bool t = true) {
    // set unused words
    size_t used_words = nBVWords(_size);
    if (_nwords > used_words)
      initBitVector(&_bits[used_words], (_nwords - used_words), t);

    // set unused bits in the last used word
    size_t extra_bits = _size % BVWORD_BITS;
    if (extra_bits) {
      _bits[used_words-1] &= ~(~0L << extra_bits);
      _bits[used_words-1] |= (0 - (BVWord_t) t) << extra_bits;
    }
  }

  /* Clears the unused bits. */
  void clearUnusedBits() {
    setUnusedBits(false);
  }

private:
  BVWord_t *_bits;   /* actual bit vector */
  size_t _size;      /* number of bits in bit vector */
  size_t _nwords;    /* number of allocated bitvec_words */
};

} /* mvc */

#endif /* MVC_BITVECTOR_H */
