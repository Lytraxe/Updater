#pragma once

#include <include/api_wrapper/wrapper.h>

#include <string>

class GitHubApi : public Wrapper {
private:
    std::string _auth{};

public:
    GitHubApi(const std::string& repo, const std::string& auth = "");

    void parse(Result& result);
    void applyHeaders(Request& req);
};
