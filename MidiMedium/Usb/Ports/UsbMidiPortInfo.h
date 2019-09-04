#ifndef USB_MIDI_PORT_INFO_H
#define USB_MIDI_PORT_INFO_H

#include "RtMidiAdaptTypes.h"
#include <set>

namespace midi
{

struct PortInfo
{
    PortInfo(const rtmidiadapt::PortIndex& _portIndex,
             const rtmidiadapt::DeviceOnUsbPort& _deviceOnPort):
        portIndex(_portIndex),
        deviceOnPort(_deviceOnPort)
        {}
    bool operator<(const PortInfo& rhs) const { return deviceOnPort < rhs.deviceOnPort; }
    rtmidiadapt::PortIndex    portIndex;
    rtmidiadapt::DeviceOnUsbPort deviceOnPort;
    struct Less
    {
        bool operator()(const PortInfo& lhs, const PortInfo& rhs) const
        {
            return lhs < rhs;
        }
    };
};

using PortInfoSet = std::set<PortInfo, PortInfo::Less>;

} // namespace midi
#endif
