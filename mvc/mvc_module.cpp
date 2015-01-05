/**
 * @file mvc_module.cpp
 */
#include "mvc_module.h"

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
