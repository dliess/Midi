/*
 * MidiInput.h
 *
 *  Created on: Oct 7, 2017
 *      Author: danita
 */

#ifndef MIDIINPUT_H_
#define MIDIINPUT_H_

#include "Midi.h"
#include "MidiMessage.h"


class HardwareSerial;

namespace midi
{


class Input
{
public:
    Input(HardwareSerial& rSerial);
	MsgType read();
	template< MsgType msgType >
	const Message<msgType>& getMessage() const;
	inline SysExPayload getSysexMessage() const;

private:
	HardwareSerial &m_rSerial;
	uint8_t	        m_runningStatusCommand;

	class MessageBuilder
	{
	public:
		MessageBuilder();
		void resetLast();
		void resetAll();
		bool isEmpty() const;
		bool isComplete() const;
		bool addCommand(uint8_t command);
		bool addData(uint8_t data);

		inline const uint8_t* message() const;
		inline uint8_t        getCommand() const;
		inline const uint8_t* getSysExArray() const;
		inline uint32_t       getSysExArrayLength() const;
	private:
	   uint8_t m_rtMessage;
	   struct MsgStruct
	   {
		  MsgStruct() : payloadSize(0), expectedPayloadSize(0) {command() = NoValidMsg;}
	      uint8_t  &command() { return data[0]; };
	      const uint8_t  &command() const { return data[0]; };
	      uint16_t payloadSize;
	      uint16_t expectedPayloadSize;
          uint8_t  data[1 + MAX_SERIAL_SYSEX_SIZE];
	   }m_msg;
	}m_message;

	bool parse();
    bool checkIfCommandIsValidAndSetRunnungStatus(uint8_t data);
};

}; //namespace Midi

#include "MidiInput_inl.h"


#endif /* MIDIINPUT_H_ */
