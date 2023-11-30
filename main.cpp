#include <iostream>
#include <vector>
#include <unordered_map>

#include "include/utils/curl_utils.h"
#include "include/api_wrapper/github_api.h"
#include "include/types.h"
#include "include/utils/file_utils.h"
#include "include/utils/zip_utils.h"
#include "include/utils/yaml_utils.h"

std::vector<std::string> pluginsPath;
std::vector<std::unordered_map<std::string, PluginInfo>> servers;
std::string cachePath;
std::unordered_map<std::string, PluginInfo> cache;

void loadConfig() {
	try {
		YAML::Node config = YAML::LoadFile("config.yml");
		if (!config["pluginsPath"]) {
			std::cerr << "Could not find pluginsPath in config.yml\n";
			return;
		}
		for (const auto& dir : config["pluginsPath"]) {
			pluginsPath.push_back(dir.as<std::string>());
		}

		if (!config["cachePath"]) {
			std::cerr << "Could not find cachePath in config.yml\n";
			return; 
		}
		cachePath = config["cachePath"].as<std::string>();

		if (!config["plugins"]) {
			std::cerr << "Could not find plugins node in config.yml\n";
			return;
		}

		for (const auto& plugin : config["plugins"]) {
			PluginInfo info;
			info.name = plugin.first.as<std::string>();
			info.source.type = plugin.second["type"].as<std::string>();
			info.source.value = plugin.second["value"].as<std::string>();
			info.source.auth = plugin.second["auth"].as<std::string>();

			cache[info.name] = info;
		}
	}
	catch (const YAML::Exception& e) {
		std::cerr << "Yaml Error: " << e.what() << '\n';
	}
}

void init() {
	loadConfig();

	if (!std::filesystem::exists(cachePath) && !std::filesystem::create_directories(cachePath)) {
		std::cerr << "Could not create the cache directory.. Aborting..\n";
		std::exit(1);
	}
}

int main() {
	init();

	for (const auto& pl : cache) {
		std::cout << pl.first << '\n'
			<< pl.second.name << '\n' << pl.second.source.type << '\n'
			<< pl.second.source.value << '\n'
			<< pl.second.source.auth << '\n';
	 }
	return 0;

}
