#ifndef INTERFACE_MIDI_MEDIUM
#define INTERFACE_MIDI_MEDIUM

#include <string>

namespace midi
{

class IMidiMedium
{
public:
    enum Type
    {
        USB = 0,
        DIN,
        BLE
    };
    virtual ~IMidiMedium(){}
    virtual Type getType() const = 0;
    virtual std::string getPortName() const = 0;
    virtual std::string getDeviceName() const = 0;
};

} // namespace midi

#endif