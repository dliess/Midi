#include "UsbMidiIn.h"
#include "RtMidiAdaptTypes.h"
//#include <iostream>

using namespace midi;

UsbMidiIn::UsbMidiIn() noexcept
{
}

UsbMidiIn::UsbMidiIn(UsbMidiIn&& other) noexcept :
    m_pRtMidiIn(std::move(other.m_pRtMidiIn)),
    m_usbPortName(std::move(other.m_usbPortName)),
    m_deviceName(std::move(other.m_deviceName)),
    m_cb(std::move(other.m_cb))
{
}

bool UsbMidiIn::openPort(int portNmbr) noexcept
{
    if(m_pRtMidiIn)
    {
        return true;
    }
    try
    {
        m_pRtMidiIn = std::make_unique<RtMidiIn>(__RTMIDI_API__);
        m_pRtMidiIn->openPort(portNmbr);
        m_pRtMidiIn->setCallback(helperCb, this);
        m_pRtMidiIn->ignoreTypes( /*sysex*/false, /*timimng*/true, /*sense*/true );
        m_usbPortName = rtmidiadapt::DeviceOnUsbPort( m_pRtMidiIn->getPortName(portNmbr) ).getUsbPortName();
        m_deviceName = rtmidiadapt::DeviceOnUsbPort( m_pRtMidiIn->getPortName(portNmbr) ).getDeviceName();
    }
    catch ( RtMidiError &error )
    {
        m_pRtMidiIn.reset();
        error.printMessage();
        return false;
    }
    return true;
}

void UsbMidiIn::closePort() noexcept
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
    return m_usbPortName;
}

std::string UsbMidiIn::getDeviceName() const
{
    return m_deviceName;
}

void UsbMidiIn::registerCallback(Callback cb)
{
    m_cb.push_back(cb);
}

void UsbMidiIn::update()
{
    // TODO for later
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
    for(auto& cb : m_cb)
    {
        cb(timeStamp, message);
    }
}


