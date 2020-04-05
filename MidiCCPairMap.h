#ifndef MIDI_CC_PAIR_MAP_H
#define MIDI_CC_PAIR_MAP_H

#include <array>
#include "MidiMessage.h"

namespace midi
{

class CCPairMap
{
public:
    struct Data
    {
        enum class Role
        {
            SingleValue,
            MSB,
            LSB
        };
        static constexpr int NO_PAIR = -1;
        Role role{Role::SingleValue};
        int pairId{NO_PAIR};
    };
    inline constexpr bool setPair(int msbId, int lsbId) noexcept;
    inline constexpr const Data& operator[](int index) const noexcept;
    inline Data& operator[](int index) noexcept;
private:
    std::array<Data, Message<ControlChange>::RES_MAX> m_data;
};

inline
constexpr bool CCPairMap::setPair(int msbId, int lsbId) noexcept
{
   if (msbId < 0 || msbId >= m_data.size())
      return false;
   if (lsbId < 0 || lsbId >= m_data.size())
      return false;

   m_data[msbId].role   = Data::Role::MSB;
   m_data[msbId].pairId = lsbId;
   m_data[msbId].role   = Data::Role::LSB;
   m_data[lsbId].pairId = msbId;
   return true;
}

inline 
constexpr const CCPairMap::Data& CCPairMap::operator[](int index) const noexcept
{
    return m_data[index];
}

inline 
CCPairMap::Data& CCPairMap::operator[](int index) noexcept
{
    return m_data[index];
}

} // namespace midi

#endif