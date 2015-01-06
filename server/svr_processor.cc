/**
 * @file svr_processor.cc
 */
#include <cstdio>            /* perror */
#include <cassert>           /* assert */
#include <pthread.h>         /* pthread_t */
#include "svr_device.hh"     /* DeviceManager */
#include "svr_processor.hh"

namespace mv { namespace server {


/**
 * @class MessageProcessorImpl
 */
class MessageProcessorImpl : public MessageProcessor  {
public:
  MessageProcessorImpl(mv_mqueue_t *mq);
  ~MessageProcessorImpl();

  void run();
  void stop();
  void shutdown();

private:
  static void *process_thread(void *arg);

  static void process(Message *msg);
  static void processDeviceAdd(Message *msg);
  static void processDeviceRemove(Message *msg);
  static void processTagAdd(Message *msg);
  static void processTagRemove(Message *msg);
  static void processDeviceAddTag(Message *msg);
  static void processDeviceRemoveTag(Message *msg);
  static void processDeviceSignOn(Message *msg);
  static void processDeviceSignOff(Message *msg);
  static void processDeviceGetAddr(Message *msg);

private:
  static DeviceManager *_devmgr;
  pthread_t _thr;
};

DeviceManager *MessageProcessorImpl::_devmgr = NULL;
MessageProcessorImpl::MessageProcessorImpl(mv_mqueue_t *mq) 
  : MessageProcessor(mq)
{
  _devmgr = DeviceManager::getInstance();
}

MessageProcessorImpl::~MessageProcessorImpl()
{
}

void MessageProcessorImpl::run()
{
  if (pthread_create(&_thr, NULL, process_thread, (void *) _mq) != 0) {
    perror("pthread_create@MessageProcessorImpl::run");
    assert(0 && "pthread_create failed.");
  }
}

void MessageProcessorImpl::stop()
{
}

void MessageProcessorImpl::shutdown()
{
}

void *MessageProcessorImpl::process_thread(void *arg)
{
  mv_mqueue_t *mq = (mv_mqueue_t *) arg;
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 5000000;
  
  while (1) {
    char *str = mv_mqueue_get(mq);
    if (!str) {
      // sleep for 5ms
      nanosleep(&ts, NULL);
      continue;
    }

    Message *msg = MessageFactory::build(std::string(str));
    process(msg);
  }
}


void MessageProcessorImpl::process(Message *msg)
{
  switch (msg->getTag()) {
  case Message::DeviceAdd:
    processDeviceAdd(msg);
    break;
  case Message::DeviceRemove:
    processDeviceRemove(msg);
    break;
  case Message::TagAdd:
    processTagAdd(msg);
    break;
  case Message::TagRemove:
    processTagRemove(msg);
    break;
  case Message::DeviceAddTag:
    processDeviceAddTag(msg);
    break;
  case Message::DeviceRemoveTag:
    processDeviceRemoveTag(msg);
    break;
  default:
    assert(0 && "Invalid message tag.");
    break;    
  }
}

void MessageProcessorImpl::processDeviceAdd(Message *msg)
{
  /* DEVADD dev */
  const std::string& dev = msg->getArgAt(0);
  _devmgr->registerDevice(dev);
}

void MessageProcessorImpl::processDeviceRemove(Message *msg)
{
  /* DEVREM dev */
  const std::string& dev = msg->getArgAt(0);
  _devmgr->deregisterDevice(dev);
}

void MessageProcessorImpl::processTagAdd(Message *msg)
{
  /* TAGADD tag */
  const std::string& tag = msg->getArgAt(0);
  _devmgr->registerDeviceTag(tag);
}

void MessageProcessorImpl::processTagRemove(Message *msg)
{
  /* TAGREM tag */
  const std::string& tag = msg->getArgAt(0);
  _devmgr->deregisterDeviceTag(tag);
}

void MessageProcessorImpl::processDeviceAddTag(Message *msg)
{
  /* DADDTG dev tag */
  const std::string& dev = msg->getArgAt(0);
  const std::string& tag = msg->getArgAt(1);
  
  Device *device = _devmgr->lookup(dev);
  if (!device)
    return;

  device->addTag(tag);
}

void MessageProcessorImpl::processDeviceRemoveTag(Message *msg)
{
  /* DREMTG dev tag */
  const std::string& dev = msg->getArgAt(0);
  const std::string& tag = msg->getArgAt(1);
  
  Device *device = _devmgr->lookup(dev);
  if (!device)
    return;

  device->removeTag(tag);
}

void MessageProcessorImpl::processDeviceSignOn(Message *msg)
{
  const std::string& dev = msg->getArgAt(0);
  const std::string& addr = msg->getArgAt(1);
  
  Device *device = _devmgr->signon(dev, addr);
  if (!device)
    assert(0);
}

void MessageProcessorImpl::processDeviceSignOff(Message *msg)
{
  const std::string& dev = msg->getArgAt(0);
  const std::string& addr = msg->getArgAt(1);
  
  _devmgr->signoff(dev);
}

void MessageProcessorImpl::processDeviceGetAddr(Message *msg)
{
  const std::string& dev = msg->getArgAt(0);
  const std::string& src = msg->getArgAt(1);

  std::string retval = "0.0.0.0";

  Device *device = _devmgr->lookup(dev);
  if (device and device->isOnline())
    retval = device->getAddr();

  retval += " ";
}


/*
 * MessageProcessor
 */
static MessageProcessorImpl *_mproc_instance = NULL;
MessageProcessor *MessageProcessor::getInstance(mv_mqueue_t *mq) 
{
  if (!_mproc_instance) {
    _mproc_instance = new MessageProcessorImpl(mq);
  }
  return _mproc_instance;
}

MessageProcessor::MessageProcessor(mv_mqueue_t *mq) : _mq(mq)
{
}

MessageProcessor::~MessageProcessor()
{
}

}} /* mv::server */
