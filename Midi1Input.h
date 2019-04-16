#ifndef MIDI_1_INPUT_H
#define MIDI_1_INPUT_H

#include <vector>
#include <memory>
#include "IMidiInMedium.h"
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
    void registerMidiInCb(IMidi1InputCallback* pMidiInCb);
    void update();
private:
    std::unique_ptr<IMidiInMedium> m_pMedium;
    IMidi1InputCallback*           m_pMidiInCb;
    void processIncomingData(double timestamp, std::vector<uint8_t>& data);
};

} // namespace midi

#endif