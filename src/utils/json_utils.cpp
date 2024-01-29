#include <include/utils/json_utils.h>

JsonParser::JsonParser(std::string& data) {
    Json::CharReaderBuilder reader;
    std::istringstream str(data);
    Json::parseFromStream(reader, str, &jsonData, nullptr);
}

std::string JsonParser::getString(const char* name) {
    return jsonData[name].asString();
}

int JsonParser::getInt(const char* name) {
    return jsonData[name].asInt();
}

bool JsonParser::getBool(const char* name) {
    return jsonData[name].asBool();
}

Json::Value JsonParser::get(const char* name) {
    return jsonData[name];
}

void JsonParser::set(Json::Value& value) {
    jsonData = value;
}