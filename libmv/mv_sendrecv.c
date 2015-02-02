/**
 * @file mv_sendrecv.c
 *
 * @brief Implementation of mv_message_send and mv_message_recv functions.
 */

/* 
#define USE_SOCKTCP
#define USE_ZEROMQ
*/
#define USE_SOCKTCP

#if defined(USE_SOCKTCP)
#  include "mv_sendrecv_sock.c"
#elif defined(USE_ZEROMQ)
#  include "mv_sendrecv_zmq.c"
#else
#  error Only ZeroMQ-based implementation is available.
#endif
