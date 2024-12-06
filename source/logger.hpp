#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <mutex>

/**
 * @class Logger
 * @brief A singleton class for logging messages.
 *
 * The Logger class provides a simple thread-safe logging mechanism. It allows logging
 * messages to the standard output. It follows the Singleton pattern, ensuring that
 * only one instance of the Logger exists throughout the application's lifetime.
 */
class Logger {
public:
    /**
     * @brief Retrieves the singleton instance of the Logger.
     * @return The singleton instance of the Logger.
     *
     * This static method ensures that only one instance of the Logger class is used
     * throughout the application. It is thread-safe and creates the instance if it
     * does not exist yet.
     */
    static Logger& getInstance();

    /**
     * @brief Logs a message to the standard output.
     * @param message The message to log.
     *
     * This method outputs the provided message to the standard output. It is thread-safe
     * and ensures that messages are logged in a synchronized manner.
     */
    void log(const std::string& message);

private:
    Logger() = default; ///< Private constructor to prevent instantiation outside the class.
    std::mutex m_mutex; ///< Mutex to ensure thread-safe logging.
};

#endif // LOGGER_HPP
