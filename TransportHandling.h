#ifndef MIDI_TRANSPORT_HANDLING_H
#define MIDI_TRANSPORT_HANDLING_H

#include <vector>
#include <tuple>
#include <functional>

namespace midi
{

template<class Derived>
class TransportHandling
{
public:
    bool start();
    bool stop();
    bool toggleStartStop();
    void setTransportMasked(bool masked);
    using StartedChangedCb = std::function<void(bool)>;
    void registerStartedChangedCb(void* id, StartedChangedCb cb);
    using TransportMaskChangedCb = std::function<void(bool)>;
    void registerTransportMaskChangedCb(void* id, TransportMaskChangedCb cb);
    void unregisterStartedChangedCb(void* id);
    void unregisterTransportMaskChangedCb(void* id);
private:
    Derived& derived();
    const Derived& derived() const;
    bool m_started{false};
    bool m_transportMasked{false};
    std::vector<std::pair<void*, StartedChangedCb>> m_startedChangedCbs;
    std::vector<std::pair<void*, TransportMaskChangedCb>> m_transportMaskChangedCbs;
};
}   // namespace midi


#include "TransportHandling.tcc"
#endif