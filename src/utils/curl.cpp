#include <include/utils/curl.h>
#include <include/user_interface/output.h>

int progress_callback(void* ptr, curl_off_t dltotal, curl_off_t dlnow, curl_off_t, curl_off_t) {
    bool begin{ dltotal != 0 };
    float progress = (begin ? dlnow / (float)dltotal : 0);
    if (dlnow == dltotal && begin) ((output::ProgressBar*)ptr)->finish();
    else ((output::ProgressBar*)ptr)->update(progress);
    return 0;
}

size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userd) {
    size_t totalSize = size * nmemb;
    ((std::string*)userd)->append(ptr, totalSize);
    return totalSize;
}

CurlWrapper::CurlWrapper(Request* req, Result* res) : _req{ req }, _res{ res } {
    _curl = curl_easy_init();
    if (!_curl) {
        _fail = true;
        return;
    }

    if (!_req->_filePath.empty()) {
        _file = fopen(req->_filePath.c_str(), "wb");
        if (!_file) {
            _fail = true;
            clean();
            return;
        }
    }
}

void CurlWrapper::clean() {
    if (_curl) {
        curl_easy_cleanup(_curl);
        _curl = nullptr;
    }

    if (_file) {
        fclose(_file);
        _file = nullptr;
    }

    if (_req->_headers) {
        curl_slist_free_all(_req->_headers);
        _req->_headers = nullptr;
    }
}

void CurlWrapper::download(bool progressBar = false) {
    curl_easy_setopt(_curl, CURLOPT_URL, _req->_url.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, _file);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1);

    if (progressBar) {
        output::ProgressBar bar{ _req->_filePath.filename().string() + " " };
        curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(_curl, CURLOPT_XFERINFODATA, &bar);
        curl_easy_setopt(_curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
    }

    if (_req->_headers)
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _req->_headers);

    _res->_curlCode = curl_easy_perform(_curl);

    if (_res->_curlCode == CURLE_OK) {
        curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &(_res->_httpRes));
        curl_easy_getinfo(_curl, CURLINFO_TOTAL_TIME, &(_res->_totalTime));
        //curl_easy_getinfo(_curl, CURLINFO_CONTENT_TYPE, &(_res->_contentType));
    }
}

void CurlWrapper::json() {
    curl_easy_setopt(_curl, CURLOPT_URL, _req->_url.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void*)&(_res->_content));
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    _req->_headers = curl_slist_append(_req->_headers, "Accept: application/json");
    _req->_headers = curl_slist_append(_req->_headers, "User-Agent: Lytraxe/updater");

    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _req->_headers);

    _res->_curlCode = curl_easy_perform(_curl);

    if (_res->_curlCode == CURLE_OK) {
        curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &(_res->_httpRes));
        curl_easy_getinfo(_curl, CURLINFO_TOTAL_TIME, &(_res->_totalTime));
        //curl_easy_getinfo(_curl, CURLINFO_CONTENT_TYPE, &(_res->_contentType));
    }
}
