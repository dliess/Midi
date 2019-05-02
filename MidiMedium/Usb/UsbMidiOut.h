#ifndef USB_MIDI_OUT_H
#define USB_MIDI_OUT_H

#include "IMidiOutMedium.h"
#include <rtmidi/RtMidi.h>
#include <vector>
#include <cstdint>
#include <optional>

class RtMidiOut;

namespace midi
{

class UsbMidiOut : public IMidiOutMedium
{
public:
    bool openPort(int portNmbr) noexcept;
    void closePort() noexcept;
    Type getType() const override;
    std::string getPortName() const override;
    std::string getDeviceName() const override;
    bool send(const std::vector<uint8_t>& message) override;
private:
    std::optional<RtMidiOut> m_rtMidiOut;
    std::string m_portName;
    std::string m_deviceName;
};

} // namespace midi

#endif