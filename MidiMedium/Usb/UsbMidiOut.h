#ifndef USB_MIDI_OUT_H
#define USB_MIDI_OUT_H

#include <vector>
#include <cstdint>
#include "IMidiOutMedium.h"

class RtMidiOut;

namespace midi
{

class UsbMidiOut : public IMidiOutMedium
{
public:
    UsbMidiOut();
    ~UsbMidiOut();
    bool openPort(int portNmbr);
    void closePort();
    Type getType() const override;
    std::string getPortName() const override;
    std::string getDeviceName() const override;
    bool send(const std::vector<uint8_t>& message) override;
private:
    RtMidiOut* m_pRtMidiOut;
    std::string m_portName;
    std::string m_deviceName;
};

} // namespace midi

#endif