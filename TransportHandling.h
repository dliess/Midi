#ifndef MIDI_TRANSPORT_HANDLING_H
#define MIDI_TRANSPORT_HANDLING_H

#include <vector>
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
    void registerStartedChangedCb(StartedChangedCb cb);
    using TransportMaskChangedCb = std::function<void(bool)>;
    void registerTransportMaskChangedCb(TransportMaskChangedCb cb);

private:
    Derived& derived();
    const Derived& derived() const;
    bool m_started{false};
    bool m_transportMasked{false};
    std::vector<StartedChangedCb> m_startedChangedCbs;
    std::vector<TransportMaskChangedCb> m_transportMaskChangedCbs;
};
}   // namespace midi


#include "TransportHandling.tcc"
#endif