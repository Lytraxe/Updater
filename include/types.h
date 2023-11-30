#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <filesystem>

struct UpdateSource {
    std::string type;
    std::string value;
    std::string auth;
};

struct ReleaseInfo {
    std::string tag;
    std::string fileName;
    std::string fileDownloadUrl;
};

struct PluginInfo {
    std::string name;
    std::filesystem::path path;
    const char* version;
    UpdateSource source;
    ReleaseInfo update;
};

#endif