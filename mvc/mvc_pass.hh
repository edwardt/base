/**
 * @file mvc_pass.hh 
 *
 * @brief Passes for the MVC compiler.
 */
#ifndef MVC_PASS_HH
#define MVC_PASS_HH

namespace mvc {

enum PassTag {
  PT_CONTAINER,
  PT_PASS
};

enum PassTypeTag {
  PT_ANALYZE,
  PT_OPTIMIZE
};

/**
 * @class Pass
 */
class Pass {
public:
  Pass(PassTag tag, PassTypeTag type, const std::string& name) :
    _tag(tag), _type(type), _name(name) { }
  virtual ~Pass() { }

  virtual int run() = 0;

  const std::string& getName() { return _name; }

  /*
    void addRequired(Pass *pass);
  */

private:
  /* do not implement */
  void operator=(const Pass&);
  Pass(const Pass&);

private:
  PassTag _tag;
  PassTypeTag _type;

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

} /* mvc */

#endif /* MVC_PASS_HH */
