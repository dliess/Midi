#include "Usb2DeviceMapper.h"
#include "FileReader.h"

using namespace midi;

bool DeviceMappingConfigParser::init(const std::string& jsonFile)
{
    m_pRoot = &m_pJsonBuffer->parseObject(tools::get_file_contents(jsonFile.c_str()));
    if(!m_pRoot->success())
    {
        std::cerr << "parsing '" << jsonFile << "' failed" << std::endl;
        return false;
    }
    return true;
}

bool DeviceMappingConfigParser::getDeviceByPort(const rtmidiadapt::DeviceOnPort& deviceOnPort,
                                                DeviceId& deviceId) const
{
    deviceId = (*m_pRoot)["devices on usb port"][deviceOnPort.toStr()].as<std::string>();
    return deviceId.size();
}

bool DeviceMappingConfigParser::getDevice(const rtmidiadapt::DeviceName& usbMidiDeviceId,
                                          DeviceId& deviceId) const
{
    deviceId = (*m_pRoot)["devices"][usbMidiDeviceId].as<std::string>();
    return deviceId.size();
}

bool Usb2DeviceMapper::init()
{
    if(!m_localConfigParser.init("config/local/UsbDeviceMapping.json"))
    {
       return false;
    }
    if(!m_baseConfigParser.init("config/base/UsbDeviceMapping.json"))
    {
       return false;
    }
    return true;
}


bool Usb2DeviceMapper::map2Device(const rtmidiadapt::DeviceOnPort& deviceOnPort,
                                  DeviceId& deviceId) const
{
    if(!m_localConfigParser.getDeviceByPort(deviceOnPort, deviceId))
    {
        if(!m_localConfigParser.getDevice(deviceOnPort.getDeviceName(), deviceId))
        {
            if(!m_baseConfigParser.getDevice(deviceOnPort.getDeviceName(), deviceId))
            {
                return false;
            }
        }
    }
    return true;
}