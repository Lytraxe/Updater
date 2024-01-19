#include "../../include/cache.h"
#include "../../include/utils/file_utils.h"
#include "../../include/utils/curl_utils.h"
#include "../../include/utils/zip_utils.h"
#include "../../include/utils/yaml_utils.h"
#include "../../include/api_wrapper/github_api.h"

#include <iostream>

void readPlugins(std::string& pluginPath, std::unordered_map<std::string, PluginInfo>& map) {
    std::vector<std::filesystem::path> jars{};

    auto err = getFiles(pluginPath, jars);
    if (err) {
        std::cerr << "Error occured while reading files at " << pluginPath << " (" + err.message() + ")\n";
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

        if (!yml["version"]) {
            std::cerr << "File " << file << " does not contain `version` in plugin.yml. Skipping.\n";
            continue;
        }
        std::string version{ yml["version"].as<std::string>() };

        if (map.contains(name)) {
            auto& pInfo = map[name];
            pInfo.version = version;
            pInfo.path = file;
            continue;
        }

        map[name] = PluginInfo{ .name {name}, .path {file}, .version {version} };
    }
}

//- TODO move this to a class and make the wrappers inherit it

bool download(const std::string& url, const std::filesystem::path& path) {
    request_t req{ url, path, nullptr };
    req.headers = curl_slist_append(req.headers, "Accept: application/vnd.github+json");
    req.headers = curl_slist_append(req.headers, "User-Agent: Lytraxe/updater");

    response_t res{};

//- TODO not the best return value
    return !downloadFile(&req, &res);
}

bool checkAndDownload(PluginInfo& plugin, const std::string& cachePath) {
    auto& source = plugin.source;

    if (source.type == "github") {
        auto wrapper = GitHubApi(source.value);
        if (plugin.version == wrapper.getLatestVersion()) return false;
        auto path{ std::filesystem::path{cachePath}.append(wrapper._release.fileName) };
        std::cout << "Downloading " << wrapper._release.tag << " from " << wrapper._release.fileDownloadUrl << '\n';
        if (download(wrapper._release.fileDownloadUrl, path)) {
            plugin.version = wrapper.getLatestVersion();
            plugin.path = path;
            return true;
        }
    }
    else if (source.type == "gitlab") {

    }
    else if (source.type == "spigot") {

    }
    else {

        return false;
    }
    return false;
}

