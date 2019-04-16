#ifndef USB_2_DEVICE_MAPPER_H
#define USB_2_DEVICE_MAPPER_H

#include "RtMidiAdaptTypes.h"
#include "ArduinoJson-v5.13.4.h"
#include "DeviceTypes.h"

namespace midi
{

class DeviceMappingConfigParser
{
public:
    bool init(const std::string& jsonFile);
    bool getDeviceByPort(const rtmidiadapt::DeviceOnPort& deviceOnPort,
                         DeviceId& deviceId) const;
    bool getDevice(const rtmidiadapt::DeviceName& usbMidiDeviceId,
                   DeviceId& deviceId) const;
private:
    StaticJsonBuffer<2000>* m_pJsonBuffer;
    JsonObject*             m_pRoot;
};

class Usb2DeviceMapper
{
public:
    bool init();
    bool map2Device(const rtmidiadapt::DeviceOnPort& deviceOnPort,
                    DeviceId& deviceId) const;
private:
    DeviceMappingConfigParser  m_localConfigParser;
    DeviceMappingConfigParser  m_baseConfigParser;
};

}

#endif