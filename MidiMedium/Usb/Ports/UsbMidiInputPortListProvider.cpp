#include "UsbMidiInputPortListProvider.h"

#ifdef __RTMIDI_ADDED__
    #include <RtMidi.h>
#else
    #include <rtmidi/RtMidi.h>
#endif

#include <cassert>
#include <iostream>

using namespace midi;

InputPortListProvider::InputPortListProvider() : m_pMidiIn(nullptr) {}

InputPortListProvider::~InputPortListProvider() { delete m_pMidiIn; }

bool InputPortListProvider::init()
{
   try
   {
      if (!m_pMidiIn)
         m_pMidiIn = new RtMidiIn(__RTMIDI_API__);
   }
   catch (RtMidiError &error)
   {
      error.printMessage();
      return false;
   }
   return true;
}

bool InputPortListProvider::getPorts(PortInfoSet &rPortSet)
{
   assert(m_pMidiIn != nullptr);
   for (unsigned int i = 0; i < m_pMidiIn->getPortCount(); ++i)
   {
      try
      {
         rPortSet.insert(PortInfo(i, m_pMidiIn->getPortName(i)));
      }
      catch (RtMidiError &error)
      {
         error.printMessage();
         return false;
      }
   }
   return true;
}