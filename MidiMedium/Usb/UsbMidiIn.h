#ifndef USB_MIDI_IN_H
#define USB_MIDI_IN_H

#include "IMidiInMedium.h"
#include <rtmidi/RtMidi.h>
#include <vector>
#include <cstdint>
#include <functional>
#include <optional>

namespace midi
{

class UsbMidiIn : public IMidiInMedium
{
public:
    UsbMidiIn();
    ~UsbMidiIn();
    bool openPort(int portNmbr);
    void closePort();

    Type getType() const override;
    std::string getPortName() const override;
    std::string getDeviceName() const override;
    void registerCallback(Callback cb) override;
    void update() override;

private:
    std::optional<RtMidiIn> m_rtMidiIn;
    std::string m_portName;
    std::string m_deviceName;
    Callback    m_cb;
    static void helperCb(double timeStamp,
                         std::vector<unsigned char> *pMessage,
                         void *userData);
    void cb(double timeStamp, std::vector<uint8_t> &message);
};

} // namespace midi

#endif