#include "MidiConfigParser.h"
#include "MidiParameterSettings.h"
#include <iostream>

using namespace midi

MidiConfigParser::MidiConfigParser() :
    m_jsonBuffer(2000),
    m_pObj(nullptr)
{}

bool MidiConfigParser::init(const std::string& jsonStr)
{
    m_pObj = &m_jsonBuffer.parseObject(jsonStr);
    if(!m_pObj->success())
    {
        std::cerr << "parsing json failed" << std::endl;
        return false;
    }
    return true;
}

std::unique_ptr<sounddevice::midi::ParameterSettings> 
MidiConfigParser::getMidiParameterSettings() const
{
    std::unique_ptr<sounddevice::midi::ParameterSettings> pSettings(new sounddevice::midi::ParameterSettings);
    auto &parameters = m_pObj->get<JsonArray>("parameters");
    for(JsonObject& paramObj : parameters)
    {
        auto id      = paramObj.get<unsigned int>("id");
        auto &medium = paramObj.get<JsonObject>("medium");
        auto &cc     = medium.get<JsonArray>("cc");
        sounddevice::midi::ParameterSettings::Element element;
        switch(cc.size())
        {
            case 1:
                element.numBitsPrecision = 7;
                element.msbCcId = cc[0];
                element.lsbCcId = 0;
                break;
            case 2:
                element.numBitsPrecision = 14;
                element.msbCcId = cc[0];
                element.lsbCcId = cc[1];
                break;
            default:
                continue;
        }
        pSettings->insert(id, element);
    }
    return pSettings;
}