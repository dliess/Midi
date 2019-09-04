#ifndef USB_MIDI_OUT_H
#define USB_MIDI_OUT_H

#include "IMidiOutMedium.h"
#include <rtmidi/RtMidi.h>
#include <vector>
#include <cstdint>
#include <memory>

class RtMidiOut;

namespace midi
{

class UsbMidiOut : public IMidiOutMedium
{
public:
    UsbMidiOut() noexcept;
    UsbMidiOut(const UsbMidiOut& other) = delete;
    UsbMidiOut(UsbMidiOut&& other) noexcept;
    UsbMidiOut& operator=(const UsbMidiOut& other) = delete;

    bool openPort(int portNmbr) noexcept;
    void closePort() noexcept;
    Type getType() const override;
    std::string getPortName() const override;
    std::string getDeviceName() const override;
    bool send(const std::vector<uint8_t>& message) override;
private:
    std::unique_ptr<RtMidiOut> m_pRtMidiOut;
    std::string m_usbPortName;
    std::string m_deviceName;
};

} // namespace midi

#endif