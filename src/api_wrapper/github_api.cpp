#include <include/api_wrapper/github_api.h>
#include <include/utils/curl.h>
#include <include/utils/json_utils.h>

#include <iostream>

GitHubApi::GitHubApi(const std::string& repo, const std::string& auth) : _auth{ auth } {
    this->_releasesUrl = "https://api.github.com/repos/" + repo + "/releases";
}

void GitHubApi::parse(Result& result) {
    JsonParser parser{ result._content };
    parser.set(parser.jsonData[0]);
    if (parser.jsonData) {
        this->_release.tag = parser.getString("tag_name");
        this->_release.fileName = parser.get("assets")[0]["name"].asString();
        this->_release.fileDownloadUrl = parser.get("assets")[0]["browser_download_url"].asString();
    }
}

void GitHubApi::applyHeaders(Request& req) {
    req.header("Accept: application/vnd.github+json");
    if (!_auth.empty()) {
        req.header("Authorization: Bearer " + _auth);
    }
}
