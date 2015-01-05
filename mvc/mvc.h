/**
 * @file mvc.h
 *
 * @brief Interface to services related to MVC Compiler.
 */
#ifndef MVC_H
#define MVC_H

#include <string>

namespace mvc {

enum SourceTag {
  SCT_NONE,
  SCT_FILE,
  SCT_STDIN,
  SCT_STR,
  SCT_NTAGS
};

/**
 * @class Server
 * 
 * @brief Server is responsible for providing services related to 
 * compilation of EPL modules.
 */
class Server {
public:
  /* Send MVC statements to server for compilation. If source is SCT_FILE, 
     str should be the name of the file. If source is SCT_STR, str itself 
     should contain the EPL program. If source is STDIN, str is not used. 

     Returns 0 on success and returns -1 on error. */
  static int compile(SourceTag source, const std::string& str);

private:
  Server();
  ~Server();
};

} /* mvc */

#endif /* MVC_BASE_H */
