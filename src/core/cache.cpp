#include "../../include/core/cache.h"
#include "../../include/utils/file_utils.h"
#include "../../include/utils/zip_utils.h"
#include "../../include/utils/curl.h"
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
    Request req{ url, path };
    req.header("Accept: application/vnd.github+json");
    req.header("User-Agent: Lytraxe/updater");

    Result res{};

    CurlWrapper wrapper{ &req, &res };
    if (wrapper.failed()) {
        return false;
    }
    wrapper.download(true);
    if (res._curlCode == 0) {
        if (res._httpRes == 200 || res._httpRes == 201 || res._httpRes == 202) {
            return true;
        }

        std::cerr << "Received non OK http response: " << res._httpRes << '\n';
        return false;
    }
    std::cerr << "Received non OK curl code (" << res._curlCode << "): " << curl_easy_strerror(res._curlCode) << '\n';
    return false;
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

