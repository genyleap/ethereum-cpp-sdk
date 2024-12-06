#include "utility.hpp"

#include "logger.hpp"
#include <fstream>

std::optional<std::string> loadConfig(const std::string& filename) {
    std::ifstream configFile(filename);
    if (!configFile.is_open()) {
        Logger::getInstance().log("Could not open config file.");
        return std::nullopt;
    }

    Json::Value config;
    configFile >> config;
    if (config.isMember("nodeUrl")) {
        return config["nodeUrl"].asString();
    }

    Logger::getInstance().log("nodeUrl not found in config file.");
    return std::nullopt;
}
