/**
 * @file svr_message.hh
 *
 */
#ifndef MV_SVR_MESSAGE_HH
#define MV_SVR_MESSAGE_HH

#include <string>         /* string */
#include <vector>         /* vector */
#include <mq/mqueue.h>    /* mv_mqueue_t */

namespace mv { namespace server {

/**
 * @class Message
 */
class Message {
public:
  enum MessageTag {
    DeviceAdd,         /* DEVADD dev src */
    DeviceRemove,      /* DEVREM dev src */
    TagAdd,            /* TAGADD tag src */
    TagRemove,         /* TAGREM tag src */
    DeviceAddTag,      /* DADDTG dev tag src */
    DeviceRemoveTag,   /* DREMTG dev tag src */
    DeviceSignOn,      /* DEVON  dev addr src */
    DeviceSignOff,     /* DEVOFF dev addr src */
    DeviceGetAddr,     /* DEVADR dev src */
    MessageNTags
  };

  enum MessageType {
    Request,
    Reply
  };

public:
  Message(const std::string& str);
  ~Message();

  const MessageTag getTag() const { return _tag; }

  const std::string& getArgAt(size_t i);

private:
  MessageTag _tag;

  std::vector<std::string> _args;
  size_t _nargs;
};


/**
 * @class MessageFactory
 */
class MessageFactory {
public:
  static Message *build(const std::string& str);
};

}} /* mv::server */

#endif /* MV_SVR_MESSAGE_HH */
