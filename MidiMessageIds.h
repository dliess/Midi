#ifndef MIDI_MESSAGE_IDS_H
#define MIDI_MESSAGE_IDS_H

#ifdef USE_MPARK_VARIANT
#include <mpark/variant.hpp>
#define VARIANT_NS mpark
#else
#include <variant>
#define VARIANT_NS std
#endif

#include "Midi.h"

namespace midi
{
template <midi::MsgType> struct MidiMsgId
{
};

template <> struct MidiMsgId<midi::NoteOff>
{
   int note;
};

template <> struct MidiMsgId<midi::NoteOn>
{
   int note;
};

template <> struct MidiMsgId<midi::ControlChange>
{
   int id;
};

template <> struct MidiMsgId<midi::ControlChangeHighRes>
{
   int idMsb;
   int idLsb;
};

template <> struct MidiMsgId<midi::ControlChangeDoubleRes>
{
   int bottomHalfId;
   int topHalfId;
};

template <> struct MidiMsgId<midi::NRPN>
{
   int idMsb;
   int idLsb;
};

template <> struct MidiMsgId<midi::RPN>
{
   int idMsb;
   int idLsb;
};

template <> struct MidiMsgId<midi::AfterTouchPoly>
{
   int note;
};

template <> struct MidiMsgId<midi::AfterTouchChannel>
{
   int dummy;   // if not here, to_json segfaults
};

template <> struct MidiMsgId<midi::PitchBend>
{
   int dummy;   // if not here, to_json segfaults
};

template <> struct MidiMsgId<midi::SystemExclusive>
{
   int dummy;   // if not here, to_json segfaults
};

using MidiMessageId = VARIANT_NS::variant<
    VARIANT_NS::monostate, MidiMsgId<midi::NoteOff>, MidiMsgId<midi::NoteOn>,
    MidiMsgId<midi::ControlChange>, MidiMsgId<midi::ControlChangeHighRes>,
    MidiMsgId<midi::ControlChangeDoubleRes>, MidiMsgId<midi::NRPN>, MidiMsgId<midi::RPN>,
    MidiMsgId<midi::AfterTouchPoly>, MidiMsgId<midi::AfterTouchChannel>,
    MidiMsgId<midi::PitchBend>, MidiMsgId<midi::SystemExclusive>>;

}   // namespace midi

#endif