#ifndef USB_MIDI_OUTPUT_PORT_LIST_PROVIDER_H
#define USB_MIDI_OUTPUT_PORT_LIST_PROVIDER_H

#include "UsbMidiPortInfo.h"

class RtMidiOut;

namespace midi
{ 

class OutputPortListProvider
{
public:
    OutputPortListProvider();
    ~OutputPortListProvider();
    bool init();
    bool getPorts(PortInfoSet& rPortSet);
private:
    RtMidiOut *m_pMidiOut;
};

} // namespace midi
#endif