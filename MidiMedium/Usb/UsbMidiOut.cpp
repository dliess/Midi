#include "UsbMidiOut.h"
#include "RtMidiAdaptTypes.h"
//#include <iostream>

using namespace midi;

bool UsbMidiOut::openPort(int portNmbr) noexcept
{
    if(m_pRtMidiOut)
    {
        return true;
    }
    try
    {
        m_pRtMidiOut = std::make_unique<RtMidiOut>(__RTMIDI_API__);
        const auto rtMidiPort = rtmidiadapt::DeviceOnUsbPort(m_pRtMidiOut->getPortName(portNmbr));
        m_deviceName = rtMidiPort.getDeviceName();
        m_devicePortName = rtMidiPort.getMidiPort();
        m_usbPortName = rtMidiPort.getUsbPortName();
        m_pRtMidiOut->openPort(portNmbr, m_devicePortName);
    }
    catch ( RtMidiError &error )
    {
        m_pRtMidiOut.reset();
        error.printMessage();
        return false;
    }
    return true;
}

bool UsbMidiOut::openVirtualPort(std::string portName) noexcept
{
    if(m_pRtMidiOut)
    {
        return true;
    }
    try
    {
        m_pRtMidiOut = std::make_unique<RtMidiOut>(__RTMIDI_API__);
        m_pRtMidiOut->openVirtualPort(portName);
        m_deviceName = std::move(portName);
        m_devicePortName = m_deviceName;
        m_usbPortName = "0000";
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

std::string UsbMidiOut::getDeviceName() const
{
    return m_deviceName;
}

std::string UsbMidiOut::getDevicePortName() const
{
    return m_devicePortName;
}

std::string UsbMidiOut::getHostConnectorPortName() const
{
    return m_usbPortName;
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