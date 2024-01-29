#include "../../include/api_wrapper/github_api.h"
#include "../../include/utils/curl.h"
#include "../../include/utils/json_utils.h"
#include <iostream>

GitHubApi::GitHubApi(const std::string& repo) : _repo{ repo } {
    this->_releasesUrl = "https://api.github.com/repos/" + this->_repo + "/releases";
}

const std::string& GitHubApi::getLatestVersion() {
    if (!this->_release.tag.empty()) return this->_release.tag;
    fetchRelease();
    return this->_release.tag;
}

void GitHubApi::fetchRelease() {
    Request req{ this->_releasesUrl };
    Result res{};
    {
        CurlWrapper wrapper{ &req, &res };
        if (wrapper.failed()) {
            std::cerr << "Error occured while fetching release\n";
            return;
        }
        wrapper.json();
    }
    if (res._curlCode != 0) {
        std::cerr << "Received non OK curl code (" << res._curlCode << "): " << curl_easy_strerror(res._curlCode) << '\n';
        return;
    }

    if (!(res._httpRes == 200 || res._httpRes == 201 || res._httpRes == 202)) {
        std::cerr << "Received non OK http response: " << res._httpRes << '\n';
        return;
    }

    JsonParser parser{ res._content };
    parser.set(parser.jsonData[0]);
    if (parser.jsonData) {
        this->_release.tag = parser.getString("tag_name");
        this->_release.fileName = parser.get("assets")[0]["name"].asString();
        this->_release.fileDownloadUrl = parser.get("assets")[0]["browser_download_url"].asString();
    }
}
