#ifndef MIDI_XRPN_COMMON_H
#define MIDI_XRPN_COMMON_H

#include <cstdint>

namespace midi::xrpn
{

static constexpr int CC_ID_RPN_ID_MSB = 101;
static constexpr int CC_ID_RPN_ID_LSB = 100;
static constexpr int CC_ID_NRPN_ID_MSB = 99;
static constexpr int CC_ID_NRPN_ID_LSB = 98;
static constexpr int CC_ID_VALUE_MSB = 6;
static constexpr int CC_ID_VALUE_LSB = 38;
static constexpr uint8_t NULL_VALUE = 127;

} // namespace midi::xrpn

#endif