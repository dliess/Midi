#ifndef MIDI_INPUT_CB_BASE_H
#define MIDI_INPUT_CB_BASE_H

#include <functional>
#include <vector>

struct MidiMessage;

namespace midi
{

class MidiInputCbBase
{
public:
   using Callback = std::function<void(const MidiMessage&)>;
   void registerMidiInCb(Callback cb) { m_callbacks.emplace_back(cb); };
protected:
   template<typename T>
   void fromBuffer(T&& msg)
   {
      for (auto& cb : m_callbacks) cb(std::forward<T>(msg));
   }

private:
   std::vector<Callback> m_callbacks;
};

} // namespace midi
#endif