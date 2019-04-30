#include "UsbMidiIn.h"
#include "RtMidiAdaptTypes.h"
//#include <iostream>

using namespace midi;

UsbMidiIn::UsbMidiIn() :
    m_rtMidiIn()
{
}

UsbMidiIn::~UsbMidiIn()
{
    if(m_rtMidiIn)
    {
        m_rtMidiIn->closePort();
    }
}

bool UsbMidiIn::openPort(int portNmbr)
{
    try
    {
        if(!m_rtMidiIn)
        {
            m_rtMidiIn.emplace();
            m_rtMidiIn->setCallback(helperCb, this);
        }
        m_rtMidiIn->openPort(portNmbr);
        m_rtMidiIn->ignoreTypes( /*sysex*/false, /*timimng*/true, /*sense*/true );
        m_portName = rtmidiadapt::DeviceOnPort( m_rtMidiIn->getPortName(portNmbr) ).getPortName();
        m_deviceName = rtmidiadapt::DeviceOnPort( m_rtMidiIn->getPortName(portNmbr) ).getDeviceName();
    }
    catch ( RtMidiError &error )
    {
        m_rtMidiIn.reset();
        error.printMessage();
        return false;
    }
    return true;
}

void UsbMidiIn::closePort()
{
    if(m_rtMidiIn)
    {
        m_rtMidiIn->closePort();
    }
}

IMidiMedium::Type UsbMidiIn::getType() const
{
    return USB;
}

std::string UsbMidiIn::getPortName() const
{
    return m_portName;
}

std::string UsbMidiIn::getDeviceName() const
{
    return m_deviceName;
}

void UsbMidiIn::registerCallback(Callback cb)
{
    m_cb = cb;
}

void UsbMidiIn::update()
{

}

void UsbMidiIn::helperCb(double timeStamp,
                               std::vector<unsigned char> *pMessage,
                               void *userData)
{
    auto pThis = static_cast<UsbMidiIn*>(userData);
    pThis->cb(timeStamp, *pMessage);
}

void UsbMidiIn::cb(double timeStamp,
                   std::vector<uint8_t>& message)
{
    if(m_cb)
    {
        m_cb(timeStamp, message);
    }
}


