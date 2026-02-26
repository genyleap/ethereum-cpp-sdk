#ifndef NETWORKADAPTER_HPP
#define NETWORKADAPTER_HPP

#include "common.hpp"
#include <curl/curl.h>

/**
 * @class NetworkAdapter
 * @brief A class that handles HTTP requests to an Ethereum node using libcurl.
 *
 * This class provides methods to send HTTP POST requests to an Ethereum node,
 * retrieve the response, and handle the low-level network communication using libcurl.
 * It is designed to be used by other classes to interact with an Ethereum node.
 */
class PROJECT_EXPORT NetworkAdapter {
public:
    /**
     * @brief Constructs a NetworkAdapter instance.
     *
     * Initializes the libcurl library and prepares the curl handle for making requests.
     */
    NetworkAdapter();

    /**
     * @brief Destructs the NetworkAdapter instance.
     *
     * Cleans up the curl handle and shuts down the libcurl library.
     */
    ~NetworkAdapter();

    NetworkAdapter(const NetworkAdapter&) = delete;
    NetworkAdapter& operator=(const NetworkAdapter&) = delete;
    NetworkAdapter(NetworkAdapter&&) = delete;
    NetworkAdapter& operator=(NetworkAdapter&&) = delete;

    /**
     * @brief Sends a POST request to the specified URL with the provided data.
     * @param url The URL to send the POST request to (e.g., the Ethereum node endpoint).
     * @param data The data to send in the body of the POST request (usually a JSON-RPC request).
     * @return The response body as a string if successful, or an empty std::optional if an error occurs.
     */
    std::optional<std::string> sendPostRequest(const std::string& url, const std::string& data);

private:
    CURL* curlHandle = nullptr; ///< The libcurl handle used for making HTTP requests.

    /**
     * @brief Callback function used by libcurl to write the response data.
     * @param contents The received data chunk.
     * @param size The size of each chunk of data.
     * @param nmemb The number of chunks.
     * @param userp A pointer to the user data (used to store the response).
     * @return The number of bytes successfully written.
     */
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // NETWORKADAPTER_HPP
