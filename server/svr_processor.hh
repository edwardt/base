/**
 * @file svr_processor.hh
 *
 */
#ifndef MV_SVR_PROCESSOR_HH
#define MV_SVR_PROCESSOR_HH

#include <string>          /* string */
#include <vector>          /* vector */
#include <mq/mqueue.h>     /* mv_mqueue_t */
#include "svr_message.hh"  /* Message */

namespace mv { namespace server {

/**
 * @class MessageProcessor
 */
class MessageProcessor {
public:
  static MessageProcessor *getInstance(mv_mqueue_t *mq);

  virtual void run() = 0;
  virtual void stop() = 0;
  virtual void shutdown() = 0;
  
protected:
  MessageProcessor(mv_mqueue_t *mq);
  virtual ~MessageProcessor();

protected:
  mv_mqueue_t *_mq;
};

}} /* mv::server */

#endif /* MV_SVR_MESSAGE_HH */
