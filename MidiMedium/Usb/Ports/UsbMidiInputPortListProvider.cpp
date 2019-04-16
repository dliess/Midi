#include "UsbMidiInputPortListProvider.h"
#include <rtmidi/RtMidi.h>
#include <iostream>

using namespace midi;
  
InputPortListProvider::InputPortListProvider() :
    m_pMidiIn(nullptr)
{
}

InputPortListProvider::~InputPortListProvider()
{
    delete m_pMidiIn;
}

bool InputPortListProvider::init()
{
    try
    {
        if(!m_pMidiIn)
            m_pMidiIn = new RtMidiIn;
    }
    catch ( RtMidiError &error )
    {
        error.printMessage();
        return false;
    }
    return true;
}

bool InputPortListProvider::getPorts(PortInfoSet& rPortSet)
{
    for(unsigned int i=0; i < m_pMidiIn->getPortCount(); ++i) 
    {
        try {
            rPortSet.insert(PortInfo(i, m_pMidiIn->getPortName(i)));
        }
        catch ( RtMidiError &error ) {
            error.printMessage();
            return false;
        }
    }
    return true;
}