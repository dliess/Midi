#include "XRpnInputHandler.h"

midi::MidiMessage midi::xrpn::XRpnInputHandler::handleMsg(const midi::Message<midi::ControlChange>& ccMsg) noexcept
{
    const auto channelIdx = ccMsg.channel() - 1;
    switch(ccMsg.controllerNumber()){
        case Message<RPN>::CC_ID_MSB: [[falltrough]];
        case Message<RPN>::CC_ID_LSB: return handle<Message<RPN>>(ccMsg); 
        case Message<NRPN>::CC_ID_MSB: [[falltrough]];
        case Message<NRPN>::CC_ID_LSB: return handle<Message<NRPN>>(ccMsg);
        case RpnBase::CC_ID_VALUE_MSB: [[falltrough]];
        case RpnBase::CC_ID_VALUE_LSB:
        {
            MidiMessage ret;
            mpark::visit(overload{
                [](const mpark::monostate& empty) {},
                [this, &ret, &ccMsg](Message<RPN>& rpn)  { ret = handle<Message<RPN>>(ccMsg); },
                [this, &ret, &ccMsg](Message<NRPN>& nrpn) {ret = handle<Message<NRPN>>(ccMsg); },
                [](auto&& all){}
                },m_activeXRpn[channelIdx]
            );
            return ret;
        }
        default: break;
    }
    return MidiMessage();
}