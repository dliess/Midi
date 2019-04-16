#ifndef MIDI_CONFIG_PARSER_H
#define MIDI_CONFIG_PARSER_H

#include <string>
#include <memory>
#include "ArduinoJson-v5.13.4.h"


namespace midi
{
class ParameterSettings;
}

namespace midi
{

class MidiConfigParser
{
public:
    MidiConfigParser();
    bool init(const std::string& config);
    std::unique_ptr<midi::ParameterSettings> getMidiParameterSettings() const;
    //sdUi::ParameterSettings* getUiParameterSettings() const;
private:
    DynamicJsonBuffer m_jsonBuffer;
    JsonObject*       m_pObj;
};

} // namespace midi

#endif // MIDI_CONFIG_PARSER_H