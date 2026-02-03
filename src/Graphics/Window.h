#pragma once
#include "../Core/Base.h"
#include "../Core/Event.h"
#include "../Input/Input.h"
#include <functional>
#include <string>

/** 
 * @class WindowResizeEvent
 * @brief Event triggered when the window is resized.
 * 
 * This event is dispatched whenever the window size changes, providing
 * the new width and height. It allows the application to adjust rendering
 * parameters or UI layouts accordingly.
 */
class WindowResizeEvent : public Event {
public:
    /** 
     * @brief Constructs a WindowResizeEvent with the new dimensions.
     * 
     * @param width The new width of the window
     * @param height The new height of the window
     */
    WindowResizeEvent(u32 width, u32 height) 
        : m_Width(width), m_Height(height) {}
    
    /**
     * @brief Gets the new width of the window.
     * @return u32 The new width
     */
    u32 GetWidth() const { return m_Width; }

    /**
     * @brief Gets the new height of the window.
     * @return u32 The new height
     */
    u32 GetHeight() const { return m_Height; }
    
    /** 
     * @brief Converts the event to a string representation.
     * 
     * @return std::string A string representation of the event
     */
    std::string ToString() const override {
        return "WindowResizeEvent: " + std::to_string(m_Width) + "x" + std::to_string(m_Height);
    }
    
private:
    /** 
     * @var m_Width
     * @brief The new width of the window.
     * @var m_Height
     * @brief The new height of the window.
     */
    u32 m_Width, m_Height;
};

/** 
 * @class WindowCloseEvent
 * @brief Event triggered when the window is requested to close.
 * 
 * This event is dispatched when the user attempts to close the window,
 * allowing the application to handle cleanup or prompt the user before exiting.
 */
class WindowCloseEvent : public Event {
public:
    /**
     * @brief Converts the event to a string representation.
     *
     * @return std::string A string representation of the event
     */
    std::string ToString() const override {
        return "WindowCloseEvent";
    }
};

/** 
 * @struct WindowProps
 * @brief Properties for configuring the window on creation.
 * 
 * Contains settings such as title, width, and height for initializing
 * a window instance.
 */
struct WindowProps {
    /** 
     * @var Title
     * @brief The title of the window.
     * @var Width
     * @brief The width of the window in pixels.
     * @var Height
     * @brief The height of the window in pixels.
     */
    std::string Title;
    u32 Width;
    u32 Height;
    
    /** 
     * @brief Constructs WindowProps with default or specified values.
     * 
     * @param title The title of the window (default: "My Game Engine")
     * @param width The width of the window in pixels (default: 1280)
     * @param height The height of the window in pixels (default: 720)
     */
    WindowProps(const std::string& title = "My Game Engine",
                u32 width = 1280,
                u32 height = 720)
        : Title(title), Width(width), Height(height) {}
};

/** 
 * @class Window
 * @brief Encapsulates a window using GLFW for context and event handling.
 * 
 * This class manages the creation, updating, and destruction of a window.
 * It also handles input events and provides a callback mechanism for event
 * processing.
 * 
 * Key features:
 * - Create and destroy a window with specified properties
 * - Poll events and swap buffers each frame
 * - Set event callback functions for handling input and window events
 * - Query window dimensions and native handle
 * 
 * @note Requires GLFW library to be linked with the project.
 * 
 * @example
 * @code
 * Window window;
 * window.Initialize(WindowProps("Game", 800, 600));
 * 
 * window.SetEventCallback([](Event& e) {
 *     if (e.IsType<WindowResizeEvent>()) {
 *         auto& re = static_cast<WindowResizeEvent&>(e);
 *         std::cout << "Window resized to " << re.GetWidth() << "x" << re.GetHeight() << std::endl;
 *     }
 * });
 * 
 * while (!window.ShouldClose()) {
 *     window.BeginFrame();
 *     // Render here
 *     window.EndFrame();
 * }
 * 
 * window.Shutdown();
 * @endcode
 */
