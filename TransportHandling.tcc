#include "MidiMessage.h"
namespace midi
{
template <class Derived> Derived& TransportHandling<Derived>::derived()
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
   if (!m_started)
   {
      ret       = derived().sendStart();
      m_started = true;
      for (auto& e : m_startedChangedCbs) { e.second(m_started); };
   }
   return ret;
}

template <class Derived> bool TransportHandling<Derived>::stop()
{
   if (m_transportMasked)
   {
      return true;
   }
   bool ret{false};
   if (m_started)
   {
      ret       = derived().sendStop();
      m_started = false;
      for (auto& e : m_startedChangedCbs) { e.second(m_started); };
   }
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
   for (auto& e : m_transportMaskChangedCbs) { e.second(m_transportMasked); }
}

template <class Derived>
void TransportHandling<Derived>::registerStartedChangedCb(void* id,
                                                          StartedChangedCb cb)
{
   m_startedChangedCbs.push_back(std::make_pair(id, cb));
   // TODO: m_startedChangedCbs.emplace_back(std::piecewise_construct, id, cb);
}

template <class Derived>
void TransportHandling<Derived>::registerTransportMaskChangedCb(
    void* id, TransportMaskChangedCb cb)
{
   m_transportMaskChangedCbs.push_back(std::make_pair(id, cb));
   // TODO: m_transportMaskChangedCbs.emplace_back(std::piecewise_construct, id,
   // cb);
}

template <class Derived>
void TransportHandling<Derived>::unregisterStartedChangedCb(void* id)
{
   for (auto it = m_startedChangedCbs.begin(); it != m_startedChangedCbs.end();
        ++it)
   {
      if (it->first == id)
      {
         m_startedChangedCbs.erase(it--);
      }
   }
}

template <class Derived>
void TransportHandling<Derived>::unregisterTransportMaskChangedCb(void* id)
{
   for (auto it = m_transportMaskChangedCbs.begin();
        it != m_transportMaskChangedCbs.end(); ++it)
   {
      if (it->first == id)
      {
         m_transportMaskChangedCbs.erase(it--);
      }
   }
}

}   // namespace midi