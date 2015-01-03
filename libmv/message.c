/**
 * @file message.c
 */
#include <stdio.h>       /* printf */
#include <stdlib.h>      /* malloc */
#include <string.h>      /* strcpy, strdup */
#include <strings.h>     /* bzero */
#include <assert.h>      /* assert */
#include <jsmn.h>        /* jsmn_parse */
#include <mv/message.h>


typedef struct _parsedata {
  char *msg;         /* JSON message */
  int len;           /* JSON message length */
  jsmntok_t *toks;   /* array of tokens generated by JSMN */
  jsmntok_t *ctok;   /* current token */
  
} _parsedata_t;

static mv_message_t *_message_new();
static mv_value_t _message_parse(char *s);
static mv_message_t *_message_build(mv_value_t value, char *s);
static int _message_gettag(char *tag_s);

static mv_value_t _parse_token(_parsedata_t *data);
static mv_value_t _parse_token_object(_parsedata_t *data);
static mv_value_t _parse_token_array(_parsedata_t *data);
static mv_value_t _parse_token_string(_parsedata_t *data);
static mv_value_t _parse_token_primitive(_parsedata_t *data);
static int _parse_tokenize(char *s, _parsedata_t *data);



static unsigned long long _mid = 0;
mv_message_t *_message_new()
{
  mv_message_t *msg = malloc(sizeof(mv_message_t));
  if (!msg)
    return NULL;
  
  msg->tag = MV_MESSAGE_NTAGS;
  msg->arg = mv_value_null();
  msg->src = mv_value_null();

  return msg;
}

mv_value_t _message_parse(char *s) 
{
  _parsedata_t data;
  int toksz;

  if (_parse_tokenize(s, &data) == -1) {
    fprintf(stderr, "Failed in _parse_tokenize.\n");
    assert(0);
    return 0;
  }

  return _parse_token(&data);
}

int _parse_tokenize(char *s, _parsedata_t *data)
{
  jsmn_parser parser;           /* parser */
  unsigned int maxtokens;       /* max number of tokens */
  int ret;                      /* return value from jsmn_parse */

  jsmn_init(&parser);

  maxtokens = 64;
  data->msg = s;
  data->len = strlen(s);
  data->toks = malloc(sizeof(jsmntok_t) * maxtokens);
  do {
    maxtokens = maxtokens * 2 + 1;
    if (maxtokens > (1 << 16))
      break;

    data->toks = realloc(data->toks, sizeof(jsmntok_t) * maxtokens);
    ret = jsmn_parse(&parser, s, strlen(s), data->toks, maxtokens);
  } while (ret == JSMN_ERROR_NOMEM);

  switch (ret) {
  case JSMN_ERROR_INVAL: /* bad token, JSON string is corrupted */
  case JSMN_ERROR_NOMEM: /* not enough tokens, JSON string is too large */
  case JSMN_ERROR_PART:  /* JSON string too short, expecting more JSON data */
    return -1;
  default:
    break;
  }

  data->ctok = data->toks;

  return 0;
}

mv_value_t _parse_token(_parsedata_t *data)
{
  mv_value_t retval = 0;

#if 0
  char tokstr[1024];
  int toksz;
  int tokint;
  jsmntok_t *tok;
  tok = data->ctok;
  toksz = tok->end - tok->start;
  strncpy(tokstr, data->msg + tok->start, toksz);
  tokstr[toksz] = '\0';
  printf("_parse_token[%d:%d]: %s\n", tok->start, tok->end, tokstr);
#endif

  switch (data->ctok->type) {
  case JSMN_PRIMITIVE:
    retval = _parse_token_primitive(data);
    break;
  case JSMN_STRING:
    retval = _parse_token_string(data);
    break;
  case JSMN_ARRAY:
    retval = _parse_token_array(data);
    break;
  case JSMN_OBJECT:
    retval = _parse_token_object(data);
    break;
  default:
    assert(0);
  }

#if 0
  mv_value_print(retval);
#endif

  return retval;
}

mv_value_t _parse_token_primitive(_parsedata_t *data)
{
  jsmntok_t *tok;       /* token */
  char tokstr[1024];    /* token string */
  int toksz;            /* token length */
  int tokint;           /* integer */

  tok = data->ctok;
  toksz = tok->end - tok->start;
  if (toksz <= 0)
    return 0;
  assert(toksz <= 1023);
    
  strncpy(tokstr, data->msg + tok->start, toksz);
  tokstr[toksz] = '\0';

  /* TODO: for now, only support integers */
  sscanf(tokstr, "%d", &tokint);
  mv_value_t value = mv_value_int(tokint);

  return value;
}

