#include "UsbMidiOut.h"
#include "RtMidiAdaptTypes.h"
//#include <iostream>

using namespace midi;

UsbMidiOut::UsbMidiOut() :
    m_rtMidiOut()
{
}

UsbMidiOut::~UsbMidiOut()
{
    if(m_rtMidiOut)
    {
        m_rtMidiOut->closePort();
    }
}

bool UsbMidiOut::openPort(int portNmbr)
{
    try
    {
        if(!m_rtMidiOut)
        {
            m_rtMidiOut.emplace();
        }
        m_rtMidiOut->openPort(portNmbr);
        m_portName = rtmidiadapt::DeviceOnPort( m_rtMidiOut->getPortName(portNmbr) ).getPortName();
        m_deviceName = rtmidiadapt::DeviceOnPort( m_rtMidiOut->getPortName(portNmbr) ).getDeviceName();
    }
    catch ( RtMidiError &error )
    {
        m_rtMidiOut.reset();
        error.printMessage();
        return false;
    }
    return true;
}

void UsbMidiOut::closePort()
{
    if(m_rtMidiOut)
    {
        m_rtMidiOut->closePort();
    }
}

IMidiMedium::Type UsbMidiOut::getType() const
{
    return USB;
}

std::string UsbMidiOut::getPortName() const
{
    return m_portName;
}

std::string UsbMidiOut::getDeviceName() const
{
    return m_deviceName;
}

bool UsbMidiOut::send(const std::vector<uint8_t>& message)
{
    try
    {
        m_rtMidiOut->sendMessage( &message );
    }
    catch ( RtMidiError &error )
    {
        error.printMessage();
        return false;
    }
    return true;
}