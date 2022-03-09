#include "MidiMediumDummy.h"

using namespace midi;

MidiInMediumDummy::MidiInMediumDummy(const std::string& usbDeviceName, IMidiMedium::Type type) : 
    m_usbDeviceName(usbDeviceName),
    m_type(type)
{
}

IMidiMedium::Type MidiInMediumDummy::getType() const
{
    return m_type;
}

std::string MidiInMediumDummy::getDevicePortName() const
{
    return "UsbDummyPort";
}

std::string MidiInMediumDummy::getDeviceName() const
{
    return m_usbDeviceName;
}

void MidiInMediumDummy::registerCallback(Callback cb)
{

}

void MidiInMediumDummy::update()
{

}

MidiOutMediumDummy::MidiOutMediumDummy(const std::string& usbDeviceName, IMidiMedium::Type type) : 
    m_usbDeviceName(usbDeviceName),
    m_type(type)
{
}

IMidiMedium::Type MidiOutMediumDummy::getType() const
{
    return m_type;
}

std::string MidiOutMediumDummy::getDevicePortName() const
{
    return "UsbDummyPort";
}

std::string MidiOutMediumDummy::getDeviceName() const
{
    return m_usbDeviceName;
}

bool MidiOutMediumDummy::send(const std::vector<uint8_t>& message)
{
    return true;
}

MidiMediumDummy::MidiMediumDummy(const std::string& usbDeviceName, IMidiMedium::Type type) :
    m_pMidiInMediumDummy(std::make_unique<MidiInMediumDummy>(usbDeviceName, type)),
    m_pMidiOutMediumDummy(std::make_unique<MidiOutMediumDummy>(usbDeviceName, type))
{    
}

std::unique_ptr<MidiInMediumDummy> MidiMediumDummy::hijackInMedium()
{
    return std::move(m_pMidiInMediumDummy);
}

std::unique_ptr<MidiOutMediumDummy> MidiMediumDummy::hijackOutMedium()
{
    return std::move(m_pMidiOutMediumDummy);
}
