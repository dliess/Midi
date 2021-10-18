#ifndef MIDI_1_INPUT_H
#define MIDI_1_INPUT_H

#include <vector>
#include <memory>
#include <functional>
#include "IMidiInMedium.h"
#include "MidiMessage.h"
#include "XRpnInputHandler.h"
#include "MidiCCInputHandler.h"
#include "NonBufferedMessageDrain.h"

namespace midi { class IMidi1InputCallback; }

namespace midi
{

template<typename MessageDrain = NonBufferedMessageDrain>
class Midi1Input : public MessageDrain
{
public:
    Midi1Input(std::unique_ptr<IMidiInMedium> pMedium);
    bool setCCHighResPair(int msbId, int lsbId) noexcept;
    IMidiInMedium& medium();
    const IMidiInMedium& medium() const;
    void registerMidiInCbIf(IMidi1InputCallback* pMidiInCb);
private:
    std::unique_ptr<IMidiInMedium> m_pMedium;
    IMidi1InputCallback*           m_pMidiInCb;
    xrpn::XRpnInputHandler         m_xrpnHandler;
    CCInputHandler                 m_ccInputHandler;
    void processIncomingData(double timestamp, std::vector<uint8_t>& data);
};

} // namespace midi

#include "Midi1Input.tcc"

#endif