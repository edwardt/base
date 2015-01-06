/**
 * @file svr_device.cc
 */
#include <iostream>
#include <map>
#include "svr_device.hh"

namespace mv { namespace server {


/*
 * DeviceTag
 */
DeviceTag::DeviceTag(const std::string& name) : _name(name)
{
}

DeviceTag::~DeviceTag()
{
}

void DeviceTag::addDevice(const std::string& name)
{
  std::set<std::string>::iterator iter;
  if ((iter = _devset.find(name)) == _devset.end())
    _devset.insert(name);
}

void DeviceTag::removeDevice(const std::string& name)
{
  std::set<std::string>::iterator iter;
  if ((iter = _devset.find(name)) != _devset.end())
    _devset.erase(iter);
}

/*
 * Device
 */
Device::Device(const std::string& name) : _name(name), _online(0), _dirty(0)
{
}

Device::~Device()
{
}

int Device::addTag(const std::string& tag)
{
  DeviceManager *mgr = DeviceManager::getInstance();
  DeviceTag *devtag = mgr->getDeviceTag(tag);
  if (!devtag)
    return -1;

  devtag->addDevice(getName());
  if (_tags.find(tag) == _tags.end())
    _tags.insert(tag);
  return 0;
}

int Device::removeTag(const std::string& tag)
{
  DeviceManager *mgr = DeviceManager::getInstance();
  DeviceTag *devtag = mgr->getDeviceTag(tag);
  if (!devtag)
    return -1;

  devtag->removeDevice(getName());
  std::set<std::string>::iterator iter;
  if ((iter = _tags.find(tag)) != _tags.end())
    _tags.insert(tag);
  return 0;
}

/*
 * DeviceManagerImpl
 */
class DeviceManagerImpl : public DeviceManager {
public:
  DeviceManagerImpl() { }
  ~DeviceManagerImpl() { }

  Device *lookup(const std::string& name) const;

  Device *signon(const std::string& name, const std::string& addr);
  int signoff(const std::string& name);

  int registerDevice(const std::string &name);
  int deregisterDevice(const std::string &name);

  DeviceTag *getDeviceTag(const std::string& tag) const;
  int registerDeviceTag(const std::string& tag);
  int deregisterDeviceTag(const std::string& tag);

private:
  typedef std::map<const std::string, Device *> DeviceMapType;
  typedef DeviceMapType::iterator DeviceMapIter;
  typedef DeviceMapType::const_iterator DeviceMapConstIter;

  DeviceMapType _devmap;    /* online devices */
  DeviceMapType _dirtymap;  /* offline but dirty devices; writeback needed */

private:
  typedef std::map<const std::string, DeviceTag *> DeviceTagMapType;
  typedef DeviceTagMapType::iterator DeviceTagMapIter;
  typedef DeviceTagMapType::const_iterator DeviceTagMapConstIter;

  DeviceTagMapType _tagmap; /* device tags */
};


Device *DeviceManagerImpl::lookup(const std::string& name) const
{
  DeviceMapConstIter iter = _devmap.find(name);
  if (iter == _devmap.end())
    return NULL;

  return iter->second;
}

Device *DeviceManagerImpl::signon(const std::string& name, 
                                  const std::string& addr)
{
  Device *dev = lookup(name);
  if (dev) {
    std::cerr << "Device already signed on: " << name << std::endl;
    return NULL;
  }

  DeviceMapIter iter = _dirtymap.find(name);
  if (iter == _dirtymap.end()) {
    dev = new Device(name);
    dev->setAddr(addr);
    dev->setOnline(true);
  }
  else {
    _dirtymap.erase(iter);
    dev = iter->second;
  }
  
  _devmap[name] = dev;

  return dev;
}

int DeviceManagerImpl::signoff(const std::string& name)
{
  DeviceMapIter iter = _devmap.find(name);
  if (iter == _devmap.end()) {
    std::cerr << "Device is not signed on: " << name << std::endl;
    return -1;
  }

  Device *dev = iter->second;
  dev->setOnline(false);
  if (dev->isDirty())
    _dirtymap[dev->getName()] = dev;
  _devmap.erase(iter);

  return 0;
}

int DeviceManagerImpl::registerDevice(const std::string &name)
{
  /* TODO: check DB if device is already registered; then, add to DB */
  
  return 0;
}

int DeviceManagerImpl::deregisterDevice(const std::string &name)
{
  return 0;
}

DeviceTag *DeviceManagerImpl::getDeviceTag(const std::string& tag) const
{
  DeviceTagMapConstIter iter = _tagmap.find(tag);
  if (iter == _tagmap.end())
    return NULL;

  return iter->second;
}

int DeviceManagerImpl::registerDeviceTag(const std::string &tag)
{
  /* TODO: check DB if device tag is already registered; then, add to DB */
  DeviceTag *devtag = new DeviceTag(tag);
  _tagmap[tag] = devtag;

  return 0;
}

int DeviceManagerImpl::deregisterDeviceTag(const std::string &tag)
{
  return 0;
}

/*
 * DeviceManager
 */
static DeviceManagerImpl *_devtab_instance = NULL;
DeviceManager *DeviceManager::getInstance() 
{
  if (!_devtab_instance) {
    _devtab_instance = new DeviceManagerImpl();
  }
  return _devtab_instance;
}

DeviceManager::DeviceManager()
{
}

DeviceManager::~DeviceManager()
{
}

}} /* mv:server */
