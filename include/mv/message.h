/**
 * @file message.h
 *
 * @brief Interface to messages which is transferred between devices. A
 * message is a string (or an array of chars) in JSON format. 
 */
#ifndef MV_MESSAGE_H
#define MV_MESSAGE_H

#include <common/defs.h>   /* mv_uint64_t */
#include <mv/value.h>      /* mv_value_t */

/* A message transferred between devices is a JSON message. Such messages
   represent either service requests or replies to requests. An example 
   of such message is given below.
     
    {
      "tag": "FUNC_CALL",
      "arg": {
        "name": "foo",
        "funarg" : [ 10 12 ]
      },
      "src": {
        "dev": "myiphone001",
        "addr": "tcp://152.38.9.193:5555"
      }
    }

    An mv_message_t message contains the parsed information about the given
    JSON string. 

    To add a new device-to-device service protocol, add a new message tag
    and add handlers inside the runtime.
*/
typedef enum {
  /* events */
  MV_MESSAGE_EVENT_ADD,          /* add event */
  MV_MESSAGE_EVENT_DEL,          /* delete event */
  MV_MESSAGE_EVENT_CHK,          /* check if given event exists */

  MV_MESSAGE_EVENT_SUB,          /* subscribe to event */
  MV_MESSAGE_EVENT_UNSUB,        /* unsubscribe from event */
  MV_MESSAGE_EVENT_OCCUR,        /* notify the event ocurrence */

  /* properties */
  MV_MESSAGE_PROP_ADD,           /* add property */
  MV_MESSAGE_PROP_DEL,           /* delete property */
  MV_MESSAGE_PROP_CHK,           /* check if given property exists */

  MV_MESSAGE_PROP_GET,           /* get the given property */
  MV_MESSAGE_PROP_SET,           /* set the given property */
  
  /* funtions */
  MV_MESSAGE_FUNC_ADD,           /* add function */
  MV_MESSAGE_FUNC_DEL,           /* delete function */
  MV_MESSAGE_FUNC_CHK,           /* check if given function exists */

  MV_MESSAGE_FUNC_CALL,          /* call function: don't need return val  */
  MV_MESSAGE_FUNC_CALL_RET,      /* call function: need return val */

  /* reactors */
  MV_MESSAGE_REACT_ADD,          /* add reactor */
  MV_MESSAGE_REACT_DEL,          /* delete reactor */
  MV_MESSAGE_REACT_CHK,          /* check if given reactor exists */

  /* reply */
  MV_MESSAGE_REPLY,              /* reply to a message */

  MV_MESSAGE_NTAGS
} mv_mtag_t;


/* An mv_message_t is an internal representation of JSON message which 
   consists of three fields: tag, arg, src, each of which correponds to
   a value in the JSON object. */
typedef struct {
  mv_mtag_t tag;                   /* message tag: value for "tag" key */
  mv_value_t arg;                  /* value for "arg" key */
  mv_value_t src;                  /* value for "src" key */
} mv_message_t;


/* Creates a mv_message_t data by parsing the given JSON string. Upon 
   faliure, NULL is returned. */
extern mv_message_t *mv_message_parse(char *s);

/* Deletes the given message. */
extern void mv_message_delete(mv_message_t *m);

/* Returns the string for the given message tag. For example, for 
   MV_MESSAGE_FUNC_CALL, "FUNC_CALL" will be returned. Returns NULL on
   failure. */
extern const char *mv_message_tagstr(mv_mtag_t tag);

/* Prints the message for debugging purpose. */
extern void mv_message_print(mv_message_t *m);

#endif /* MV_MESSAGE_H */