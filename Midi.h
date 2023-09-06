#ifndef SOUND_DEVICE_MIDI_H
#define SOUND_DEVICE_MIDI_H

#include <cstdint>
#include <string>

#include <fmt/format.h>


namespace midi
{

static constexpr uint32_t BAUDRATE         = 31250;
static constexpr uint32_t CHANNEL_OMNI     = 0;
static constexpr uint32_t CHANNEL_MAX      = 16;
static constexpr uint32_t NUM_CHANNELS     = 16;
static constexpr uint32_t MAX_SERIAL_SYSEX_SIZE = 255;  // Maximum size is 65535 bytes.
static constexpr uint32_t MAX_USB_SYSEX_SIZE = 64;


/*! Enumeration of MIDI types
 *  Voice Commands will contain the midi channel(0..15) encoded in the LSB
 */
enum MsgType
{
	NoValidMsg            = 0x00,   ///< No or invalid Message
	MsgOmni               = 0x01,   ///< All Msg Types Except Sysex
	/*                      0x00    ///< data bytes
	 *                      ...     ///< data bytes
	 *                      0x7F    ///< data bytes
	 */
	NoteOff	              = 0x80,	///< Voice command - Note Off                        CIN: 0x08
	NoteOn                = 0x90,	///< Voice command - Note On                         CIN: 0x09
	AfterTouchPoly        = 0xA0,	///< Voice command - Polyphonic AfterTouch           CIN: 0x0A
	ControlChange         = 0xB0,	///< Voice command - Control Change / Channel Mode   CIN: 0x0B
	ProgramChange         = 0xC0,	///< Voice command - Program Change                  CIN: 0x0C
	AfterTouchChannel     = 0xD0,	///< Voice command - Channel (monophonic) AfterTouch CIN: 0x0D
	PitchBend             = 0xE0,	///< Voice command - Pitch Bend                      CIN: 0x0E
	SystemExclusive       = 0xF0,	///< System Common - System Exclusive                CIN: 0x04
	TimeCodeQuarterFrame  = 0xF1,	///< System Common - MIDI Time Code Quarter Frame    CIN: 0x08 ?? 2 bytes, so why not 0x02 ????
	SongPosition          = 0xF2,	///< System Common - Song Position Pointer           CIN: 0x03  because   3 bytes
	SongSelect            = 0xF3,	///< System Common - Song Select                     CIN: 0x02  because   2 bytes
	TuneRequest           = 0xF6,	///< System Common - Tune Request                    CIN: 0x05  because   1 byte
	SystemExclusiveEnd    = 0xF7,	///< System Common - End of System Exclusive data    CIN: 0x05, 0x06, 0x07
	Clock                 = 0xF8,	///< System Real Time - Timing Clock                 CIN: 0x0F
	Start                 = 0xFA,	///< System Real Time - Start                        CIN: 0x0F
	Continue              = 0xFB,	///< System Real Time - Continue                     CIN: 0x0F
	Stop                  = 0xFC,	///< System Real Time - Stop                         CIN: 0x0F
	ActiveSensing         = 0xFE,	///< System Real Time - Active Sensing               CIN: 0x0F
	SystemReset           = 0xFF,	///< System Real Time - System Reset                 CIN: 0x0F
	RPN                   = 0x0100,
	NRPN                  = 0x0101,
	ControlChangeHighRes  = 0x0110,
	ControlChangeDoubleRes  = 0x0111
};

constexpr MsgType getMsgTypeFromCommand(uint8_t data) noexcept;

inline constexpr
std::string_view command2Str(uint8_t command) noexcept
{
	switch(getMsgTypeFromCommand(command))
	{
		case NoteOff                : return "NOff";         
		case NoteOn                 : return "NOn";             
		case AfterTouchPoly         : return "ATPoly";     
		case ControlChange          : return "CC";      
		case ProgramChange          : return "PC";      
		case AfterTouchChannel      : return "ATChannel";  
		case PitchBend              : return "PBend";          
		case SystemExclusive        : return "SysEx";    
		case TimeCodeQuarterFrame   : return "TimeCodeQF";
		case SongPosition           : return "SongPos";       
		case SongSelect             : return "SongSelect";         
		case TuneRequest            : return "TuneReq";        
		case SystemExclusiveEnd     : return "SysExEnd"; 
		case Clock                  : return "Clock";              
		case Start                  : return "Start";              
		case Continue               : return "Continue";          
		case Stop                   : return "Stop";               
		case ActiveSensing          : return "ActiveSensing";      
		case SystemReset            : return "SysReset";
		case RPN                    : return "RPN";
		case NRPN                   : return "NRPN";
		case ControlChangeHighRes   : return "CCHighRes";
		case ControlChangeDoubleRes : return "CCDoubleRes";
		default                     : return "Unknown";       
	}
}

inline constexpr bool isCommand(uint8_t data) noexcept
{
   return data & 0x80;
}

inline constexpr bool isVoiceCommand(uint8_t data) noexcept
{
   return isCommand(data) && (data < SystemExclusive);
}

class CommandBase
{
public:
	constexpr operator uint8_t() noexcept {return m_command;}
protected:
	constexpr CommandBase(MsgType msgType) noexcept: m_command(msgType){}
	uint8_t m_command;
};

class VoiceCommand : public CommandBase
{
protected:
	constexpr VoiceCommand(MsgType msgType, uint8_t channel) noexcept: CommandBase(msgType){m_command |= ((channel-1) & 0x0F);}
};

template< MsgType msgType > struct Command {};
template<> struct Command<NoteOff>              : public VoiceCommand{ constexpr Command(uint8_t channel) noexcept:VoiceCommand(NoteOff, channel){} };
template<> struct Command<NoteOn>               : public VoiceCommand{ constexpr Command(uint8_t channel) noexcept:VoiceCommand(NoteOn, channel){}};
template<> struct Command<AfterTouchPoly>       : public VoiceCommand{ constexpr Command(uint8_t channel) noexcept:VoiceCommand(AfterTouchPoly, channel){}};
template<> struct Command<ControlChange>        : public VoiceCommand{ constexpr Command(uint8_t channel) noexcept:VoiceCommand(ControlChange, channel){}};
template<> struct Command<ProgramChange>        : public VoiceCommand{ constexpr Command(uint8_t channel) noexcept:VoiceCommand(ProgramChange, channel){}};
template<> struct Command<AfterTouchChannel>    : public VoiceCommand{ constexpr Command(uint8_t channel) noexcept:VoiceCommand(AfterTouchChannel, channel){}};
template<> struct Command<PitchBend>            : public VoiceCommand{ constexpr Command(uint8_t channel) noexcept:VoiceCommand(PitchBend, channel){}};
template<> struct Command<SystemExclusive>      : public CommandBase{ constexpr Command() noexcept:CommandBase(SystemExclusive){}};
template<> struct Command<TimeCodeQuarterFrame> : public CommandBase{ constexpr Command() noexcept:CommandBase(TimeCodeQuarterFrame){}};
template<> struct Command<SongPosition>         : public CommandBase{ constexpr Command() noexcept:CommandBase(SongPosition){}};
template<> struct Command<SongSelect>           : public CommandBase{ constexpr Command() noexcept:CommandBase(SongSelect){}};
template<> struct Command<TuneRequest>          : public CommandBase{ constexpr Command() noexcept:CommandBase(TuneRequest){}};
template<> struct Command<SystemExclusiveEnd>   : public CommandBase{ constexpr Command() noexcept:CommandBase(SystemExclusiveEnd){}};
template<> struct Command<Clock>                : public CommandBase{ constexpr Command() noexcept:CommandBase(Clock){}};
template<> struct Command<Start>                : public CommandBase{ constexpr Command() noexcept:CommandBase(Start){}};
template<> struct Command<Continue>             : public CommandBase{ constexpr Command() noexcept:CommandBase(Continue){}};
template<> struct Command<Stop>                 : public CommandBase{ constexpr Command() noexcept:CommandBase(Stop){}};
template<> struct Command<ActiveSensing>        : public CommandBase{ constexpr Command() noexcept:CommandBase(ActiveSensing){}};
template<> struct Command<SystemReset>          : public CommandBase{ constexpr Command() noexcept:CommandBase(SystemReset){}};

template <unsigned int Size> class MsgLayout {};

template <>
class MsgLayout<1>
{
public:
	inline constexpr uint8_t command() const noexcept { return m_command; }
	inline std::string_view toString() const noexcept { return command2Str(command()); }
	inline constexpr bool operator==(const MsgLayout<1>& rhs) const noexcept { return (m_command == rhs.m_command); }
	inline constexpr bool operator!=(const MsgLayout<1>& rhs) const noexcept { return !operator==(rhs); }
protected:
	inline constexpr MsgLayout(uint8_t command) noexcept:
			  m_command(command) {}
private:
	uint8_t m_command;
};

template <>
class MsgLayout<2>
{
public:
	inline constexpr uint8_t command() const noexcept{ return m_command; }
	inline constexpr uint8_t data1() const noexcept { return m_data1; }
	inline std::string toString() const noexcept{ return fmt::format("{0} {1}", command2Str(command()), data1()); } 
	inline constexpr bool operator==(const MsgLayout<2>& rhs) const noexcept { return (m_command == rhs.m_command) &&
	                                                                        (m_data1 == rhs.m_data1); }
	inline constexpr bool operator!=(const MsgLayout<2>& rhs) const noexcept { return !operator==(rhs); }
protected:
	inline constexpr MsgLayout(uint8_t command, uint8_t data1) noexcept:
			  m_command(command),
			  m_data1(data1)
	{
		m_data1 &= 0x7F; // Protection: remove MSBs on data
	}
private:
	uint8_t m_command;
	uint8_t m_data1;
};

template <>
class MsgLayout<3>
{
public:
	inline constexpr uint8_t command() const noexcept{ return m_command; }
	inline constexpr uint8_t data1() const noexcept{ return m_data1; }
	inline constexpr uint8_t data2() const noexcept{ return m_data2; }
	inline std::string toString() const noexcept{
		return fmt::format("{0} {1} {2}", command2Str(command()), data1(), data2()); }
	inline constexpr bool operator==(const MsgLayout<3>& rhs) const noexcept 
	{
		return (m_command == rhs.m_command) &&
	          (m_data1 == rhs.m_data1) &&
		       (m_data2 == rhs.m_data2); 
	}
	inline constexpr bool operator!=(const MsgLayout<3>& rhs) const noexcept { return !operator==(rhs); }
protected:
	inline constexpr MsgLayout(uint8_t command, uint8_t data1, uint8_t data2) noexcept:
	          m_command(command),
	          m_data1(data1),
	          m_data2(data2)
	{
		m_data1 &= 0x7F; // Protection: remove MSBs on data
		m_data2 &= 0x7F; // Protection: remove MSBs on data
	}
private:
	uint8_t m_command;
	uint8_t m_data1;
	uint8_t m_data2;
};



inline constexpr bool isRealtimeCommand(uint8_t data) noexcept
{
   switch(data)
   {
	   case Clock:
	   case Start:
	   case Continue:
	   case Stop:
	   case ActiveSensing:
	   case SystemReset:
		   return true;
	   default:
		   return false;
   }
}

inline constexpr uint16_t getExpectedPayloadSize(uint8_t data) noexcept
{
	if(isVoiceCommand(data))
	{
		data &= 0xF0;
	}
	switch(data)
	{
		case Start:
		case Continue:
		case Stop:
		case Clock:
		case ActiveSensing:
		case SystemReset:
		case TuneRequest:
			return 0;
		case ProgramChange:
		case AfterTouchChannel:
		case TimeCodeQuarterFrame:
		case SongSelect:
			return 1;
		case NoteOn:
		case NoteOff:
		case ControlChange:
		case PitchBend:
		case AfterTouchPoly:
		case SongPosition:
			return 2;
		case SystemExclusive:
			return MAX_SERIAL_SYSEX_SIZE;
		case NoValidMsg:
		default:
			return 0;
	}
}

inline constexpr MsgType getMsgTypeFromCommand(uint8_t data) noexcept
{
	if(!isCommand(data))
	{
		return NoValidMsg;
	}
	if(data < SystemExclusive)
	{
		data = data & 0xF0;
	}
	return static_cast<MsgType>(data);
}

inline constexpr uint8_t getChannelFromVoiceCommand(uint8_t voiceCommand) noexcept
{
   return (voiceCommand & 0x0F) + 1;
}

class SysExPayload
{
public:
	SysExPayload() : m_length(0), m_pArray(0) {}
	SysExPayload(const uint8_t* pArray, uint32_t length) :
		m_length(length),
		m_pArray(pArray){}
	SysExPayload& operator=(const SysExPayload& rhs) {m_length = rhs.m_length; m_pArray = rhs.m_pArray; return *this; };
	uint32_t getLength() const { return m_length; }
	const uint8_t* getArray() const { return m_pArray;}
	uint8_t operator[](unsigned int i) const { return m_pArray[i%m_length] & 0x7F; }
private:
	uint32_t m_length;
	const uint8_t* m_pArray;
};

} // namespace midi

#endif // SOUND_DEVICE_MIDI_H
