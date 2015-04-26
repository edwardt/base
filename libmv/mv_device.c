/**
 * @file mv_device.c
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* free, exit */
#include <string.h>      /* strdup */
#include <assert.h>      /* assert */
#include <mv/device.h>   /* mv_deviceid_t */


#define MAX_DEVICE_TABLE 4096
typedef struct _device {
  char *name;               /* globally-unique name */
  mv_addr_t addr;           /* transport addr */

  unsigned free     : 1;    /* free or not */
  unsigned free_idx : 12;   /* index to free list */
  unsigned free_nxt : 12;   /* index of next free entry */
  unsigned pad      : 7;
} _device_t;
static _device_t _device_table[MAX_DEVICE_TABLE];
static _device_t *_free_device;

static void _device_table_init();
static _device_t *_device_get_free();
static int _device_delete(_device_t *device);
static _device_t *_device_lookup(const char *name);
static int _device_tokenize(char *line, char **dev, char **addr);

void _device_table_init()
{
  int i;
  _device_t *dev;
  for (i = 0; i < MAX_DEVICE_TABLE; i++) {
    dev = _device_table + i;
    dev->name = NULL;
    dev->addr = 0;

    dev->free = 1;
    dev->free_idx = i;
    dev->free_nxt = i + 1;
  }
  /* the last element */
  dev->free_idx = i;
  dev->free_nxt = 0;

  /* The first device, _device_table[0], is reserved to represent "no more
     device". If free_nxt == 0, it means no more free device is available. */
  _free_device = _device_table + 1;
  _device_table[0].free = 0;
}

_device_t *_device_get_free()
{
  if (_free_device == _device_table)
    return NULL;

  _device_t *dev = _free_device;
  dev->free = 0;
  _free_device = _device_table + dev->free_nxt;

  return dev;
}

int _device_delete(_device_t *dev)
{
  dev->free = 1;
  dev->free_nxt = dev->free_idx;
  _free_device = dev;

  return 0;
}

_device_t *_device_lookup(const char *name)
{
  /* TODO: hash table */
  int i;
  _device_t *dev;
  for (i = 1; i < MAX_DEVICE_TABLE; i++) {
    dev = _device_table + i;
    if (!dev->free && !strcmp(dev->name, name))
      return dev;
  }

  return NULL;
}

int _device_tokenize(char *line, char **dev, char **addr)
{
  char *token;
  if ((token = strtok(line, " \t")) == NULL)
    return -1;
  *dev = token;
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *addr = token;
  return 0;
}


/*
 * Functions for the device API.
 */
int mv_device_module_init(const char *file)
{
  _device_table_init();

  /* For now, to emulate the device server, read a device-address
     table from a file. */
  FILE *fp;
  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stderr, "Failed to open %s\n", file);
    return -1;
  }

  char line[1024];
  char *name;
  char *addr;
  _device_t *mvdev;
  while (fgets(line, 1024, fp)) {
    char *charp = strstr(line, "\n");
    if (charp)
      *charp = '\0';
    _device_tokenize(line, &name, &addr);
    
    mvdev = _device_get_free(name);
    if (!mvdev)
      continue;
    mvdev->name = strdup(name);
    mvdev->addr = mv_addr(addr);
  }

  fprintf(stdout, "Device address lookup service initiated...\n");
  return 0;
}

mv_device_t mv_device_register(const char *name)
{
  /* 
     TODO: Later, connect to the "server" to register the device globally.
     Note that registration itself is implemented using a message sent to 
     the server.

     e.g. mv_mqueue_put("DEVADD name myname");
  */

  /*
  _device_t *dev = _device_get_free();
  if (!dev) {
    fprintf(stderr, "mv_device_register: Failed to register - %s\n", name);
    return -1;
  }

  if (dev->name) 
    free(dev->name);
  if (dev->addr)
    mv_addr_delete(dev->addr);
  dev->name = strdup(name);
  dev->addr = 0;

  fprintf(stdout, "Device registered: %s\n", dev->name);
  */
  
  return 0;
}

int mv_device_deregister(mv_device_t dev)
{
  if (dev == 0)
    return -1;
  
  _device_t *pdev = (_device_t *) dev;

  return _device_delete(pdev);
}

_device_t *_self = 0;
mv_device_t mv_device_signon(const char *name, mv_addr_t addr)
{
  _device_t *pdev = _device_lookup(name);
  if (!pdev) {
    fprintf(stderr, "mv_device_signon: Failed to sign on - %s\n", name);
    return -1;
  }

  if (addr == 0) {
    fprintf(stderr, "mv_device_signon: Invalid transport address.\n");
    return -1;
  }
  pdev->addr = addr;

  _self = pdev;

  return (mv_device_t) pdev;
}

int mv_device_signoff(mv_device_t dev)
{
  if (dev == 0)
    return -1;

  _device_t *pdev = (_device_t *) dev;
  if (pdev != _self) {
    fprintf(stderr, "mv_device_signoff: Failed - %s\n", pdev->name);
    return -1;
  }

  mv_addr_delete(pdev->addr);
  pdev->addr = 0;

  _self = 0;

  return 0;
}

mv_device_t mv_device_self()
{
  return (mv_device_t) _self;
}

mv_device_t mv_device_lookup(const char *name)
{
  return (mv_device_t) _device_lookup(name);
}

const char *mv_device_name(mv_device_t dev)
{
  _device_t *pdev = (_device_t *) dev;

  return pdev->name;
}

mv_addr_t mv_device_addr(mv_device_t dev)
{
  _device_t *pdev = (_device_t *) dev;

  return pdev->addr;
}


