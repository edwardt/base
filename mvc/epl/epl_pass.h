/**
 * @file epl_pass.h 
 * @brief Passes for the EPL compiler.
 *
 * @author cjeong
 */
#ifndef MV_EPL_PASS_H
#define MV_EPL_PASS_H

#include "mv/common.h"

namespace mv { namespace epl {

/**
 * @class Pass
 */
class Pass {
public:
  enum PassTag {
    PT_CONTAINER,
    PT_PASS
  };

public:
  Pass(PassTag tag, const std::string& name) : _tag(tag), _name(name);
  virtual ~Pass();

  void run();


  const std::string& getName() { return _name; }

private:
  /* do not implement */
  void operator=(const Pass&);
  Pass(const Pass&);

private:
  PassTag _tag;
  std::string _name;
  std::list<Pass *> *_subpasses;
};

/**
 * @class PassMgr
 */
class PassMgr {
public:
  void addPass(Pass *pass);

private:
  std::list<Pass *> _passes;
};

}} /* mv::epl */

#endif /* MV_EPL_CODE_H */
