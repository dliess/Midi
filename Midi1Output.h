#ifndef MIDI_1_OUTPUT_H
#define MIDI_1_OUTPUT_H

#include "IMidiOutMedium.h"
#include "Midi.h"
#include <memory>
#include <vector>
#include <cstdint>

namespace midi
{

class Midi1Output
{
public:
    Midi1Output(std::unique_ptr<IMidiOutMedium> pMedium);
    IMidiOutMedium& medium();
    bool noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    bool noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    bool pitchBend(uint8_t channel, int16_t value);
    bool controlParameter(uint8_t channel, uint8_t ccId, uint8_t value);
    bool sysEx(const std::vector<uint8_t>& msg);
private:
    std::unique_ptr<IMidiOutMedium> m_pMedium;
    bool send(const MsgLayout<1>& midiMessage);
    bool send(const MsgLayout<2>& midiMessage);
    bool send(const MsgLayout<3>& midiMessage);
};

} // namespace midi

#endif