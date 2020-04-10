#ifndef MIDI_CC_HIGH_RES_OUTPUT_HANDLER_H
#define MIDI_CC_HIGH_RES_OUTPUT_HANDLER_H

#include <optional>
#include <functional>

#include "MidiMessage.h"

namespace midi
{

class CCHighResOutputHandler
{
public:
   using CCConsumerCb = std::function<void(const Message<ControlChange>&)>;
   CCHighResOutputHandler(CCConsumerCb cb) noexcept : m_ccConsumerCb(std::move(cb)) {}
   void send(const Message<ControlChangeHighRes>& msg) noexcept
   {
      auto [msb, lsb] = msg.toCCPair();
      if(m_lastSentCcMsb != msb)
      {
         m_ccConsumerCb(msb);
         m_lastSentCcMsb = msb;
      }
      m_ccConsumerCb(lsb);
   }

private:
   CCConsumerCb                          m_ccConsumerCb;
   std::optional<Message<ControlChange>> m_lastSentCcMsb;
};

} // namespace midi

#endif