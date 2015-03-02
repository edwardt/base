#include <stdio.h>
#include "mv/mvcc/mvcc.hh"

class KeyboardReader : public MV_Process {
public:
  KeyboardReader() {
  }

  init() {
  }

  loop() {
    int ival;
    fscanf(stdin, "%d", &ival);
    triggerEvent(
  }
  
};
