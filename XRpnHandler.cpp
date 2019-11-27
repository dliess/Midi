#include "XRpnHandler.h"

midi::MidiMessage midi::XRpnHandler::handleMsg(const midi::Message<midi::ControlChange>& ccMsg) noexcept
{
    const auto channelIdx = ccMsg.channel() - 1;
    bool shouldSend = false;
    mpark::visit(overload{
            [this, channelIdx, &ccMsg](const mpark::monostate& empty) {
                switch(ccMsg.controllerNumber()){
                    case CC_ID_RPN_ID_MSB:
                    {
                        m_activeXRpn[channelIdx].emplace<Message<RPN>>(ccMsg.channel());
                        handleMsg(ccMsg);
                        break;
                    }
                    case CC_ID_RPN_ID_LSB:
                    {
                        break;
                    }
                    case CC_ID_NRPN_ID_MSB:
                    {
                        m_activeXRpn[channelIdx].emplace<Message<NRPN>>(ccMsg.channel());
                        handleMsg(ccMsg);
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
            [this, channelIdx, &ccMsg, &shouldSend](Message<RPN>& rpn) {
                switch(ccMsg.controllerNumber()){
                    case CC_ID_RPN_ID_MSB:
                    {
                        rpn.idMsb = ccMsg.controllerValue();
                        if(rpn.isCleared()) m_activeXRpn[channelIdx].emplace<mpark::monostate>();
                        break;
                    }
                    case CC_ID_RPN_ID_LSB:
                    {
                        rpn.idLsb = ccMsg.controllerValue();
                        if(rpn.isCleared()) m_activeXRpn[channelIdx].emplace<mpark::monostate>();
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
                        if(!rpn.idIsValid()){
                            // TODO: log some error or throw exception
                            break;
                        }
                        rpn.valueMsb = ccMsg.controllerValue();
                        rpn.valueLsb = 0;
                        shouldSend = true;
                        break;
                    }
                    case CC_ID_VALUE_LSB:
                    {
                        if(!rpn.idIsValid()){
                            // TODO: log some error or throw exception
                            break;
                        }
                        rpn.valueLsb = ccMsg.controllerValue();
                        shouldSend = true;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            },
            [this, channelIdx, &ccMsg, &shouldSend](Message<NRPN>& nrpn) {
                switch(ccMsg.controllerNumber()){
                    case CC_ID_RPN_ID_MSB:
                    {
                        if(ccMsg.controllerValue() == CLEAR){
                            nrpn.idMsb = Message<NRPN>::MAX;
                            if(nrpn.isCleared()) m_activeXRpn[channelIdx].emplace<mpark::monostate>();
                        }
                        break;
                    }
                    case CC_ID_RPN_ID_LSB:
                    {
                        if(ccMsg.controllerValue() == CLEAR){
                            nrpn.idLsb = Message<NRPN>::MAX;
                            if(nrpn.isCleared()) m_activeXRpn[channelIdx].emplace<mpark::monostate>();
                        }
                        break;
                    }
                    case CC_ID_NRPN_ID_MSB:
                    {
                        nrpn.idMsb = ccMsg.controllerValue();
                        break;
                    }
                    case CC_ID_NRPN_ID_LSB:
                    {
                        nrpn.idLsb = ccMsg.controllerValue();
                        break;
                    }
                    case CC_ID_VALUE_MSB:
                    {
                        if(!nrpn.idIsValid()){
                            // TODO: log some error or throw exception
                            break;
                        }
                        nrpn.valueMsb = ccMsg.controllerValue();
                        nrpn.valueLsb = 0;
                        shouldSend = true;
                        break;
                    }
                    case CC_ID_VALUE_LSB:
                    {
                        if(!nrpn.idIsValid()){
                            // TODO: log some error or throw exception
                            break;
                        }
                        nrpn.valueLsb = ccMsg.controllerValue();
                        shouldSend = true;
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
    if(shouldSend)
    {
        return m_activeXRpn[channelIdx];
    }
    return mpark::monostate{};
}