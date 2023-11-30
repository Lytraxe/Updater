#include "../../include/api_wrapper/github_api.h"
#include "../../include/utils/curl_utils.h"
#include <iostream>

GitHubApi::GitHubApi(const char* owner, const char* repo) {
    GitHubApi::owner = owner;
    GitHubApi::repo = repo;

    GitHubApi::releasesUrl = "https://api.github.com/repos/" + GitHubApi::owner + "/" + GitHubApi::repo + "/releases";
}

std::string GitHubApi::getLatestVersion() {
    if (!release.tag.empty()) return release.tag;
    fetchRelease();
    return release.tag;
}

void GitHubApi::fetchRelease() {
    JsonParser parser;
    request_t req{ 0 };
    req.url = releasesUrl.c_str();

    short res = getJsonResponse(req, parser);
    if (res != 0) {
        // TODO: Handle this
        std::cout << "Non zero curl response " << res << '\n';
        return;
    }
    parser.set(parser.jsonData[0]);
    if (parser.jsonData) {
        release.tag = parser.getString("tag_name");
        release.fileName = parser.get("assets")[0]["name"].asString();
        release.fileDownloadUrl = parser.get("assets")[0]["browser_download_url"].asString();
    }
}
