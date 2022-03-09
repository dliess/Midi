#include "UsbMidiIn.h"
#include "RtMidiAdaptTypes.h"
//#include <iostream>

using namespace midi;

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
        m_pRtMidiIn->setErrorCallback(errorCb, this);
        m_pRtMidiIn->ignoreTypes( /*sysex*/false, /*timimng*/true, /*sense*/true );
        const auto rtMidiPort = rtmidiadapt::DeviceOnUsbPort( m_pRtMidiIn->getPortName(portNmbr) );
        m_deviceName = rtMidiPort.getDeviceName();
        m_devicePortName = rtMidiPort.getMidiPort();
        m_usbPortName = rtMidiPort.getUsbPortName();
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

std::string UsbMidiIn::getDeviceName() const
{
    return m_deviceName;
}

std::string UsbMidiIn::getDevicePortName() const
{
    return m_usbPortName;
}

std::string UsbMidiIn::getHostConnectorPortName() const
{
    return m_usbPortName;
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

void UsbMidiIn::errorCb(RtMidiError::Type type, const std::string &errorText, void *userData)
{
    std::cerr << "UsbMidiIn::errorCb type: " << type << " errorText: " << errorText;
}

