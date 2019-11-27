#ifndef MIDI_XRDP_OUTPUT_HANDLER_H
#define MIDI_XRDP_OUTPUT_HANDLER_H

#include "MidiMessage.h"
#include <array>
#include <functional>

namespace midi::xrpn
{

class XRpnOutputHandler
{
public:
    using CCConsumerCb = std::function<void(const Message<ControlChange>&)>;
    XRpnOutputHandler(CCConsumerCb cb) noexcept;
    void send(const Message<RPN>& msg) noexcept;
    void send(const Message<NRPN>& msg) noexcept;
private:
    std::array<MidiMessage, NUM_CHANNELS> m_activeXRpn;
    CCConsumerCb                          m_ccConsumerCb;
    void sendClearRpn(int channelNr) noexcept;
};

} // namespace midi::xrpn

#endif