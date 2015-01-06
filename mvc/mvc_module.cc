/**
 * @file mvc_module.cc
 */
#include "mvc_module.hh"

namespace mvc {

ModuleFactory *ModuleFactory::_instance = NULL;
ModuleFactory *ModuleFactory::getInstance()
{
  if (!_instance)
    _instance = new ModuleFactory();
  return _instance;
}

ModuleFactory::ModuleFactory()
{
}

ModuleFactory::~ModuleFactory()
{
}

Module *ModuleFactory::createModule()
{
  Module *module = new Module();

  return module;
}

} /* mvc */
