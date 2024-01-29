#pragma once

#include <include/types.h>

#include <string>
#include <unordered_map>

void readPlugins(std::string& pluginPath, std::unordered_map<std::string, PluginInfo>& map);
bool checkAndDownload(PluginInfo& plugin, const std::string& cachePath);
