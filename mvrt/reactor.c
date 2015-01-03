/**
 * @file reactor.c
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* malloc */
#include <string.h>      /* strdup */
#include <assert.h>      /* assert */
#include "rtprop.h"
#include "rtfunc.h"
#include "reactor.h"


#define MAX_REACTORS 1024
static mvrt_reactor_t *_reactors[MAX_REACTORS];

static mvrt_reactor_t *_reactor_new();
static void _reactor_delete(mvrt_reactor_t *reactor);

static void _reactor_parse_file(const char *file);
static int _reactor_parse_nargs(int operatg);
static int _reactor_token_tag(const char *token);

mvrt_reactor_t *_reactor_new()
{
  mvrt_reactor_t *reactor = malloc(sizeof(mvrt_reactor_t));
  reactor->id = 0;
  reactor->name = NULL;
  reactor->code = NULL;

  return reactor;
}

void _reactor_delete(mvrt_reactor_t *reactor)
{
  free(reactor);
}

enum {
  _TOKEN_INVALID = 0,
  _TOKEN_REACTOR = 1,
  _TOKEN_LPAREN  = 2,
  _TOKEN_RPAREN  = 3,
  _TOKEN_ID      = 4,
  _TOKEN_COMMENT = 5,
  _TOKEN_NTAGS   = 6
};

enum {
  _STATE_EXPECT_REACTOR        = 0,
  _STATE_EXPECT_NAME           = 1,
  _STATE_EXPECT_LPAREN         = 2,
  _STATE_EXPECT_OPER_OR_RPAREN = 3,
  _STATE_EXPECT_ARG            = 4,
  _STATE_NTAGS                 = 5
};

int _reactor_token_tag(const char *token)
{
  int len = strlen(token);
  if (len == 1) {
    if (token[0] == '#')
      return _TOKEN_COMMENT;
    else if (token[0] == '{') 
      return _TOKEN_LPAREN;
    else if (token[0] == '}')
      return _TOKEN_RPAREN;
  }
  else if (len > 0) {
    if (token[0] == '#')
      return _TOKEN_COMMENT;
    else if (!strcmp(token, "reactor"))
      return _TOKEN_REACTOR;
    else
      return _TOKEN_ID;
  }

  return _TOKEN_INVALID;
}

#define MAX_REACTOR_LINE 256
void _reactor_parse_file(const char *file)
{

  FILE *fp = fopen(file, "r");
  if (!fp) {
    fprintf(stdout, "Failed to open -- %s\n", file);
    return;
  }
  
  /* read reactor file of the following format:

       # comment starts with # on 0th column
       reactor r0 {
         pushi 1
         pushi 2
         add
       }
  */
  
  char line[MAX_REACTOR_LINE];
  char *token;
  char *charp;
  int state = _STATE_EXPECT_REACTOR;
  int token_tag;
  int oper_tag;
  int nopers = 0;
  mvrt_reactor_t *reactor = NULL;
  while (fgets(line, MAX_REACTOR_LINE, fp)) {
    if ((charp = strchr(line, '\n')) != NULL)
      *charp = '\0';

    if (strlen(line) == MAX_REACTOR_LINE) {
      fprintf(stderr, "A line in function file, \"%s\", is too long. "
              "Limit line size to < %d.\n", file, MAX_REACTOR_LINE);
      exit(1);
    }

    if (line[0] == '#')
      continue;
    
    if ((token = strtok(line, " \t")) == NULL)
      continue;

    while (token) {
      token_tag = _reactor_token_tag(token);

      if (token_tag == _TOKEN_COMMENT)
        break;
      
      switch (state) {
      case _STATE_EXPECT_REACTOR:
        if (token_tag == _TOKEN_REACTOR) {
          state = _STATE_EXPECT_NAME;
        }
        else {
          fprintf(stderr, "S%d: Parse error at token \"%s\".\n", state, token);
          exit(1);
        }
        break;
      case _STATE_EXPECT_NAME:
        if (token_tag == _TOKEN_ID) {
          // reactor = _reactor_new(token);
          reactor = mvrt_reactor(token);
          reactor->code = mvrt_code_new();
          state = _STATE_EXPECT_LPAREN;
        }
        else {
          fprintf(stderr, "S%d: Parse error at token \"%s\".\n", state, token);
          exit(1);
        }
        break;
      case _STATE_EXPECT_LPAREN:
        if (token_tag == _TOKEN_LPAREN) {
          state = _STATE_EXPECT_OPER_OR_RPAREN;
        }
        else {
          fprintf(stderr, "S%d: Parse error at token \"%s\".\n", state, token);
          exit(1);
        }
        break;
      case _STATE_EXPECT_ARG:
        switch (oper_tag) {
        case MVRT_OP_PUSHS:
          {
            char *arg = strdup(token+1);
            arg[strlen(arg)-1] = '\0';
            fprintf(stdout, "\treactor[%d]: %s \"%s\"\n", nopers,  
                    mvrt_opcode_str(reactor->code->instrs[nopers].opcode), arg);
            reactor->code->instrs[nopers++].ptr = (mv_ptr_t) arg;
          }
          break;
        case MVRT_OP_PUSHI:
        case MVRT_OP_BEQ:
          {
            int arg = atoi(token);
            fprintf(stdout, "\treactor[%d]: %s %d\n", nopers,  
                    mvrt_opcode_str(reactor->code->instrs[nopers].opcode), arg);
            reactor->code->instrs[nopers++].ptr = (mv_ptr_t) arg;
          }
          break;
        default:
          break;
        }
        state = _STATE_EXPECT_OPER_OR_RPAREN;
        break;
      case _STATE_EXPECT_OPER_OR_RPAREN:
        if (token_tag == _TOKEN_ID) {
          if ((oper_tag = mvrt_opcode_tag(token)) == -1) {
            fprintf(stderr, "Invalid operator -- \"%s\"", token);
            exit(1);
          }
          else
            reactor->code->instrs[nopers].opcode = oper_tag;

          if (_reactor_parse_nargs(oper_tag) > 0) {
            state = _STATE_EXPECT_ARG;
          }
          else {
            state = _STATE_EXPECT_OPER_OR_RPAREN;
            fprintf(stdout, "\treactor[%d]: %s\n", nopers,  
                    mvrt_opcode_str(reactor->code->instrs[nopers].opcode));
            nopers++;
          }
        }
        else if (token_tag == _TOKEN_RPAREN) {
          reactor->code->size = nopers;
          state = _STATE_EXPECT_REACTOR;
        }
        else {
          fprintf(stderr, "S%d: Parse error at token \"%s\".\n", state, token);
          exit(1);
        }
        break;
      default:
        assert(0);
        break;
      }
      token = strtok(NULL, " \t");
    }
  }
}

