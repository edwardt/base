/**
 * @file mv_netutil.h
 *
 * @brief Utilities for network programming. Many of the functions are
 * from "Unix Network Programming, Vol. 1. 3ed" by R. Stevens et al.
 */
#ifndef MV_NETUTIL_H
#define MV_NETUTIL_H

#include <sys/types.h>     /* sockaddr */
#include <sys/socket.h>    /* sockaddr */

#define SA  struct sockaddr



/* Reads a message from the socket until no more bytes available. The buf
   argument will point to the message string. The caller must not free 
   this message string buffer. Returns the number of bytes read on success;
   returns -1 on failure. */
ssize_t mv_readmsg(int fd, char **buf);

/* Writes a message to the socket. */
ssize_t mv_writemsg(int fd, const char *buf);


#endif /* MV_VALUE_H */
