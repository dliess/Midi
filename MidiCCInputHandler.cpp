#include "MidiCCInputHandler.h"

midi::CCInputHandler::Ret midi::CCInputHandler::handleIncomingCCMsg(const Message<ControlChange>& ccMsg) noexcept
{
   switch (m_ccPairMap[ccMsg.controllerNumber()].role)
   {
      case CCPairMap::Data::Role::SingleValue:
      {
         if (m_lastReceivedCcMsb)
         {
            m_lastReceivedCcMsb.reset();
         }
         return ccMsg;
      }
      case CCPairMap::Data::Role::MSB:
      {
         m_lastReceivedCcMsb.emplace(ccMsg);
         break;
      }
      case CCPairMap::Data::Role::LSB:
      {
         if (m_lastReceivedCcMsb)
         {
            return Message<ControlChangeHighRes>(*m_lastReceivedCcMsb, ccMsg);
         }
         break;
      }
   }
   return Ret();
}