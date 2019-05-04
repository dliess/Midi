#include <algorithm> // std::set_difference
#include <iterator>  // std::inserter
#include <vector>
/*
namespace util
{
inline
std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
} // namespace util
*/
template<class PortListProvider>
bool midi::PortNotifier<PortListProvider>::init()
{
    return m_portListProvider.init();
}

template<class PortListProvider>
void midi::PortNotifier<PortListProvider>::registerNewPortCb(NewPortCb cb, const CbFilter& filter)
{
    m_newPortCbs.push_back({cb, filter});
}

template<class PortListProvider>
void midi::PortNotifier<PortListProvider>::registerRemovedPortCb(RemovedPortCb cb, const CbFilter& filter)
{
    m_removedPortCbs.push_back({cb, filter});
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
            if(included(portInfo.deviceOnPort.getDeviceName(), portCb.filter.includes))
            {
                if(!excluded(portInfo.deviceOnPort.getDeviceName(), portCb.filter.excludes))
                {
                    portCb.cb(portInfo.portIndex, portInfo.deviceOnPort);
                }
            }
        }
    }

    for(const auto& portInfo : removedPortsSet)
    {
        for(auto& portCb : m_removedPortCbs)
        {
            if(included(portInfo.deviceOnPort.getDeviceName(), portCb.filter.includes))
            {
                if(!excluded(portInfo.deviceOnPort.getDeviceName(), portCb.filter.excludes))
                {
                    portCb.cb(portInfo.deviceOnPort);
                }
            }
        }
    }
    m_lastPorts = actualPortsSet;
    return true;
}

template<class PortListProvider>
bool midi::PortNotifier<PortListProvider>::included(const std::string& devName, const std::vector<std::string>& includes)
{
    if(0 == includes.size())
    {
        return true;
    }
    for(auto& include : includes)
    {
        if(0 == devName.find(include))
        {
            return true;
        }
    }
    return false;
}

template<class PortListProvider>
bool midi::PortNotifier<PortListProvider>::excluded(const std::string& devName, const std::vector<std::string>& excludes)
{
    if(0 == excludes.size())
    {
        return false;
    }
    for(auto& exclude : excludes)
    {
        if(0 == devName.find(exclude))
        {
            return true;
        }
    }
    return false;
}
