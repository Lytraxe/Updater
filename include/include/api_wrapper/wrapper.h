#pragma once

#include <include/types.h>
#include <include/utils/curl.h>

#include <iostream>
#include <filesystem>

class Wrapper {
protected:
    std::string _releasesUrl{};
    ReleaseInfo _release{};

public:
    void fetchRelease();
    const std::string& getLatestVersion();
    bool download(const std::filesystem::path& path);

    virtual void parse(Result&) {}
    virtual void applyHeaders(Request&) {}

    const ReleaseInfo& getRelease() const { return _release; }
};
