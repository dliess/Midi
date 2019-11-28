#include "XRpnOutputHandler.h"

midi::xrpn::XRpnOutputHandler::XRpnOutputHandler(CCConsumerCb cb) noexcept :
    m_ccConsumerCb(cb)
{
}

void midi::xrpn::XRpnOutputHandler::sendClearRpn(int channelNr) noexcept
{
    m_ccConsumerCb(Message<midi::ControlChange>(channelNr, Message<RPN>::CC_ID_MSB, RpnBase::NULL_VALUE));
    m_ccConsumerCb(Message<midi::ControlChange>(channelNr, Message<RPN>::CC_ID_LSB, RpnBase::NULL_VALUE));
}