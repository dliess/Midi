#ifndef INTERFACE_MIDI_IN_MEDIUM
#define INTERFACE_MIDI_IN_MEDIUM

#include "IMidiMedium.h"
#include <cstdint>
#include <functional>
#include <vector>

namespace midi
{

class IMidiInMedium : public IMidiMedium
{
public:
    using Callback = std::function<void(double, std::vector<uint8_t>&)>;
    virtual void registerCallback(Callback cb) = 0;
    virtual void update() = 0;
};

} // namespace midi

#endif