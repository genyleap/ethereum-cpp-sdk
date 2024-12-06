Here’s a **README** for the **`ethereum-cpp-sdk`**:

---

# ethereum-cpp-sdk

**ethereum-cpp-sdk** is a C++ SDK for interacting with the Ethereum blockchain. This SDK provides a set of tools for performing common Ethereum operations such as querying block information, estimating gas for transactions, and interacting with smart contracts using the Ethereum JSON-RPC API.

## Features

- **Blockchain Queries**: Retrieve block details, block numbers, and transaction data.
- **Gas Estimation**: Estimate gas for transactions.
- **Smart Contract Interaction**: Send and receive data from Ethereum smart contracts.
- **Ethereum Network Info**: Fetch information about Ethereum network version, syncing status, and more.
- **Customizable**: Easy integration into C++ applications with minimal dependencies.

## Requirements

- C++20 or higher.
- [PT](https://github.com/genyleap/Project-Template) Based on Project Template
- [libcurl](https://curl.se/libcurl/) for HTTP requests. (Enabled as default by PT)
- [JSONcpp](https://github.com/open-source-parsers/jsoncpp) for JSON parsing. (Enabled as default by PT)

## Installation

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/ethereum-cpp-sdk.git
cd ethereum-cpp-sdk
```

### 2. Install dependencies

Ensure that you have `libcurl` and `jsoncpp` installed. You can install them using your system's package manager.

For Ubuntu/Debian:

```bash
sudo apt-get install libcurl4-openssl-dev libjsoncpp-dev
```

For MacOS (using Homebrew):

```bash
brew install curl jsoncpp
```

### 3. Build the project

```bash
mkdir build
cd build
cmake ..
make
```

### 4. Link to your project

To use `ethereum-cpp-sdk` in your project, link the compiled library files or copy the header and source files directly.

---

## Usage

### Configuration

Before using the SDK, ensure you have an Ethereum node URL in a `config.json` file. Here’s an example of what the file might look like:

```json
{
  "nodeUrl": "https://your-ethereum-node-url"
}
```

### Example: Interacting with Ethereum

Here's an example that demonstrates how to use the SDK to interact with Ethereum. This example fetches the current block number and estimates gas for a transaction.

```cpp
#include "utility.hpp"
#include "logger.hpp"
#include "ethereumclient.hpp"
#include <iostream>

int main() {
    // Load the configuration (node URL)
    auto nodeUrlOpt = loadConfig("config.json");
    if (!nodeUrlOpt) {
        Logger::getInstance().log("Failed to load configuration.");
        return -1;
    }
    std::string nodeUrl = *nodeUrlOpt;

    // Create NetworkAdapter and EthereumClient
    NetworkAdapter networkAdapter;
    EthereumClient client(nodeUrl, networkAdapter);

    // Example 1: Get the current block number
    auto blockNumber = client.getBlockNumber();
    if (blockNumber) {
        std::cout << "Current Block Number: " << *blockNumber << std::endl;
    }

    // Example 2: Get block information by block number
    std::string blockNumberStr = "0x5d5f"; // Block number in hexadecimal
    auto blockData = client.getBlockByNumber(blockNumberStr, true); // Fetch full transaction data
    if (blockData) {
        std::cout << "Block Data: " << blockData->toStyledString() << std::endl;
    }

    // Example 3: Estimate gas for a transaction
    std::string from = "0x7960f1b90b257bff29d5164d16bca4c8030b7f6d";
    std::string to = "0x7960f1b90b257bff29d5164d16bca4c8030b7f6d";
    std::string value = "0x9184e72a"; // Value in hex
    auto gasEstimate = client.estimateGas(from, to, value);
    if (gasEstimate) {
        std::cout << "Estimated Gas: " << *gasEstimate << std::endl;
    }

    // Example 4: Get Ethereum network version
    auto protocolVersion = client.getNetworkVersion();
    if (protocolVersion) {
        std::cout << "Ethereum Protocol Version: " << *protocolVersion << std::endl;
    }

    return 0;
}
```

### Supported Ethereum RPC Methods

The SDK supports the following Ethereum RPC methods:

- `eth_blockNumber`: Get the current block number.
- `eth_getBlockByNumber`: Get block data by block number.
- `eth_getBlockByHash`: Get block data by block hash.
- `eth_estimateGas`: Estimate gas for a transaction.
- `eth_gasPrice`: Get the current gas price.
- `eth_sendTransaction`: Send a transaction.
- `eth_getLogs`: Fetch logs from the Ethereum network.
- `eth_getTransactionReceipt`: Get the transaction receipt for a given hash.
- `eth_getTransactionCount`: Get the transaction count (nonce) for an address.
- `eth_chainId`: Get the Ethereum chain ID.
- `eth_protocolVersion`: Get the protocol version of the connected Ethereum node.
- `eth_syncing`: Check the syncing status of the node.

---

## API Reference

### `EthereumClient` Class

- **`EthereumClient(const std::string& nodeUrl, NetworkAdapter& networkAdapter)`**: Initializes the client with the Ethereum node URL and a network adapter for communication.
  
- **`std::optional<std::string> getBlockNumber()`**: Retrieves the current block number.

- **`std::optional<Json::Value> getBlockByNumber(const std::string& blockNumber, bool fullTransactionData)`**: Retrieves block data by block number.

- **`std::optional<Json::Value> getBlockByHash(const std::string& blockHash, bool fullTransactionData)`**: Retrieves block data by block hash.

- **`std::optional<std::string> estimateGas(const std::string& from, const std::string& to, const std::string& value)`**: Estimates the gas required for a transaction.

- **`std::optional<std::string> getGasPrice()`**: Retrieves the current gas price.

- **`std::optional<std::string> sendTransaction(const std::string& rawTransaction)`**: Sends a raw transaction to the Ethereum network.

- **`std::optional<Json::Value> getLogs(const Json::Value& params)`**: Fetches logs based on the provided parameters.

- **`std::optional<Json::Value> getTransactionReceipt(const std::string& txHash)`**: Retrieves the receipt for a transaction by hash.

---

## Contributing

Feel free to contribute to the development of this SDK. You can contribute by:

1. Forking the repository and submitting a pull request with your changes.
2. Reporting issues and bugs using the GitHub issue tracker.
3. Suggesting new features or improvements.

Please ensure that any contributions follow the coding style and standards outlined in the repository.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
