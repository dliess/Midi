#ifndef MIDI_CC_INPUT_HANDLER_H
#define MIDI_CC_INPUT_HANDLER_H

#include "MidiCCPairMap.h"
#include "MidiMessage.h"
#ifdef USE_MPARK_VARIANT
#include <mpark/variant.hpp>
#define VARIANT_NS mpark
#else
#include <variant>
#define VARIANT_NS std
#endif

namespace midi
{

class CCInputHandler
{
public:
   bool setHighResPair(int msbId, int lsbId) noexcept;
   bool setDoubleResPair(int bottomHalfId, int topHalfId) noexcept;
   using Ret = VARIANT_NS::variant<VARIANT_NS::monostate, Message<ControlChange>, Message<ControlChangeHighRes>, Message<ControlChangeDoubleRes>>; 
   Ret handleIncomingCCMsg(const Message<ControlChange>& ccMsg) noexcept;

private:
   CCPairMap                             m_ccPairMap;
};

} // namespace midi

inline
bool midi::CCInputHandler::setHighResPair(int msbId, int lsbId) noexcept
{
   return m_ccPairMap.setHighResPair(msbId, lsbId);
}

inline
bool midi::CCInputHandler::setDoubleResPair(int bottomHalfId, int topHalfId) noexcept
{
   return m_ccPairMap.setDoubleResPair(bottomHalfId, topHalfId);
}

#endif