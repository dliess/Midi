#ifndef USB_MIDI_PORT_NOTIFIER_H
#define USB_MIDI_PORT_NOTIFIER_H

#include <functional>
#include <vector>
#include "RtMidiAdaptTypes.h"

#include "UsbMidiInputPortListProvider.h"
#include "UsbMidiOutputPortListProvider.h"

namespace midi
{

template<class PortListProvider>
class PortNotifier
{
public:
    using NewPortCb = std::function<void(rtmidiadapt::PortIndex, const rtmidiadapt::DeviceOnPort&)>;
    using RemovedPortCb = std::function<void(const rtmidiadapt::DeviceOnPort&)>;
    bool init();
    void registerNewPortCb(NewPortCb cb);
    void registerRemovedPortCb(RemovedPortCb cb);
    bool update();
private:
    PortListProvider           m_portListProvider;
    PortInfoSet                m_lastPorts;
    std::vector<NewPortCb>     m_newPortCbs;
    std::vector<RemovedPortCb> m_removedPortCbs;
};

struct PortNotifiers
{
    static PortNotifiers& instance()
    {
        static PortNotifiers _portNotifiers;
        return _portNotifiers;
    }
    PortNotifier<InputPortListProvider>  inputs;
    PortNotifier<OutputPortListProvider> outputs;
    bool init()
    {
        return inputs.init() && outputs.init();
    }
    void update()
    {
        inputs.update();
        outputs.update();
    }
private:
    PortNotifiers() = default;
    PortNotifiers(const PortNotifiers&) = delete;
    PortNotifiers& operator=(const PortNotifiers&) = delete;
};

} // namespace midi

#include "UsbMidiPortNotifier.tcc"

#endif