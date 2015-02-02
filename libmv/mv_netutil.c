/**
 * @file mv_netutil.c
 */
#include <unistd.h>        /* ssize_t */
#include <errno.h>         /* errno */
#include <string.h>        /* strlen */
#include "mv_netutil.h"

static ssize_t _readn(int fd, void *vptr, size_t n);
static ssize_t _writen(int fd, const void *vptr, size_t n);


/* Reads n bytes from a descriptor. */
ssize_t _readn(int fd, void *vptr, size_t n)
{
  size_t  nleft;
  ssize_t nread;
  char  *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
    if ((nread = read(fd, ptr, nleft)) < 0) {
      if (errno == EINTR) {
        /* call read() again */
        nread = 0;
      }
      else
        return -1;
    } 
    else if (nread == 0) {
      /* EOF */
      break;
    }

    nleft -= nread;
    ptr += nread;
  }

  return (n - nleft);
}

/* Writes n bytes to a descriptor. */
ssize_t _writen(int fd, const void *vptr, size_t n)
{
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = vptr;
  nleft = n;
  while (nleft > 0) {
    if ((nwritten = write(fd, ptr, nleft)) == -1) {
      if (nwritten < 0 && errno == EINTR)
        nwritten = 0;
      else
        return -1;
    }

    nleft -= nwritten;
    ptr += nwritten;
  }

  return n;
}



static size_t _rdbufsz = 1024;
static char _rdbuf[1024];
ssize_t mv_readmsg(int fd, char **buf)
{
  *buf = _rdbuf;

  /* TODO: create a loop to recv arbitrary length message -- do realloc
     if needed */
  return _readn(fd, (void *) _rdbuf, 1024);
}


ssize_t mv_writemsg(int fd, const char *buf)
{
  return _writen(fd, (const void *) buf, strlen(buf));
}
