/**
 * @file reader.cc
 */
#include <stdio.h>
#include <mv/value.hh>
#include <mvcc/mvcc.hh>


class KeyboardReader : public mvcc::Process {
public:

  KeyboardReader() {
  }

  ~KeyboardReader() {
  }

  void init() {
    mvcc::registerEvent(std::string("KeyPressedEvent"));
  }

  void loop() {
    int iv;
    fscanf(stdin, "%d", &iv);

    mv_value_t intval = mv_value_int(iv);
    mvcc::Event keyev("KeyPressedEvent", intval);
    triggerEvent(keyev);
  }
};
