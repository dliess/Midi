#ifndef MIDI_1_INPUT_H
#define MIDI_1_INPUT_H

#include <vector>
#include <memory>
#include <functional>
#include "IMidiInMedium.h"
#include "MidiMessage.h"
#include "XRpnInputHandler.h"
#include "MidiCCInputHandler.h"
//class IMidiInMedium;

namespace midi { class IMidi1InputCallback; }

namespace midi
{

class Midi1Input
{
public:
    Midi1Input(std::unique_ptr<IMidiInMedium> pMedium);
    bool setCCHighResPair(int msbId, int lsbId) noexcept;
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
    xrpn::XRpnInputHandler         m_xrpnHandler;
    CCInputHandler                 m_ccInputHandler;
    void processIncomingData(double timestamp, std::vector<uint8_t>& data);
};

} // namespace midi

#endif