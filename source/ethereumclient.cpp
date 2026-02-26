#include "ethereumclient.hpp"
#include <iostream>
#include <sstream>
#include "logger.hpp"

namespace {
std::string toCompactJson(const Json::Value& value) {
    Json::StreamWriterBuilder writer;
    writer["indentation"] = "";
    return Json::writeString(writer, value);
}
}

EthereumClient::EthereumClient(const std::string& nodeUrl, NetworkAdapter& networkAdapter)
    : nodeUrl(nodeUrl), networkAdapter(networkAdapter) {}

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
        const Json::Value& resultValue = jsonResponse["result"];
        std::string result = resultValue.isString() ? resultValue.asString() : toCompactJson(resultValue);
        std::cout << "Result: " << result << std::endl;
    } else {
        Logger::getInstance().log("Error: 'result' field not found in response.");
    }
}

std::optional<Json::Value> EthereumClient::executeAndExtractResult(const std::string& method, const Json::Value& params) {
    auto response = executeCommand(method, params);
    if (!response) {
        return std::nullopt;
    }

    auto jsonResponse = parseResponse(*response);
    if (!jsonResponse) {
        return std::nullopt;
    }

    if (jsonResponse->isMember("error")) {
        const Json::Value& error = (*jsonResponse)["error"];
        const std::string message = error.isMember("message") ? error["message"].asString() : "Unknown RPC error";
        Logger::getInstance().log("RPC method '" + method + "' failed: " + message);
        return std::nullopt;
    }

    if (!jsonResponse->isMember("result")) {
        Logger::getInstance().log("RPC method '" + method + "' returned no 'result' field.");
        return std::nullopt;
    }

    return (*jsonResponse)["result"];
}

std::optional<std::string> EthereumClient::executeAndExtractStringResult(const std::string& method, const Json::Value& params) {
    auto result = executeAndExtractResult(method, params);
    if (!result) {
        return std::nullopt;
    }

    if (result->isString()) {
        return result->asString();
    }

    return toCompactJson(*result);
}

std::optional<std::string> EthereumClient::getTransactionCount(const std::string& address, const std::string& blockTag) {
    Json::Value params;
    params[0] = address;
    params[1] = blockTag;

    return executeAndExtractStringResult("eth_getTransactionCount", params);
}

std::optional<std::string> EthereumClient::getChainId() {
    Json::Value params;
    return executeAndExtractStringResult("eth_chainId", params);
}

std::optional<std::string> EthereumClient::getNetworkVersion() {
    Json::Value params;
    return executeAndExtractStringResult("net_version", params);
}

std::optional<Json::Value> EthereumClient::getSyncingStatus() {
    Json::Value params;
    return executeAndExtractResult("eth_syncing", params);
}

std::optional<std::string> EthereumClient::getBlockNumber() {
    Json::Value params;
    return executeAndExtractStringResult("eth_blockNumber", params);
}

std::optional<Json::Value> EthereumClient::getBlockByNumber(const std::string& blockNumber, bool fullTransactionData) {
    Json::Value params;
    params[0] = blockNumber;
    params[1] = fullTransactionData;

    return executeAndExtractResult("eth_getBlockByNumber", params);
}

std::optional<Json::Value> EthereumClient::getBlockByHash(const std::string& blockHash, bool fullTransactionData) {
    Json::Value params;
    params[0] = blockHash;
    params[1] = fullTransactionData;

    return executeAndExtractResult("eth_getBlockByHash", params);
}

std::optional<Json::Value> EthereumClient::getTransactionByHash(const std::string& txHash) {
    Json::Value params;
    params[0] = txHash;

    return executeAndExtractResult("eth_getTransactionByHash", params);
}

std::optional<std::string> EthereumClient::estimateGas(const std::string& from, const std::string& to, const std::string& value) {
    Json::Value params;
    Json::Value txData;
    txData["from"] = from;
    txData["to"] = to;
    txData["value"] = value;
    params[0] = txData;

    return executeAndExtractStringResult("eth_estimateGas", params);
}

std::optional<std::string> EthereumClient::getGasPrice() {
    Json::Value params;
    return executeAndExtractStringResult("eth_gasPrice", params);
}

std::optional<std::string> EthereumClient::sendTransaction(const std::string& rawTransaction) {
    Json::Value params;
    params[0] = rawTransaction;

    return executeAndExtractStringResult("eth_sendRawTransaction", params);
}

std::optional<Json::Value> EthereumClient::getLogs(const Json::Value& params) {
    Json::Value rpcParams;
    if (params.isArray()) {
        rpcParams = params;
    } else {
        rpcParams = Json::arrayValue;
        rpcParams.append(params);
    }

    return executeAndExtractResult("eth_getLogs", rpcParams);
}

std::optional<Json::Value> EthereumClient::getTransactionReceipt(const std::string& txHash) {
    Json::Value params;
    params[0] = txHash;

    return executeAndExtractResult("eth_getTransactionReceipt", params);
}
