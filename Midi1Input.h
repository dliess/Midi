#ifndef MIDI_1_INPUT_H
#define MIDI_1_INPUT_H

#include <vector>
#include <memory>
#include <functional>
#include "IMidiInMedium.h"
#include "MidiMessage.h"
#include "XRpnHandler.h"
//class IMidiInMedium;

namespace midi
{
class IMidi1InputCallback;
}

namespace midi
{

class Midi1Input
{
public:
    Midi1Input(std::unique_ptr<IMidiInMedium> pMedium);
    IMidiInMedium& medium();
    const IMidiInMedium& medium() const;
    using Callback = std::function<void(const MidiMessage&)>;
    void registerMidiInCb(IMidi1InputCallback* pMidiInCb);
    void registerMidiInCb(Callback cb);
    void update();
private:
    std::unique_ptr<IMidiInMedium> m_pMedium;
    IMidi1InputCallback*           m_pMidiInCb;
    std::vector<Callback>          m_callbacks;
    XRpnHandler                    m_xrpnHandler;
    void processIncomingData(double timestamp, std::vector<uint8_t>& data);
};

} // namespace midi

#endif