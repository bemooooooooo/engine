#pragma once
#ifdef _MSC_VER
#pragma warning(disable : 4996)  // _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Base.h"

/**
 * @namespace Log
 * @brief Logging utilities and logger class for engine diagnostics.
 * 
 * Provides a flexible logging system with multiple severity levels (Trace, Debug,
 * Info, Warn, Error, Critical). Supports logging to console and optional file
 * output. Designed for easy integration into the engine for runtime diagnostics,
 * debugging, and error reporting.
 * 
 * Features:
 * - Stream-style logging with operator<< overloads
 * - Configurable log levels to filter messages
 * - Optional file logging with append/overwrite modes
 * 
 * Usage:
 * @code
 * ENGINE_INFO << "Engine started successfully.";
 * ENGINE_WARN << "Low memory warning.";
 * ENGINE_ERROR << "Failed to load texture: " << texturePath;
 * @endcode
 */
namespace Log {

/**
 * @enum Level
 * @brief Log severity levels.
 * 
 * Defines the various log levels used to categorize log messages by severity.
 * Allows filtering of log output based on the configured log level.
 */
enum class Level {
  _Trace = 0,
  _Debug = 1,
  _Info = 2,
  _Warn = 3,
  _Error = 4,
  _Critical = 5
};

/**
 * @class Logger
 * @brief Logger class for creating log messages with various severity levels.
 * 
 * The Logger class provides a stream-style interface for constructing log messages.
 * It captures the log level, source file, and line number for each message.
 * Messages are output to the console and optionally to a log file based on
 * the configured log level.
 * 
 * Usage:
 * @code
 * Log::Logger(Level::_Info, __FILE__, __LINE__) << "This is an info message.";
 * @endcode
 */
class Logger {
 public:
 // Constructor and Destructor
  Logger(Level lvl, const char* file, int line);
  ~Logger();

  /**
   * @brief Stream insertion operator for logging various data types.
   * 
   * Allows chaining of log message components using the << operator.
   * Supports any type that can be output to an std::ostream.
   * @param value The value to log
   * @return Logger& Reference to this logger for chaining
   * @note Uses perfect forwarding to handle all types efficiently.
   * @example
   * @code
   * ENGINE_INFO << "Player position: " << player.position << ", Health: " << player.health;
   * @endcode
   */
  template <typename T>
  Logger& operator<<(T&& value) {
    m_Stream_ << std::forward<T>(value);
    return *this;
  }

 private:
  // Member variables

  // Log level of this message
  Level m_Level_;
  // Source file where the log was generated
  std::ostringstream m_Stream_;
};

/**
 * @brief Initializes the logging system.
 * 
 * Sets up any necessary state for logging. Currently a no-op but can be
 * extended in the future for more complex initialization.
 */
void Init();

/**
 * @brief Sets the global log level.
 * 
 * Configures the minimum log level that will be output. Messages below
 * this level will be ignored.
 * 
 * @param lvl The log level to set
 * 
 * @example
 * @code
 * Log::SetLevel(Log::Level::_Warn);  // Only log warnings and above
 * @endcode
 */
void SetLevel(Level lvl);

inline Scope<std::ofstream> s_LogFile;
inline std::streambuf* s_OldCoutBuf = nullptr;
inline std::streambuf* s_OldCerrBuf = nullptr;
inline std::streambuf* s_OldClogBuf = nullptr;

/**
 * @brief Enables logging to a file.
 * 
 * Redirects log output to the specified file. Can either append to
 * an existing file or overwrite it.
 * 
 * @param path The file path to log to
 * @param append True to append to existing file, false to overwrite
 * @return bool True if file logging was successfully enabled, false otherwise
 * @example
 * @code
 * if (Log::EnableFileLogging("engine.log", true)) {
 *   ENGINE_INFO << "File logging enabled.";
 * } else {
 *   ENGINE_ERROR << "Failed to enable file logging.";
 * }
 * @endcode
 * @note If enabling file logging fails, the log output remains directed to console.
 * @note Call DisableFileLogging() to stop logging to the file and restore console output.
 */
inline bool EnableFileLogging(const std::string& path = "engine.log", bool append = true) {
  try {
    std::ios_base::openmode mode = std::ios::out;
    if (append) mode |= std::ios::app;
    s_LogFile = CreateScope<std::ofstream>(path, mode);
    if (!s_LogFile->is_open()) {
      s_LogFile.reset();
      return false;
    }
    s_OldCoutBuf = std::cout.rdbuf();
    s_OldCerrBuf = std::cerr.rdbuf();
    s_OldClogBuf = std::clog.rdbuf();

    std::cout.rdbuf(s_LogFile->rdbuf());
    std::cerr.rdbuf(s_LogFile->rdbuf());
    std::clog.rdbuf(s_LogFile->rdbuf());
    return true;
  } catch (...) {
    s_LogFile.reset();
    return false;
  }
}

/**
 * @brief Disables file logging and restores console output.
 * 
 * Stops logging to the file and restores the original console output streams.
 * Closes the log file if it was open.
 * 
 * @example
 * @code
 * Log::DisableFileLogging();
 * ENGINE_INFO << "File logging disabled, back to console.";
 * @endcode
 */
inline void DisableFileLogging() {
  if (s_LogFile) {
    if (s_OldCoutBuf) std::cout.rdbuf(s_OldCoutBuf);
    if (s_OldCerrBuf) std::cerr.rdbuf(s_OldCerrBuf);
    if (s_OldClogBuf) std::clog.rdbuf(s_OldClogBuf);
    s_LogFile->close();
    s_LogFile.reset();
  }
}

};  // namespace Log

// Convenience macros for logging at various levels
#define ENGINE_TRACE Log::Logger(Log::Level::_Trace, __FILE__, __LINE__)
#define ENGINE_DEBUG Log::Logger(Log::Level::_Debug, __FILE__, __LINE__)
#define ENGINE_INFO Log::Logger(Log::Level::_Info, __FILE__, __LINE__)
#define ENGINE_WARN Log::Logger(Log::Level::_Warn, __FILE__, __LINE__)
#define ENGINE_ERROR Log::Logger(Log::Level::_Error, __FILE__, __LINE__)
#define ENGINE_CRITICAL Log::Logger(Log::Level::_Critical, __FILE__, __LINE__)

// Convenience macros for file logging
#define ENABLE_FILE_LOGGING(path) ::Log::EnableFileLogging((path), true)
#define ENABLE_FILE_LOGGING_APPEND(path) ::Log::EnableFileLogging((path), true)
#define ENABLE_FILE_LOGGING_OVERWRITE(path) ::Log::EnableFileLogging((path), false)
#define DISABLE_FILE_LOGGING() ::Log::DisableFileLogging()
