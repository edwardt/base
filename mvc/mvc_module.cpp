/**
 * @file mvc_module.cpp
 */
#include "mvc_module.h"

namespace mvc {


/**
 * @class ModuleFactoryImpl
 */
class ModuleFactoryImpl : public ModuleFactory {
public:
  ModuleFactoryImpl();
  ~ModuleFactoryImpl();
  
public:
  Module *createModule();
};

ModuleFactoryImpl::ModuleFactoryImpl()
{
}

ModuleFactoryImpl::~ModuleFactoryImpl()
{
}

Module *ModuleFactoryImpl::createModule()
{
  Module *module = new Module();

  return module;
}


/*
 * ModuleFactory
 */
ModuleFactory *ModuleFactory::_instance = NULL;
ModuleFactory *ModuleFactory::getInstance()
{
  if (!_instance)
    _instance = new ModuleFactoryImpl();
  return _instance;
}

ModuleFactory::ModuleFactory()
{
}

ModuleFactory::~ModuleFactory()
{
}

} /* mvc */
