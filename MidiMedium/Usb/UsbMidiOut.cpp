#include "UsbMidiOut.h"
#include "RtMidiAdaptTypes.h"
#include <rtmidi/RtMidi.h>
//#include <iostream>

using namespace midi;

UsbMidiOut::UsbMidiOut() :
    m_pRtMidiOut(nullptr)
{
}

UsbMidiOut::~UsbMidiOut()
{
    if(m_pRtMidiOut)
    {
        m_pRtMidiOut->closePort();
    }
    delete m_pRtMidiOut;
}

bool UsbMidiOut::openPort(int portNmbr)
{
    try
    {
        if(!m_pRtMidiOut)
        {
            m_pRtMidiOut = new RtMidiOut;
        }
        m_pRtMidiOut->openPort(portNmbr);
        m_portName = rtmidiadapt::DeviceOnPort( m_pRtMidiOut->getPortName(portNmbr) ).getPortName();
        m_deviceName = rtmidiadapt::DeviceOnPort( m_pRtMidiOut->getPortName(portNmbr) ).getDeviceName();
    }
    catch ( RtMidiError &error )
    {
        m_pRtMidiOut = nullptr;
        error.printMessage();
        return false;
    }
    return true;
}

void UsbMidiOut::closePort()
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
        m_pRtMidiOut->sendMessage( &message );
    }
    catch ( RtMidiError &error )
    {
        error.printMessage();
        return false;
    }
    return true;
}