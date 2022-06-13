#ifndef USB_MIDI_OUT_H
#define USB_MIDI_OUT_H

#include "IMidiOutMedium.h"
#ifdef __RTMIDI_ADDED__
    #include <RtMidi.h>
#else
    #include <rtmidi/RtMidi.h>
#endif
#include <vector>
#include <cstdint>
#include <memory>

class RtMidiOut;

namespace midi
{

class UsbMidiOut : public IMidiOutMedium
{
public:
    UsbMidiOut() noexcept = default;
    UsbMidiOut(const UsbMidiOut& other) = delete;
    UsbMidiOut(UsbMidiOut&& other) noexcept = default;
    UsbMidiOut& operator=(const UsbMidiOut& other) = delete;
    UsbMidiOut& operator=(UsbMidiOut&& other) = default;

    bool openPort(int portNmbr) noexcept;
    bool openVirtualPort(std::string portName) noexcept;
    void closePort() noexcept;
    Type getType() const override;
    std::string getDeviceName() const override;
    std::string getDevicePortName() const override;
    std::string getHostConnectorPortName() const override;
    bool send(const std::vector<uint8_t>& message) override;
private:
    std::unique_ptr<RtMidiOut> m_pRtMidiOut;
    std::string m_deviceName;
    std::string m_devicePortName;
    std::string m_usbPortName;
};

} // namespace midi

#endif