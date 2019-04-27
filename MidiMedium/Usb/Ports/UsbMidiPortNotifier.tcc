#include <algorithm> // std::set_difference
#include <iterator>  // std::inserter

template<class PortListProvider>
bool midi::PortNotifier<PortListProvider>::init()
{
    return m_portListProvider.init();
}

template<class PortListProvider>
void midi::PortNotifier<PortListProvider>::registerNewPortCb(NewPortCb cb)
{
    m_newPortCbs.push_back(cb);
}

template<class PortListProvider>
void midi::PortNotifier<PortListProvider>::registerRemovedPortCb(RemovedPortCb cb)
{
    m_removedPortCbs.push_back(cb);
}

template<class PortListProvider>
bool midi::PortNotifier<PortListProvider>::update()
{
    PortInfoSet actualPortsSet;
    PortInfoSet newPortsSet;
    PortInfoSet removedPortsSet;

    if(!m_portListProvider.getPorts(actualPortsSet))
    {
        return false;
    }

    std::set_difference(actualPortsSet.begin(), actualPortsSet.end(), 
                        m_lastPorts.begin(),    m_lastPorts.end(),
                        std::inserter(newPortsSet, newPortsSet.begin()));

    std::set_difference(m_lastPorts.begin(),    m_lastPorts.end(),
                        actualPortsSet.begin(), actualPortsSet.end(), 
                        std::inserter(removedPortsSet, removedPortsSet.begin()));

    for(const auto& portInfo : newPortsSet)
    {
        for(auto& portCb : m_newPortCbs)
        {
            portCb(portInfo.portIndex, portInfo.deviceOnPort);
        }
    }

    for(const auto& portInfo : removedPortsSet)
    {
        for(auto& portCb : m_removedPortCbs)
        {
            portCb(portInfo.deviceOnPort);
        }
    }
    m_lastPorts = actualPortsSet;
    return true;
}
