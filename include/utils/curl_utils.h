#ifndef CURL_UTILS
#define CURL_UTILS

#include <curl/curl.h>
#include "json_utils.h"

struct request_t {
    const char* url;
    const char* path;
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

size_t handleOutput(char* ptr, size_t size, size_t nmemb, void* userd);
size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userd);

#endif