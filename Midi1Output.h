#ifndef MIDI_1_OUTPUT_H
#define MIDI_1_OUTPUT_H

#include "IMidiOutMedium.h"
#include "MidiMessage.h"
#include "XRpnOutputHandler.h"
#include "MidiCCHighResOutputHandler.h"
#include "Midi.h"
#include <memory>
#include <vector>
#include <cstdint>
#include <functional>
namespace midi
{

class Midi1Output
{
public:
    Midi1Output(std::unique_ptr<IMidiOutMedium> pMedium);
    IMidiOutMedium& medium();
    const IMidiOutMedium& medium() const;
    bool noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    bool noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    bool pitchBend(uint8_t channel, int16_t value);
    bool afterTouchPoly(uint8_t channel, uint8_t note, int16_t value);
    bool afterTouch(uint8_t channel, int16_t value);
    bool controlParameter(uint8_t channel, uint8_t ccId, uint8_t value);
    bool start();
    bool stop();
    bool toggleStartStop();
    void setTransportMasked(bool masked);
    using StartedChangedCb = std::function<void(bool)>;
    void registerStartedChangedCb(StartedChangedCb cb);
    using TransportMaskChangedCb = std::function<void(bool)>;
    void registerTransportMaskChangedCb(TransportMaskChangedCb cb);
    bool sysEx(const std::vector<uint8_t>& msg);
    bool send(const MidiMessage& msg);
    bool send(const MsgLayout<1>& midiMessage);
    bool send(const MsgLayout<2>& midiMessage);
    bool send(const MsgLayout<3>& midiMessage);
private:
    std::unique_ptr<IMidiOutMedium> m_pMedium;
    xrpn::XRpnOutputHandler         m_xrpnOutputHandler;
    CCHighResOutputHandler          m_CCHighResOutputHandler;
    bool m_started{false};
    bool m_transportMasked{false};
    std::vector<StartedChangedCb> m_startedChangedCbs;
    std::vector<TransportMaskChangedCb> m_transportMaskChangedCbs;
};

} // namespace midi

#endif