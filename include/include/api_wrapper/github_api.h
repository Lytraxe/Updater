#pragma once

#include <include/types.h>

#include <string>

class GitHubApi {
public:
    std::string _repo;
    ReleaseInfo _release;

    GitHubApi(const std::string& repo);

    void fetchRelease();
    const std::string& getLatestVersion();

private:
    std::string _releasesUrl;
};
