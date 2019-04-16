#ifndef INTERFACE_MIDI_1_INPUT_CALLBACK
#define INTERFACE_MIDI_1_INPUT_CALLBACK

#include "MidiInputMessage.h"
#include <vector>

namespace midi
{

class IMidi1InputCallback
{
public:
    virtual ~IMidi1InputCallback() {};
    virtual void onNoteOff(double timestamp, const InputMessage<NoteOff>& msg){};
    virtual void onNoteOn(double timestamp, const InputMessage<NoteOn>& msg){};
    virtual void onAfterTouchPoly(double timestamp, const InputMessage<AfterTouchPoly>& msg){};
    virtual void onControlChange(double timestamp, const InputMessage<ControlChange>& msg){};
    virtual void onProgramChange(double timestamp, const InputMessage<ProgramChange>& msg){};
    virtual void onAfterTouchChannel(double timestamp, const InputMessage<AfterTouchChannel>& msg){};
    virtual void onPitchBend(double timestamp, const InputMessage<PitchBend>& msg){};
    virtual void onSystemExclusive(double timestamp, const std::vector<uint8_t>& msg){};
    virtual void onTimeCodeQuarterFrame(double timestamp, const InputMessage<TimeCodeQuarterFrame>& msg){};
    virtual void onSongPosition(double timestamp, const InputMessage<SongPosition>& msg){};
    virtual void onSongSelect(double timestamp, const InputMessage<SongSelect>& msg){};
    virtual void onTuneRequest(double timestamp, const InputMessage<TuneRequest>& msg){};
    virtual void onSystemExclusiveEnd(double timestamp, const InputMessage<SystemExclusiveEnd>& msg){}; // TODO
    virtual void onClock(double timestamp, const InputMessage<Clock>& msg){};
    virtual void onStart(double timestamp, const InputMessage<Start>& msg){};
    virtual void onContinue(double timestamp, const InputMessage<Continue>& msg){};
    virtual void onStop(double timestamp, const InputMessage<Stop>& msg){};
    virtual void onActiveSensing(double timestamp, const InputMessage<ActiveSensing>& msg){};
    virtual void onSystemReset(double timestamp, const InputMessage<SystemReset>& msg){};
};

}

#endif