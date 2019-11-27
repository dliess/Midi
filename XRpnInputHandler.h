#ifndef MIDI_XRPN_HANDLER_H
#define MIDI_XRPN_HANDLER_H

#include "MidiMessage.h"

namespace midi
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
    static constexpr int CC_ID_RPN_ID_MSB = 101;
    static constexpr int CC_ID_RPN_ID_LSB = 100;
    static constexpr int CC_ID_NRPN_ID_MSB = 99;
    static constexpr int CC_ID_NRPN_ID_LSB = 98;
    static constexpr int CC_ID_VALUE_MSB = 6;
    static constexpr int CC_ID_VALUE_LSB = 38;

    std::array<MidiMessage, NUM_CHANNELS> m_activeXRpn;
};

} // namespace midi

#endif