/*
 * MidiInputMessage.h
 *
 *  Created on: Oct 31, 2017
 *      Author: danita
 */

#ifndef MIDI_INPUT_MESSAGE_H_
#define MIDI_INPUT_MESSAGE_H_

#include "Midi.h"

namespace midi
{

template< unsigned int SIZE >
class VoiceMsgLayout;

template <>
class VoiceMsgLayout<2> : public MsgLayout<2>
{
public:
	uint8_t channel() const { return getChannelFromVoiceCommand( command() ); }
};

template <>
class VoiceMsgLayout<3> : public MsgLayout<3>
{
public:
	uint8_t channel() const { return getChannelFromVoiceCommand( command() ); }
};




template <MsgType msgType>
class InputMessage{};

template <> class InputMessage<MsgOmni> : public VoiceMsgLayout<3> {};


template <>
class InputMessage<NoteOff> : public VoiceMsgLayout<3>
{
public:
	uint8_t noteNumber() const {return data1();}
	uint8_t velocity() const {return data2();}
};

template <>
class InputMessage<NoteOn> : public VoiceMsgLayout<3>
{
public:
	uint8_t noteNumber() const {return data1();}
	uint8_t velocity() const {return data2();}
};

template <>
class InputMessage<AfterTouchPoly> : public VoiceMsgLayout<3>
{
public:
	uint8_t noteNumber() const {return data1();}
	uint8_t pressure() const {return data2();}
};

template <>
class InputMessage<ControlChange> : public VoiceMsgLayout<3>
{
public:
	uint8_t controllerNumber() const {return data1();}
	uint8_t controllerValue() const {return data2();}
};

template <>
class InputMessage<ProgramChange> : public VoiceMsgLayout<2>
{
public:
	uint8_t programNumber() const {return data1();}
};

template <>
class InputMessage<AfterTouchChannel> : public VoiceMsgLayout<2>
{
public:
	uint8_t value() const {return data1();}
};

template <>
class InputMessage<PitchBend> : public VoiceMsgLayout<3>
{
public:
	int16_t value() const {return (static_cast<uint16_t>(data1()) | (static_cast<uint16_t>(data2()) << 7)) - 0x2000;}
};

template <>
class InputMessage<TimeCodeQuarterFrame> : public MsgLayout<2>
{
public:
	uint8_t value() const {return data1();}
};

template <>
class InputMessage<SongPosition> : public MsgLayout<3>
{
public:
	uint16_t beats() const {return static_cast<uint16_t>(data1()) | (static_cast<uint16_t>(data2()) << 7);}
};

template <>
class InputMessage<SongSelect> : public MsgLayout<2>
{
public:
	uint8_t song() const {return data1();}
};


template<> class InputMessage<TuneRequest> : public MsgLayout<1> {};
template<> class InputMessage<Clock> : public MsgLayout<1> {};
template<> class InputMessage<Start> : public MsgLayout<1> {};
template<> class InputMessage<Continue> : public MsgLayout<1> {};
template<> class InputMessage<Stop> : public MsgLayout<1> {};
template<> class InputMessage<ActiveSensing> : public MsgLayout<1> {};
template<> class InputMessage<SystemReset> : public MsgLayout<1> {};


} //namespace Midi

#endif //MIDI_INPUT_MESSAGE_H_
