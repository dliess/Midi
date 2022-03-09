#ifndef _RTMIDI_ADAPT_TYPES_H
#define _RTMIDI_ADAPT_TYPES_H

#include <charconv>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

namespace rtmidiadapt
{
using DeviceName = std::string;
using PortName   = std::string;

class DeviceOnUsbPort
{
public:
   DeviceOnUsbPort(std::string devOnPortStr) : m_id(std::move(devOnPortStr)) {}
   bool operator<(const DeviceOnUsbPort& rhs) const { return m_id < rhs.m_id; }
   [[nodiscard]] DeviceName getDeviceName() const
   {
      return DeviceName(m_id.substr(0, getPortSeparatorColonPos()));
   }

   [[nodiscard]] PortName getUsbPortName() const
   {
      
      const std::size_t lastSpace = m_id.rfind(' ');
      if(lastSpace == std::string::npos)
      {
          return PortName();
      }
      return m_id.substr(lastSpace + 1, std::string::npos);
   }

   [[nodiscard]] std::string getMidiPort() const noexcept
   {
      const std::size_t lastSpace = m_id.rfind(' ');
      const std::size_t portSeparatorColonPos = getPortSeparatorColonPos();
      return m_id.substr(portSeparatorColonPos + 1, lastSpace - (portSeparatorColonPos + 1));
   }

   [[nodiscard]] std::string getFullMidiPort() const noexcept
   {
      return m_id.substr(0, m_id.rfind(' '));
   }

   [[nodiscard]] std::string toStr() const { return m_id; }

   operator std::string() const { return m_id; }

private:
   friend std::ostream& operator<<(std::ostream& ostr,
                                   const DeviceOnUsbPort& devOnUsbPort);
   [[nodiscard]] size_t getPortSeparatorColonPos() const noexcept {
      std::string_view devName(m_id);
      size_t colonPos = 0;
      while ((colonPos = devName.find(':', colonPos + 1)) !=
             std::string_view::npos)
      {
         if (devName.substr(0, colonPos) ==
             devName.substr(colonPos + 1, colonPos))
         {
            return colonPos;
         }
      }
      return devName.find(':');
   }
   std::string m_id;
};

inline std::ostream& operator<<(std::ostream& ostr,
                                const DeviceOnUsbPort& devOnUsbPort)
{
   ostr << devOnUsbPort.m_id;
   return ostr;
}

using PortIndex = uint32_t;

}   // namespace rtmidiadapt

#endif   // _RTMIDI_ADAPT_TYPES_H