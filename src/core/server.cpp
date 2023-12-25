#include "../../include/server.h"
#include "../../include/cache.h"

#include <iostream>

Server::Server(std::string& name, std::string& dir) {
    this->_name = name;
    this->_directory = dir;

    refresh();
}

void Server::refresh() {
    auto& mapRef{ this->_localPlugins };
    mapRef.clear();

    readPlugins(this->_directory, mapRef);
}

bool Server::contains(std::string& name) {
    return this->_localPlugins.contains(name);
}