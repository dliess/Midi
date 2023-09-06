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
            LSB,
            BottomHalf,
            TopHalf
        };
        static constexpr int NO_PAIR = -1;
        Role role{Role::SingleValue};
        int pairId{NO_PAIR};
        std::array<uint8_t, midi::NUM_CHANNELS> lastValue{0};
    };
    constexpr bool setHighResPair(int msbId, int lsbId) noexcept;
    constexpr bool setDoubleResPair(int bottomHalfId, int topHalfId) noexcept;
    constexpr const Data& operator[](int index) const noexcept;
    Data& operator[](int index) noexcept;
private:
    std::array<Data, Message<ControlChange>::RES_MAX> m_data;
};

inline
constexpr bool CCPairMap::setHighResPair(int msbId, int lsbId) noexcept
{
   if (msbId < 0 || msbId >= m_data.size())
      return false;
   if (lsbId < 0 || lsbId >= m_data.size())
      return false;

   m_data[msbId].role   = Data::Role::MSB;
   m_data[msbId].pairId = lsbId;
   m_data[lsbId].role   = Data::Role::LSB;
   m_data[lsbId].pairId = msbId;
   return true;
}

inline
constexpr bool CCPairMap::setDoubleResPair(int bottomHalfId, int topHalfId) noexcept
{
   if (bottomHalfId < 0 || bottomHalfId >= m_data.size())
      return false;
   if (topHalfId < 0 || topHalfId >= m_data.size())
      return false;

   m_data[bottomHalfId].role   = Data::Role::BottomHalf;
   m_data[bottomHalfId].pairId = topHalfId;
   m_data[topHalfId].role   = Data::Role::TopHalf;
   m_data[topHalfId].pairId = bottomHalfId;
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