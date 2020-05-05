#ifndef MIDI_MESSAGE_IDS_H
#define MIDI_MESSAGE_IDS_H

#include <mpark/variant.hpp>

#include "Midi.h"

namespace midi
{

template<midi::MsgType>
struct MidiMsgId
{
};

template<>
struct MidiMsgId<midi::NoteOff>
{
   int note;
};

template<>
struct MidiMsgId<midi::NoteOn>
{
   int note;
};

template<>
struct MidiMsgId<midi::ControlChange>
{
   int id;
};

template<>
struct MidiMsgId<midi::ControlChangeHighRes>
{
   int idMsb;
   int idLsb;
};

template<>
struct MidiMsgId<midi::NRPN>
{
   int idMsb;
   int idLsb;
};

template<>
struct MidiMsgId<midi::RPN>
{
   int idMsb;
   int idLsb;
};

template<>
struct MidiMsgId<midi::AfterTouchPoly>
{
   int note;
};

template<>
struct MidiMsgId<midi::AfterTouchChannel>
{
   int dummy; // if not here, to_json segfaults
};

template<>
struct MidiMsgId<midi::PitchBend>
{
   int dummy; // if not here, to_json segfaults
};

using MidiMessageId = mpark::variant< mpark::monostate,
   MidiMsgId<midi::NoteOff>, MidiMsgId<midi::NoteOn>,
   MidiMsgId<midi::ControlChange>, MidiMsgId<midi::ControlChangeHighRes>,
   MidiMsgId<midi::NRPN>, MidiMsgId<midi::RPN>, MidiMsgId<midi::AfterTouchPoly>,
   MidiMsgId<midi::AfterTouchChannel>, MidiMsgId<midi::PitchBend>>;

} // namespace midi

#endif