/*
 * MidiOutputMessage.h
 *
 *  Created on: Oct 31, 2017
 *      Author: danita
 */

#ifndef MIDIOUTPUTMESSAGE_H_
#define MIDIOUTPUTMESSAGE_H_

#include "Midi.h"

namespace midi
{

template <MsgType msgType>
class OutputMessage{};

template <>
class OutputMessage<NoteOff> : public MsgLayout<3>
{
public:
	OutputMessage(uint8_t channel, uint8_t noteNumber, uint8_t velocity) :
		MsgLayout<3>(Command<NoteOff>(channel), noteNumber, velocity){}
};

template <>
class OutputMessage<NoteOn> : public MsgLayout<3>
{
public:
	OutputMessage(uint8_t channel, uint8_t noteNumber, uint8_t velocity) :
		MsgLayout<3>(Command<NoteOn>(channel), noteNumber, velocity){}
};

template <>
class OutputMessage<AfterTouchPoly> : public MsgLayout<3>
{
public:
	OutputMessage(uint8_t channel, uint8_t noteNumber, uint8_t pressure) :
		MsgLayout<3>(Command<AfterTouchPoly>(channel), noteNumber, pressure){}
};

template <>
class OutputMessage<ControlChange> : public MsgLayout<3>
{
public:
	OutputMessage(uint8_t channel, uint8_t controllerNumber, uint8_t controllerValue) :
		MsgLayout<3>(Command<ControlChange>(channel), controllerNumber, controllerValue){}
};

template <>
class OutputMessage<ProgramChange> : public MsgLayout<2>
{
public:
	OutputMessage(uint8_t channel, uint8_t programNumber) :
		MsgLayout<2>(Command<ProgramChange>(channel), programNumber){}
};

template <>
class OutputMessage<AfterTouchChannel> : public MsgLayout<2>
{
public:
	OutputMessage(uint8_t channel, uint8_t value) :
		MsgLayout<2>(Command<AfterTouchChannel>(channel), value){}
};

template <>
class OutputMessage<PitchBend> : public MsgLayout<3>
{
public:
	OutputMessage(uint8_t channel, int16_t value) :
		MsgLayout<3>(Command<PitchBend>(channel), (value + 0x2000) & 0x7F, ((value + 0x2000) >> 7) & 0x7F){}
};

template <>
class OutputMessage<TimeCodeQuarterFrame> : public MsgLayout<2>
{
public:
	OutputMessage(uint8_t data) :
		MsgLayout<2>(Command<TimeCodeQuarterFrame>(), data){}
	OutputMessage(uint8_t nibbleType, uint8_t nibbleValue) :
		MsgLayout<2>(Command<TimeCodeQuarterFrame>(),  ((nibbleType & 0x07) << 4) | (nibbleValue & 0x0F) ){}
};

template <>
class OutputMessage<SongPosition> : public MsgLayout<3>
{
public:
	OutputMessage(uint16_t beats) :
		MsgLayout<3>(Command<SongPosition>(), beats & 0x7F, (beats >> 7) & 0x7F){}
};


template <>
class OutputMessage<SongSelect> : public MsgLayout<2>
{
public:
	OutputMessage(uint8_t song) :
		MsgLayout<2>(Command<SongSelect>(), song){}
};

template <>
class OutputMessage<TuneRequest> : public MsgLayout<1>
{
public:
	OutputMessage() :
		MsgLayout<1>(Command<TuneRequest>()){}
};

template <>
class OutputMessage<Clock> : public MsgLayout<1>
{
public:
	OutputMessage() :
		MsgLayout<1>(Command<Clock>()){}
};

template <>
class OutputMessage<Start> : public MsgLayout<1>
{
public:
	OutputMessage() :
		MsgLayout<1>(Command<Start>()){}
};

template <>
class OutputMessage<Continue> : public MsgLayout<1>
{
public:
	OutputMessage() :
		MsgLayout<1>(Command<Continue>()){}
};

template <>
class OutputMessage<Stop> : public MsgLayout<1>
{
public:
	OutputMessage() :
		MsgLayout<1>(Command<Stop>()){}
};

template <>
class OutputMessage<ActiveSensing> : public MsgLayout<1>
{
public:
	OutputMessage() :
		MsgLayout<1>(Command<ActiveSensing>()){}
};

template <>
class OutputMessage<SystemReset> : public MsgLayout<1>
{
public:
	OutputMessage() :
		MsgLayout<1>(Command<SystemReset>()){}
};


} // namespace midi

#endif /* MIDIOUTPUTMESSAGE_H_ */
