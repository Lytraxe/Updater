#include "../../include/server.h"
#include "../../include/cache.h"
#include "../../include/utils/file_utils.h"

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
    this->_localPlugins[plugin.name].update = { plugin.version, plugin.path, plugin.path.filename() };
    return true;
}

const std::string& Server::name() const {
    return this->_name;
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

bool Server::update() const {
    const std::filesystem::path updatePath{ std::filesystem::path{this->_directory}.append("update") };
    if (std::error_code er{}; !std::filesystem::exists(updatePath)
        && !std::filesystem::create_directories(updatePath, er)) {

        std::cerr << "\tCould not create directory " << updatePath << ": " << er.message() << '\n';
        return false;
    }

    for (const auto& itr : this->_localPlugins) {
        const auto& plugin = itr.second;
        if (!plugin.staged) continue;
        
        if (const auto err = copyFile(plugin.update.filePath, std::filesystem::path{updatePath}.append(plugin.update.fileName))) {
            std::cerr << "\tCould not copy `" << plugin.name << "` to " << updatePath << ": " << err.message() << ". skipping..\n";
            continue;
        }
    }
    return true;
}
