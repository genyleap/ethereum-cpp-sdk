#include "networkadapter.hpp"
#include "logger.hpp"

NetworkAdapter::NetworkAdapter() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curlHandle = curl_easy_init();
}

NetworkAdapter::~NetworkAdapter() {
    if (curlHandle) {
        curl_easy_cleanup(curlHandle);
    }
    curl_global_cleanup();
}

std::optional<std::string> NetworkAdapter::sendPostRequest(const std::string& url, const std::string& data) {
    std::string response;
    CURLcode res;

    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_POST, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDSIZE, data.length());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 30L);

    res = curl_easy_perform(curlHandle);
    if (res != CURLE_OK) {
        Logger::getInstance().log("CURL error: " + std::string(curl_easy_strerror(res)));
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
