#ifndef MIDI_MESSAGE_H_
#define MIDI_MESSAGE_H_

#include <fmt/format.h>

#include "Midi.h"
#include "mpark/variant.hpp"

namespace midi
{
template <unsigned int SIZE> class VoiceMsgLayout;

template <> class VoiceMsgLayout<2> : public MsgLayout<2>
{
public:
   constexpr uint8_t channel() const noexcept
   {
      return getChannelFromVoiceCommand(command());
   }
   std::string toString() const noexcept
   {
      return fmt::format("{0} ch:{1} {2}", command2Str(command()), channel(),
                         data1());
   }

protected:
   inline constexpr VoiceMsgLayout(uint8_t command, uint8_t data1) noexcept :
       MsgLayout<2>(command, data1)
   {
   }
};

template <> class VoiceMsgLayout<3> : public MsgLayout<3>
{
public:
   constexpr uint8_t channel() const noexcept
   {
      return getChannelFromVoiceCommand(command());
   }
   std::string toString() const noexcept
   {
      return fmt::format("{0} ch:{1} {2} {3}", command2Str(command()),
                         channel(), data1(), data2());
   }

protected:
   inline constexpr VoiceMsgLayout(uint8_t command, uint8_t data1,
                                   uint8_t data2) noexcept :
       MsgLayout<3>(command, data1, data2)
   {
   }
};

template <MsgType msgType> class Message
{
};

template <> class Message<MsgOmni> : public VoiceMsgLayout<3>
{
};

template <> class Message<NoteOff> : public VoiceMsgLayout<3>
{
public:
   constexpr Message(uint8_t channel, uint8_t noteNumber,
                     uint8_t velocity) noexcept :
       VoiceMsgLayout<3>(Command<NoteOff>(channel), noteNumber, velocity)
   {
   }
   constexpr uint8_t noteNumber() const noexcept { return data1(); }
   constexpr uint8_t velocity() const noexcept { return data2(); }
   constexpr float relativeVelocity() const noexcept
   {
      return velocity() / float(1 << 7);
   }
};

template <> class Message<NoteOn> : public VoiceMsgLayout<3>
{
public:
   constexpr Message(uint8_t channel, uint8_t noteNumber,
                     uint8_t velocity) noexcept :
       VoiceMsgLayout<3>(Command<NoteOn>(channel), noteNumber, velocity)
   {
   }
   constexpr uint8_t noteNumber() const noexcept { return data1(); }
   constexpr uint8_t velocity() const noexcept { return data2(); }
   constexpr float relativeVelocity() const noexcept
   {
      return velocity() / float(1 << 7);
   }
};

template <> class Message<AfterTouchPoly> : public VoiceMsgLayout<3>
{
public:
   constexpr Message(uint8_t channel, uint8_t noteNumber,
                     uint8_t pressure) noexcept :
       VoiceMsgLayout<3>(Command<AfterTouchPoly>(channel), noteNumber, pressure)
   {
   }
   constexpr uint8_t noteNumber() const noexcept { return data1(); }
   constexpr uint8_t pressure() const noexcept { return data2(); }
   constexpr float relativePressure() const noexcept
   {
      return pressure() / float(1 << 7);
   }
};

template <> class Message<ControlChange> : public VoiceMsgLayout<3>
{
public:
   static constexpr int RES_MAX = (1 << 7);

   constexpr Message(uint8_t channel, uint8_t controllerNumber,
                     uint8_t controllerValue) noexcept :
       VoiceMsgLayout<3>(Command<ControlChange>(channel), controllerNumber,
                         controllerValue)
   {
   }
   static Message<ControlChange> fromRelativeValue(uint8_t channelNr,
                                                   uint8_t controllerNumber,
                                                   float value)
   {
      return Message(channelNr, controllerNumber, value * RES_MAX);
   }
   static Message<ControlChange> fromRelativeValue(uint8_t channelNr,
                                                   uint8_t controllerNumber,
                                                   float value, uint16_t from,
                                                   uint16_t to)
   {
      return Message(channelNr, controllerNumber,
                     from + (value * (to - from + 1)));
   }
   constexpr uint8_t controllerNumber() const noexcept { return data1(); }
   constexpr uint8_t controllerValue() const noexcept { return data2(); }
   constexpr float getRelativeValue() const noexcept
   {
      return controllerValue() / float(RES_MAX);
   }
   constexpr float getRelativeValue(uint16_t from, uint16_t to) const noexcept
   {
      if (controllerValue() <= from)
         return 0.0;
      if (controllerValue() > to)
         return 1.0;
      const uint16_t res = to - from + 1;
      return (2.0 * (controllerValue() - from) + 1) / (2.0 * res);
   }
};

template <> class Message<ProgramChange> : public VoiceMsgLayout<2>
{
public:
   constexpr Message(uint8_t channel, uint8_t programNumber) noexcept :
       VoiceMsgLayout<2>(Command<ProgramChange>(channel), programNumber)
   {
   }
   constexpr uint8_t programNumber() const noexcept { return data1(); }
};

template <> class Message<AfterTouchChannel> : public VoiceMsgLayout<2>
{
public:
   constexpr Message(uint8_t channel, uint8_t value) noexcept :
       VoiceMsgLayout<2>(Command<AfterTouchChannel>(channel), value)
   {
   }
   constexpr uint8_t value() const noexcept { return data1(); }
   constexpr float relativeValue() const noexcept
   {
      return value() / float(1 << 7);
   }
};

template <> class Message<PitchBend> : public VoiceMsgLayout<3>
{
public:
   constexpr Message(uint8_t channel, int16_t value) noexcept :
       VoiceMsgLayout<3>(Command<PitchBend>(channel), (value + 0x2000) & 0x7F,
                         ((value + 0x2000) >> 7) & 0x7F)
   {
   }
   constexpr int16_t value() const noexcept
   {
      return (static_cast<uint16_t>(data1()) |
              (static_cast<uint16_t>(data2()) << 7)) -
             0x2000;
   }
   constexpr float relativeValue() const noexcept
   {
      return (value() * 2) / float(1 << 14);
   }
};

template <> class Message<TimeCodeQuarterFrame> : public MsgLayout<2>
{
public:
   constexpr Message(uint8_t data) noexcept :
       MsgLayout<2>(Command<TimeCodeQuarterFrame>(), data)
   {
   }
   constexpr Message(uint8_t nibbleType, uint8_t nibbleValue) noexcept :
       MsgLayout<2>(Command<TimeCodeQuarterFrame>(),
                    ((nibbleType & 0x07) << 4) | (nibbleValue & 0x0F))
   {
   }
   constexpr uint8_t value() const noexcept { return data1(); }
};

template <> class Message<SongPosition> : public MsgLayout<3>
{
public:
   constexpr Message(uint16_t beats) noexcept :
       MsgLayout<3>(Command<SongPosition>(), beats & 0x7F, (beats >> 7) & 0x7F)
   {
   }
   constexpr uint16_t beats() const noexcept
   {
      return static_cast<uint16_t>(data1()) |
             (static_cast<uint16_t>(data2()) << 7);
   }
};

template <> class Message<SongSelect> : public MsgLayout<2>
{
public:
   constexpr Message(uint8_t song) noexcept :
       MsgLayout<2>(Command<SongSelect>(), song)
   {
   }
   constexpr uint8_t song() const noexcept { return data1(); }
};

template <> class Message<TuneRequest> : public MsgLayout<1>
{
public:
   constexpr Message() noexcept : MsgLayout<1>(Command<TuneRequest>()) {}
};

template <> class Message<Clock> : public MsgLayout<1>
{
public:
   constexpr Message() noexcept : MsgLayout<1>(Command<Clock>()) {}
};

template <> class Message<Start> : public MsgLayout<1>
{
public:
   constexpr Message() noexcept : MsgLayout<1>(Command<Start>()) {}
};

template <> class Message<Continue> : public MsgLayout<1>
{
public:
   constexpr Message() noexcept : MsgLayout<1>(Command<Continue>()) {}
};

template <> class Message<Stop> : public MsgLayout<1>
{
public:
   constexpr Message() noexcept : MsgLayout<1>(Command<Stop>()) {}
};

template <> class Message<ActiveSensing> : public MsgLayout<1>
{
public:
   constexpr Message() noexcept : MsgLayout<1>(Command<ActiveSensing>()) {}
   // std::string toString() const { return "ActiveSensing";}
};

template <> class Message<SystemReset> : public MsgLayout<1>
{
public:
   constexpr Message() noexcept : MsgLayout<1>(Command<SystemReset>()) {}
   // std::string toString() const { return "SystemReset";}
};

struct RpnBase
{
   static constexpr int BASE_RES         = 7;
   static constexpr int BASE_RES_BITMASK = (1 << BASE_RES) - 1;
   static constexpr int RES_MAX          = (1 << (2 * BASE_RES));