class Window {
public:
    /** 
     * @typedef EventCallbackFn
     * @brief Function type for event callbacks.
     * 
     * Represents a function that takes an Event reference and returns void.
     */
    using EventCallbackFn = std::function<void(Event&)>;
    
    /** 
     * @brief Constructs a Window instance.
     */
    Window();

    /** 
     * @brief Destructor that cleans up the window resources.
     */
    ~Window();
    
    /** 
     * @brief Initializes the window with specified properties.
     * 
     * Creates the window using GLFW and sets up necessary callbacks.
     * 
     * @param props The properties to configure the window
     * @return bool True if initialization was successful, false otherwise
     * 
     * @example
     * @code
     * Window window;
     * if (!window.Initialize(WindowProps("Game", 800, 600))) {
     *     // Handle initialization failure
     * }
     * @endcode
     */
    bool Initialize(const WindowProps& props = WindowProps());

    /** 
     * @brief Shuts down and destroys the window.
     * 
     * Cleans up GLFW resources associated with the window.
     * 
     * @example
     * @code
     * window.Shutdown();
     * @endcode
     */
    void Shutdown();
    
    /** 
     * @brief Updates the window (polls events and swaps buffers).
     * 
     * Should be called once per frame to process events and present the
     * rendered frame to the screen.
     * 
     * @example
     * @code
     * while (!window.ShouldClose()) {
     *     window.Update();
     * }
     * @endcode
     */
    void Update();
    
    /** 
     * @brief Gets the current width of the window.
     * @return u32 The width in pixels
     */
    u32 GetWidth() const { return m_Data.Width; }

    /** 
     * @brief Gets the current height of the window.
     * @return u32 The height in pixels
     */
    u32 GetHeight() const { return m_Data.Height; }

    /** 
     * @brief Begins the frame by polling events.
     * 
     * Should be called at the start of each frame before rendering.
     */
    void BeginFrame();

    /** 
     * @brief Ends the frame by swapping buffers.
     * 
     * Should be called at the end of each frame after rendering.
     */
    void EndFrame();
    
    /** 
     * @brief Sets the event callback function.
     * 
     * Registers a callback function that will be invoked for each event
     * received by the window (e.g., resize, close, input).
     * 
     * @param callback The function to call on events
     * 
     * @example
     * @code
     * window.SetEventCallback([](Event& e) {
     *     // Handle event
     * });
     * @endcode
     */
    void SetEventCallback(const EventCallbackFn& callback) { 
        m_Data.EventCallback = callback; 
    }
    
    /** 
     * @brief Checks if the window should close.
     * 
     * @return bool True if the window is requested to close, false otherwise
     * 
     * @example
     * @code
     * while (!window.ShouldClose()) {
     *     // Main loop
     * }
     * @endcode
     */
    bool ShouldClose() const;
    
    /** 
     * @brief Gets the native window handle.
     * 
     * @return void* Pointer to the native window (GLFWwindow*)
     * 
     * @example
     * @code
     * void* nativeWindow = window.GetNativeWindow();
     * @endcode
     */
    void* GetNativeWindow() const { return m_Window; }

private:
    /** 
     * @brief Sets up GLFW callbacks for window events.
     * 
     * Configures GLFW to call appropriate functions on window events
     * such as resize and close, forwarding them to the event callback.
     */
    void SetupCallbacks();
    
private:
    /** 
     * @struct WindowData
     * @brief Internal data structure for window properties and state.
     */
    struct WindowData {
        std::string Title;
        u32 Width, Height;
        EventCallbackFn EventCallback;
    };

    /** 
     * @var m_Data
     * @brief The window's properties and state data.
     */
    WindowData m_Data;

    /** 
     * @var m_Window
     * @brief Pointer to the native GLFW window.
     */
    void* m_Window = nullptr;
};