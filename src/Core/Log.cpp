#include "Log.h"

#include <chrono>
#include <iomanip>

namespace Log {
static Level _current_level_ = Level::_Trace;

void Init() { ENGINE_INFO << "Logger initialized"; }

void SetLevel(Level lvl) { _current_level_ = lvl; }

Logger::Logger(Level lvl, const char* file, int line) : m_Level_{lvl} {
  if (lvl < _current_level_) return;
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);

  m_Stream_ << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "]";
  const char* levelStrings[] = {"TRACE", "DEBUG", "INFO",
                                "WARN",  "ERROR", "CRITICAL"};
  m_Stream_ << "[" << levelStrings[static_cast<int>(lvl)] << "]";

  if (lvl >= Level::_Error) {
    m_Stream_ << "[" << file << ":" << line << "]";
  }
}

Logger::~Logger() {
  if (m_Level_ < _current_level_) return;

  std::cout << m_Stream_.str() << std::endl;

  if (m_Level_ == Level::_Critical) {
    std::abort();
  }
}

};  // namespace Log