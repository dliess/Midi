#ifndef MIDI_XRPN_HANDLER_H
#define MIDI_XRPN_HANDLER_H

#include "MidiMessage.h"
#include "XRpnCommon.h"
#include <array>

namespace midi::xrpn
{

class XRpnInputHandler
{
public:
    static constexpr bool isXRpnMsg(const Message<ControlChange>& ccMsg) noexcept{
        return ccMsg.controllerNumber() == CC_ID_RPN_ID_MSB ||
               ccMsg.controllerNumber() == CC_ID_RPN_ID_LSB ||
               ccMsg.controllerNumber() == CC_ID_NRPN_ID_MSB ||
               ccMsg.controllerNumber() == CC_ID_NRPN_ID_LSB ||
               ccMsg.controllerNumber() == CC_ID_VALUE_MSB ||
               ccMsg.controllerNumber() == CC_ID_VALUE_LSB;
    }

    MidiMessage handleMsg(const Message<ControlChange>& ccMsg) noexcept;

private:
    std::array<MidiMessage, NUM_CHANNELS> m_activeXRpn;
};

} // namespace midi::xrpn

#endif