   constexpr RpnBase(uint8_t channelNr) noexcept : channelNr(channelNr) {}
   constexpr RpnBase(uint8_t channelNr, int idMsb, int idLsb,
                     uint16_t value) noexcept :
       channelNr(channelNr),
       idMsb(idMsb),
       idLsb(idLsb),
       valueMsb(value >> BASE_RES),
       valueLsb(value & BASE_RES_BITMASK)
   {
   }
   static RpnBase fromRelativeValue(uint8_t channelNr, int idMsb, int idLsb,
                                    float value)
   {
      return RpnBase(channelNr, idMsb, idLsb, value * RES_MAX);
   }
   static RpnBase fromRelativeValue(uint8_t channelNr, int idMsb, int idLsb,
                                    float value, uint16_t from, uint16_t to)
   {
      return RpnBase(channelNr, idMsb, idLsb, from + (value * (to - from + 1)));
   }

   static constexpr uint8_t UNSET       = 0xFF;
   static constexpr uint8_t MAX         = 127;
   static constexpr int CC_ID_VALUE_MSB = 6;
   static constexpr int CC_ID_VALUE_LSB = 38;
   static constexpr uint8_t NULL_VALUE  = 127;

   uint8_t channelNr;
   uint8_t idMsb{UNSET};
   uint8_t idLsb{UNSET};
   uint8_t valueMsb{UNSET};
   uint8_t valueLsb{UNSET};

