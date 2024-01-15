#pragma once

#include <string>
#include <filesystem>

struct UpdateSource {
    std::string type;
    std::string value;
    std::string auth;
};

struct ReleaseInfo {
    std::string tag;
    std::filesystem::path filePath;
    std::string fileName;
    std::string fileDownloadUrl;
};

struct PluginInfo {
    std::string name;
    std::filesystem::path path;
    std::string version;
    bool staged{ false };
    UpdateSource source;
    ReleaseInfo update;
};
