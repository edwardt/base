/**
 * @file mvcc.hh
 *
 * @brief Classes for MVCC.
 */
#ifndef MVCC_HH
#define MVCC_HH

#include <mv/value.h>

namespace mvcc {

/*
 * Runtime functions.
 */
void registerEvent(const std::string s);
void waitEvent(Event& e);
void triggerEvent(Event& e);

/**
 * @class Event
 */
class Event {
public:
  Event(const std::string s, mv_value_t v) : _name(s), _value(v) { }
  ~Event() {
    delete _value;
  }

  std::string& getName() { return _name; }
  mv_value_t getValue() { return _value; }

private:
  std::string _name;
  mv_value_t _value;
};


/**
 * @class Process
 */
class Process {
public:
  Process();
  virtual ~Process() = 0;

  virtual init() = 0;

  virtual loop() = 0;

private:
  Process(const Process&) = delete;
  Process& operator=(const Process&) = delete;
};

} /* mvcc */

#endif MVCC_HH
