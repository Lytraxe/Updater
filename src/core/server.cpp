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

const std::string& Server::directory() const {
    return this->_directory;
}
