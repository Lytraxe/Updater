#include "../../include/utils/curl_utils.h"
#include "../../include/user_interface/output.h"
#include <iostream>
#include <fstream>

int progress_callback(void* ptr, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    float progress = (dltotal != 0 ? dlnow / (float)dltotal : 0);
    ((output::ProgressBar*)ptr)->update(progress);
    return 0;
}

size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userd) {
    size_t totalSize = size * nmemb;
    ((std::string*)userd)->append(ptr, totalSize);
    return totalSize;
}

short downloadFile(request_t* req, response_t* res) {
    CURL* handler;

    if ((handler = curl_easy_init())) {
        FILE* fout = fopen(req->path.c_str(), "wb");

        if (!fout) {
            std::cerr << "Could not write to file " << req->path << '\n';
            return 1;
        }

        output::ProgressBar bar{ req->path.filename() };
        curl_easy_setopt(handler, CURLOPT_URL, req->url.c_str());
        curl_easy_setopt(handler, CURLOPT_WRITEDATA, fout);
        curl_easy_setopt(handler, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(handler, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(handler, CURLOPT_XFERINFODATA, &bar);
        curl_easy_setopt(handler, CURLOPT_XFERINFOFUNCTION, progress_callback);

        if (req->headers)
            curl_easy_setopt(handler, CURLOPT_HTTPHEADER, req->headers);

        res->curl_code = curl_easy_perform(handler);

        if (res->curl_code == CURLE_OK) {
            curl_easy_getinfo(handler, CURLINFO_RESPONSE_CODE, &(res->res_code));
            curl_easy_getinfo(handler, CURLINFO_TOTAL_TIME, &(res->total_time));
            curl_easy_getinfo(handler, CURLINFO_CONTENT_TYPE, &(res->content_type));
        }

        curl_easy_cleanup(handler);
        if (req->headers)
            curl_slist_free_all(req->headers);
        //fout.close();
        fclose(fout);
    }
    return 0;
}
/**
 * Get JSON response from the given request.
 * The response will be parsed and `res` will be initialized with the parser and the method will return `0`
 * In canse of non-zero http response, negative response code will be returned (yes).
 * In case of any CURL error, the CURLcode will be returned.
*/
long getJsonResponse(request_t& req, JsonParser& res) {
    CURL* handler;
    long val = -1;
    CURLcode ccode;

    if ((handler = curl_easy_init())) {
        std::string out;
        curl_easy_setopt(handler, CURLOPT_URL, req.url.c_str());
        curl_easy_setopt(handler, CURLOPT_WRITEDATA, (void*)&out);
        curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION, writeCallback);
        req.headers = curl_slist_append(req.headers, "Accept: application/json");
        req.headers = curl_slist_append(req.headers, "User-Agent: Lytraxe/updater");

        curl_easy_setopt(handler, CURLOPT_HTTPHEADER, req.headers);

        if ((ccode = curl_easy_perform(handler)) == CURLE_OK) {
            curl_easy_getinfo(handler, CURLINFO_RESPONSE_CODE, &val);
            if (val == 200) {
                val = 0;
                res.init(out.c_str());
            }
            else val *= -1;
        }
        else val = ccode;
        curl_easy_cleanup(handler);
        curl_slist_free_all(req.headers);
    }
    return val;
}
