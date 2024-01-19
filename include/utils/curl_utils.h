#pragma once

#include <curl/curl.h>
#include "json_utils.h"
#include <filesystem>

struct request_t {
    std::string url;
    std::filesystem::path path;
    curl_slist* headers;
};

struct response_t {
    short res_code;
    CURLcode curl_code;
    double total_time;
    char* content_type;
};

short downloadFile(request_t* req, response_t* res);
long getJsonResponse(request_t& req, JsonParser& res);
