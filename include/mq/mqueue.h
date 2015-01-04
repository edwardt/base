/**
 * @file mqueue.h
 *
 * @brief Interface to input and output message queues, which is the gateway 
 *        to other devices (and the server). All communication with outside
 *        world should happen through putting and getting string messages
 *        into message queues.
 *
 * @note  Currently, mqueue is based on ZMQ. Decouple this from ZMQ and
 *        make it replaceable with other MQs such as MQTT.
 */
#ifndef MV_MQUEUE_H
#define MV_MQUEUE_H

#define DEFAULT_PORT 5557

/* Opaque id for a message queue. */
typedef void mv_mqueue_t;

#ifdef __cplusplus
extern "C" {
#endif


/* Create a new message queue which will send and receive messages 
   to and from outside of the device. If a port is set to 0, DEFAULT_PORT
   will be used for ZMQ REP socket. */
extern mv_mqueue_t *mv_mqueue_init(unsigned port);

/* Delete the message queue. It will stop the message queue and discard 
   all buffered messages. All subsequent calls to mv_mqueue_get or 
   mv_mqueue_put calls over the deleted queue will fail. Returns 0 on 
   sucess and -1 on failure. */
extern int mv_mqueue_delete(mv_mqueue_t *mq);

/* Returns the address of the given mqueue server. This is the address that 
   other devices need to know to talk to the device running the queue. 
   Currently, mqueue is based on ZMQ and the address is a ZMQ address, e.g.
   "tcp://192.168.8.102:5557". */
extern const char *mv_mqueue_addr(mv_mqueue_t *mq);

/* Start/stop running the message queue. Returns 0 on success and -1 
   on failure. */
extern int mv_mqueue_run(mv_mqueue_t *mq);
extern int mv_mqueue_stop(mv_mqueue_t *mq);

/* Blocking put which will send a message to the destination device. The
   format of the message is given below. 

     DEST PAYLOAD

   For example,
   
     tcp://192.168.8.110:5543 { "tag" : "FUNC_CALL", ... }

   MQ will transfer the PAYLOAD to the destination MQ. PAYLOAD can have 
   multiple format depending on the nature of communication. Typical
   device-to-device remote service request (e.g. FUNC_CALL or PROP_GET)
   are stored in a JSON message. */
extern int mv_mqueue_put(mv_mqueue_t *mq, char *s);

/* Blocking put will receive a message from outside. The message
   will be the JSON-MESSAGE sent from another device through mv_mqueue_put. */
extern char *mv_mqueue_get(mv_mqueue_t *mq);

#ifdef __cplusplus
}
#endif

#endif /* MV_MQUEUE_H */
