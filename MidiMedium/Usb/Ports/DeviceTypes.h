#ifndef DEVICE_TYPES_H
#define DEVICE_TYPES_H

#include <string>

using DeviceId = std::string;

struct Device
{
    enum DeviceType
    {
        eSoundDevice      = 0,
        eControllerDevice
    };
    DeviceType  deviceType;
    std::string deviceName;
};

#endif