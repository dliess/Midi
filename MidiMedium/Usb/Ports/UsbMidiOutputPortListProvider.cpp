#include "UsbMidiOutputPortListProvider.h"
#include <RtMidi.h>
#include <iostream>

using namespace midi;
  
OutputPortListProvider::OutputPortListProvider() :
    m_pMidiOut(nullptr)
{
}

OutputPortListProvider::~OutputPortListProvider()
{
    delete m_pMidiOut;
}

bool OutputPortListProvider::init()
{
    try
    {
        if(!m_pMidiOut)
            m_pMidiOut = new RtMidiOut(__RTMIDI_API__);
    }
    catch ( RtMidiError &error )
    {
        error.printMessage();
        return false;
    }
    return true;
}

bool OutputPortListProvider::getPorts(PortInfoSet& rPortSet)
{
    for(unsigned int i=0; i < m_pMidiOut->getPortCount(); ++i) 
    {
        try {
            rPortSet.insert(PortInfo(i, m_pMidiOut->getPortName(i)));
        }
        catch ( RtMidiError &error ) {
            error.printMessage();
            return false;
        }
    }
    return true;
}