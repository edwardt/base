/**
 * @file svr_device.hh
 *
 * @brief Interface to device objects which abstracts devices and their
 *        capabilities. Also, includes a cache which stores device objects
 *        in server memory, which mirrors the device data stored in a
 *        persistent storage.
 *
 * @note  Should we just use redis, or would manually-built server better 
 *        serve our purpose? Or mixture of two?
 */
#ifndef MV_SVR_DEVICE_HH
#define MV_SVR_DEVICE_HH

#include <string>
#include <set>

namespace mv { namespace server {

/**
 * @class DeviceTag
 *
 * @brief End devices subscribe device tags (not actual devices) for the
 *        discovery service -- i.e. a device tags is a unit of subscription.
 */
class Device;
class DeviceTag {
public:
  DeviceTag(const std::string& name);
  ~DeviceTag();

  void addDevice(const std::string& name);
  void removeDevice(const std::string& name);

  const std::set<std::string>& getDevices() { return _devset; }

private:
  std::string _name;
  std::set<std::string> _devset;
};

/**
 * @class Device
 */
class Device {
public:
  Device(const std::string& name);
  ~Device();
  
public:
  const std::string& getName() const { return _name; }

  const std::string& getAddr() const { return _addr; }
  void setAddr(const std::string& addr) { 
    setDirty(true);
    _addr = addr; 
  }

  const inline bool isOnline() const { return _online ? true : false; }
  inline void setOnline(bool b) { _online = b ? 1 : 0; }
  
  const inline bool isDirty() const { return _dirty ? true : false; }
  inline void setDirty(bool b) { _dirty = b ? 1 : 0; }

  int addTag(const std::string& tag);
  int removeTag(const std::string& tag);
  
private:
  std::string _name;
  std::string _addr;
  std::set<std::string> _tags;

  unsigned _online : 1;     /* device is on line */
  unsigned _dirty  : 1;     /* device info is updated */
  unsigned _pad    : 31;
};


/**
 * @class DeviceManager
 */
class DeviceManager {
public:
  static DeviceManager *getInstance();

  /* devices */
  virtual Device *lookup(const std::string& name) const = 0;
  virtual Device *signon(const std::string& name, const std::string& addr) = 0;
  virtual int signoff(const std::string& name) = 0;

  virtual int registerDevice(const std::string &name) = 0;
  virtual int deregisterDevice(const std::string& name) = 0;

  /* device tags */
  virtual DeviceTag *getDeviceTag(const std::string& tag) const = 0;
  virtual int registerDeviceTag(const std::string& tag) = 0;
  virtual int deregisterDeviceTag(const std::string& tag) = 0;
  

protected:
  DeviceManager();
  virtual ~DeviceManager();
};

}} /* mv::server */


#endif /* MV_SVR_DEVICE_HH */

