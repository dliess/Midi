#include "XRpnHandler.h"

midi::MidiMessage midi::XRpnHandler::handleMsg(const midi::Message<midi::ControlChange>& ccMsg) noexcept
{
    const auto channelIdx = ccMsg.channel() - 1;
    mpark::visit(overload{
            [this, channelIdx, &ccMsg](const mpark::monostate& empty) {
                switch(ccMsg.controllerNumber()){
                    case CC_ID_RPN_ID_MSB:
                    {
                        break;
                    }
                    case CC_ID_RPN_ID_LSB:
                    {
                        break;
                    }
                    case CC_ID_NRPN_ID_MSB:
                    {
                        break;
                    }
                    case CC_ID_NRPN_ID_LSB:
                    {
                        break;
                    }
                    case CC_ID_VALUE_MSB:
                    {
                        break;
                    }
                    case CC_ID_VALUE_LSB:
                    {
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            },
            [&ccMsg](const Message<RPN>& rpn) {
                switch(ccMsg.controllerNumber()){
                    case CC_ID_RPN_ID_MSB:
                    {
                        break;
                    }
                    case CC_ID_RPN_ID_LSB:
                    {
                        break;
                    }
                    case CC_ID_NRPN_ID_MSB:
                    {
                        break;
                    }
                    case CC_ID_NRPN_ID_LSB:
                    {
                        break;
                    }
                    case CC_ID_VALUE_MSB:
                    {
                        break;
                    }
                    case CC_ID_VALUE_LSB:
                    {
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            },
            [&ccMsg](const Message<NRPN>& nrpn) {
                switch(ccMsg.controllerNumber()){
                    case CC_ID_RPN_ID_MSB:
                    {
                        break;
                    }
                    case CC_ID_RPN_ID_LSB:
                    {
                        break;
                    }
                    case CC_ID_NRPN_ID_MSB:
                    {
                        break;
                    }
                    case CC_ID_NRPN_ID_LSB:
                    {
                        break;
                    }
                    case CC_ID_VALUE_MSB:
                    {
                        break;
                    }
                    case CC_ID_VALUE_LSB:
                    {
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            },
            [](auto&& all){}
        },
        m_activeXRpn[channelIdx]
    );
    return mpark::monostate{};
}