#ifndef NON_BUFFERED_MESSAGE_DRAIN_H
#define NON_BUFFERED_MESSAGE_DRAIN_H

#include "MidiInputCbBase.h"

namespace midi
{

class NonBufferedMessageDrain : public MidiInputCbBase
{
protected:
    template<typename T>
    void toBuffer(T&& msg){
        MidiInputCbBase::fromBuffer(std::forward<T>(msg));
    }
};

} // namespace midi

#endif