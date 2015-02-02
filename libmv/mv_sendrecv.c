/**
 * @file mv_sendrecv.c
 *
 * @brief Implementation of mv_message_send and mv_message_recv functions.
 */

/* 
   #define USE_ZEROMQ
*/

#if defined(USE_ZEROMQ)
#  include "mv_sendrecv_zmq.c"
#else
#  include "mv_sendrecv_sock.c"
#endif
