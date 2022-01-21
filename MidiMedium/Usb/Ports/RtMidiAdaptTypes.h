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
      std::string_view devName(m_id);
      size_t colonPos = 0;
      while ((colonPos = devName.find(':', colonPos + 1)) !=
             std::string_view::npos)
      {
         if (devName.substr(0, colonPos) ==
             devName.substr(colonPos + 1, colonPos))
         {
            return DeviceName(devName.substr(0, colonPos));
         }
      }
      return DeviceName(devName.substr(0, devName.find(':')));
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

   [[nodiscard]] int getMidiPort() const noexcept
   {
      const std::size_t lastColon = m_id.rfind(':');
      int result{-1};
      if (lastColon == std::string::npos)
         return result;
      std::from_chars(m_id.data() + lastColon + 1, m_id.data() + m_id.size(),
                      result);
      return result;
   }
   [[nodiscard]] std::string toStr() const { return m_id; }

   operator std::string() const { return m_id; }

private:
   friend std::ostream& operator<<(std::ostream& ostr,
                                   const DeviceOnUsbPort& devOnUsbPort);
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