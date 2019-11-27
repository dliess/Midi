#include "XRpnOutputHandler.h"

void midi::XRpnOutputHandler::send(const midi::Message<midi::RPN>& msg) noexcept
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
                //sendClearRpn();
                rpn.idMsb = msg.idMsb;
                rpn.idLsb = msg.idLsb;
                /*m_midiOut.send();*/
                /*m_midiOut.send();*/
                /*m_midiOut.send();*/
                //if(msg.valueLsb || msg.valueLsb != 0) /*m_midiOut.send();*/
            }
            else
            {
                if(rpn.valueMsb != msg.valueMsb){
                    /*m_midiOut.send();  */
                }
                if(msg.valueLsb && msg.valueLsb != msg.valueLsb){
                    /*m_midiOut.send();  */
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

void midi::XRpnOutputHandler::send(const midi::Message<midi::NRPN>& msg) noexcept
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
