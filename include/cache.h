#pragma once

#include "types.h"
#include <string>
#include <unordered_map>

void readPlugins(std::string& pluginPath, std::unordered_map<std::string, PluginInfo>& map);
