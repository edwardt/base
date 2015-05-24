/**
 * @file mvc_value.hh
 *
 * @brief Interface to values which represent compile-time objects such
 * as functions, properties, etc.
 */
#ifndef MVC_VALUE_HH
#define MVC_VALUE_HH

#include "mvc_stm.hh"

namespace mvc {

enum ValueTag {
  VT_NAME,
  VT_PROP,
  VT_FUNC,
  VT_EVENT,
  VT_REACTOR,
  VT_NTAGS       /* number of tags */
};

/**
 * @class Value
 */
class Value {
public:
  Value(ValueTag t) : _tag(t) { }
  virtual ~Value() { }
  
  ValueTag getTag() { return _tag; }
  const std::string& getTagstr();

  virtual std::string& getName() = 0;

private:
  Value(const Value& v) = delete;
  Value& operator=(const Value& v) = delete;

protected:
  ValueTag _tag;
};

/**
 * @class Name
 *
 * @brief Name represents an identifier which identifies a compile-time
 * object. 
 */
class Name : public Value {
public:
  Name(const std::string& dev, const std::string& name) :
    Value(VT_NAME), _dev(dev), _name(name) { }
  ~Name() { }

  std::string& getDev() { return _dev; }
  std::string& getName() { return _name; }

private:
  Name(const Name& v) = delete;
  Name& operator=(const Name& v) = delete;
  
private:
  std::string _dev;
  std::string _name;
};

/**
 * @class Prop
 */
class Prop : public Value {
public:
  Prop(const std::string& name) : Value(VT_PROP), _name(name) { }
  ~Prop() { }

  std::string& getName() { return _name; }
  
private:
  Prop(const Prop& v) = delete;
  Prop& operator=(const Prop& v) = delete;
  
private:
  std::string _name;
};

/**
 * @class Event
 */
class Event : public Value {
public:
  Event(const std::string& name) : Value(VT_EVENT), _name(name) { }
  ~Event() { }
  
  std::string& getName() { return _name; }
  
private:
  Event(const Event& v) = delete;
  Event& operator=(const Event& v) = delete;
  
private:
  std::string _name;
};

/**
 * @class Function
 */
class Function : public Value {
public:
  Function(const std::string& name) : Value(VT_FUNC), _name(name) { }
  ~Function() { }
  
  std::string& getName() { return _name; }
  
private:
  Function(const Function& v) = delete;
  Function& operator=(const Function& v) = delete;
  
private:
  std::string _name;
  unsigned _nargs;
};

/**
 * @class Reactor
 */
class Reactor : public Value {
public:
  Reactor(const std::string& name) : Value(VT_REACTOR), _name(name) { }
  ~Reactor() { }
  
  std::string& getName() { return _name; }
  
private:
  Reactor(const Reactor& v) = delete;
  Reactor& operator=(const Reactor& v) = delete;
  
private:
  std::string _name;
};


/**
 * @class ValueFactory
 */
class ValueFactory {
public:
  static Name *createName(SymbolExp *sym);
  static Prop *createProp(const std::string& name);
  static Event *createEvent(const std::string& name);
  static Function *createFunction(const std::string& name);
  static Reactor *createReactor(const std::string& name);

private:
  ValueFactory() { }
  ~ValueFactory() { } 
};
  
} /* mvc */

#endif /* MVC_VALUE_HH */
