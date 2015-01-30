/**
 * @file sendrecv.c
 *
 * @brief Implementation of mv_message_send and mv_message_recv functions.
 */
#include <mv/message.h>

#define USE_ZEROMQ

#if defined(USE_SOCK)
#  include "sendrecv_sock.c"
#elif defined(USE_ZEROMQ)
#  include "sendrecv_zmq.c"
#else
#  error Only ZeroMQ-based implementation is available.
#endif
