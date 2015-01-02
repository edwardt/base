/**
 * @file callback.h
 *
 * @brief Inteface to callback service.
 */
#ifndef MV_CALLBACK_H
#define MV_CALLBACK_H

#include <common/defs.h>    /* mv_uint32_t */

/* Callbacks are usually used when the exact time of invocation of the 
   "callback function" is not known in advance. In MV, remote services are 
   implemented using callbacks -- both in the client side and the server side.

   Consider a remote function call, from inside device CLI, to a function
   defined in device SVR. This can be implemented by adding callbacks to
   both sides as follows.

   A. Protocol setup

      - Define FUNC_CALL protocol, including
        
        a. how messages are defined, how it's organized

        b. assign a prefined CBID numbers, e.g. FUNC_CALL = 10, FUNC_RET = 11

        c. define what CLI and SVR do for REQUEST and REPLY (see B, C below)

        d. SVR: On CBID = FUNC_CALL = 10, add a callback, which implements B,
           to MQ (i.e. which performs callback sequence #2 below)

        e. CLI: On CBID = FUNC_RET = 11, add a callback which implements C,
           to MQ (i.e. which performs return sequence #2 below)

   B. CALLER side (CLI)

      - CALL SEQUENCE #1 (first half)

        a. Build a JSON message and put it to MQ, together with addr of SVR.
           The message should contain tag (FUNC_CALL), arguments, and 
           return id (to be used by callee when sending a return value).

        b. Create a continuation, which contains the following information:

           . reactor/function associated
           . instruction pointer to the code of reactor/function
           . stack (we cannot maintain too many stacks, so limit the number
                    of concurrent, live task calls)

        c. Register the given continuation to contination table, indexed
           by the i) return id and ii) FUNC_RET.

      - RETURN SEQUENCE #2 (second half)

        a. The callback for FUNC_RET should obtain the return id.
           Use the return id to fetch the continuation.

        b. The continuation is supposed to perform the following.

           . Push the return value to the top of stack.
           . Start execution starting from the saved instruction pointer.

           To execute the continuation, there are multiple ways.

           1) Execute them directly.

           2) Schedule the execution context (reactor + IP + stack).

   C. Callee side (SVR)

      - CALL SEQUENCE #2 (second half)

        a. Get the function name and look it up to see if it's a local,
           exported function.

        b.Execute the function with the arguments. There are multiple
          ways to do this.

          1) Directly execute it and perform return sequence #1.
             - Doable for native functions but not easy for MV functions.
               MV function may, in turn, call remote function on the 3rd
               device.

          2) As in MVRT, create an event-based system so that reactor,
             say FuncCallReactor, which performs execution of function 
             will be added to a special event, say EV_FUNC_CALL. 
             AND THEN, generate an EV_FUNC_CALL event with appropriate 
             event payload.
             The reactor will perform the execution of function and cause
             the return sequence #1 to happen (how?).

      - RETURN SEQUENCE #1 (first half)

        a. Build a JSON message and put it to MQ, together with addr of CLI.
           The message should contain tag (FUNC_RET), return value and 
           return id.
*/

/* Opaque id for the callback server. */
typedef void mv_cbserver_t;

/* Callback ID used to identify reply messages. */
typedef mv_uint32_t mv_cbid_t;

/* Callback function type. */
typedef void *(mv_cbfunc_t)(void *);

typedef enum {
  MV_CB_ONETIME,
  MV_CB_RECURRENT
} mv_cbtag_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new callback server. */
mv_cbserver_t *mv_cbserver_init();

/* Adds a callback to the callback server. 
*/
mv_cbid_t mv_callback_add(mv_cbserver_t *cb, mv_cbfunc_t fun, mv_cbtag_t tag);

/* Returns the callback for the given id. Message queue will call this when
   the message is tagged with the given id. */
mv_cbfunc_t *mv_callback_get(mv_cbserver_t *cb, mv_cbid_t id);

#ifdef __cplusplus
}
#endif

#endif /* MV_CALLBACK_H */
