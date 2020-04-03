#ifndef MIDI_MESSAGE_H_
#define MIDI_MESSAGE_H_

#include "Midi.h"
#include "mpark/variant.hpp"

namespace midi
{

template< unsigned int SIZE >
class VoiceMsgLayout;

template <>
class VoiceMsgLayout<2> : public MsgLayout<2>
{
public:
	uint8_t channel() const { return getChannelFromVoiceCommand( command() ); }
	std::string toString() const { return command2Str(command()).append(" ch:")
																.append(std::to_string(channel()))
																.append(" ")
																.append(std::to_string(data1())); }
protected:
	inline VoiceMsgLayout(uint8_t command, uint8_t data1) :
		MsgLayout<2>(command, data1){}
};

template <>
class VoiceMsgLayout<3> : public MsgLayout<3>
{
public:
	uint8_t channel() const { return getChannelFromVoiceCommand( command() ); }
	std::string toString() const { return command2Str(command()).append(" ch:")
																.append(std::to_string(channel()))
																.append(" ")
																.append(std::to_string(data1()))
																.append(" ")
																.append(std::to_string(data2())); }
protected:
	inline VoiceMsgLayout(uint8_t command, uint8_t data1, uint8_t data2) :
		MsgLayout<3>(command, data1, data2){}
};


template <MsgType msgType>
class Message{};

template <> class Message<MsgOmni> : public VoiceMsgLayout<3> {};


template <>
class Message<NoteOff> : public VoiceMsgLayout<3>
{
public:
	Message(uint8_t channel, uint8_t noteNumber, uint8_t velocity) :
		VoiceMsgLayout<3>(Command<NoteOff>(channel), noteNumber, velocity){}
	uint8_t noteNumber() const {return data1();}
	uint8_t velocity() const {return data2();}
};

template <>
class Message<NoteOn> : public VoiceMsgLayout<3>
{
public:
	Message(uint8_t channel, uint8_t noteNumber, uint8_t velocity) :
		VoiceMsgLayout<3>(Command<NoteOn>(channel), noteNumber, velocity){}
	uint8_t noteNumber() const {return data1();}
	uint8_t velocity() const {return data2();}
};

template <>
class Message<AfterTouchPoly> : public VoiceMsgLayout<3>
{
public:
	Message(uint8_t channel, uint8_t noteNumber, uint8_t pressure) :
		VoiceMsgLayout<3>(Command<AfterTouchPoly>(channel), noteNumber, pressure){}
	uint8_t noteNumber() const {return data1();}
	uint8_t pressure() const {return data2();}
};

template <>
class Message<ControlChange> : public VoiceMsgLayout<3>
{
public:
	Message(uint8_t channel, uint8_t controllerNumber, uint8_t controllerValue) :
		VoiceMsgLayout<3>(Command<ControlChange>(channel), controllerNumber, controllerValue){}
	constexpr uint8_t controllerNumber() const {return data1();}
	constexpr uint8_t controllerValue() const {return data2();}
	constexpr float getRelativeValue() const noexcept { return controllerValue() / 128.0f; }
};

template <>
class Message<ProgramChange> : public VoiceMsgLayout<2>
{
public:
	Message(uint8_t channel, uint8_t programNumber) :
		VoiceMsgLayout<2>(Command<ProgramChange>(channel), programNumber){}
	uint8_t programNumber() const {return data1();}
};

template <>
class Message<AfterTouchChannel> : public VoiceMsgLayout<2>
{
public:
	Message(uint8_t channel, uint8_t value) :
		VoiceMsgLayout<2>(Command<AfterTouchChannel>(channel), value){}
	uint8_t value() const {return data1();}
};

template <>
class Message<PitchBend> : public VoiceMsgLayout<3>
{
public:
	Message(uint8_t channel, int16_t value) :
		VoiceMsgLayout<3>(Command<PitchBend>(channel), (value + 0x2000) & 0x7F, ((value + 0x2000) >> 7) & 0x7F){}
	int16_t value() const {return (static_cast<uint16_t>(data1()) | (static_cast<uint16_t>(data2()) << 7)) - 0x2000;}
};

template <>
class Message<TimeCodeQuarterFrame> : public MsgLayout<2>
{
public:
	Message(uint8_t data) :
		MsgLayout<2>(Command<TimeCodeQuarterFrame>(), data){}
	Message(uint8_t nibbleType, uint8_t nibbleValue) :
		MsgLayout<2>(Command<TimeCodeQuarterFrame>(),  ((nibbleType & 0x07) << 4) | (nibbleValue & 0x0F) ){}
	uint8_t value() const {return data1();}
};

template <>
class Message<SongPosition> : public MsgLayout<3>
{
public:
	Message(uint16_t beats) :
		MsgLayout<3>(Command<SongPosition>(), beats & 0x7F, (beats >> 7) & 0x7F){}
	uint16_t beats() const {return static_cast<uint16_t>(data1()) | (static_cast<uint16_t>(data2()) << 7);}
};

template <>
class Message<SongSelect> : public MsgLayout<2>
{
public:
	Message(uint8_t song) :
		MsgLayout<2>(Command<SongSelect>(), song){}
	uint8_t song() const {return data1();}
};

template<> 
class Message<TuneRequest> : public MsgLayout<1>
{
public:
	Message() :
		MsgLayout<1>(Command<TuneRequest>()){}
};

template<> 
class Message<Clock> : public MsgLayout<1>
{
public:
	Message() :
		MsgLayout<1>(Command<Clock>()){}
};

template<> 
class Message<Start> : public MsgLayout<1> 
{
public:
	Message() :
		MsgLayout<1>(Command<Start>()){}
};

template<> 
class Message<Continue> : public MsgLayout<1> 
{
public:
	Message() :
		MsgLayout<1>(Command<Continue>()){}
};

template<> 
class Message<Stop> : public MsgLayout<1> 
{
public:
	Message() :
		MsgLayout<1>(Command<Stop>()){}
};

template<> 
class Message<ActiveSensing> : public MsgLayout<1>
{
public:
	Message() :
		MsgLayout<1>(Command<ActiveSensing>()){}
};

template<> 
class Message<SystemReset> : public MsgLayout<1> 
{
public:
	Message() :
		MsgLayout<1>(Command<SystemReset>()){}
};

struct RpnBase
{
	constexpr RpnBase(uint8_t channelNr) noexcept : channelNr(channelNr) {}
	static constexpr uint8_t UNSET = 0xFF;
	static constexpr uint8_t MAX = 127;
	static constexpr int CC_ID_VALUE_MSB = 6;
	static constexpr int CC_ID_VALUE_LSB = 38;
	static constexpr uint8_t NULL_VALUE = 127;

