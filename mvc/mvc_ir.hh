/**
 * @file mvc_ir.hh
 *
 * @brief Intermediate representation.
 */
#ifndef MVC_IR_HH
#define MVC_IR_HH

#include <list>

namespace mvc {

/**
 * @class Quad
 */
class Quad {
public:
  Quad();
  ~Qual();

private:
  Quad(const Quad& quad) = delete;
  Quad& operator=(const Quad& quad) = delete;

private:
  Name *lhs;
  Name *rhs0;
  Name *rhs1;
};

/**
 * @class BasicBlock
 */
class BasicBlock {
public:
  BasicBlock();
  ~BasicBlock();

  int addQuad(Quad *qad);
  std::list<Quad *>& getQuads() { return _quads; }

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
