#ifndef MIDI_CC_INPUT_HANDLER_H
#define MIDI_CC_INPUT_HANDLER_H

#include "MidiCCPairMap.h"
#include "MidiMessage.h"
#include <mpark/variant.hpp>

namespace midi
{

class CCInputHandler
{
public:
   inline bool setPair(int msbId, int lsbId) noexcept;
   using Ret = mpark::variant<mpark::monostate, Message<ControlChange>, Message<ControlChangeHighRes>>; 
   Ret handleIncomingCCMsg(const Message<ControlChange>& ccMsg) noexcept;

private:
   CCPairMap                             m_ccPairMap;
};

} // namespace midi

inline
bool midi::CCInputHandler::setPair(int msbId, int lsbId) noexcept
{
   return m_ccPairMap.setPair(msbId, lsbId);
}

#endif