int _reactor_parse_nargs(int opertag)
{
  switch (opertag) {
  case MVRT_OP_PUSHS:
  case MVRT_OP_PUSHI:
  case MVRT_OP_BEQ:
    return 1;
  default:
    break;
  }

  return 0;
}


/*
 * Functions for the reactor API.
 */
static mvrt_reactorid_t _reactor_id;

int mvrt_reactor_module_init(const char *file)
{
  _reactor_id = 0;

  if (file)
    _reactor_parse_file(file);
  fprintf(stdout, "Reactor module initialized...\n"); 

  return 0;
}

mvrt_reactor_t *mvrt_reactor(const char *name)
{
  mvrt_reactor_t *reactor = _reactor_new();
  reactor->id = _reactor_id;
  _reactors[_reactor_id++] = reactor;

  if (_reactor_id >= MAX_REACTORS) {
    fprintf(stderr, "Too many reactors created.\n");
    exit(1);
  }

  if (name)
    reactor->name = strdup(name);

  return reactor;
}

int mvrt_reactor_delete(mvrt_reactor_t *reactor)
{
  _reactor_delete(reactor);
}

mvrt_reactor_t *mvrt_reactor_lookup(const char *name)
{
  int i;
  for (i = 0; i < _reactor_id; i++) {
    mvrt_reactor_t *reactor = _reactors[i];
    if (!strcmp(name, reactor->name))
      return reactor;
  }

  return NULL;
}


/*
 * Functions for event-reactor table.
 */
#define MAX_EVENT_REACTOR_TABLE 1001
typedef struct _event_reactor_assoc {
  mvrt_event_t event;                 /* event */
  mvrt_reactor_list_t *head;          /* head of reactor list */
  mvrt_reactor_list_t *tail;          /* tail of reactor list */
  struct _event_reactor_assoc *next;  /* assoc chain, in case of collision */
} _event_reactor_assoc_t;
static _event_reactor_assoc_t *_event_reactor_table[MAX_EVENT_REACTOR_TABLE];

static _event_reactor_assoc_t  *_event_reactor_assoc_new(mvrt_event_t event)
{
  _event_reactor_assoc_t *assoc = malloc(sizeof(_event_reactor_assoc_t));
  assoc->event = event;
  assoc->head = NULL;
  assoc->tail = NULL;
  assoc->next = NULL;
  return assoc;
}

static int _event_reactor_table_hash(mvrt_event_t ev)
{
  return (ev % MAX_EVENT_REACTOR_TABLE);
}

static mvrt_reactor_list_t *_reactor_list_new()
{
  mvrt_reactor_list_t *reactor_list = malloc(sizeof(mvrt_reactor_list_t));
  reactor_list->reactor = NULL;
  reactor_list->next = NULL;

  return reactor_list;
}

int mvrt_add_reactor_to_event(mvrt_event_t ev, mvrt_reactor_t *react)
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

int mvrt_remove_reactor_from_event(mvrt_event_t ev, mvrt_reactor_t *react)
{
  return 0;
}

mvrt_reactor_list_t *mvrt_get_reactors_for_event(mvrt_event_t ev)
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
