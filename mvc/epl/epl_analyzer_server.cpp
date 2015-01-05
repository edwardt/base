/**
 * @file epl_analyzer_server.cpp
 * @brief 
 *
 * @author cjeong
 */
#include "mv/common.h"
#include "mv/epl/epl_analyzer.h"
#include "mv/epl/epl_analyzer_driver.h"

namespace mv { namespace epl {

/**
 * @class AnalyzerServer
 */
class AnalyzerServer {
public:
  AnalyzerServer() {
    _driver = new AnalyzerDriver();
  }

  ~AnalyzerServer() {
    delete _driver;
  }

  int analyze(Module *module, SymbolTable *symtab) {
    return _driver->analyze(module, symtab);
  }

private:
  AnalyzerDriver *_driver;
};

/**
 * @class AnalyzerServerMgr
 */
class AnalyzerServerMgr {
public:
  static const size_t DEFAULT_POOL_SIZE = 8;

public:
  static AnalyzerServerMgr *getInstance();
  static void shutdown();

  AnalyzerServer *acquire() {
    if (_free.empty())
      return NULL;

    AnalyzerServer *server = _free.front();
    _free.pop_front();

    return server;
  }

  void release(AnalyzerServer *server) {
    _used.remove(server);
    _free.push_back(server);
  }

private:
  AnalyzerServerMgr(size_t poolsize = DEFAULT_POOL_SIZE) : _poolsize(poolsize) {
    for (size_t i = 0; i < _poolsize; ++i) {
      AnalyzerServer *server = new AnalyzerServer();
      _free.push_back(server);
    }
  }

  ~AnalyzerServerMgr() {
    assert(_free.size() == _poolsize && "Some analyzer servers not freed");
  }

private:
  static AnalyzerServerMgr *_instance;

private:
  std::list<AnalyzerServer *> _free;
  std::list<AnalyzerServer *> _used;

  size_t _poolsize;
};

AnalyzerServerMgr *AnalyzerServerMgr::_instance = NULL;
AnalyzerServerMgr *AnalyzerServerMgr::getInstance() 
{
  if (_instance == NULL) {
    _instance = new AnalyzerServerMgr();
  }
  return _instance;
}

void AnalyzerServerMgr::shutdown()
{
  delete _instance;
  _instance = NULL;
}

/*
 * Analyzer
 */
int Analyzer::analyze(Module *module, SymbolTable *symtab)
{
  AnalyzerServerMgr *mgr = AnalyzerServerMgr::getInstance();
  AnalyzerServer *server = mgr->acquire();
  if (!server)
    return -1;

  int result = server->analyze(module, symtab);

  mgr->release(server);

  return result;
}

}} /* mv::epl */
