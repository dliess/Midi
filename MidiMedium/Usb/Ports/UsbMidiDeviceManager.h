#ifndef USB_MIDI_DEVICE_MANAGER_H
#define USB_MIDI_DEVICE_MANAGER_H

#include "UsbMidiInputPortListProvider.h"
#include "UsbMidiOutputPortListProvider.h"
#include "UsbMidiPortNotifier.h"
#include "Usb2DeviceMapper.h"
#include "RtMidiAdaptTypes.h"
#include "Midi1Output.h"
#include "UsbMidiOut.h"

#include <vector>
#include <memory>

namespace midi
{

class DeviceManager
{
public:
    DeviceManager();
    bool init();
    void update();
private:
    InputPortListProvider  m_inputPortListProvider;
    OutputPortListProvider m_outputPortListProvider;
    PortNotifier<InputPortListProvider>  m_inputPortListChangeHandler;
    PortNotifier<OutputPortListProvider> m_outputPortListChangeHandler;

    void onNewInputPort(rtmidiadapt::PortIndex i, const rtmidiadapt::DeviceOnPort& devOnPort);
    void onRemovedInputPort(const rtmidiadapt::DeviceOnPort& devOnPort);
    void onNewOutputPort(rtmidiadapt::PortIndex i, const rtmidiadapt::DeviceOnPort& devOnPort);
    void onRemovedOutputPort(const rtmidiadapt::DeviceOnPort& devOnPort);

    Usb2DeviceMapper m_usb2DeviceMapper;
    std::vector< std::unique_ptr<Midi1Output> > m_usbMidiOutputPorts;
    std::vector<rtmidiadapt::DeviceOnPort> m_unmappedUsbMidiOutPorts;
};

}
#endif