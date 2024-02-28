#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>

#include <include/api_wrapper/github_api.h>
#include <include/types.h>
#include <include/core/cache.h>
#include <include/core/server.h>
#include <include/user_interface/input.h>
#include <include/user_interface/output.h>
#include <include/utils/file_utils.h>
#include <include/utils/zip_utils.h>
#include <include/utils/yaml_utils.h>

std::vector<Server> servers{};
std::string cachePath{};
std::unordered_map<std::string, PluginInfo> cache{};

void loadConfig() {
	try {
		YAML::Node config = YAML::LoadFile("config.yml");

		if (!config["cachePath"]) {
			output::error("Could not find cachePath in config.yml\n");
			return;
		}
		cachePath = config["cachePath"].as<std::string>();

		if (!config["servers"]) {
			output::error("Could not find servers node in config.yml\n");
			return;
		}

		std::cout << ANSI::Effects::BOLD << "Loading servers..\n" << ANSI::Effects::BOLD_RESET;
		for (const auto& server : config["servers"]) {
			try {
				servers.push_back(Server{ server.first.as<std::string>(), server.second.as<std::string>() });
			}
			catch (int) {
				std::cerr << "Skipping " << server.first.as<std::string>() << '\n';
			}
		}

		if (!config["plugins"]) {
			output::error("Could not find plugins node in config.yml\n");
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
		std::cerr << ANSI::Effects::BOLD << ANSI::Foreground::RED
			<< "Yaml Error: " << e.what() << ANSI::RESET << '\n';
	}
}

void init() {
	loadConfig();

	if (std::error_code e{}; !std::filesystem::exists(cachePath) && !std::filesystem::create_directories(cachePath, e)) {
		std::cerr << ANSI::Effects::BOLD << ANSI::Foreground::RED
			<< "Could not create the cache directory: " << e.message() << "\nAborting..\n" << ANSI::RESET;
		std::exit(1);
	}

	std::cout << ANSI::Effects::BOLD << "Loading cache..\n" << ANSI::Effects::BOLD_RESET;
	//- Read cache
	readPlugins(cachePath, cache);

	std::cout << ANSI::Effects::BOLD << "Fetching plugin updates..\n" << ANSI::Effects::BOLD_RESET;
	for (auto& itr : cache) {
		if (!itr.second.source) continue;
		//- TODO: checks around here in case the update fails
		checkAndDownload(itr.second, cachePath);
	}
}

int main() {
	init();

	//- TODO: Better responses

	//- Go through servers and look for updated plugins
	for (auto& server : servers) {
		for (const auto& cached : cache) {
			//- Check if the cached plugin is in server, if yes then check if its the same version
			//- assuming the cache is always the up-to-date version
			if (!server.contains(cached.first)) continue;
			if (server.get(cached.first).version == cached.second.version) continue;
			server.stage(cached.second);
		}
	}

	//- Show a summary first
	std::cout << ANSI::Effects::BOLD << "Summary\n" << ANSI::Effects::BOLD_RESET;
	for (const auto& server : servers) server.summary();

	//- Ask for confirmation
	if (std::cout << ANSI::Effects::BOLD << "Continue? (y/n): "; input::get<char>() != 'y') {
		std::cout << ANSI::RESET << "Aborting..\n";
		exit(0);
	}

	//- Update
	for (const auto& server : servers) {
		std::cout << ANSI::Effects::BOLD << "Updating " << server.name() << ANSI::Effects::BOLD_RESET << '\n';
		if (!server.update()) {
			std::cerr << ANSI::Effects::BOLD << ANSI::Foreground::RED
				<< "Could not update " << server.name() << ANSI::RESET << '\n';
		}
	}

	std::cout << "Done. Please restart the server(s) for the update to take effect.\n" << ANSI::RESET;
	return 0;
}
