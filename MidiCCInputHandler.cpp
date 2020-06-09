#include "MidiCCInputHandler.h"

midi::CCInputHandler::Ret midi::CCInputHandler::handleIncomingCCMsg(const Message<ControlChange>& ccMsg) noexcept
{
   switch (m_ccPairMap[ccMsg.controllerNumber()].role)
   {
      case CCPairMap::Data::Role::SingleValue:
      {
         return ccMsg;
      }
      case CCPairMap::Data::Role::MSB:
      {
         m_ccPairMap[ccMsg.controllerNumber()].lastValue[ccMsg.channel() - 1] = ccMsg.controllerValue();
         break;
      }
      case CCPairMap::Data::Role::LSB:
      {
         const uint8_t msbCtrlNr = m_ccPairMap[ccMsg.controllerNumber()].pairId;
         const uint8_t msbLastValue = m_ccPairMap[msbCtrlNr].lastValue[ccMsg.channel() - 1];
         Message<ControlChange> msb(ccMsg.channel(), msbCtrlNr, msbLastValue);
         return Message<ControlChangeHighRes>(msb, ccMsg);
      }
   }
   return Ret();
}