#ifndef USB_MIDI_IN_H
#define USB_MIDI_IN_H

#include "IMidiInMedium.h"
#ifdef __RTMIDI_ADDED__
    #include <RtMidi.h>
#else
    #include <rtmidi/RtMidi.h>
#endif
#include <vector>
#include <cstdint>
#include <functional>
#include <memory>

namespace midi
{

class UsbMidiIn : public IMidiInMedium
{
public:
    UsbMidiIn() noexcept = default;
    UsbMidiIn(const UsbMidiIn& other) = delete;
    UsbMidiIn(UsbMidiIn&& other) noexcept = default;
    UsbMidiIn& operator=(const UsbMidiIn& other) = delete;
    UsbMidiIn& operator=(UsbMidiIn&& other) = default;

    bool openPort(int portNmbr) noexcept;
    bool openVirtualPort(std::string portName) noexcept;
    void closePort() noexcept;

    Type getType() const override;
    std::string getDevicePortName() const override;
    std::string getDeviceName() const override;
    std::string getHostConnectorPortName() const override;
    void registerCallback(Callback cb) override;
    void update() override;

private:
    std::unique_ptr<RtMidiIn> m_pRtMidiIn;
    std::string m_deviceName;
    std::string m_devicePortName;
    std::string m_usbPortName;
    std::vector<Callback> m_cb;
    static void helperCb(double timeStamp,
                         std::vector<unsigned char> *pMessage,
                         void *userData);
    void cb(double timeStamp, std::vector<uint8_t> &message);
    static void errorCb(RtMidiError::Type type, const std::string &errorText, void *userData);
};

} // namespace midi

#endif