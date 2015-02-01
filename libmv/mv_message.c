/**
 * @file mv_message.c
 */
#include <stdio.h>       /* printf */
#include <stdlib.h>      /* malloc */
#include <string.h>      /* strcpy, strdup */
#include <strings.h>     /* bzero */
#include <assert.h>      /* assert */
#include <mv/value.h>    /* mv_str_to_value */
#include <mv/message.h>


static mv_message_t *_message_new();
static mv_message_t *_message_build(mv_value_t value, char *s);
static int _message_gettag(char *tag_s);
static const char *_message_tagstr(int tag);

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

const char *_message_tagstr(int tag)
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
  printf("VVV\n");
  mv_value_print(value);
  printf("VVV\n");

  mv_value_t tagkey = mv_value_string("tag");
  mv_value_t tagval = mv_value_map_lookup(value, tagkey);
  if (mv_value_is_null(tagval)) {
    fprintf(stdout, "Message does not contain \"tag\" field:\n%s\n", s);
    return NULL;
  }

  mv_value_t argkey = mv_value_string("arg");
  mv_value_t argval = mv_value_map_lookup(value, argkey);
  if (mv_value_is_null(argval)) {
    fprintf(stdout, "Message does not contain \"arg\" field:\n%s\n", s);
    return NULL;
  }

  mv_value_t srckey = mv_value_string("src");
  mv_value_t srcval = mv_value_map_lookup(value, srckey);
  if (mv_value_is_null(srcval)) {
    fprintf(stdout, "Message does not contain \"src\" field:\n%s\n", s);
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
  mv_value_t value = mv_value_from_str(s);
  if (value == 0) {
    fprintf(stderr, "Failed to parse message: %s\n", s);
    return NULL;
  }

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
  return _message_tagstr(tag);
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
