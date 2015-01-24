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
Value *ValueFactory::createValue(Stm *stm)
{
  switch (stm->getTag()) {
  case ST_VARDEF: {
    VardefStm *vardef = (VardefStm *) stm;
    Prop *prop = new Prop(vardef->getName()->getName());
    return prop;
  }
  case ST_EVENTDEF: {
    EventdefStm *evdef = (EventdefStm *) stm;
    Event *ev = new Event(evdef->getName()->getName());
    return ev;
  }
  case ST_FUNDEF: {
    FundefStm *fundef = (FundefStm *) stm;
    Function *fun = new Function(fundef->getName()->getName());
    return fun;
  }
  case ST_PROCDEF: {
    ProcdefStm *procdef = (ProcdefStm *) stm;
    Reactor *reactor = new Reactor(procdef->getName()->getName());
    return reactor;
  }
  default:
    break;
  }
  return NULL;
}

} /* mvc */

