/**
 * @file mvcc.hh
 *
 * @brief Classes for MVCC.
 */
#ifndef MVCC_HH
#define MVCC_HH

enum MV_ProcessTag {
  MV_ProcessPassive,
  MV_ProcessReactive
};

/**
 * @class MV_Event
 */
class MV_Event {
public:
  MV_EVent();
  virtual ~MV_Event() = 0;
};

/**
 * @class MV_Process
 */
class MV_Process {
public:
  MV_Process();
  virtual ~MV_Process() = 0;

  virtual init() = 0;

  virtual loop() = 0;

private:
  MV_Event *getEvent(const std::string s);

  void waitEvent(MV_Event& e);

  void triggerEvent(MV_Event& e);

private:
  MV_Process(const MV_Process&) = delete;
  MV_Process& operator=(const MV_Process&) = delete;

};


#endif MVCC_HH
