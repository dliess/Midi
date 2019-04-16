#ifndef DEVICE_CONFIG_PARSER_H
#define DEVICE_CONFIG_PARSER_H

#include <string>

class DeviceConfigParser
{
public:
    bool init(const std::string& jsonFile);
private:
};

#endif