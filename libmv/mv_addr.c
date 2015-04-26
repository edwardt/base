/**
 * @file mv_addr.c
 */
#include <stdlib.h>     /* free */
#include <string.h>     /* strdup */
#include <assert.h>     /* assert */
#include <mv/addr.h>


#define _ADDR_TAG(addr) ((addr) & 0x7)
#define _ADDR_PTR(addr) ((void *) ((addr) & ~((mv_addr_t) 0x7)))
#define _ADDR_TAGPTR(ptr, tag) ((mv_addr_t) (ptr) | (tag))

typedef struct {
  union {
    char *ipaddr;
  } u;
} _addr_t;


mv_addr_t mv_addr(const char *s)
{
  /* ip://192.168.8.14 */
  if (s[0] == 'i' && s[1] == 'p' && s[2] == ':' && s[3] == '/' &&
      s[4] == '/') {
    _addr_t *addr = malloc(sizeof(_addr_t));
    addr->u.ipaddr = strdup(s+5);
    return _ADDR_TAGPTR(addr, MV_TRANSPORT_IPv4);
  }
  else {
    assert(0 && "mv_addr: Not implemented yet");
  }
  
  return (mv_addr_t) 0;
}

int mv_addr_delete(mv_addr_t addr)
{
  _addr_t *adr = (_addr_t *) _ADDR_PTR(addr);
  if (adr)
    free(adr);

  return 0;
}



