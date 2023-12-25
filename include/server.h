#pragma once

#include "types.h"

#include <string>
#include <unordered_map>

class Server {
private:
    std::string _directory{};
    std::string _name{};
    std::unordered_map<std::string, PluginInfo> _localPlugins{};

public:
    Server(std::string& name, std::string& dir);
    void refresh();
    bool contains(std::string& name);
};