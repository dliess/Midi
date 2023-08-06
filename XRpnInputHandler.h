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
    template<typename XRPN_TYPE>
    MidiMessage handle(const Message<ControlChange>& ccMsg) noexcept;
};

} // namespace midi::xrpn

template<typename XRPN_TYPE>
midi::MidiMessage midi::xrpn::XRpnInputHandler::handle(const Message<ControlChange>& ccMsg) noexcept
{
    const auto channelIdx = ccMsg.channel() - 1;
    if(!VARIANT_NS::holds_alternative<XRPN_TYPE>(m_activeXRpn[channelIdx])){
        m_activeXRpn[channelIdx].emplace<XRPN_TYPE>(channelIdx + 1);
    }
    auto& xrpn = VARIANT_NS::get<XRPN_TYPE>(m_activeXRpn[channelIdx]);
    switch(ccMsg.controllerNumber()){
        case XRPN_TYPE::CC_ID_MSB:
        {
            xrpn.idMsb = ccMsg.controllerValue();
            if(xrpn.isCleared()) m_activeXRpn[channelIdx].emplace<VARIANT_NS::monostate>();
            break;
        }
        case XRPN_TYPE::CC_ID_LSB:
        {
            xrpn.idLsb = ccMsg.controllerValue();
            if(xrpn.isCleared()) m_activeXRpn[channelIdx].emplace<VARIANT_NS::monostate>();
            break;
        }
        case RpnBase::CC_ID_VALUE_MSB:
        {
            if(!xrpn.idIsValid()){
                // TODO: log some error or throw exception
                break;
            }
            xrpn.valueMsb = ccMsg.controllerValue();
            xrpn.valueLsb = 0;
            break;
        }
        case RpnBase::CC_ID_VALUE_LSB:
        {
            if(!xrpn.idIsValid() || xrpn.valueMsb == xrpn.UNSET ){
                // TODO: log some error or throw exception
                break;
            }
            xrpn.valueLsb = ccMsg.controllerValue();
            return xrpn;
        }
        default:
        {
            break;
        }
    }
    return MidiMessage();
}

#endif