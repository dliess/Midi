#ifndef USB_MIDI_IN_H
#define USB_MIDI_IN_H

#include "IMidiInMedium.h"
#include <RtMidi.h>
#include <vector>
#include <cstdint>
#include <functional>
#include <memory>

namespace midi
{

class UsbMidiIn : public IMidiInMedium
{
public:
    UsbMidiIn() noexcept;
    UsbMidiIn(const UsbMidiIn& other) = delete;
    UsbMidiIn(UsbMidiIn&& other) noexcept;
    UsbMidiIn& operator=(const UsbMidiIn& other) = delete;
    
    bool openPort(int portNmbr) noexcept;
    void closePort() noexcept;

    Type getType() const override;
    std::string getPortName() const override;
    std::string getDeviceName() const override;
    void registerCallback(Callback cb) override;
    void update() override;

private:
    std::unique_ptr<RtMidiIn> m_pRtMidiIn;
    std::string m_usbPortName;
    std::string m_deviceName;
    std::vector<Callback> m_cb;
    static void helperCb(double timeStamp,
                         std::vector<unsigned char> *pMessage,
                         void *userData);
    void cb(double timeStamp, std::vector<uint8_t> &message);
};

} // namespace midi

#endif