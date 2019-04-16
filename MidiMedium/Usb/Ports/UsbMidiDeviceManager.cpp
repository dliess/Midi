#include "UsbMidiDeviceManager.h"
#include "DeviceTypes.h"
#include "MidiConfigParser.h"
#include <iostream>

using namespace midi;

DeviceManager::DeviceManager() :
    m_inputPortListChangeHandler(m_inputPortListProvider),
    m_outputPortListChangeHandler(m_outputPortListProvider)
{
}

bool DeviceManager::init()
{
    if(!m_inputPortListProvider.init())
    {
        std::cerr << "could not initialize inputPortListProvider" << std::endl;
        return false;
    }
    if(!m_outputPortListProvider.init())
    {
        std::cerr << "could not initialize outputPortListProvider" << std::endl;
        return false;
    }
    if(!m_usb2DeviceMapper.init())
    {
        std::cerr << "error at usb2DeviceMapper.init()" << std::endl;
        return false;
    }
    m_inputPortListChangeHandler.registerNewPortCb([this](rtmidiadapt::PortIndex i, const rtmidiadapt::DeviceOnPort& devOnPort){
        std::cout << "New input " << i << " : " << devOnPort << std::endl;
        onNewInputPort(i, devOnPort);
    });
    m_inputPortListChangeHandler.registerRemovedPortCb([this](const rtmidiadapt::DeviceOnPort& devOnPort){
        std::cout << "Removed input " << devOnPort << std::endl;
        onRemovedInputPort(devOnPort);
    });
    m_outputPortListChangeHandler.registerNewPortCb([this](rtmidiadapt::PortIndex i, const rtmidiadapt::DeviceOnPort& devOnPort){
        std::cout << "New output " << i << " : " << devOnPort << std::endl;
        onNewOutputPort(i, devOnPort);
    });
    m_outputPortListChangeHandler.registerRemovedPortCb([this](const rtmidiadapt::DeviceOnPort& devOnPort){
        std::cout << "Removed output " << devOnPort << std::endl;
        onRemovedOutputPort(devOnPort);
    });
    return true;
}

void DeviceManager::update()
{
    m_inputPortListChangeHandler.update();
    m_outputPortListChangeHandler.update();
}


void DeviceManager::onNewInputPort(rtmidiadapt::PortIndex i, const rtmidiadapt::DeviceOnPort& devOnPort)
{
}

void DeviceManager::onRemovedInputPort(const rtmidiadapt::DeviceOnPort& devOnPort)
{

}

void DeviceManager::onNewOutputPort(rtmidiadapt::PortIndex i, const rtmidiadapt::DeviceOnPort& devOnPort)
{
    
    std::unique_ptr<Midi1Output<UsbMidiOut>> pMidiOut(new Midi1Output<UsbMidiOut>());
    if(!pMidiOut->medium().openPort(i))
    {
        std::cerr << "pMidiOut->medium.init(i) failed for i: " << i << " devOnPort: " << devOnPort << std::endl;
        return;
    }
    m_usbMidiOutputPorts.push_back(std::move( pMidiOut ));

    // get sound device properties like name, midi props
    // TODO: try it first over midi2
    // if it has no midi2 capabilities, then look in config files:
    DeviceId deviceId;
    if(!m_usb2DeviceMapper.map2Device(devOnPort, deviceId))
    {
        m_unmappedUsbMidiOutPorts.push_back(devOnPort);
        return;
    }
    MidiConfigParser midiConfigParser;
    if(!midiConfigParser.init(deviceId))
    {
        std::cerr << "midiConfigParser.init() failed for " << deviceId << std::endl;
        return;
    }
    
/*
    auto pSounDevice = m_rMidiOutPorts.add(m_rUsbMidiOutMediumFactory.createUsbMidiOutMedium(i, configParser));

    switch(deviceDescription.type())
    {
        case DeviceDesription::Type::SoundDevice
        {
            SoundDevice* pSoundDevice = m_rSoundDevices.get(deviceDescription.name();
            if(!pSoundDevice)
            {
                m_rSoundDevices.add(m_rSoundDeviceFactory.create(pSoundDevice, configParser));
            }
            pSoundDevice->setOutputMedium();
            break;
        }
        case DeviceDesription::Type::ControllerDevice
        {
            SoundDevice* pSoundDevice = m_rSoundDevices.get(deviceDescription.name();
            if(!pSoundDevice)
            {
                m_rSoundDevices.add(pSoundDevice, configParser);
            }
            pSoundDevice->setOutputMedium(m_rUsbMidiOutMediumFactory.createUsbMidiOutMedium(i, configParser));
            break;
        }
        default:
            std::cerr << "Undefined device type";
            return;
    }
*/
}

void DeviceManager::onRemovedOutputPort(const rtmidiadapt::DeviceOnPort& devOnPort)
{

}
