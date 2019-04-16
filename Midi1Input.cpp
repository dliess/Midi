#include "Midi1Input.h"
#include "MidiInputMessage.h"
#include "IMidi1InputCallback.h"
#include <iostream>

using namespace midi;

Midi1Input::Midi1Input(std::unique_ptr<IMidiInMedium> pMedium) :
    m_pMedium(std::move(pMedium)),
    m_pMidiInCb(nullptr)
{
    m_pMedium->registerCallback([this](double timestamp, std::vector<uint8_t>& data){
        this->processIncomingData(timestamp, data);
    });
}

IMidiInMedium& Midi1Input::medium()
{
    return *m_pMedium;
}

void Midi1Input::registerMidiInCb(IMidi1InputCallback* pMidiInCb)
{
    m_pMidiInCb = pMidiInCb;
}

void Midi1Input::update()
{
}

void Midi1Input::processIncomingData(double timestamp, std::vector<uint8_t>& data)
{
    if(data.empty())
    {
        return;
    }
    if(!m_pMidiInCb)
    {
        return;
    }
    /*
    const auto expextedMidiMsgSize = getExpectedPayloadSize(data[0]) + 1;
    if(data.size() != expextedMidiMsgSize)
    {
        std::cerr << "size(" << data.size() << ") mismatches midi msg type(" 
                  << NoteOff << ") size: " << expextedMidiMsgSize << std::endl;
        return;
    }
    */
	switch(getMsgTypeFromCommand(data[0]))
	{
	    //Channel Voice Commands 
		case NoteOff:
        {
            auto pEvent = reinterpret_cast<InputMessage<NoteOff>*>(&data[0]);
            m_pMidiInCb->onNoteOff(timestamp, *pEvent);
            break;
        }
		case NoteOn:
        {
            auto pEvent = reinterpret_cast<InputMessage<NoteOn>*>(&data[0]);
            m_pMidiInCb->onNoteOn(timestamp, *pEvent);
            break;
        }
		case AfterTouchPoly:
        {
            auto pEvent = reinterpret_cast<InputMessage<AfterTouchPoly>*>(&data[0]);
            m_pMidiInCb->onAfterTouchPoly(timestamp, *pEvent);
            break;
        }
    	case ControlChange:
        {
            auto pEvent = reinterpret_cast<InputMessage<ControlChange>*>(&data[0]);
            m_pMidiInCb->onControlChange(timestamp, *pEvent);
            break;
        }
		case ProgramChange:
        {
            auto pEvent = reinterpret_cast<InputMessage<ProgramChange>*>(&data[0]);
            m_pMidiInCb->onProgramChange(timestamp, *pEvent);
            break;
        }
		case AfterTouchChannel:
        {
            auto pEvent = reinterpret_cast<InputMessage<AfterTouchChannel>*>(&data[0]);
            m_pMidiInCb->onAfterTouchChannel(timestamp, *pEvent);
            break;
        }
		case PitchBend:
        {
            auto pEvent = reinterpret_cast<InputMessage<PitchBend>*>(&data[0]);
            m_pMidiInCb->onPitchBend(timestamp, *pEvent);
            break;
        }
        //System Common Commands
		case SystemExclusive:
        {
            m_pMidiInCb->onSystemExclusive(timestamp, data);
            break;
        }
		case TimeCodeQuarterFrame:
        {
            auto pEvent = reinterpret_cast<InputMessage<TimeCodeQuarterFrame>*>(&data[0]);
            m_pMidiInCb->onTimeCodeQuarterFrame(timestamp, *pEvent);
            break;
        }
		case SongPosition:
        {
            auto pEvent = reinterpret_cast<InputMessage<SongPosition>*>(&data[0]);
            m_pMidiInCb->onSongPosition(timestamp, *pEvent);
            break;
        }
		case SongSelect:
        {
            auto pEvent = reinterpret_cast<InputMessage<SongSelect>*>(&data[0]);
            m_pMidiInCb->onSongSelect(timestamp, *pEvent);
            break;
        }
		case TuneRequest:
        {
            auto pEvent = reinterpret_cast<InputMessage<TuneRequest>*>(&data[0]);
            m_pMidiInCb->onTuneRequest(timestamp, *pEvent);
            break;
        }
		case SystemExclusiveEnd:
        {
            auto pEvent = reinterpret_cast<InputMessage<SystemExclusiveEnd>*>(&data[0]);
            m_pMidiInCb->onSystemExclusiveEnd(timestamp, *pEvent);
            break;
        }
        //Realtime Commands
		case Clock:
        {
            auto pEvent = reinterpret_cast<InputMessage<Clock>*>(&data[0]);
            m_pMidiInCb->onClock(timestamp, *pEvent);
            break;
        }
		case Start:
        {
            auto pEvent = reinterpret_cast<InputMessage<Start>*>(&data[0]);
            m_pMidiInCb->onStart(timestamp, *pEvent);
            break;
        }
		case Continue:
        {
            auto pEvent = reinterpret_cast<InputMessage<Continue>*>(&data[0]);
            m_pMidiInCb->onContinue(timestamp, *pEvent);
            break;
        }
		case Stop:
        {
            auto pEvent = reinterpret_cast<InputMessage<Stop>*>(&data[0]);
            m_pMidiInCb->onStop(timestamp, *pEvent);
            break;
        }
		case ActiveSensing:
        {
            auto pEvent = reinterpret_cast<InputMessage<ActiveSensing>*>(&data[0]);
            m_pMidiInCb->onActiveSensing(timestamp, *pEvent);
            break;
        }
		case SystemReset:
        {
            auto pEvent = reinterpret_cast<InputMessage<SystemReset>*>(&data[0]);
            m_pMidiInCb->onSystemReset(timestamp, *pEvent);
            break;
        }
		//invalid command received
		default:
        {
            break;
        }
	}
}


