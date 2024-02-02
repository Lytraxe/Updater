#pragma once

#include <iostream>
#include <filesystem>
#include <curl/curl.h>
#include <include/user_interface/output.h>

struct Result {
    CURLcode _curlCode{};
    int _httpRes{};
    double _totalTime{};
    std::string _content{};
    //std::string _contentType{};
    //- TODO _headers{};
};

struct Request {
    std::string _url{};
    std::filesystem::path _filePath{};
    curl_slist* _headers{ nullptr };

    void header(const std::string& value) { _headers = curl_slist_append(_headers, value.c_str()); }
};

class CurlWrapper {
private:
    Request* _req{ nullptr };
    Result* _res{ nullptr };
    CURL* _curl{ nullptr };
    FILE* _file{ nullptr };
    output::ProgressBar* _bar{ nullptr };
    bool _fail{ false };

public:
    CurlWrapper(Request* req, Result* res);
    ~CurlWrapper() { clean(); }

    void clean();
    void download(bool);
    void json();

    bool failed() { return _fail; }
};
