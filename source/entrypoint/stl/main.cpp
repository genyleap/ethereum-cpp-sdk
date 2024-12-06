#include "utility.hpp"
#include "logger.hpp"
#include "ethereumclient.hpp"

/**
 * @brief Main function that demonstrates interaction with the Ethereum client.
 *
 * This example loads the configuration, creates an Ethereum client, and performs
 * several Ethereum RPC method calls to get block numbers, estimate gas, and more.
 *
 * Steps:
 * 1. Load configuration from the config file.
 * 2. Create a `NetworkAdapter` and `EthereumClient` instance.
 * 3. Call Ethereum RPC methods to get block number, block data, estimate gas, etc.
 * 4. Print the results to the console.
 *
 * @return int Exit code of the program.
 */
int main() {
    // Load the configuration (node URL) from the config.json file.
    /**
     * @brief Load the Ethereum node URL from a configuration file.
     *
     * The loadConfig function tries to read the node URL from the provided configuration file.
     * If the configuration is missing or there is an issue loading it, the program will log an error
     * and exit.
     */
    auto nodeUrlOpt = loadConfig("config.json");
    if (!nodeUrlOpt) {
        Logger::getInstance().log("Failed to load configuration.");
        return -1; ///< Return -1 if the configuration failed to load.
    }
    std::string nodeUrl = *nodeUrlOpt; ///< Get the Ethereum node URL from the config file.

           // Create NetworkAdapter and EthereumClient
    /**
     * @brief Create instances of NetworkAdapter and EthereumClient.
     *
     * NetworkAdapter is used to send HTTP requests to the Ethereum node, while EthereumClient
     * is used to interact with the Ethereum node via the RPC methods.
     */
    NetworkAdapter networkAdapter;
    EthereumClient client(nodeUrl, networkAdapter);

           // Example 1: Get the current block number
    /**
     * @brief Get the current block number using the `eth_blockNumber` method.
     *
     * The `getBlockNumber` method sends a request to the Ethereum node to retrieve the current block
     * number. The block number is returned as a string, which is printed to the console.
     */
    auto blockNumber = client.getBlockNumber();
    if (blockNumber) {
        std::print("Current Block Number: {}", *blockNumber);
    }

           // Example 2: Get block information by block number
    /**
     * @brief Get block data by block number using the `eth_getBlockByNumber` method.
     *
     * The `getBlockByNumber` method sends a request to the Ethereum node to retrieve the block data
     * corresponding to a given block number (in hexadecimal format). It fetches the block's transactions
     * if `fullTransactionData` is true.
     */
    std::string blockNumberStr = "0x5d5f"; ///< Example block number in hexadecimal format.
    auto blockData = client.getBlockByNumber(blockNumberStr, true); // Fetch full transaction data
    if (blockData) {
        std::print("Block Data: {}", blockData->toStyledString());
    }

           // Example 3: Estimate gas for a transaction
    /**
     * @brief Estimate the gas required for a transaction using the `eth_estimateGas` method.
     *
     * The `estimateGas` method estimates the gas required to send a transaction from one address to
     * another, with a specified value (in hexadecimal format). It returns the estimated gas as a string.
     */
    std::string from = "0x7960f1b90b257bff29d5164d16bca4c8030b7f6d"; ///< Example "from" address.
    std::string to = "0x7960f1b90b257bff29d5164d16bca4c8030b7f6d"; ///< Example "to" address.
    std::string value = "0x9184e72a"; ///< Example value in hexadecimal.
    auto gasEstimate = client.estimateGas(from, to, value);
    if (gasEstimate) {
        std::print("Estimated Gas: ", *gasEstimate);
    }

           // Example 4: Get Ethereum network version
    /**
     * @brief Get the Ethereum network version using the `net_version` method.
     *
     * The `getNetworkVersion` method retrieves the version of the Ethereum network the node is connected to.
     * It returns the network version as a string.
     */
    auto protocolVersion = client.getNetworkVersion();
    if (protocolVersion) {
        std::print("Ethereum Protocol Version: ", *protocolVersion);
    }

    return 0; ///< Return 0 if the program executed successfully.
}
