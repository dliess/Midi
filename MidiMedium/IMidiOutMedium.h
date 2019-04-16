#ifndef INTERFACE_MIDI_OUT_MEDIUM
#define INTERFACE_MIDI_OUT_MEDIUM

#include "IMidiMedium.h"
#include <cstdint>
#include <vector>

namespace midi
{

class IMidiOutMedium : public IMidiMedium
{
public:
    virtual bool send(const std::vector<uint8_t>& message) = 0;
};

} // namespace midi

#endif