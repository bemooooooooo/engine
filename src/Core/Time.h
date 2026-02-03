#pragma once
#include <chrono>

#include "Base.h"

/**
 * @namespace Time
 * @brief Namespace containing timing and measurement utilities for the game engine.
 * 
 * Provides high-precision timer functionality for measuring elapsed time,
 * frame-to-frame delta times, and other timing-related operations essential
 * for game logic and frame-rate independent updates.
 */
namespace Time {

/**
 * @class Timer
 * @brief High-precision timer for measuring elapsed time and frame delta times.
 * 
 * This class provides a flexible timer that can measure both total elapsed time
 * since creation/reset and frame-to-frame delta times. It uses the system's
 * high-resolution clock for accurate timing measurements needed in game loops.
 * 
 * The timer is useful for:
 *
 * - Frame timing and delta time calculation
 *
 * - Performance profiling and benchmarking
 *
 * - Game logic that depends on elapsed time
 *
 * - Animations and transitions
 *
 * - Input response timing
 * 
 * @note This class is not thread-safe. Use separate Timer instances for different threads.
 * 
 * @example
 * @code
 * Time::Timer gameTimer;
 * 
 * while (gameRunning) {
 *     f32 deltaTime = gameTimer.Tick();  // Get time since last frame
 *     gameLogic.Update(deltaTime);
 *     
 *     if (gameTimer.Elapsed() > 60.0f) {  // After 60 seconds
 *         GameOver();
 *     }
 * }
 * @endcode
 */
class Timer {
 public:
  /**
   * @typedef Clock
   * @brief High-resolution clock type used for timing measurements.
   * 
   * Uses std::chrono::high_resolution_clock which provides the highest
   * available resolution for time measurements on the system.
   */
  using Clock = std::chrono::high_resolution_clock;
  
  /**
   * @typedef TimePoint
   * @brief Type representing a specific point in time from the high-resolution clock.
   * 
   * Represents a single moment in time with the precision of the high-resolution clock.
   * Used internally to track timer start time and last frame time.
   */
  using TimePoint = std::chrono::time_point<Clock>;
  
  /**
   * @typedef Duration
   * @brief Type representing a duration of time in floating-point seconds.
   * 
   * A duration type that measures time intervals in floating-point seconds,
   * providing fractional second precision for frame delta times and other timing needs.
   */
  using Duration = std::chrono::duration<f32>;

  /**
   * @brief Constructs a new Timer instance and starts it immediately.
   * 
   * The constructor initializes the timer with the current time as the start time.
   * The timer begins measuring immediately upon construction.
   * 
   * @note The timer tracks both total elapsed time and frame-to-frame delta times.
   * 
   * @example
   * @code
   * Time::Timer timer;  // Timer starts now
   * @endcode
   */
  Timer();

  /**
   * @brief Resets the timer to its initial state.
   * 
   * When called, this function sets the start time of the timer to the current time,
   * effectively restarting the timer. The elapsed time becomes 0, and subsequent
   * calls to Elapsed() will measure time from this reset point.
   * 
   * This is useful when you want to:
   *
   * - Restart timing for a new section
   *
   * - Reset timing between game phases
   *
   * - Clear accumulated timing for fresh measurements
   * 
   * @note This does not affect the last frame time used by Tick().
   * 
   * @example
   * @code
   * timer.Reset();
   * assert(timer.Elapsed() < 0.001f);  // Should be very close to 0
   * @endcode
   */
  void Reset();

  /**
   * @brief Returns the total time elapsed since timer start or last reset.
   * 
   * This function returns the cumulative time that has passed since the timer was
   * constructed or Reset() was called. The returned value is in seconds as a
   * floating-point number, allowing for fractional second precision.
   * 
   * @return f32 The elapsed time in seconds (e.g., 1.5 = 1 second and 500 milliseconds)
   * 
   * @note This is a const function and does not modify the timer state.
   * @note Call Reset() to reset this counter.
   * 
   * @example
   * @code
   * Time::Timer timer;
   * // ... do some work ...
   * f32 elapsedSeconds = timer.Elapsed();
   * ENGINE_DEBUG_LOG("Operation took: " << elapsedSeconds << " seconds");
   * @endcode
   */
  f32 Elapsed() const;

  /**
   * @brief Calculates and returns the time elapsed since the last Tick() call.
   * 
   * This function calculates the delta time (time difference) since the last time
   * Tick() was called. On the first call, it returns the time since the timer was
   * constructed or Reset(). Subsequent calls return the frame-to-frame delta time.
   * 
   * This is the primary function used for frame-rate independent game logic updates.
   * It allows game physics, movement, and animations to advance proportionally to
   * the time that has actually passed, not the number of frames rendered.
   * 
   * @return f32 The time since last Tick() call in seconds
   * 
   * @note This function updates the internal last-frame timestamp.
   * @note The returned delta time includes all system overhead (rendering, input, etc.)
   * 
   * @example
   * @code
   * Time::Timer timer;
   * 
   * while (gameRunning) {
   *     f32 deltaTime = timer.Tick();  // Get time since last frame
   *     
   *     // Update game logic with frame-rate independent timing
   *     player.position += player.velocity * deltaTime;
   *     particle.lifetime -= deltaTime;
   *     
   *     // Render frame...
   * }
   * @endcode
   * 
   * @example Usage in physics:
   * @code
   * f32 deltaTime = gameTimer.Tick();
   * entity.velocity.y -= gravity * deltaTime;  // Frame-rate independent gravity
   * entity.position += entity.velocity * deltaTime;
   * @endcode
   */
  f32 Tick();

 private:
  /**
   * @var m_StartTime_
   * @brief The time point when the timer was created or last reset.
   * 
   * Used to calculate total elapsed time via the Elapsed() function.
   * Updated when Reset() is called.
   */
  TimePoint m_StartTime_;
  
  /**
   * @var m_LastFrameTime_
   * @brief The time point of the last Tick() call.
   * 
   * Used to calculate the delta time (time between frames) in the Tick() function.
   * Updated after each Tick() call.
   */
  TimePoint m_LastFrameTime_;
};

}  // namespace Time