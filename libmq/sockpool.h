/**
 * @file mqutil.h
 *
 * @brief Socket pool for outgoing connections.
 */
#ifndef MV_MQUTIL_H
#define MV_MQUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

void *zmq_getsock(const char *addr);

#ifdef __cplusplus
}
#endif

#endif /* MV_MQUTIL_H */
