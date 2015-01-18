/**
 * @file reactor.c
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* malloc */
#include <string.h>      /* strstr, strdup */
#include <assert.h>      /* assert */
#include "rtcode.h"      /* mvrt_code_t */
#include "rtevent.h"     /* mvrt_event_lookup */
#include "rtreactor.h"
#include "rtobj.h"


typedef struct _rtreactor {
  mvrt_code_t *code;     /* code */
} _rtreactor_t;


static _rtreactor_t *_rtreactor_new();
static int _rtreactor_delete(_rtreactor_t *p);
static int _rtreactor_token_tag(const char *token);
static int _rtreactor_parse_nargs(int op);
static int _rtreactor_tokenize(char *line, char **name);
static _rtreactor_t *_rtreactor_parse(char *line, FILE *fp, char **name);
static int _reactor_assoc_tokenize(char *line, char **name, char **reactor);

_rtreactor_t *_rtreactor_new()
{
  _rtreactor_t *reactor = malloc(sizeof(_rtreactor_t));
  if (!reactor)
    return NULL;

  reactor->code = NULL;

  return reactor;
}

int _rtreactor_delete(_rtreactor_t *reactor)
{
  if (reactor->code)
    mvrt_code_delete(reactor->code);
  
  free(reactor);
  
  return 0;
}

int _rtreactor_tokenize(char *line, char **name)
{
  char *token;

  /* skip "reactor" */
  if ((token = strtok(line, " \t")) == NULL)
    return -1;

  if(strcmp(token, "reactor"))
    return -1;

  /* get name */
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *name = token;

  return 0;
}

#define MAX_REACTOR_LINE 256
_rtreactor_t *_rtreactor_parse(char *line, FILE *fp, char **name)
{

  /* reactor r0 
     {
       pushi 1
       pushi 2
       add
     } 
  */
  char *token;
  char *type = NULL;
  if (_rtreactor_tokenize(line, name) == -1) {
    fprintf(stderr, "Line not recognized: %s\n", line);
    return NULL;
  }

  mvrt_code_t *code = mvrt_code_load_file(fp);
  if (!code)
    return NULL;
  _rtreactor_t *reactor = _rtreactor_new();
  reactor->code = code;

  return reactor;
}

int _reactor_assoc_tokenize(char *line, char **name, char **reactor)
{
  char *token;

  /* skip "assoc" */
  if ((token = strtok(line, " \t")) == NULL)
    return -1;

  /* get "event" */
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *name = token;

  /* get "reactor" */
  if ((token = strtok(NULL, " \t")) == NULL)
    return -1;
  *reactor = token;

  return 0;
}

/* 
 * Functions for rtreactor API.
 */
mvrt_reactor_t *mvrt_reactor_new(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (obj) {
    fprintf(stderr, "A reactor already exists with name: %s.\n", name);
    return NULL;
  }

  obj = mvrt_obj_new(name, NULL);
  obj->tag = MVRT_OBJ_REACTOR;
  obj->data = (void *) _rtreactor_new();

  return (mvrt_reactor_t *) obj;
}

mvrt_reactor_t *mvrt_reactor_load_str(char *line, FILE *fp)
{
  _rtreactor_t *rtreactor;

  char *name = NULL;
  if ((rtreactor = _rtreactor_parse(line, fp, &name)) == NULL)
    return NULL;

  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (obj) {
    fprintf(stderr, "A reactor already exists with name: %s.\n", name);
    _rtreactor_delete(rtreactor);
    return NULL;
  }

  obj = mvrt_obj_new(name, NULL);
  obj->tag = MVRT_OBJ_REACTOR;
  obj->data = (void *) rtreactor;

  return (mvrt_reactor_t *) obj;
}

char *mvrt_reactor_save_str(mvrt_reactor_t *reactor)
{
  
  return NULL;
}

mvrt_reactor_t *mvrt_reactor_lookup(const char *name)
{
  mvrt_obj_t *obj = mvrt_obj_lookup(name, NULL);
  if (!obj)
    return NULL;

  assert(obj->tag == MVRT_OBJ_REACTOR);

  return (mvrt_reactor_t *) obj;
}

mvrt_code_t *mvrt_reactor_getcode(mvrt_reactor_t *reactor)
{
  mvrt_obj_t *obj = (mvrt_obj_t *) reactor;
  if (!obj)
    return NULL;

  assert(obj->tag == MVRT_OBJ_REACTOR);
  _rtreactor_t *rtreactor = (_rtreactor_t *) obj->data;
  
  return rtreactor->code;
}

/*
 * Functions for event-reactor table.
 */
