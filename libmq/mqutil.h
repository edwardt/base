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

extern void mqutil_init();

extern void *mqutil_getsock(const char *addr);

extern const char *mqutil_getaddr();

#ifdef __cplusplus
}
#endif

#endif /* MV_MQUTIL_H */
