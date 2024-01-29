#pragma once

#include <include/types.h>

#include <string>
#include <unordered_map>
#include <vector>

class Server {
private:
    std::string _name{};
    std::string _directory{};
    std::unordered_map<std::string, PluginInfo> _localPlugins{};

public:
    Server(const std::string& name, const std::string& dir);
    void refresh();
    bool stage(const PluginInfo& plugin);
    const PluginInfo& get(const std::string& name);
    bool contains(const std::string& name) const;
    const std::string& name() const;
    const std::string& directory() const;
    void summary() const;
    bool update() const;
};