mvrt_reactor_t *mvrt_reactor_assoc_load_str(char *line)
{
  /* Event-reactor assoc table:

     assoc ev0 reactor0
     assoc ev0 reactor1
     assoc dev:ev1 reactor0
  */
  char *name_s = NULL;
  char *reactor_s = NULL;
  if (_reactor_assoc_tokenize(line, &name_s, &reactor_s) == -1) {
    fprintf(stderr, "Line not recognized: %s\n", line);
    return NULL;
  }

  char *charp;
  mvrt_event_t *ev;
  if (((charp = strstr(name_s, ":")) == NULL) || charp == name_s) {
    /* "foo" or ":foo" - local event */
    if (charp == name_s)
      ev = mvrt_event_lookup(name_s+1, NULL);
    else
      ev = mvrt_event_lookup(name_s, NULL);
  }
  else {
    /* "dev:foo" - remote event */
    char *name_cpy = strdup(charp + 1);
    char save = *charp;
    *charp = '\0';
    ev = mvrt_event_lookup(name_cpy, name_s);
    if (ev == NULL) {
      /* External event is not inside the runtime. Add one. */
      ev = mvrt_event_new(name_cpy, name_s);
    }
    *charp = save;
    free(name_cpy);
  }
  
  if (!ev) {
    fprintf(stderr, "Event not found: %s\n", name_s);
    return NULL;
  }

  mvrt_reactor_t *reactor;
  if ((reactor = mvrt_reactor_lookup(reactor_s)) == NULL) {
    fprintf(stderr, "Reactor not found: %s\n", reactor_s);
    return NULL;
  }
  
  if (mvrt_add_reactor_to_event(ev, reactor) != -1)
    return reactor;
  else
    return NULL;
}


#define MAX_EVENT_REACTOR_TABLE 1001
typedef struct _event_reactor_assoc {
  mvrt_event_t *event;                /* event */
  mvrt_reactor_list_t *head;          /* head of reactor list */
  mvrt_reactor_list_t *tail;          /* tail of reactor list */
  struct _event_reactor_assoc *next;  /* assoc chain, in case of collision */
} _event_reactor_assoc_t;
static _event_reactor_assoc_t *_event_reactor_table[MAX_EVENT_REACTOR_TABLE];

static _event_reactor_assoc_t  *_event_reactor_assoc_new(mvrt_event_t *ev)
{
  _event_reactor_assoc_t *assoc = malloc(sizeof(_event_reactor_assoc_t));
  assoc->event = ev;
  assoc->head = NULL;
  assoc->tail = NULL;
  assoc->next = NULL;
  return assoc;
}

static int _event_reactor_table_hash(mvrt_event_t *ev)
{
  return (((mv_ptr_t) ev) % MAX_EVENT_REACTOR_TABLE);
}

static mvrt_reactor_list_t *_reactor_list_new()
{
  mvrt_reactor_list_t *reactor_list = malloc(sizeof(mvrt_reactor_list_t));
  reactor_list->reactor = NULL;
  reactor_list->next = NULL;

  return reactor_list;
}

int mvrt_add_reactor_to_event(mvrt_event_t *ev, mvrt_reactor_t *react)
{
  int hash = _event_reactor_table_hash(ev);
  _event_reactor_assoc_t *assoc = _event_reactor_table[hash];
  while (assoc) {
    if (assoc->event == ev)
      break; 
    assoc = assoc->next;
  }

  if (assoc) {
    if (assoc->event == ev) {
      /* assoc for the given event found */
      mvrt_reactor_list_t *entry = _reactor_list_new();
      entry->reactor = react;
      if (!assoc->head) {
        assoc->head = entry;
        assoc->tail = entry;
      }
      else {
        assoc->tail->next = entry;
        assoc->tail = entry;
      }
    }
    else {
      /* there are assoc for other events but not for this event */
      _event_reactor_assoc_t *nassoc = _event_reactor_assoc_new(ev);
      mvrt_reactor_list_t *entry = _reactor_list_new();
      entry->reactor = react;
      assoc->head = entry;
      assoc->tail = entry;
      assoc->next = nassoc;
    }
  }
  else {
    /* no assoc at all at this hash address */
    assoc = _event_reactor_assoc_new(ev);
    mvrt_reactor_list_t *entry = _reactor_list_new();
    entry->reactor = react;
    assoc->head = entry;
    assoc->tail = entry;
    _event_reactor_table[hash] = assoc;
  }

  return 0;
}

int mvrt_remove_reactor_from_event(mvrt_event_t *ev, mvrt_reactor_t *react)
{
  return 0;
}

mvrt_reactor_list_t *mvrt_get_reactors_for_event(mvrt_event_t *ev)
{
  int hash = _event_reactor_table_hash(ev);
  _event_reactor_assoc_t *assoc = _event_reactor_table[hash];
  while (assoc) {
    if (assoc->event == ev)
      break; 
    assoc = assoc->next;
  }

  if (assoc) {
    if (assoc->event == ev) {
      /* assoc for the given event found */
      return assoc->head;
    }
    else {
      /* there are assoc for other events but not for this event */
      return NULL;
    }
  }
  else {
    /* no assoc at all at this hash address */
    return NULL;
  }

  return NULL;
}
