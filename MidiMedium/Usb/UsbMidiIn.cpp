#include "UsbMidiIn.h"
#include "RtMidiAdaptTypes.h"
#include <rtmidi/RtMidi.h>
//#include <iostream>

using namespace midi;

UsbMidiIn::UsbMidiIn() :
    m_pRtMidiIn(nullptr)
{
}

UsbMidiIn::~UsbMidiIn()
{
    if(m_pRtMidiIn)
    {
        m_pRtMidiIn->closePort();
    }
    delete m_pRtMidiIn;
}

bool UsbMidiIn::openPort(int portNmbr)
{
    try
    {
        if(!m_pRtMidiIn)
        {
            m_pRtMidiIn = new RtMidiIn;
            m_pRtMidiIn->setCallback(helperCb, this);
        }
        m_pRtMidiIn->openPort(portNmbr);
        m_pRtMidiIn->ignoreTypes( /*sysex*/false, /*timimng*/true, /*sense*/true );
        m_portName = rtmidiadapt::DeviceOnPort( m_pRtMidiIn->getPortName(portNmbr) ).getPortName();
        m_deviceName = rtmidiadapt::DeviceOnPort( m_pRtMidiIn->getPortName(portNmbr) ).getDeviceName();
    }
    catch ( RtMidiError &error )
    {
        m_pRtMidiIn = nullptr;
        error.printMessage();
        return false;
    }
    return true;
}

void UsbMidiIn::closePort()
{
    if(m_pRtMidiIn)
    {
        m_pRtMidiIn->closePort();
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


