#include "UsbMidiOut.h"
#include "RtMidiAdaptTypes.h"
//#include <iostream>

using namespace midi;

UsbMidiOut::UsbMidiOut() noexcept
{
}

UsbMidiOut::UsbMidiOut(UsbMidiOut&& other) noexcept :
    m_pRtMidiOut(std::move(other.m_pRtMidiOut)),
    m_usbPortName(std::move(m_usbPortName)),
    m_deviceName(std::move(m_deviceName))
{
}


bool UsbMidiOut::openPort(int portNmbr) noexcept
{
    if(m_pRtMidiOut)
    {
        return true;
    }
    try
    {
        m_pRtMidiOut = std::make_unique<RtMidiOut>(__RTMIDI_API__);
        m_pRtMidiOut->openPort(portNmbr);
        m_usbPortName = rtmidiadapt::DeviceOnUsbPort( m_pRtMidiOut->getPortName(portNmbr) ).getUsbPortName();
        m_deviceName = rtmidiadapt::DeviceOnUsbPort( m_pRtMidiOut->getPortName(portNmbr) ).getDeviceName();
    }
    catch ( RtMidiError &error )
    {
        m_pRtMidiOut.reset();
        error.printMessage();
        return false;
    }
    return true;
}

void UsbMidiOut::closePort() noexcept
{
    if(m_pRtMidiOut)
    {
        m_pRtMidiOut->closePort();
    }
}

IMidiMedium::Type UsbMidiOut::getType() const
{
    return USB;
}

std::string UsbMidiOut::getPortName() const
{
    return m_usbPortName;
}

std::string UsbMidiOut::getDeviceName() const
{
    return m_deviceName;
}

bool UsbMidiOut::send(const std::vector<uint8_t>& message)
{
    try
    {
        m_pRtMidiOut->sendMessage( &message );
    }
    catch ( RtMidiError &error )
    {
        error.printMessage();
        return false;
    }
    return true;
}