#ifndef USB_MIDI_PORT_NOTIFIER_H
#define USB_MIDI_PORT_NOTIFIER_H

#include <functional>
#include <vector>
#include "RtMidiAdaptTypes.h"

namespace midi
{

template<class PortListProvider>
class PortNotifier
{
public:
    PortNotifier(PortListProvider& rPortListProvider);
    using NewPortCb = std::function<void(rtmidiadapt::PortIndex, const rtmidiadapt::DeviceOnPort&)>;
    using RemovedPortCb = std::function<void(const rtmidiadapt::DeviceOnPort&)>;
    void registerNewPortCb(NewPortCb cb);
    void registerRemovedPortCb(RemovedPortCb cb);
    bool update();
private:
    PortListProvider& m_rPortListProvider;
    PortInfoSet       m_lastPorts;
    std::vector<NewPortCb>     m_newPortCbs;
    std::vector<RemovedPortCb> m_removedPortCbs;
};

} // namespace midi

#include "UsbMidiPortNotifier.tcc"

#endif