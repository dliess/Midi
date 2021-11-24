#include "Midi1Input.h"
#include "MidiMessage.h"
#include "IMidi1InputCallback.h"
#include <iostream>

template<typename MessageDrain>
midi::Midi1Input<MessageDrain>::Midi1Input(std::unique_ptr<IMidiInMedium> pMedium) :
    m_pMedium(std::move(pMedium)),
    m_pMidiInCb(nullptr)
{
    m_xrpnHandler.emplace();
    m_pMedium->registerCallback([this](double timestamp, std::vector<uint8_t>& data){
        this->processIncomingData(timestamp, data);
    });
}

template<typename MessageDrain>
void midi::Midi1Input<MessageDrain>::disableXRPN()
{
    m_xrpnHandler.reset();
}

template<typename MessageDrain>
bool midi::Midi1Input<MessageDrain>::setCCHighResPair(int msbId, int lsbId) noexcept
{
    return m_ccInputHandler.setPair(msbId, lsbId);
}

template<typename MessageDrain>
midi::IMidiInMedium& midi::Midi1Input<MessageDrain>::medium()
{
    return *m_pMedium;
}

template<typename MessageDrain>
const midi::IMidiInMedium& midi::Midi1Input<MessageDrain>::medium() const
{
    return *m_pMedium;
}

template<typename MessageDrain>
void midi::Midi1Input<MessageDrain>::registerMidiInCbIf(IMidi1InputCallback* pMidiInCb)
{
    m_pMidiInCb = pMidiInCb;
}

template<typename MessageDrain>
void midi::Midi1Input<MessageDrain>::processIncomingData(double timestamp, std::vector<uint8_t>& data)
{
    if(data.empty())
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
            auto pEvent = reinterpret_cast<Message<NoteOff>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onNoteOff(timestamp, *pEvent);
            break;
        }
		case NoteOn:
        {
            auto pEvent = reinterpret_cast<Message<NoteOn>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onNoteOn(timestamp, *pEvent);
            break;
        }
		case AfterTouchPoly:
        {
            auto pEvent = reinterpret_cast<Message<AfterTouchPoly>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onAfterTouchPoly(timestamp, *pEvent);
            break;
        }
    	case ControlChange:
        {
            auto pEvent = reinterpret_cast<Message<ControlChange>*>(&data[0]);
            if(m_xrpnHandler && xrpn::XRpnInputHandler::isXRpnMsg(*pEvent))
            {
                auto xrpnMsg = m_xrpnHandler->handleMsg(*pEvent);
                if(!mpark::holds_alternative<mpark::monostate>(xrpnMsg)){
                    MessageDrain::toBuffer(xrpnMsg);
                }
            }
            else
            {
                const auto ret = m_ccInputHandler.handleIncomingCCMsg(*pEvent);
                mpark::visit(midi::overload{
                    [this, &timestamp](const Message<ControlChange>& cc){
                        MessageDrain::toBuffer(cc);
                        if(m_pMidiInCb) m_pMidiInCb->onControlChange(timestamp, cc);                
                    },
                    [this](const Message<ControlChangeHighRes>& cchr){
                        MessageDrain::toBuffer(cchr);
                    },
                    [](auto&& rest) {}
                }, ret);
            }
            break;
        }
		case ProgramChange:
        {
            auto pEvent = reinterpret_cast<Message<ProgramChange>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onProgramChange(timestamp, *pEvent);
            break;
        }
		case AfterTouchChannel:
        {
            auto pEvent = reinterpret_cast<Message<AfterTouchChannel>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onAfterTouchChannel(timestamp, *pEvent);
            break;
        }
		case PitchBend:
        {
            auto pEvent = reinterpret_cast<Message<PitchBend>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onPitchBend(timestamp, *pEvent);
            break;
        }
        //System Common Commands
		case SystemExclusive:
        {
            auto pEvent = reinterpret_cast<Message<SystemExclusive>*>(&data);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onSystemExclusive(timestamp, data);
            break;
        }
		case TimeCodeQuarterFrame:
        {
            auto pEvent = reinterpret_cast<Message<TimeCodeQuarterFrame>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onTimeCodeQuarterFrame(timestamp, *pEvent);
            break;
        }
		case SongPosition:
        {
            auto pEvent = reinterpret_cast<Message<SongPosition>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onSongPosition(timestamp, *pEvent);
            break;
        }
		case SongSelect:
        {
            auto pEvent = reinterpret_cast<Message<SongSelect>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onSongSelect(timestamp, *pEvent);
            break;
        }
		case TuneRequest:
        {
            auto pEvent = reinterpret_cast<Message<TuneRequest>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onTuneRequest(timestamp, *pEvent);
            break;
        }
		case SystemExclusiveEnd:
        {
            auto pEvent = reinterpret_cast<Message<SystemExclusiveEnd>*>(&data[0]);
            if(m_pMidiInCb) m_pMidiInCb->onSystemExclusiveEnd(timestamp, *pEvent);
            break;
        }
        //Realtime Commands
		case Clock:
        {
            auto pEvent = reinterpret_cast<Message<Clock>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onClock(timestamp, *pEvent);
            break;
        }
		case Start:
        {
            auto pEvent = reinterpret_cast<Message<Start>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onStart(timestamp, *pEvent);
            break;
        }
		case Continue:
        {
            auto pEvent = reinterpret_cast<Message<Continue>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onContinue(timestamp, *pEvent);
            break;
        }
		case Stop:
        {
            auto pEvent = reinterpret_cast<Message<Stop>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onStop(timestamp, *pEvent);
            break;
        }
		case ActiveSensing:
        {
            auto pEvent = reinterpret_cast<Message<ActiveSensing>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onActiveSensing(timestamp, *pEvent);
            break;
        }
		case SystemReset:
        {
            auto pEvent = reinterpret_cast<Message<SystemReset>*>(&data[0]);
            MessageDrain::toBuffer(*pEvent);
            if(m_pMidiInCb) m_pMidiInCb->onSystemReset(timestamp, *pEvent);
            break;
        }
		//invalid command received
		default:
        {
            break;
        }
	}
}