   constexpr uint8_t channel() const noexcept { return channelNr; }

   std::string toString() const noexcept
   {
      return fmt::format("ch:{0} [{1},{2}] [{3},{4}]", channelNr, idMsb, idLsb,
                         valueMsb, valueLsb);
   }
   constexpr bool idIsValid() const noexcept
   {
      return (idMsb != UNSET) && (idLsb != UNSET);
   }
   constexpr bool isCleared() const noexcept
   {
      return (idMsb == MAX) && (idLsb == MAX);
   }
   constexpr int getValue() const noexcept
   {
      return ((valueMsb << 7) + valueLsb);
   }
   constexpr float getRelativeValue() const noexcept
   {
      return ((valueMsb << 7) + valueLsb) / float(RES_MAX);
   }
   constexpr float getRelativeValue(uint16_t from, uint16_t to) const noexcept
   {
      const uint16_t value = getValue();
      if (value <= from)
         return 0.0;
      if (value > to)
         return 1.0;
      const uint16_t res = to - from + 1;
      return (2.0 * (value - from) + 1) / (2.0 * res);
   }
   constexpr uint16_t getId() const noexcept { return (idMsb << 7) + idLsb; }
};
template <> struct Message<RPN> : public RpnBase
{
   constexpr Message<RPN>(uint8_t channelNr) noexcept : RpnBase(channelNr) {}
   constexpr Message<RPN>(uint8_t channelNr, int idMsb, int idLsb,
                          uint16_t value) noexcept :
       RpnBase(channelNr, idMsb, idLsb, value)
   {
   }
   static Message<RPN> fromRelativeValue(uint8_t channelNr, int idMsb,
                                         int idLsb, float value)
   {
      return Message<RPN>(channelNr, idMsb, idLsb, value * RES_MAX);
   }
   static Message<RPN> fromRelativeValue(uint8_t channelNr, int idMsb,
                                         int idLsb, float value, uint16_t from,
                                         uint16_t to)
   {
      return Message<RPN>(channelNr, idMsb, idLsb, from + value * (to - from + 1));
   }

   std::string toString() const noexcept
   {
      return std::string("RPN ") + RpnBase::toString();
   }

   static constexpr int CC_ID_MSB = 101;
   static constexpr int CC_ID_LSB = 100;
};

template <> struct Message<NRPN> : public RpnBase
{
   constexpr Message<NRPN>(uint8_t channelNr) noexcept : RpnBase(channelNr) {}
   constexpr Message<NRPN>(uint8_t channelNr, int idMsb, int idLsb,
                           uint16_t value) noexcept :
       RpnBase(channelNr, idMsb, idLsb, value)
   {
   }
   static Message<NRPN> fromRelativeValue(uint8_t channelNr, int idMsb,
                                          int idLsb, float value)
   {
      return Message<NRPN>(channelNr, idMsb, idLsb, value * RES_MAX);
   }
   static Message<NRPN> fromRelativeValue(uint8_t channelNr, int idMsb,
                                          int idLsb, float value, uint16_t from,
                                          uint16_t to)
   {
      return Message<NRPN>(channelNr, idMsb, idLsb, from + value * (to - from + 1));
   }
   std::string toString() const noexcept
   {
      return std::string("NRPN ") + RpnBase::toString();
   }

   static constexpr int CC_ID_MSB = 99;
   static constexpr int CC_ID_LSB = 98;
};

template <> struct Message<ControlChangeHighRes>
{
   uint8_t channelId;
   uint8_t msbCCId;
   uint8_t lsbCCId;
   uint8_t msbValue;
   uint8_t lsbValue;

   static constexpr int RES_MAX = 1 << 14;

   constexpr Message(uint8_t channelId, uint8_t msbId, uint8_t lsbId,
                     uint16_t value) noexcept :
       channelId(channelId),
       msbCCId(msbId),
       lsbCCId(lsbId),
       msbValue(value >> 7),
       lsbValue(value & 0x7F)
   {
   }

   constexpr Message(const Message<ControlChange>& msb,
                     const Message<ControlChange>& lsb) noexcept :
       channelId(msb.channel()),
       msbCCId(msb.controllerNumber()),
       lsbCCId(lsb.controllerNumber()),
       msbValue(msb.controllerValue()),
       lsbValue(lsb.controllerValue())
   {
   }

   static Message<ControlChangeHighRes> fromRelativeValue(uint8_t channelNr,
                                                          int idMsb, int idLsb,
                                                          float value)
   {
      return Message(channelNr, idMsb, idLsb, value * RES_MAX);
   }
   static Message<ControlChangeHighRes> fromRelativeValue(uint8_t channelNr,
                                                          int idMsb, int idLsb,
                                                          float value,
                                                          uint16_t from,
                                                          uint16_t to)
   {
      return Message(channelNr, idMsb, idLsb, from + (value * (to - from + 1)));
   }

   constexpr uint8_t channel() const noexcept { return channelId; }

   constexpr uint8_t controllerValue() const noexcept
   {
      return (msbValue << 7) + lsbValue;
   }

   constexpr float getRelativeValue() const noexcept
   {
      return ((msbValue << 7) + lsbValue) / float(RES_MAX);
   }
   constexpr float getRelativeValue(uint16_t from, uint16_t to) const noexcept
   {
      const uint16_t value = controllerValue();
      if (value <= from)
         return 0.0;
      if (value > to)
         return 1.0;
      const uint16_t res = to - from + 1;
      return (2.0 * (value - from) + 1) / (2.0 * res);
   }

   constexpr uint8_t controllerNumber() const noexcept { return msbCCId; }

   std::pair<Message<ControlChange>, Message<ControlChange>> toCCPair()
       const noexcept
   {
      return std::make_pair(
          Message<ControlChange>(channelId, msbCCId, msbValue),
          Message<ControlChange>(channelId, lsbCCId, lsbValue));
   }

   std::string toString() const noexcept
   {
      return fmt::format("CCHighRes ch:{0} [{1},{2}] [{3},{4}]", channelId,
                         msbCCId, lsbCCId, msbValue, lsbValue);
   }
};

template <> struct Message<SystemExclusive> : public std::vector<uint8_t>
{
   std::string toString() const noexcept
   {
      std::string ret;
      for(const auto&e : *this)
      {
         ret.append(fmt::format("{}, ", int(e)));
      }
      return ret;
   }
};

using MidiMessage = mpark::variant<
    mpark::monostate, Message<NoteOff>, Message<NoteOn>,
    Message<AfterTouchPoly>, Message<ControlChange>, Message<ProgramChange>,
    Message<AfterTouchChannel>, Message<PitchBend>,
    Message<TimeCodeQuarterFrame>, Message<SongPosition>, Message<SongSelect>,
    Message<TuneRequest>, Message<Clock>, Message<Start>, Message<Continue>,
    Message<Stop>, Message<ActiveSensing>, Message<SystemReset>, Message<RPN>,
    Message<NRPN>, Message<ControlChangeHighRes>, Message<SystemExclusive>>;

template <class... Ts> struct overload : Ts...
{
   using Ts::operator()...;
};
template <class... Ts> overload(Ts...) -> overload<Ts...>;

inline std::string toString(const MidiMessage& msg)
{
   return mpark::visit(
       midi::overload{[](const mpark::monostate& mono) -> std::string {
                         return "Unknown Midi Message";
                      },
                      [](auto&& all) -> std::string {
                         return std::string(all.toString());
                      }},
       msg);
}

}   // namespace midi

#endif   // MIDI_INPUT_MESSAGE_H_
