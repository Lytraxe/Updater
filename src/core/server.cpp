#include "../../include/server.h"
#include "../../include/cache.h"

#include <iostream>

Server::Server(const std::string& name, const std::string& dir) : _name{ name }, _directory{ dir } {
    refresh();
}

void Server::refresh() {
    auto& mapRef{ this->_localPlugins };
    mapRef.clear();

    readPlugins(this->_directory, mapRef);
}

bool Server::contains(const std::string& name) const {
    return this->_localPlugins.contains(name);
}

const PluginInfo& Server::get(const std::string& name) {
    return this->_localPlugins[name];
}

bool Server::stage(const PluginInfo& plugin) {
    if (!contains(plugin.name)) return false;
    this->_localPlugins[plugin.name].staged = true;
    this->_localPlugins[plugin.name].update = { plugin.version, plugin.path };
    return true;
}

const std::string& Server::directory() const {
    return this->_directory;
}

void Server::summary() const {
    int staged{ 0 };
    std::cout << this->_name << " (" << this->_directory << ") : \n";
    for (const auto& plugin : this->_localPlugins) {
        if (!plugin.second.staged) {
            std::cout << '\t' << plugin.second.name << " (not staged)\n";
            continue;
        }
        std::cout << '\t' << plugin.second.name << '\t'
            << plugin.second.version << " -> " << plugin.second.update.tag << '\n';
        ++staged;
    }
    std::cout << staged << " out of " << this->_localPlugins.size() << " plugins staged for update\n";
}
