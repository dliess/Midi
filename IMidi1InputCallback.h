#ifndef INTERFACE_MIDI_1_INPUT_CALLBACK
#define INTERFACE_MIDI_1_INPUT_CALLBACK

#include "MidiMessage.h"
#include <vector>

namespace midi
{

class IMidi1InputCallback
{
public:
    virtual ~IMidi1InputCallback() {};
    virtual void onNoteOff(double timestamp, const Message<NoteOff>& msg){};
    virtual void onNoteOn(double timestamp, const Message<NoteOn>& msg){};
    virtual void onAfterTouchPoly(double timestamp, const Message<AfterTouchPoly>& msg){};
    virtual void onControlChange(double timestamp, const Message<ControlChange>& msg){};
    virtual void onProgramChange(double timestamp, const Message<ProgramChange>& msg){};
    virtual void onAfterTouchChannel(double timestamp, const Message<AfterTouchChannel>& msg){};
    virtual void onPitchBend(double timestamp, const Message<PitchBend>& msg){};
    virtual void onSystemExclusive(double timestamp, const std::vector<uint8_t>& msg){};
    virtual void onTimeCodeQuarterFrame(double timestamp, const Message<TimeCodeQuarterFrame>& msg){};
    virtual void onSongPosition(double timestamp, const Message<SongPosition>& msg){};
    virtual void onSongSelect(double timestamp, const Message<SongSelect>& msg){};
    virtual void onTuneRequest(double timestamp, const Message<TuneRequest>& msg){};
    virtual void onSystemExclusiveEnd(double timestamp, const Message<SystemExclusiveEnd>& msg){}; // TODO
    virtual void onClock(double timestamp, const Message<Clock>& msg){};
    virtual void onStart(double timestamp, const Message<Start>& msg){};
    virtual void onContinue(double timestamp, const Message<Continue>& msg){};
    virtual void onStop(double timestamp, const Message<Stop>& msg){};
    virtual void onActiveSensing(double timestamp, const Message<ActiveSensing>& msg){};
    virtual void onSystemReset(double timestamp, const Message<SystemReset>& msg){};
};

}

#endif