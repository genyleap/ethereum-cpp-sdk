#include "logger.hpp"
#include <print>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::print("[LOG]: {}", message);
}
