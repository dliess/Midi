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
      case CCPairMap::Data::Role::BottomHalf:
      {
         const uint8_t topHalfCCId = m_ccPairMap[ccMsg.controllerNumber()].pairId;
         return Message<ControlChangeDoubleRes>::fromBottomHalfCC(ccMsg, topHalfCCId);
      }
      case CCPairMap::Data::Role::TopHalf:
      {
         const uint8_t bottomHalfCCId = m_ccPairMap[ccMsg.controllerNumber()].pairId;
         return Message<ControlChangeDoubleRes>::fromTopHalfCC(ccMsg, bottomHalfCCId);
      }
   }
   return Ret();
}