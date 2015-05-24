/**
 * @file mvc_error.hh
 *
 * @brief Functions for error reporting.
 */
#ifndef MVC_ERROR_HH
#define MVC_ERROR_HH

#include <iostream>
#include <string>

namespace mvc {

#if defined(__gnu_linux__)
#  define MVC_ASSERT(P) \
  Util::stacktrace( (unsigned long) (P) )
#else
#  include <cassert>
#  define MVC_ASSERT(P) assert( (P) )
#endif /* defined(__gnu_linux__) */

typedef enum {
  ERR_ERROR    = 0,
  ERR_WARNING  = 1,
  ERR_INFO     = 2,
  ERR_DEBUG    = 3,
  ERR_ASSERT   = 4,
  ERR_NTAGS
} ErrorTag;

/* Debugging tag, which is used for emitting debug message only related to
   some aspects of code. For example, if you want only debug messages from
   constant propagation code, created a tag DBG_CONSTPROP and call
   debug("constant propgation at file foo.c", 1, DBG_CONSTPROP); */
typedef enum {
  DBG_ALL,     = 0,   /* will be dumped always */
  DBG_PARSE,   = 0,   /* parser related debug messages */
  DBG_NTAGS

}
class Report {

  static int setOutput(std::ostream& os);

  static void enable(ErrorTag tag);
  static void disable(ErrorTag tag);
  
  /* Emits an error/warning message and increment the error/warning count. */
  static void error(const std::string& s);
  static void warn(const std::string& s);
  
  static void getCount(ErrorTag tag);

  /* prints strerror(errcode) */
  static void perror(int errcode, const std::string& s);


  /* For debugging. 0 is for no debugging info. A debug message with level n
     will be emitted only if debug level >= n. Put more important debugging 
     info */
  void setDebugLevel(unsigned v);
  void setDebugTag(unsigned tag);
  void unsetDebugTag(unsigned tag);
  void debug(const std::string& s, unsigned level = 1, unsigned tag = 0);
};

} /* mvc */

#endif /* MVC_ERROR_HH */
