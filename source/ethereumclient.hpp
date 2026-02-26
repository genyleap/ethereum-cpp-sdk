#ifndef ETHEREUM_CLIENT_HPP
#define ETHEREUM_CLIENT_HPP

#include "common.hpp"
#include <json/json.h>
#include "networkadapter.hpp"

/**
 * @class EthereumClient
 * @brief A class to interact with an Ethereum or Ethereum-compatible node.
 *
 * This class provides methods to send RPC requests to an Ethereum node,
 * parse the responses, and process the results. It supports common Ethereum RPC methods
 * such as retrieving block data, transaction details, estimating gas, and more.
 */
class PROJECT_EXPORT EthereumClient {
public:
    /**
     * @brief Constructs an EthereumClient instance.
     * @param nodeUrl The URL of the Ethereum node (e.g., Infura, local node).
     * @param networkAdapter A reference to the NetworkAdapter used for sending requests.
     */
    EthereumClient(const std::string& nodeUrl, NetworkAdapter& networkAdapter);

    /**
     * @brief General method to send RPC requests.
     * @param method The name of the RPC method to call (e.g., "eth_blockNumber").
     * @param params The parameters for the RPC method (as a JSON object).
     * @return The raw JSON response as a string, wrapped in std::optional.
     */
    std::optional<std::string> executeCommand(const std::string& method, const Json::Value& params);

    /**
     * @brief Parses the response from the Ethereum node.
     * @param response The raw response from the Ethereum node.
     * @return The parsed JSON response, or an empty std::optional in case of failure.
     */
    std::optional<Json::Value> parseResponse(const std::string& response);

    /**
     * @brief Processes the parsed JSON response and outputs relevant information.
     * @param jsonResponse The parsed JSON response from the node.
     */
    void processResult(const Json::Value& jsonResponse);

           // Ethereum RPC Methods

    /**
     * @brief Retrieves the latest block number.
     * @return The block number as a string in hexadecimal format, or an empty std::optional if an error occurs.
     */
    std::optional<std::string> getBlockNumber();

    /**
     * @brief Retrieves block information by block number.
     * @param blockNumber The block number (hexadecimal).
     * @param fullTransactionData Flag to determine whether to fetch full transaction data.
     * @return The block data in JSON format, or an empty std::optional if an error occurs.
     */
    std::optional<Json::Value> getBlockByNumber(const std::string& blockNumber, bool fullTransactionData);

    /**
     * @brief Retrieves block information by block hash.
     * @param blockHash The block hash (hexadecimal).
     * @param fullTransactionData Flag to determine whether to fetch full transaction data.
     * @return The block data in JSON format, or an empty std::optional if an error occurs.
     */
    std::optional<Json::Value> getBlockByHash(const std::string& blockHash, bool fullTransactionData);

    /**
     * @brief Retrieves transaction information by transaction hash.
     * @param txHash The transaction hash.
     * @return The transaction data in JSON format, or an empty std::optional if an error occurs.
     */
    std::optional<Json::Value> getTransactionByHash(const std::string& txHash);

    /**
     * @brief Estimates the gas required for a transaction.
     * @param from The sender address.
     * @param to The recipient address.
     * @param value The value to be sent (in hexadecimal).
     * @return The estimated gas (in hexadecimal), or an empty std::optional if an error occurs.
     */
    std::optional<std::string> estimateGas(const std::string& from, const std::string& to, const std::string& value);

    /**
     * @brief Retrieves the current gas price.
     * @return The current gas price in hexadecimal format, or an empty std::optional if an error occurs.
     */
    std::optional<std::string> getGasPrice();

    /**
     * @brief Sends a signed raw transaction.
     * @param rawTransaction The signed raw transaction bytes encoded as a hex string.
     * @return The transaction hash (if successful), or an empty std::optional if an error occurs.
     */
    std::optional<std::string> sendTransaction(const std::string& rawTransaction);

    /**
     * @brief Retrieves logs based on filter parameters.
     * @param params The filter parameters for fetching logs.
     * @return The logs in JSON format, or an empty std::optional if an error occurs.
     */
    std::optional<Json::Value> getLogs(const Json::Value& params);

    /**
     * @brief Retrieves the transaction receipt by transaction hash.
     * @param txHash The transaction hash.
     * @return The transaction receipt data in JSON format, or an empty std::optional if an error occurs.
     */
    std::optional<Json::Value> getTransactionReceipt(const std::string& txHash);

           // Additional Methods

    /**
     * @brief Retrieves the transaction count (nonce) for an address.
     * @param address The address for which to fetch the transaction count.
     * @param blockTag The block parameter (e.g., "latest", "pending", or a block number in hex).
     * @return The transaction count (nonce) as a string, or an empty std::optional if an error occurs.
     */
    std::optional<std::string> getTransactionCount(const std::string& address, const std::string& blockTag = "latest");

    /**
     * @brief Retrieves the chain ID of the connected Ethereum network.
     * @return The chain ID as a string, or an empty std::optional if an error occurs.
     */
    std::optional<std::string> getChainId();

    /**
     * @brief Retrieves the version of the connected Ethereum network.
     * @return The network version as a string, or an empty std::optional if an error occurs.
     */
    std::optional<std::string> getNetworkVersion();

    /**
     * @brief Checks if the Ethereum node is syncing.
     * @return Syncing status in JSON format, or an empty std::optional if an error occurs.
     */
    std::optional<Json::Value> getSyncingStatus();

private:
    /**
     * @brief Executes an RPC method and extracts the "result" field from the response.
     */
    std::optional<Json::Value> executeAndExtractResult(const std::string& method, const Json::Value& params);

    /**
     * @brief Executes an RPC method and extracts the "result" field as a string.
     * If the result is not a JSON string, it is serialized as compact JSON.
     */
    std::optional<std::string> executeAndExtractStringResult(const std::string& method, const Json::Value& params);

    std::string nodeUrl; ///< The URL of the Ethereum node.
    NetworkAdapter& networkAdapter; ///< Reference to the network adapter used for sending requests.
};

#endif // ETHEREUM_CLIENT_HPP
