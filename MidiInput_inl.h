#ifndef MIDI_DIN_INPUT_INL_H
#define MIDI_DIN_INPUT_INL_H

namespace Midi
{

template < MsgType msgType >
const Message<msgType>& Input::getMessage() const
{
	return  *(reinterpret_cast< const Message<msgType>* >(m_message.message()));
}

SysExPayload Input::getSysexMessage() const
{
   return SysExPayload(m_message.getSysExArray(), m_message.getSysExArrayLength());
}

const uint8_t* Input::MessageBuilder::message() const
{
	if(NoValidMsg != m_rtMessage) return &m_rtMessage;
	return m_msg.data;
}


uint8_t Input::MessageBuilder::getCommand() const
{
   if(NoValidMsg != m_rtMessage) return m_rtMessage;
   if(SystemExclusiveEnd == m_msg.command()) return SystemExclusive;
   return m_msg.command();
}

const uint8_t *Input::MessageBuilder::getSysExArray() const
{
   return &(m_msg.data[1]);
}

uint32_t  Input::MessageBuilder::getSysExArrayLength() const
{
   return m_msg.payloadSize;
}



} //namespace Midi


#endif //MIDI_DIN_INPUT_INL_H
