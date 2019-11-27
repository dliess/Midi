#include "XRpnOutputHandler.h"
#include "XRpnCommon.h"

midi::xrpn::XRpnOutputHandler::XRpnOutputHandler(CCConsumerCb cb) noexcept :
    m_ccConsumerCb(cb)
{
}


void midi::xrpn::XRpnOutputHandler::send(const midi::Message<midi::RPN>& msg) noexcept
{
    const auto channelIdx = msg.channelNr - 1;
    auto& active = m_activeXRpn[channelIdx];
    mpark::visit(overload{
        [this, channelIdx, &msg](const mpark::monostate& empty) {
            m_activeXRpn[channelIdx].emplace<Message<RPN>>(msg.channelNr);
            send(msg);
        },
        [this, &msg](Message<RPN>& rpn) {
            if(rpn.idMsb != msg.idMsb || rpn.idLsb != msg.idLsb){
                sendClearRpn(msg.channelNr);
                rpn.idMsb = msg.idMsb;
                rpn.idLsb = msg.idLsb;
                rpn.valueMsb = msg.valueMsb;
                m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, CC_ID_RPN_ID_MSB, msg.idMsb));
                m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, CC_ID_RPN_ID_LSB, msg.idLsb));
                m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, CC_ID_VALUE_MSB, msg.valueMsb));
                if(msg.valueLsb != msg.UNSET){
                    rpn.valueLsb = msg.valueLsb;
                    m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, CC_ID_VALUE_LSB, msg.valueLsb));
                }
            }
            else
            {
                bool lsbChanged = false;
                if(rpn.valueMsb != msg.valueMsb){
                    rpn.valueMsb = msg.valueMsb;
                    m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, CC_ID_VALUE_MSB, msg.valueMsb));
                    lsbChanged = true;
                }
                if(msg.valueLsb != msg.UNSET && (msg.valueLsb != msg.valueLsb || lsbChanged)){
                    rpn.valueLsb = msg.valueLsb;
                    m_ccConsumerCb(Message<midi::ControlChange>(msg.channelNr, CC_ID_VALUE_LSB, msg.valueLsb));
                }
            }
        },
        [this, channelIdx, &msg](Message<NRPN>& nrpn) {
            m_activeXRpn[channelIdx].emplace<Message<RPN>>(msg.channelNr);
            send(msg);
        },
        [](auto&& all){},
    }, active);
}

void midi::xrpn::XRpnOutputHandler::send(const midi::Message<midi::NRPN>& msg) noexcept
{
    const auto channelIdx = msg.channelNr - 1;
    auto& active = m_activeXRpn[channelIdx];
    mpark::visit(overload{
        [this, channelIdx, &msg](const mpark::monostate& empty) {
            m_activeXRpn[channelIdx].emplace<Message<NRPN>>(msg.channelNr);
            send(msg);
        },
        [this, channelIdx, &msg](Message<RPN>& rpn) {
            m_activeXRpn[channelIdx].emplace<Message<NRPN>>(msg.channelNr);
            send(msg);
        },
        [](Message<NRPN>& nrpn) {},
        [](auto&& all){},
    }, active);
}

void midi::xrpn::XRpnOutputHandler::sendClearRpn(int channelNr) noexcept
{
    m_ccConsumerCb(Message<midi::ControlChange>(channelNr, CC_ID_RPN_ID_MSB, NULL_VALUE));
    m_ccConsumerCb(Message<midi::ControlChange>(channelNr, CC_ID_RPN_ID_LSB, NULL_VALUE));
}