mv_value_t _parse_token_string(_parsedata_t *data)
{
  jsmntok_t *tok;       /* token */
  char tokstr[1024];    /* token string */
  int toksz;            /* token length */
  
  tok = data->ctok;
  toksz = tok->end - tok->start;
  if (toksz <= 0)
    return 0;
  assert(toksz <= 1023);
  
  strncpy(tokstr, data->msg + tok->start, toksz);
  tokstr[toksz] = '\0';

  mv_value_t value = mv_value_string(tokstr);
  return value;
}

mv_value_t _parse_token_object(_parsedata_t *data)
{
  mv_value_t map;      /* map */
  mv_value_t key;      /* key */
  mv_value_t value;    /* value */

  map = mv_value_map();

  data->ctok++;
  while (data->ctok && (data->ctok->start < data->ctok->end)) {
    if ((key = _parse_token(data)) == 0)
      return map;

    data->ctok++;
    if ((value = _parse_token(data)) == 0)
      return map;

    if (mv_value_map_add(map, key, value) == mv_value_null()) {
      assert(0);
      return 0;
    }
    data->ctok++;
  } 

  return map;
}

mv_value_t _parse_token_array(_parsedata_t *data)
{
  mv_value_t cons;    /* cons */
  mv_value_t prev;    /* cons */
  mv_value_t value;   /* array element */

  prev = mv_value_null();
  cons = prev;
  data->ctok++;
  while (data->ctok && (data->ctok->start < data->ctok->end)) {
    if ((value = _parse_token(data)) == 0)
      return cons;

    cons = mv_value_cons();
    mv_value_cons_setcar(cons, value);
    mv_value_cons_setcdr(cons, prev);
    prev = cons;
    data->ctok++;
  }

  return cons;
}

static const char *_tagstrs[] = {
  "EVENT_ADD",
  "EVENT_DEL",
  "EVENT_CHK",

  "EVENT_SUB",
  "EVENT_UNSUB",
  "EVENT_OCCUR",

  /* properties */
  "PROP_ADD",
  "PROP_DEL",
  "PROP_CHK",

  "PROP_GET",
  "PROP_SET",
  
  /* funtions */
  "FUNC_ADD",
  "FUNC_DEL",
  "FUNC_CHK",

  "FUNC_CALL",
  "FUNC_CALL_RET",

  /* reactors */
  "REACT_ADD",
  "REACT_DEL",
  "REACT_CHK",

  /* CONTROL */
  "REPLY"

  ""
};

const char *_encode_tag(int tag)
{
  if (tag >= MV_MESSAGE_EVENT_ADD && tag < MV_MESSAGE_NTAGS)
    return _tagstrs[tag];

  return NULL;
}

int _message_gettag(char *tag_s)
{
  /* TODO: make this more efficient */
  int i;
  for (i = 0; i < MV_MESSAGE_NTAGS; i++) {
    if (!strcmp(tag_s, _tagstrs[i]))
      return i;
  }

  return -1;
}

mv_message_t *_message_build(mv_value_t value, char *s)
{
  mv_value_t tagkey = mv_value_string("tag");
  mv_value_t tagval = mv_value_map_lookup(value, tagkey);
  if (mv_value_is_null(tagval)) {
    fprintf(stdout, "Message does contain \"tag\" field:\n%s\n", s);
    return NULL;
  }

  mv_value_t argkey = mv_value_string("arg");
  mv_value_t argval = mv_value_map_lookup(value, argkey);
  if (mv_value_is_null(argval)) {
    fprintf(stdout, "Message does contain \"arg\" field:\n%s\n", s);
    return NULL;
  }

  mv_value_t srckey = mv_value_string("src");
  mv_value_t srcval = mv_value_map_lookup(value, srckey);
  if (mv_value_is_null(srcval)) {
    fprintf(stdout, "Message does contain \"src\" field:\n%s\n", s);
    return NULL;
  }
  
  mv_message_t *msg = _message_new();
  msg->tag = _message_gettag(mv_value_string_get(tagval));
  msg->arg = argval;
  msg->src = srcval;

  return msg;
}


/*
 * Functions for the message API.
 */
mv_message_t *mv_message_parse(char *s)
{
  mv_value_t value = _message_parse(s);
  if (mv_value_is_null(value))
    return NULL;

  mv_value_print(value);
  
  return _message_build(value, s);
}

void mv_message_delete(mv_message_t *m)
{
  if (!m) 
    return;

  free(m);
}

const char *mv_message_tagstr(mv_mtag_t tag)
{
  return _encode_tag(tag);
}

void mv_message_print(mv_message_t *m)
{
  if (!m) {
    fprintf(stdout, "Message is NULL.\n");
    return;
  }

  printf("TAG:");
  mv_value_print(m->tag);

  printf("ARG:");
  mv_value_print(m->arg);

  printf("SRC:");
  mv_value_print(m->src);
}