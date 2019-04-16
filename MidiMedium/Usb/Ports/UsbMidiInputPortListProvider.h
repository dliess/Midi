#ifndef USB_MIDI_INPUT_PORT_LIST_PROVIDER_H
#define USB_MIDI_INPUT_PORT_LIST_PROVIDER_H

#include "UsbMidiPortInfo.h"

class RtMidiIn;

namespace midi
{ 

class InputPortListProvider
{
public:
    InputPortListProvider();
    ~InputPortListProvider();
    bool init();
    bool getPorts(PortInfoSet& portSet);
private:
    RtMidiIn *m_pMidiIn;
};

} // namespace midi
#endif