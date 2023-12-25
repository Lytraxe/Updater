#include "../../include/cache.h"
#include "../../include/utils/file_utils.h"
#include "../../include/utils/zip_utils.h"
#include "../../include/utils/yaml_utils.h"

#include <iostream>

void readPlugins(std::string& pluginPath, std::unordered_map<std::string, PluginInfo>& map) {
    std::vector<std::filesystem::path> jars{};

    auto err = getFiles(pluginPath, jars);
    if (err) {
        std::cerr << "Error occured when reading files at " << pluginPath << " (" + err.message() + ")\n";
        return;
    }

    for (const auto& file : jars) {
        std::string out{};
        if (readFromJar(file.string().c_str(), "plugin.yml", out)) {
            std::cerr << "Could not parse archive " << file << '\n';
            continue;
        }

        YAML::Node yml = YAML::Load(out);
        if (!yml["name"]) {
            std::cerr << "File " << file << " does not contain `name` in plugin.yml. Skipping.\n";
            continue;
        }
        std::string name{ yml["name"].as<std::string>() };
        if (map.contains(name)) continue;

        if (!yml["version"]) {
            std::cerr << "File " << file << " does not contain `version` in plugin.yml. Skipping.\n";
            continue;
        }
        std::string version{ yml["version"].as<std::string>() };

        map[name] = PluginInfo{ .name {name}, .path {file}, .version {version} };
    }
}