/**
 * @file mvc_ir.hh
 *
 * @brief Intermediate representation.
 */
#ifndef MVC_IR_HH
#define MVC_IR_HH

#include <list>
#include "mvc_name.hh"
#include "mvc_symtab.hh"


namespace mvc {

enum QuadTag {
  QT_ADD,        /* add */
  QT_SUB,        /* subtract */
  QT_MUL,        /* multiply */
  QT_DIV,        /* divide */
  QT_FUNCALL,    /* function call */
  QT_PHI,        /* phi-function */
  QT_NTAGS       /* number of tags */
};


/**
 * @class Quad
 */
class Quad {
public:
  Quad(QuadTag tag, Name *lhs, Name *arg0, Name *arg1);
  ~Quad();

private:
  Quad(const Quad& quad) = delete;
  Quad& operator=(const Quad& quad) = delete;

private:
  QuadTag _tag;
  Name *_lhs;
  Name *_arg0;
  Name *_arg1;
};


/**
 * @class BasicBlock
 */
class BasicBlock {
public:
  BasicBlock();
  ~BasicBlock();

  int addQuad(Quad *qad);
  std::vector<Quad *>& getQuads() { return _quads; }

private:
  Basicblock(const BasicBlock& blk) = delete;
  Basicblock& operator=(const BasicBlock& blk) = delete;
  
private:
  std::list<Quad *> _quads;
};


/**
 * @class ControlGraph
 */
class ControlGraph {
};

/**
 * @class IRModule
 */ 
class IRModule {
public:
  
private:
  std::list<Function *> _funcs;
  std::list<Reactor *> _reactors;
};


/**
 * @class IRBuilder
 */
class IRBuilder {
public:
  IRBuilder();

  IRModule *build(Module *mod);
};


} /* mv */

#endif /* MVC_IR_HH */
