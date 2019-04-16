/*
 * MidiInput.cpp
 *
 *  Created on: Oct 8, 2017
 *      Author: danita
 */

#include "HardwareSerial.h"
#include "MidiInput.h"

using namespace midi;

Input::Input(HardwareSerial& rSerial) :
   m_rSerial(rSerial),
   m_runningStatusCommand(NoValidMsg)
{
}

bool Input::checkIfCommandIsValidAndSetRunnungStatus(uint8_t data)
{
	switch(getMsgTypeFromCommand(data))
	{
	    //Channel Voice Commands
		case NoteOff:
		case NoteOn:
		case AfterTouchPoly:
		case ControlChange:
		case ProgramChange:
		case AfterTouchChannel:
		case PitchBend:
        {
        	m_runningStatusCommand = data;
        	break;
        }
        //System Common Commands
		case SystemExclusive:
		case TimeCodeQuarterFrame:
		case SongPosition:
		case SongSelect:
		case TuneRequest:
		case SystemExclusiveEnd:
        {
        	m_runningStatusCommand = NoValidMsg;
        	break;
        }
        //Realtime Commands
		case Clock:
		case Start:
		case Continue:
		case Stop:
		case ActiveSensing:
		case SystemReset:
		{
			//No need to reset running status
			break;
		}
		//invalid command received
		default:
		{
			m_runningStatusCommand = NoValidMsg;
	        return false;
		}
	}
	return true;
}

MsgType Input::read()
{
	m_message.resetLast();
    do
    {
    	if(!parse())
        {
    		m_message.resetAll();
    		return NoValidMsg;
        }
    }while(!m_message.isComplete());
    return getMsgTypeFromCommand(m_message.getCommand());
}

bool Input::parse()
{
	if(m_rSerial.available() <= 0)
	{
		return false;
	}
    const uint8_t data = m_rSerial.read();

    if(isCommand(data))
    {
       if(!checkIfCommandIsValidAndSetRunnungStatus(data))
       {
    	   return false;
       }
       if(!m_message.addCommand(data))
       {
    	   return false;
       }
    }
    else
    { // data byte received
       if(m_message.isEmpty())
       {
		   if(NoValidMsg == m_runningStatusCommand)
		   {
			   return false;
		   }
		   m_message.addCommand(m_runningStatusCommand);
       }
       if(!m_message.addData(data))
       {
    	   return false;
       }
    }
    return true;
}


Input::MessageBuilder::MessageBuilder() :
   m_rtMessage(NoValidMsg)
{
}

void Input::MessageBuilder::resetLast()
{
	if(NoValidMsg != m_rtMessage)
	{
		m_rtMessage = NoValidMsg;
	}
	else
	{
		m_msg.command() = NoValidMsg;
		m_msg.payloadSize = 0;
		m_msg.expectedPayloadSize = 0;
	}
}


void Input::MessageBuilder::resetAll()
{
	m_rtMessage     = NoValidMsg;
	m_msg.command() = NoValidMsg;
	m_msg.payloadSize = 0;
	m_msg.expectedPayloadSize = 0;
}

bool Input::MessageBuilder::isComplete() const
{
	if(NoValidMsg != m_rtMessage)
	{
		return true;
	}
	switch(m_msg.command())
	{
		case NoValidMsg:
			return false;
		case SystemExclusive:
			if(MAX_SERIAL_SYSEX_SIZE == m_msg.payloadSize)
			{//means buffer is full and we have to read the next command and it has to be SystemExclusiveEnd
				return false;
			}
			break;
		case SystemExclusiveEnd:
			return true;
		default:
			break;
	}
	return m_msg.payloadSize == m_msg.expectedPayloadSize;
}

bool Input::MessageBuilder::isEmpty() const
{
	return (NoValidMsg == m_rtMessage) &&
		   (NoValidMsg == m_msg.command());

}

bool Input::MessageBuilder::addCommand(uint8_t command)
{
	if(isRealtimeCommand(command))
	{
		m_rtMessage = command;
	}
	else if(SystemExclusiveEnd == command)
	{
		if(SystemExclusive != m_msg.command())
		{
			return false;
		}
		m_msg.command() = SystemExclusiveEnd;
		//m_msg.expectedPayloadSize = m_msg.payloadSize;
	}
	else
	{
		m_msg.command() = command;
		m_msg.payloadSize = 0;
		m_msg.expectedPayloadSize = getExpectedPayloadSize(command);
	}
	return true;
}

bool Input::MessageBuilder::addData(uint8_t data)
{
	if(m_msg.payloadSize >= MAX_SERIAL_SYSEX_SIZE)
	{
		return false;
	}
	m_msg.data[m_msg.payloadSize + 1] = data;
	++m_msg.payloadSize;
	return true;
}



