/**
 * @file epl_parser_server.cpp
 * @brief 
 *
 * @author cjeong
 */
#include "mv/common.h"
#include "mv/epl/epl_parser.h"
#include "mv/epl/epl_parser_driver.h"

namespace mv { namespace epl {

/**
 * @class ParserServer
 */
class ParserServer {
public:
  ParserServer() {
    _driver = new ParserDriver();
  }

  ~ParserServer() {
    delete _driver;
  }

  Module *parse(SourceTag source, const std::string& str) {
    Module *module = _driver->parse(source, str);
    return module;
  }

private:
  ParserDriver *_driver;
};

/**
 * @class ParserServerMgr
 */
class ParserServerMgr {
public:
  static const size_t DEFAULT_POOL_SIZE = 8;

public:
  static ParserServerMgr *getInstance();
  static void shutdown();

  ParserServer *acquire() {
    if (_free.empty())
      return NULL;

    ParserServer *server = _free.front();
    _free.pop_front();

    return server;
  }

  void release(ParserServer *server) {
    _used.remove(server);
    _free.push_back(server);
  }

private:
  ParserServerMgr(size_t poolsize = DEFAULT_POOL_SIZE) : _poolsize(poolsize) {
    for (size_t i = 0; i < _poolsize; ++i) {
      ParserServer *server = new ParserServer();
      _free.push_back(server);
    }
  }

  ~ParserServerMgr() {
    assert(_free.size() == _poolsize && "Some parser servers not freed");
  }

private:
  static ParserServerMgr *_instance;

private:
  std::list<ParserServer *> _free;
  std::list<ParserServer *> _used;

  size_t _poolsize;
};

ParserServerMgr *ParserServerMgr::_instance = NULL;
ParserServerMgr *ParserServerMgr::getInstance() 
{
  if (_instance == NULL) {
    _instance = new ParserServerMgr();
  }
  return _instance;
}

void ParserServerMgr::shutdown()
{
  delete _instance;
  _instance = NULL;
}

/*
 * Parser
 */
Module *Parser::parse(SourceTag tag, const std::string& str)
{
  ParserServerMgr *mgr = ParserServerMgr::getInstance();
  ParserServer *server = mgr->acquire();
  if (!server)
    return NULL;

  Module *module = server->parse(tag, str);

  mgr->release(server);

  return module;
}

}} /* mv::epl */
