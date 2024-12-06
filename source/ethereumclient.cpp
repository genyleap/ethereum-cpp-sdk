#include "ethereumclient.hpp"
#include <iostream>
#include "logger.hpp"

EthereumClient::EthereumClient(const std::string& nodeUrl, NetworkAdapter& networkAdapter)
    : nodeUrl(nodeUrl), networkAdapter(networkAdapter) {}  // Correct initialization list

std::optional<std::string> EthereumClient::executeCommand(const std::string& method, const Json::Value& params) {
    Json::Value payload;
    payload["jsonrpc"] = "2.0";
    payload["method"] = method;
    payload["params"] = params;
    payload["id"] = 1;

    Json::StreamWriterBuilder writer;
    std::string requestData = Json::writeString(writer, payload);

    auto response = networkAdapter.sendPostRequest(nodeUrl, requestData);
    if (!response) {
        Logger::getInstance().log("Failed to get response for method: " + method);
        return std::nullopt;
    }
    return *response;
}

std::optional<Json::Value> EthereumClient::parseResponse(const std::string& response) {
    Json::Value jsonResponse;
    Json::CharReaderBuilder reader;
    std::string errs;

    std::istringstream stream(response);
    if (!Json::parseFromStream(reader, stream, &jsonResponse, &errs)) {
        Logger::getInstance().log("Error parsing response: " + errs);
        return std::nullopt;
    }

    return jsonResponse;
}

void EthereumClient::processResult(const Json::Value& jsonResponse) {
    if (jsonResponse.isMember("error")) {
        Logger::getInstance().log("Error: " + jsonResponse["error"]["message"].asString());
        return;
    }

    if (jsonResponse.isMember("result")) {
        std::string result = jsonResponse["result"].asString();
        std::cout << "Result: " << result << std::endl;
    } else {
        Logger::getInstance().log("Error: 'result' field not found in response.");
    }
}

std::optional<std::string> EthereumClient::getTransactionCount(const std::string& address) {
    Json::Value params;
    params[0] = address;

    auto response = executeCommand("eth_getTransactionCount", params);
    return response;
}

std::optional<std::string> EthereumClient::getChainId() {
    Json::Value params;
    auto response = executeCommand("eth_chainId", params);
    return response;
}

std::optional<std::string> EthereumClient::getNetworkVersion() {
    Json::Value params; // Empty params array
    auto response = executeCommand("net_version", params);
    return response;
}

std::optional<std::string> EthereumClient::getSyncingStatus() {
    Json::Value params;
    auto response = executeCommand("eth_syncing", params);
    return response;
}

std::optional<std::string> EthereumClient::getBlockNumber() {
    Json::Value params;
    auto response = executeCommand("eth_blockNumber", params);
    return response;
}

std::optional<Json::Value> EthereumClient::getBlockByNumber(const std::string& blockNumber, bool fullTransactionData) {
    Json::Value params;
    params[0] = blockNumber;
    params[1] = fullTransactionData;

    auto response = executeCommand("eth_getBlockByNumber", params);
    if (!response) {
        return std::nullopt;
    }

    return parseResponse(*response);
}

std::optional<Json::Value> EthereumClient::getBlockByHash(const std::string& blockHash, bool fullTransactionData) {
    Json::Value params;
    params[0] = blockHash;
    params[1] = fullTransactionData;

    auto response = executeCommand("eth_getBlockByHash", params);
    if (!response) {
        return std::nullopt;
    }

    return parseResponse(*response);
}

std::optional<Json::Value> EthereumClient::getTransactionByHash(const std::string& txHash) {
    Json::Value params;
    params[0] = txHash;

    auto response = executeCommand("eth_getTransactionByHash", params);
    if (!response) {
        return std::nullopt;
    }

    return parseResponse(*response);
}

std::optional<std::string> EthereumClient::estimateGas(const std::string& from, const std::string& to, const std::string& value) {
    Json::Value params;
    Json::Value txData;
    txData["from"] = from;
    txData["to"] = to;
    txData["value"] = value;
    params[0] = txData;

    auto response = executeCommand("eth_estimateGas", params);
    return response;
}

std::optional<std::string> EthereumClient::getGasPrice() {
    Json::Value params;
    auto response = executeCommand("eth_gasPrice", params);
    return response;
}

std::optional<std::string> EthereumClient::sendTransaction(const std::string& rawTransaction) {
    Json::Value params;
    params[0] = rawTransaction;

    auto response = executeCommand("eth_sendTransaction", params);
    return response;
}

std::optional<Json::Value> EthereumClient::getLogs(const Json::Value& params) {
    auto response = executeCommand("eth_getLogs", params);
    if (!response) {
        return std::nullopt;
    }

    return parseResponse(*response);
}

std::optional<Json::Value> EthereumClient::getTransactionReceipt(const std::string& txHash) {
    Json::Value params;
    params[0] = txHash;

    auto response = executeCommand("eth_getTransactionReceipt", params);
    if (!response) {
        return std::nullopt;
    }

    return parseResponse(*response);
}
