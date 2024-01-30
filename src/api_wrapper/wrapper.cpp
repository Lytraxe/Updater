#include <include/api_wrapper/wrapper.h>

#include <filesystem>

const std::string& Wrapper::getLatestVersion() {
    if (!this->_release.tag.empty()) return this->_release.tag;
    fetchRelease();
    return this->_release.tag;
}

void Wrapper::fetchRelease() {
    Request req{ this->_releasesUrl };
    applyHeaders(req);

    Result res{};
    {
        CurlWrapper wrapper{ &req, &res };
        if (wrapper.failed()) {
            std::cerr << "Error occured while fetching release\n";
            return;
        }
        wrapper.json();
    }
    if (res._curlCode != 0) {
        std::cerr << "Received non OK curl code (" << res._curlCode << "): " << curl_easy_strerror(res._curlCode) << '\n';
        return;
    }

    if (!(res._httpRes == 200 || res._httpRes == 201 || res._httpRes == 202)) {
        std::cerr << "GET " << req._url << ":\n  Received non OK http response: " << res._httpRes << '\n';
        return;
    }
    this->parse(res);
}

bool Wrapper::download(const std::filesystem::path& path) {
    Request req{ getRelease().fileDownloadUrl, path };

    req.header("User-Agent: Lytraxe/updater");
    applyHeaders(req);

    Result res{};

    CurlWrapper wrapper{ &req, &res };
    if (wrapper.failed()) {
        std::cerr << "Failed to download file\n";
        return false;
    }
    wrapper.download(true);
    if (res._curlCode == 0) {
        if (res._httpRes == 200 || res._httpRes == 201 || res._httpRes == 202) {
            return true;
        }

        std::cerr << "Received non OK http response: " << res._httpRes << '\n';
        return false;
    }
    std::cerr << "Received non OK curl code (" << res._curlCode << "): " << curl_easy_strerror(res._curlCode) << '\n';
    return false;
}
