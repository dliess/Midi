#ifndef _RTMIDI_ADAPT_TYPES_H
#define _RTMIDI_ADAPT_TYPES_H


#include <string>
#include <cstdint>
#include <iostream>


namespace rtmidiadapt
{


using DeviceName = std::string;
using PortName = std::string;

class DeviceOnUsbPort
{
public:
    DeviceOnUsbPort(const std::string& devOnPortStr) :
        m_id(devOnPortStr)
    {}
    bool operator<(const DeviceOnUsbPort& rhs) const
    {
        return m_id < rhs.m_id;
    }
    DeviceName getDeviceName() const
    {
        DeviceName retVal(m_id);
        std::size_t lastSpace = retVal.rfind(' ');
        if(lastSpace != std::string::npos)
        {
            retVal.erase(lastSpace, std::string::npos);
        }
        return retVal;
    }
    PortName getUsbPortName() const
    {
        PortName retVal(m_id);
        std::size_t lastSpace = retVal.rfind(' ');
        if(lastSpace != std::string::npos)
        {
            retVal.erase(0, lastSpace + 1);
        }
        return retVal;
    }
    std::string toStr() const { return m_id; }
private:
    friend std::ostream& operator<<(std::ostream& ostr, const DeviceOnUsbPort& devOnUsbPort);
    std::string m_id;
};

inline
std::ostream& operator<<(std::ostream& ostr, const DeviceOnUsbPort& devOnUsbPort)
{
    ostr << devOnUsbPort.m_id;
    return ostr;
}

using PortIndex = uint32_t;

}

#endif // _RTMIDI_ADAPT_TYPES_H