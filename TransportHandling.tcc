#include "MidiMessage.h"

namespace midi
{

template <class Derived>
Derived& TransportHandling<Derived>::derived()
{
    return *static_cast<Derived*>(this);
}


template <class Derived>
const Derived& TransportHandling<Derived>::derived() const
{
    return *static_cast<Derived*>(this);
}

template <class Derived> bool TransportHandling<Derived>::start()
{
   if (m_transportMasked)
   {
      return true;
   }
   bool ret{false};
   ret       = derived().send(Message<Start>());
   m_started = true;
   for (auto cb : m_startedChangedCbs) cb(m_started);
   return ret;
}

template <class Derived> bool TransportHandling<Derived>::stop()
{
   if (m_transportMasked)
   {
      return true;
   }
   bool ret{false};
   ret       = derived().send(Message<Stop>());
   m_started = false;
   for (auto cb : m_startedChangedCbs) cb(m_started);
   return ret;
}

template <class Derived> bool TransportHandling<Derived>::toggleStartStop()
{
   if (!m_started)
   {
      return start();
   }
   else
   {
      return stop();
   }
}

template <class Derived>
void TransportHandling<Derived>::setTransportMasked(bool masked)
{
   if (m_transportMasked == masked)
   {
      return;
   }
   if (masked)
   {
      if (m_started)
         stop();
   }
   m_transportMasked = masked;
   for (auto cb : m_transportMaskChangedCbs) cb(m_transportMasked);
}

template <class Derived>
void TransportHandling<Derived>::registerStartedChangedCb(StartedChangedCb cb)
{
   m_startedChangedCbs.push_back(cb);
}

template <class Derived>
void TransportHandling<Derived>::registerTransportMaskChangedCb(
    TransportMaskChangedCb cb)
{
   m_transportMaskChangedCbs.push_back(cb);
}

}   // namespace midi