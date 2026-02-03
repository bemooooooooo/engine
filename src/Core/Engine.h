#pragma once
#include "Base.h"
#include "Event.h"
#include "Log.h"
#include "Time.h"

class Window;

/**
 * @brief Core engine class managing the main application loop and subsystems.
 * The Engine class is responsible for initializing and shutting down the engine,
 * managing the main loop, processing input, updating game logic, and rendering.
 * It serves as the central hub that coordinates various subsystems such as windowing,
 * timing, and event handling.
 *
 * Key responsibilities:
 *
 * - Initialize and shut down the engine and its subsystems
 *
 * - Manage the main application loop
 *
 * - Process input events
 *
 * - Update game logic with frame-rate independent timing
 *
 * - Render frames to the window
 *
 * - Provide access to timing information (delta time, FPS)
 *
 * - Handle window events and callbacks
 *
 * @note This is a base class meant to be extended by specific game implementations.
 * Derived classes should override virtual methods to implement game-specific behavior.
 * @example
 * @code
 * class MyGameEngine : public Engine {
 * protected:
 *   void OnInitialize() override {
 *    // Game-specific initialization
 *   }
 *  void OnUpdate(f32 deltaTime) override {
 *   // Game-specific update logic
 *  }
 * void OnRender() override {
 *  // Game-specific rendering
 * }
 * void OnShutdown() override {
 * // Game-specific cleanup
 * }
 * };
 * int main() {
 * MyGameEngine engine;
 * engine.Run();
 * return 0;
 * }
 * @endcode
 */
class Engine {
 public:
  /**
    * @brief Constructs a new Engine instance.
    */
  Engine();
  /**
   * @brief Destroys the Engine instance and cleans up resources.
   */
  virtual ~Engine();

  /**
   * @brief Starts the main application loop.
   * This function initializes the engine and enters the main loop,
   * processing input, updating game logic, and rendering frames until
   * the application is closed.
   * @note This function blocks until the application exits.
   * @example
   * @code
   * MyGameEngine engine;
   * engine.Run();
   * return 0;
   * @endcode
   */
  void Run();

  /**
   * @brief Initializes the engine and its subsystems.
   * This function sets up the window, timing, and other necessary
   * subsystems before entering the main loop.
   * @return true if initialization was successful, false otherwise
   * @example
   * @code
   * Engine engine;
   * if (engine.Initialize()) {
   *   engine.Run();
   * }
   * @endcode
   */
  bool Initialize();
  /**
   * @brief Shuts down the engine and cleans up resources.
   * This function is called when the application is exiting to properly
   * release resources and shut down subsystems.
   * @example
   * @code
   * engine.Shutdown();
   * @endcode
   */
  void Shutdown();

  /**
   * @brief Handles an incoming event.
   * This virtual method can be overridden by derived classes to handle
   * specific events such as input, window resize, etc.
   * @param event The event to handle
   * @example
   * @code
   * void MyGameEngine::OnEvent(Event& event) {
   *   if (event.IsType<WindowResizeEvent>()) {
   *     // Handle window resize
   *   }
   * }
   * @endcode
   */
  virtual void OnEvent(Event& event) = 0;

  /**
   * @brief Gets the singleton instance of the Engine.
   * @return Engine& Reference to the global Engine instance
   */
  static Engine& Get() { return *s_Instance_; }
  /**
   * @brief Gets the application window.
   * @return Window& Reference to the engine's main window
   */
  f32 GetDeltaTime() const { return m_DeltaTime_; }
  /**
   * @brief Gets the current frames per second (FPS).
   * @return f32 Current FPS value
   */
  f32 GetFPS() const { return m_FPS_; }

 protected:
 // Virtual methods for game-specific behavior

  /**
   * @brief Called during engine initialization.
   * Override this method in derived classes to implement
   * game-specific initialization logic.
   */
  virtual void OnInitialize() {}

  /**
   * @brief Called each frame to update game logic.
   * Override this method in derived classes to implement
   * game-specific update logic.
   * @param deltaTime Time elapsed since the last frame in seconds
   */
  virtual void OnUpdate(f32 deltaTime) = 0;

  /**
   * @brief Called each frame to render the scene.
   * Override this method in derived classes to implement
   * game-specific rendering logic.
   */
  virtual void OnRender() {}

  /**
   * @brief Called during engine shutdown.
   * Override this method in derived classes to implement
   * game-specific cleanup logic.
   */
  virtual void OnShutdown() {}

 private:
 // Main loop functions

 /**
  * @brief Processes input events.
  * This function polls and handles input events from the windowing system.
  */
  void ProcessInput();

  /**
   * @brief Updates the engine state.
   * This function updates game logic and timing information.
   * @param deltaTime Time elapsed since the last frame in seconds
   */
  void Update(f32 deltaTime);

  /**
   * @brief Renders the current frame.
   * This function calls the game-specific rendering logic.
   */
  void Render();

 private:
 // Member variables

  /**
   * @var s_Instance_
   * @brief Static pointer to the singleton Engine instance.
   * 
   * Used to provide global access to the Engine instance via Engine::Get().
   */
  static Engine* s_Instance_;

  /**
   * @var m_Window_
   * @brief Scoped pointer to the main application window.
   * 
   * Manages the lifecycle of the window used for rendering and input.
   */
  Scope<Window> m_Window_;

  /**
   * @var m_Timer_
   * @brief Timer instance for tracking elapsed time and delta time.
   * 
   * Used to measure frame times and provide timing information to the engine.
   */
  Time::Timer m_Timer_;

  /**
   * @var m_DeltaTime_
   * @brief Time elapsed since the last frame in seconds.
   * 
   * Used for frame-rate independent updates and timing calculations.
   */
  f32 m_DeltaTime_ = 0.0f;

  /**
   * @var m_FPS_
   * @brief Current frames per second (FPS) value.
   * 
   * Updated each frame to reflect the current rendering performance.
   */
  f32 m_FPS_ = 0.0f;

  /**
   * @var m_FrameCount_
   * @brief Number of frames rendered in the current second.
   * 
   * Used to calculate the FPS value.
   */
  u32 m_FrameCount_ = 0;
  
  /**
   * @var m_FPSTimer_
   * @brief Accumulated time for FPS calculation.
   * 
   * Used to track elapsed time for updating the FPS value once per second.
   */
  f32 m_FPSTimer_ = 0.0f;

  /**
   * @var m_isRunning_
   * @brief Flag indicating whether the main loop is running.
   * 
   * Used to control the execution of the main application loop.
   */
  bool m_isRunning_ = false;

  /**
   * @var m_isMinimized
   * @brief Flag indicating whether the window is minimized.
   * 
   * Used to pause updates and rendering when the window is not visible.
   */
  bool m_isMinimized = false;
};