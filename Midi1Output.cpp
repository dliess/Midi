#include "Midi1Output.h"

#include <iostream>

#include "IMidiOutMedium.h"
#include "MidiMessage.h"

using namespace midi;

Midi1Output::Midi1Output(std::unique_ptr<IMidiOutMedium> pMedium) :
    m_pMedium(std::move(pMedium)),
    m_xrpnOutputHandler(
        [this](const Message<ControlChange> &msg) { send(msg); }),
    m_CCHighResOutputHandler(
        [this](const Message<ControlChange> &msg) { send(msg); })
{
}

IMidiOutMedium &Midi1Output::medium() { return *m_pMedium; }

const IMidiOutMedium &Midi1Output::medium() const { return *m_pMedium; }

bool Midi1Output::noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
   return send(Message<NoteOn>(channel, note, velocity));
}

bool Midi1Output::noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
   return send(Message<NoteOff>(channel, note, velocity));
}

bool Midi1Output::pitchBend(uint8_t channel, float normalizedValue)
{
   return send(Message<PitchBend>(channel, normalizedValue));
}

bool Midi1Output::pitchBend(uint8_t channel, int16_t value)
{
   return send(Message<PitchBend>(channel, value));
}

bool Midi1Output::afterTouchPoly(uint8_t channel, uint8_t note, int16_t value)
{
   return send(Message<AfterTouchPoly>(channel, note, value));
}

bool Midi1Output::afterTouch(uint8_t channel, int16_t value)
{
   return send(Message<AfterTouchChannel>(channel, value));
}

bool Midi1Output::controlParameter(uint8_t channel, uint8_t ccId, uint8_t value)
{
   return send(Message<ControlChange>(channel, ccId, value));
}

bool Midi1Output::sendStart()
{
   return send(Message<Start>());
}

bool Midi1Output::sendStop()
{
   return send(Message<Stop>());
}

bool Midi1Output::sysEx(const std::vector<uint8_t> &msg)
{
   return m_pMedium->send(msg);
}

bool Midi1Output::send(const MidiMessage &msg)
{
   mpark::visit(
       overload{
           [this](const Message<NoteOn> &msg) { send(msg); },
           [this](const Message<NoteOff> &msg) { send(msg); },
           [this](const Message<AfterTouchPoly> &msg) { send(msg); },
           [this](const Message<ControlChange> &msg) { send(msg); },
           [this](const Message<ProgramChange> &msg) { send(msg); },
           [this](const Message<AfterTouchChannel> &msg) { send(msg); },
           [this](const Message<PitchBend> &msg) { send(msg); },
           [this](const Message<TimeCodeQuarterFrame> &msg) { send(msg); },
           [this](const Message<SongPosition> &msg) { send(msg); },
           [this](const Message<SongSelect> &msg) { send(msg); },
           [this](const Message<TuneRequest> &msg) { send(msg); },
           [this](const Message<Clock> &msg) { send(msg); },
           [this](const Message<Start> &msg) { send(msg); },
           [this](const Message<Continue> &msg) { send(msg); },
           [this](const Message<Stop> &msg) { send(msg); },
           [this](const Message<ActiveSensing> &msg) { send(msg); },
           [this](const Message<SystemReset> &msg) { send(msg); },
           [this](const Message<RPN> &msg) { m_xrpnOutputHandler.send(msg); },
           [this](const Message<NRPN> &msg) { m_xrpnOutputHandler.send(msg); },
           [this](const Message<ControlChangeHighRes> &msg) {
              m_CCHighResOutputHandler.send(msg);
           },
           [](auto &&other) {
              std::cerr << "INTERNAL ERROR, unknown midi message layout"
                        << std::endl;
           }},
       msg);
   return true;
}

bool Midi1Output::send(const MsgLayout<1> &midiMessage)
{
   std::vector<uint8_t> msg(1);
   msg[0] = midiMessage.command();
   return m_pMedium->send(msg);
}

bool Midi1Output::send(const MsgLayout<2> &midiMessage)
{
   std::vector<uint8_t> msg(2);
   msg[0] = midiMessage.command();
   msg[1] = midiMessage.data1();
   return m_pMedium->send(msg);
}

bool Midi1Output::send(const MsgLayout<3> &midiMessage)
{
   std::vector<uint8_t> msg(3);
   msg[0] = midiMessage.command();
   msg[1] = midiMessage.data1();
   msg[2] = midiMessage.data2();
   return m_pMedium->send(msg);
}
