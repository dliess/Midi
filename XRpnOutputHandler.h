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

    template<typename XRPN_TYPE>
    void send(const XRPN_TYPE& msg) noexcept;
private:
    std::array<MidiMessage, NUM_CHANNELS> m_activeXRpn;
    CCConsumerCb                          m_ccConsumerCb;
    void sendClearRpn(int channelNr) noexcept;
    template<typename XRPN_TYPE>
    void sendXRpn(XRPN_TYPE& cachedXrpn, const XRPN_TYPE& msg) noexcept;
};

} // namespace midi::xrpn

template<typename XRPN_TYPE>
void midi::xrpn::XRpnOutputHandler::send(const XRPN_TYPE& msg) noexcept
{
    const auto channelIdx = msg.channelNr - 1;
    auto& active = m_activeXRpn[channelIdx];
    if(!mpark::holds_alternative<XRPN_TYPE>(active)){
        active.template emplace<XRPN_TYPE>(msg.channelNr);
    }
    try{
        sendXRpn(mpark::get<XRPN_TYPE>(active), msg);
    }catch(...) {/* should not happen */}
}

template<typename XRPN_TYPE>
void midi::xrpn::XRpnOutputHandler::sendXRpn(XRPN_TYPE& cachedXrpn, const XRPN_TYPE& msg) noexcept
{
    if(cachedXrpn.idMsb != msg.idMsb || cachedXrpn.idLsb != msg.idLsb){
        sendClearRpn(msg.channelNr);
        cachedXrpn.idMsb = msg.idMsb;
        cachedXrpn.idLsb = msg.idLsb;
        cachedXrpn.valueMsb = msg.valueMsb;
        m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, XRPN_TYPE::CC_ID_MSB, msg.idMsb));
        m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, XRPN_TYPE::CC_ID_LSB, msg.idLsb));
        m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, XRPN_TYPE::CC_ID_VALUE_MSB, msg.valueMsb));
        if(msg.valueLsb != msg.UNSET){
            cachedXrpn.valueLsb = msg.valueLsb;
            m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, XRPN_TYPE::CC_ID_VALUE_LSB, msg.valueLsb));
        }
    }
    else
    {
        bool lsbChanged = false;
        if(cachedXrpn.valueMsb != msg.valueMsb){
            cachedXrpn.valueMsb = msg.valueMsb;
            m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, XRPN_TYPE::CC_ID_VALUE_MSB, msg.valueMsb));
            lsbChanged = true;
        }
        if(msg.valueLsb != msg.UNSET && (msg.valueLsb != msg.valueLsb || lsbChanged)){
            cachedXrpn.valueLsb = msg.valueLsb;
            m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, XRPN_TYPE::CC_ID_VALUE_LSB, msg.valueLsb));
        }
    }
}


#endif