/**
 * @file message.h
 *
 * @brief Interface to messages which is transferred between VMs. A
 * message is a string (or an array of chars) in JSON format. 
 */
#ifndef MVRT_MESSAGE_H
#define MVRT_MESSAGE_H

#include <common/defs.h>   /* mv_uint64_t */
#include "rtvalue.h"       /* mvrt_value_t */

/* A message sent over MQ is a JSON message. An example of such message 
   is given below.
     
    {
      "tag": "FUNC_CALL",
      "args": {
        "name": "battery_low",
        "funarg" : [ 10 12 ]
      },
      "src": {
        "dev": "plato"
        "addr": "tcp:://127.0.0.1:5525",
        "retid": 3456
      }
    }

    A mvrt_message_t message contains the parsed information about the given
    JSON string. 

    To add a new device-to-device service protocol, add a new message tag
    nad add handlers inside the runtime.
*/
typedef enum {
  MVRT_MESSAGE_INVALID      = 0,

  /* events */
  MVRT_MESSAGE_EVENT_ADD,
  MVRT_MESSAGE_EVENT_DEL,
  MVRT_MESSAGE_EVENT_CHK,

  MVRT_MESSAGE_EVENT_SUB,
  MVRT_MESSAGE_EVENT_UNSUB,
  MVRT_MESSAGE_EVENT_OCCUR,

  /* properties */
  MVRT_MESSAGE_PROP_ADD,
  MVRT_MESSAGE_PROP_DEL,
  MVRT_MESSAGE_PROP_CHK,

  MVRT_MESSAGE_PROP_GET,
  MVRT_MESSAGE_PROP_SET,
  
  /* funtions */
  MVRT_MESSAGE_FUNC_ADD,
  MVRT_MESSAGE_FUNC_DEL,
  MVRT_MESSAGE_FUNC_CHK,

  MVRT_MESSAGE_FUNC_CALL,
  MVRT_MESSAGE_FUNC_CALL_NORET,

  /* reactors */
  MVRT_MESSAGE_REACT_ADD,
  MVRT_MESSAGE_REACT_DEL,
  MVRT_MESSAGE_REACT_CHK,

  /* reply */
  MVRT_MESSAGE_REPLY,

  MVRT_MESSAGE_NTAGS
} mvrt_mtag_t;

typedef struct {
  mvrt_mtag_t tag;             /* message tag */
  mvrt_value_t arg;            /* payload (arg field in mvrt_mapval_t) */

  /* information about source device */
  char *srcdev;                /* device name of the source */
  char *srcaddr;               /* address of source */
  mv_uint32_t srcid;           /* return id */
} mvrt_message_t;


/**
 * Creates a message by parsing the given JSON string. Upon faliure, NULL
 * is returned. Otherwise, valid message is returned and the string @s is
 * freed.
 */
extern mvrt_message_t *mvrt_message_parse(char *s);

/**
 * Creates an empty message with the given tag. The tag field will be 
 * initialized to MVRT_MESSAGE_INVLID and other fields of message to 0.
 */
extern mvrt_message_t *mvrt_message_new();

/**
 * Delete the given message.
 */
extern void mvrt_message_delete(mvrt_message_t *msg);

#endif /* MVRT_MESSAGE_H */
