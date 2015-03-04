/**
 * @file mvcc.hh
 */
#include "mvcc/mvcc.hh"

namespace mvcc {

/**
 * @class Proxy
 */
class Proxy {
public:
  static Proxy *getInstance() {
    if (!_instance)
      _instance = new Proxy();
    return _instance;
  }

private:
  Proxy *_instance;
};
static Proxy *Proxy::_instance = NULL;


/*
 * Runtime functions. 
 */
void registerEvent(const std::string s)
{
  mv_register_event();
}

void waitEvent(Event& e)
{
  mv_wait_event();
}

void triggerEvent(Event& e)
{
  mv_trigger_event();
}

} /* mvcc */
