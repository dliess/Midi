#ifndef MIDI_XRDP_OUTPUT_HANDLER_H
#define MIDI_XRDP_OUTPUT_HANDLER_H

#include "MidiMessage.h"
#include <array>

namespace midi
{

class XRpnOutputHandler
{
public:
    void send(const Message<RPN>& msg) noexcept;
    void send(const Message<NRPN>& msg) noexcept;
private:
    std::array<MidiMessage, NUM_CHANNELS> m_activeXRpn;
};

} // namespace midi

#endif