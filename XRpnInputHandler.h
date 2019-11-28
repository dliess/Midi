#ifndef MIDI_XRPN_HANDLER_H
#define MIDI_XRPN_HANDLER_H

#include "MidiMessage.h"
#include <array>

namespace midi::xrpn
{

class XRpnInputHandler
{
public:
    static constexpr bool isXRpnMsg(const Message<ControlChange>& ccMsg) noexcept{
        return ccMsg.controllerNumber() == Message<RPN>::CC_ID_MSB ||
               ccMsg.controllerNumber() == Message<RPN>::CC_ID_LSB ||
               ccMsg.controllerNumber() == Message<NRPN>::CC_ID_MSB ||
               ccMsg.controllerNumber() == Message<NRPN>::CC_ID_LSB ||
               ccMsg.controllerNumber() == RpnBase::CC_ID_VALUE_MSB ||
               ccMsg.controllerNumber() == RpnBase::CC_ID_VALUE_LSB;
    }

    MidiMessage handleMsg(const Message<ControlChange>& ccMsg) noexcept;

private:
    std::array<MidiMessage, NUM_CHANNELS> m_activeXRpn;
};

} // namespace midi::xrpn

#endif