	uint8_t channelNr;
	uint8_t idMsb{UNSET};
	uint8_t idLsb{UNSET};
	uint8_t valueMsb{UNSET};
	uint8_t valueLsb{UNSET};
	std::string toString() const{
		return std::string("ch:")
				.append(std::to_string(channelNr))
				.append(" ")
				.append(std::to_string(idMsb))
				.append(" ")
				.append(std::to_string(idLsb))
				.append(" ")
				.append(std::to_string(valueMsb))
				.append(" ")
				.append(std::to_string(valueLsb));
	}
	constexpr bool idIsValid() const noexcept{
		return (idMsb != UNSET) && (idLsb != UNSET);
	}
	constexpr bool isCleared() const noexcept{
		return (idMsb == MAX) && (idLsb == MAX);
	}
};
template<>
struct Message<RPN> : public RpnBase
{
	constexpr Message<RPN>(uint8_t channelNr) noexcept : RpnBase(channelNr) {}
	std::string toString() const{
		return std::string("RPN ") + RpnBase::toString();
	}
	static constexpr int CC_ID_MSB = 101;
	static constexpr int CC_ID_LSB = 100;
};

template<>
struct Message<NRPN> : public RpnBase
{
	constexpr Message<NRPN>(uint8_t channelNr) noexcept : RpnBase(channelNr) {}
	std::string toString() const{
		return std::string("NRPN ") + RpnBase::toString();
	}
	static constexpr int CC_ID_MSB = 99;
	static constexpr int CC_ID_LSB = 98;
};


using MidiMessage = mpark::variant<
	mpark::monostate,
 	Message<NoteOff>,
	Message<NoteOn>,
	Message<AfterTouchPoly>,
	Message<ControlChange>,
	Message<ProgramChange>,
	Message<AfterTouchChannel>,
	Message<PitchBend>,
	Message<TimeCodeQuarterFrame>,
	Message<SongPosition>,
	Message<SongSelect>,
	Message<TuneRequest>,
	Message<Clock>,
	Message<Start>,
	Message<Continue>,
	Message<Stop>,
	Message<ActiveSensing>,
	Message<SystemReset>,
	Message<RPN>,
	Message<NRPN>
	>;

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

} //namespace midi

#endif //MIDI_INPUT_MESSAGE_H_
