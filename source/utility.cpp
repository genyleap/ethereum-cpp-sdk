#include "utility.hpp"

#include "logger.hpp"
#include <fstream>
#include <vector>

std::optional<std::string> loadConfig(const std::string& filename) {
    std::vector<std::string> candidates = {filename};
    if (filename == "config.json") {
        candidates.emplace_back("config/system-config.json");
        candidates.emplace_back("system-config.json");
    }

    for (const auto& candidate : candidates) {
        std::ifstream configFile(candidate);
        if (!configFile.is_open()) {
            continue;
        }

        Json::Value config;
        Json::CharReaderBuilder reader;
        std::string errs;
        if (!Json::parseFromStream(reader, configFile, &config, &errs)) {
            Logger::getInstance().log("Failed to parse config file '" + candidate + "': " + errs);
            continue;
        }

        if (config.isMember("nodeUrl") && config["nodeUrl"].isString()) {
            return config["nodeUrl"].asString();
        }

        if (config.isMember("system")
            && config["system"].isObject()
            && config["system"].isMember("nodeUrl")
            && config["system"]["nodeUrl"].isString()) {
            return config["system"]["nodeUrl"].asString();
        }

        Logger::getInstance().log("nodeUrl not found in config file '" + candidate + "'.");
    }

    Logger::getInstance().log("Could not load nodeUrl from config. Checked starting from: " + filename);
    return std::nullopt;
}
