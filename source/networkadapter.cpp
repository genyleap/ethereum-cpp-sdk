#include "networkadapter.hpp"
#include "logger.hpp"
#include <mutex>

namespace {
std::once_flag gCurlInitFlag;
bool gCurlInitialized = false;
}

NetworkAdapter::NetworkAdapter() {
    std::call_once(gCurlInitFlag, []() {
        const CURLcode initResult = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (initResult != CURLE_OK) {
            Logger::getInstance().log("Failed to initialize libcurl globals: " + std::string(curl_easy_strerror(initResult)));
            return;
        }
        gCurlInitialized = true;
    });

    if (!gCurlInitialized) {
        Logger::getInstance().log("libcurl global initialization failed.");
        return;
    }

    curlHandle = curl_easy_init();
    if (!curlHandle) {
        Logger::getInstance().log("Failed to create CURL handle.");
    }
}

NetworkAdapter::~NetworkAdapter() {
    if (curlHandle) {
        curl_easy_cleanup(curlHandle);
        curlHandle = nullptr;
    }
}

std::optional<std::string> NetworkAdapter::sendPostRequest(const std::string& url, const std::string& data) {
    if (!curlHandle) {
        Logger::getInstance().log("Cannot send request: CURL handle is not initialized.");
        return std::nullopt;
    }

    std::string response;
    CURLcode res = CURLE_OK;

    curl_easy_reset(curlHandle);

    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (!headers) {
        Logger::getInstance().log("Failed to create HTTP headers for request.");
        return std::nullopt;
    }

    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_POST, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(data.size()));
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, 2L);

    res = curl_easy_perform(curlHandle);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, nullptr);
    curl_slist_free_all(headers);
    if (res != CURLE_OK) {
        Logger::getInstance().log("CURL error: " + std::string(curl_easy_strerror(res)));
        return std::nullopt;
    }

    long httpStatusCode = 0;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &httpStatusCode);
    if (httpStatusCode < 200 || httpStatusCode >= 300) {
        Logger::getInstance().log("HTTP error: status code " + std::to_string(httpStatusCode));
        return std::nullopt;
    }

    return response;
}

size_t NetworkAdapter::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = reinterpret_cast<std::string*>(userp);
    response->append(reinterpret_cast<char*>(contents), totalSize);
    return totalSize;
}
