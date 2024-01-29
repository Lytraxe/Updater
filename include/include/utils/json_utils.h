#pragma once

#include <json/json.h>

class JsonParser {
public:
    Json::Value jsonData{};

    JsonParser(std::string& data);

    Json::Value get(const char* name);
    void set(Json::Value& value);
    std::string getString(const char* name);
    int getInt(const char* name);
    bool getBool(const char* name);
};
