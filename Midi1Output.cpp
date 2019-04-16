#include "Midi1Output.h"
#include "MidiOutputMessage.h"
#include "IMidiOutMedium.h"
#include <iostream>

using namespace midi;

Midi1Output::Midi1Output(std::unique_ptr<IMidiOutMedium> pMedium) :
    m_pMedium(std::move(pMedium))
{}

IMidiOutMedium& Midi1Output::medium()
{
    return *m_pMedium;
}

bool Midi1Output::noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    return send( OutputMessage<NoteOn>(channel, note, velocity) );
}

bool Midi1Output::noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
    return send( OutputMessage<NoteOff>(channel, note, velocity) );
}

bool Midi1Output::pitchBend(uint8_t channel, int16_t value)
{
    return send( OutputMessage<PitchBend>(channel, value) );
}

bool Midi1Output::controlParameter(uint8_t channel, uint8_t ccId, uint8_t value)
{
    return send( OutputMessage<ControlChange>(channel, ccId, value) );
}

bool Midi1Output::sysEx(const std::vector<uint8_t>& msg)
{
    return m_pMedium->send(msg);
}

bool Midi1Output::send(const MsgLayout<1>& midiMessage)
{
    std::vector<uint8_t> msg(1);
    msg[0] = midiMessage.command();
    return m_pMedium->send(msg);
}

bool Midi1Output::send(const MsgLayout<2>& midiMessage)
{
    std::vector<uint8_t> msg(2);
    msg[0] = midiMessage.command();
    msg[1] = midiMessage.data1();
    return m_pMedium->send(msg);
}

bool Midi1Output::send(const MsgLayout<3>& midiMessage)
{
    std::vector<uint8_t> msg(3);
    msg[0] = midiMessage.command();
    msg[1] = midiMessage.data1();
    msg[2] = midiMessage.data2();
    return m_pMedium->send(msg);
}


