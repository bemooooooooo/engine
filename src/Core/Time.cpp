#include "Time.h"

namespace Time {

Timer::Timer() { Reset(); }

void Timer::Reset() {
  m_StartTime_ = Clock::now();
  m_LastFrameTime_ = m_StartTime_;
}

f32 Timer::Elapsed() const {
  auto now = Clock::now();
  auto duration = std::chrono::duration_cast<Duration>(now - m_StartTime_);
  return duration.count();
}

f32 Timer::Tick() {
  auto now = Clock::now();
  auto duration = std::chrono::duration_cast<Duration>(now - m_LastFrameTime_);
  m_LastFrameTime_ = now;
  return duration.count();
}

}  // namespace Time