#pragma once

#include <string>
#include "../types.h"

class GitHubApi {
public:
    std::string owner;
    std::string repo;
    ReleaseInfo release;

    GitHubApi(const char* owner, const char* repo);

    void fetchRelease();
    std::string getLatestVersion();

private:
    std::string releasesUrl;
};
