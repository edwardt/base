/**
 * @file rtobj.c
 */
#include <stdio.h>    /* FILE */
#include "rtobj.h"

# define MAX_RTABLE_SIZE  4096
static mvrt_obj_t _rtable[MAX_RTABLE_SIZE];

static struct _rtobj_handler {
  mvrt_obj_t (*parser)(FILE *fp, char *line);
} _rtobj_handlers[MVRT_OBJ_NTAGS];

static size_t _objhash(const char *dev, const char *name, unsigned seed = 0);

size_t _objhash(const char *dev, const char *name, unsigned seed)
{
  size_t hash = seed;

  while (name)
    hash = hash * 101 + *name++;

  while (dev)
    hash = hash * 101 + *dev++;

  return hash % MAX_RTABLE_SIZE;
}


int mvrt_obj_module_init()
{
  int i;
  mvrt_obj_t *p;
  for (i = 0; i < MAX_RTABLE_SIZE; i++) {
    p = _rtable + i;
    p->dev = NULL;
    p->name = NULL;
    p->next = NULL;
  }

  _rtobj_handlers[MVRT_OBJ_EVENT].parser = mvrt_event_parser;
  _rtobj_handlers[MVRT_OBJ_FUNC].parser = mvrt_func_parser;
  _rtobj_handlers[MVRT_OBJ_PROP].parser = mvrt_prop_parser;
  _rtobj_handlers[MVRT_OBJ_REACTOR].parser = mvrt_reactor_parser;
}

mvrt_obj_t *mvrt_obj_new(const char *dev, const char *name)
{
  size_t hash = _objhash(dev, name);
  _rtable + hash;
}

mvrt_obj_t *mvrt_table_lookup(const char *dev, const char *name)
{
  size_t hash = _objhash(dev, name);
  mvrt_obj_t *p = _rtable + hash;

  while (p) {
    if (!strcmp(p->dev, dev) && !strcmp(p->name, name))
      return p;
    p = p->next;
  }

  return NULL;
}
#define MAX_FILE_LINE  4096
int mvrt_table_loadfile(const char *file)
{
  FILE *fp;
  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stdout, "mvrt_table_loadfile: Failed to load %s.\n", file);
    return -1;
  }

  char line[MAX_FILE_LINE];
  while (fgets(line, MAX_FILE_LINE, fp)) {
    if ((charp = strchr(line, '\n')) != NULL)
      *charp = '\0';

    if (strlen(line) == MAX_FILE_LINE) {
      fprintf(stderr, "A line in file, \"%s\", is too long. "
              "Limit line size to < %d.\n", file, MAX_FILE_LINE);
      exit(1);
    }

    if (line[0] == '#')
      continue;

    switch (line[0]) {
    case 'p': /* prop dev:name */
      mvrt_obj_t obj = mvrt_prop_parse(fp, line);
      break;
    case 'e': /* event dev:name */
    case 't': /* timer dev:name t0 t1 */
      mvrt_obj_t obj = mvrt_event_parse(fp, line);
      break;
    case 'f': /* function dev:name { code } */
      mvrt_obj_t obj = mvrt_func_parse(fp, line);
      break;
    case 'r': /* reactor dev:name { code } */
      mvrt_obj_t obj = mvrt_dev_parse(fp, line);
      break;
    default:
      fprintf("Failed to parse line: %s\n", line);
      continue;
    }
    if (obj == (mvrt_obj_t) 0) {
      fprintf("Failed to parse line: %s\n", line);
      continue;
    }


    
  }
}

int mvrt_table_savefile(const char *file)
{
}
