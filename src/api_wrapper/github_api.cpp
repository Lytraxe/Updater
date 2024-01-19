#include "../../include/api_wrapper/github_api.h"
#include "../../include/utils/curl_utils.h"
#include <iostream>

GitHubApi::GitHubApi(const std::string& repo) : _repo {repo} {
    this->_releasesUrl = "https://api.github.com/repos/" + this->_repo + "/releases";
}

const std::string& GitHubApi::getLatestVersion() {
    if (!this->_release.tag.empty()) return this->_release.tag;
    fetchRelease();
    return this->_release.tag;
}

void GitHubApi::fetchRelease() {
    JsonParser parser;
    request_t req{};
    req.url = this->_releasesUrl.c_str();

    short res = getJsonResponse(req, parser);
    if (res != 0) {
        // TODO: Handle this
        std::cout << "Non zero curl response " << res << '\n';
        return;
    }
    parser.set(parser.jsonData[0]);
    if (parser.jsonData) {
        this->_release.tag = parser.getString("tag_name");
        this->_release.fileName = parser.get("assets")[0]["name"].asString();
        this->_release.fileDownloadUrl = parser.get("assets")[0]["browser_download_url"].asString();
    }
}
