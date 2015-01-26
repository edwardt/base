/**
 * @file mvc_value.cc
 */
#include "mvc_value.hh"

namespace mvc {

/*
 * Value
 */
static const std::string _vtagstr[] = {
  std::string("Property"),
  std::string("Function"),
  std::string("Event"),
  std::string("Reactor"),
  std::string("NULL"),
};
const std::string& Value::getTagstr() {
  return _vtagstr[_tag];
}


/*
 * ValueFactory
 */
Prop *ValueFactory::createProp(const std::string& name)
{
  Prop *prop = new Prop(name);

  return prop;
}

Event *ValueFactory::createEvent(const std::string& name)
{
  Event *ev = new Event(name);

  return ev;
}

Function *ValueFactory::createFunction(const std::string& name)
{
  Function *fun = new Function(name);

  return fun;
}

Reactor *ValueFactory::createReactor(const std::string& name)
{
  Reactor *reactor = new Reactor(name);

  return reactor;
}

} /* mvc */

