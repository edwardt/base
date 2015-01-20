/**
 * @file mvc_value.hh
 *
 * @brief Interface to values which represent compile-time objects such
 * as functions, properties, etc.
 */
#ifndef MVC_VALUE_HH
#define MVC_VALUE_HH

namespace mvc {

enum ValueTag {
  VT_FUNC,
  VT_PROP,
  VT_EVENT,
  VT_RECATOR,
  VT_NTAGS       /* number of tags */
};

/**
 * @class Value
 */
class Value {
public:
  Value() { }
  ~Value() { }
};

} /* mvc */

#endif /* MVC_VALUE_HH */
