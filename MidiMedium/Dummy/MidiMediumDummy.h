#ifndef MIDI_MEDIUM_DUMMY_H
#define MIDI_MEDIUM_DUMMY_H

#include "IMidiInMedium.h"
#include "IMidiOutMedium.h"
#include <memory>
#include <string>

namespace midi
{

class MidiInMediumDummy : public IMidiInMedium
{
public:
    MidiInMediumDummy(const std::string& usbDeviceName, IMidiMedium::Type type);

    IMidiMedium::Type getType() const override;
    std::string getDevicePortName() const override;
    std::string getDeviceName() const override;
    std::string getHostConnectorPortName() const override;
    void registerCallback(Callback cb) override;
    void update() override;
private:
    std::string       m_usbDeviceName;
    IMidiMedium::Type m_type;
};

class MidiOutMediumDummy : public IMidiOutMedium
{
public:
    MidiOutMediumDummy(const std::string& usbDeviceName, IMidiMedium::Type type);

    IMidiMedium::Type getType() const override;
    std::string getDevicePortName() const override;
    std::string getDeviceName() const override;
    std::string getHostConnectorPortName() const override;
    bool send(const std::vector<uint8_t>& message) override;
private:
    std::string       m_usbDeviceName;
    IMidiMedium::Type m_type;
};

class MidiMediumDummy
{
public:
    MidiMediumDummy(const std::string& usbDeviceName, IMidiMedium::Type type);
    std::unique_ptr<MidiInMediumDummy> hijackInMedium();
    std::unique_ptr<MidiOutMediumDummy> hijackOutMedium();
private:
    std::unique_ptr<MidiInMediumDummy>  m_pMidiInMediumDummy;
    std::unique_ptr<MidiOutMediumDummy> m_pMidiOutMediumDummy;
};

} // namespace midi

#endif