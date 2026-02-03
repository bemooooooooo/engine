#pragma once
#include "../Core/Base.h"
#include "../Math/Vector2.h"

/**
 * @namespace Key
 * @brief Namespace containing keyboard key code definitions.
 */
namespace Key {

    /** 
     * @enum Code
     * @brief Enumeration of keyboard key codes.
     * 
     * Defines key codes for standard keys, function keys, and modifier keys.
     */
    enum Code {
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        
        // Numbers
        Zero = 48,
        One = 49,
        Two = 50,
        Three = 51,
        Four = 52,
        Five = 53,
        Six = 54,
        Seven = 55,
        Eight = 56,
        Nine = 57,
        
        // Letters
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        
        // Function keys
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        
        // Modifier keys
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };
}

/**
 * @namespace Mouse
 * @brief Namespace containing mouse button code definitions.
 */
namespace Mouse {
    /** 
     * @enum Button
     * @brief Enumeration of mouse button codes.
     * 
     * Defines button codes for standard mouse buttons.
     */
    enum Button {
        Button1 = 0,
        Button2 = 1,
        Button3 = 2,
        Button4 = 3,
        Button5 = 4,
        Button6 = 5,
        Button7 = 6,
        Button8 = 7,
        Left = Button1,
        Right = Button2,
        Middle = Button3
    };
}

/**
 * @class Input
 * @brief Static class for handling input from keyboard and mouse.
 * 
 * Provides methods to query the current state of keys and mouse buttons,
 * as well as mouse position and scroll delta. Also allows locking the cursor.
 * 
 * @note Must be updated each frame via Update() to track state changes.
 * @example
 * @code
 * // Initialize input system
 * Input::Init();
 * 
 * // In the main loop
 * while (running) {
 *    Input::Update();
 *   if (Input::GetKeyDown(Key::Space)) {
 *      // Space key was pressed this frame
 *   }
 *   Vector2 mousePos = Input::GetMousePosition();
 * }
 * @endcode
 */
class Input {
public:
    /** 
     * @brief Initializes the input system.
     * 
     * Sets up necessary state for tracking input.
     * 
     * @note Must be called once at engine startup.
     * 
     * @example
     * @code
     * Input::Init();
     * @endcode
     */
    static void Init();
    
    /** 
     * @brief Updates the input state.
     * 
     * Should be called once per frame to update key and mouse states.
     * Tracks changes for GetKeyDown and GetKeyUp methods.
     * 
     * @note Must be called every frame.
     * 
     * @example
     * @code
     * // In the main loop
     * while (running) {
     *     Input::Update();
     *     // ... game logic ...
     * }
     * @endcode
     */
    static void Update();
    
    // ----------------Keyboard----------------

    /** 
     * @brief Checks if a key is currently pressed.
     * 
     * @param key The key code to check
     * @return bool True if the key is pressed, false otherwise
     * 
     * @example
     * @code
     * if (Input::GetKey(Key::W)) {
     *     // Move forward
     * }
     * @endcode
     */
    static bool GetKey(Key::Code key);

    /** 
     * @brief Checks if a key was pressed this frame.
     * 
     * @param key The key code to check
     * @return bool True if the key was pressed this frame, false otherwise
     * 
     * @example
     * @code
     * if (Input::GetKeyDown(Key::Space)) {
     *     // Jump action
     * }
     * @endcode
     */
    static bool GetKeyDown(Key::Code key);

    /** 
     * @brief Checks if a key was released this frame.
     * 
     * @param key The key code to check
     * @return bool True if the key was released this frame, false otherwise
     * 
     * @example
     * @code
     * if (Input::GetKeyUp(Key::E)) {
     *     // Interact action
     * }
     * @endcode
     */
    static bool GetKeyUp(Key::Code key);
    
    // ----------------Mouse----------------

    /** 
     * @brief Checks if a mouse button is currently pressed.
     * 
     * @param button The mouse button code to check
     * @return bool True if the button is pressed, false otherwise
     * 
     * @example
     * @code
     * if (Input::GetMouseButton(Mouse::Left)) {
     *     // Shooting action
     * }
     * @endcode
     */
    static bool GetMouseButton(Mouse::Button button);

    /** 
     * @brief Checks if a mouse button was pressed this frame.
     * 
     * @param button The mouse button code to check
     * @return bool True if the button was pressed this frame, false otherwise
     * 
     * @example
     * @code
     * if (Input::GetMouseButtonDown(Mouse::Left)) {
     *     // Start aiming
     * }
     * @endcode
     */
    static bool GetMouseButtonDown(Mouse::Button button);

    /** 
     * @brief Checks if a mouse button was released this frame.
     * 
     * @param button The mouse button code to check
     * @return bool True if the button was released this frame, false otherwise
     * 
     * @example
     * @code
     * if (Input::GetMouseButtonUp(Mouse::Left)) {
     *     // Fire weapon
     * }
     * @endcode
     */
    static bool GetMouseButtonUp(Mouse::Button button);
    
    /** 
     * @brief Gets the current mouse position in window coordinates.
     * 
     * @return Vector2 The current mouse position
     * 
     * @example
     * @code
     * Vector2 mousePos = Input::GetMousePosition();
     * @endcode
     */
    static Vector2 GetMousePosition();

    /** 
     * @brief Gets the mouse movement delta since the last frame.
     * 
     * @return Vector2 The mouse movement delta
     * 
     * @example
     * @code
     * Vector2 mouseDelta = Input::GetMouseDelta();
     * @endcode
     */
    static Vector2 GetMouseDelta();

    /** 
     * @brief Gets the mouse scroll wheel delta since the last frame.
     * 
     * @return Vector2 The mouse scroll delta
     * 
     * @example
     * @code
     * Vector2 scrollDelta = Input::GetMouseScrollDelta();
     * @endcode
     */
    static Vector2 GetMouseScrollDelta();
    
    // ----------------Other----------------

    /** 
     * @brief Sets the cursor mode (locked or unlocked).
     * 
     * @param locked True to lock the cursor, false to unlock
     * 
     * @example
     * @code
     * Input::SetCursorMode(true);  // Lock cursor
     * @endcode
     */
    static void SetCursorMode(bool locked);

    /** 
     * @brief Checks if the cursor is currently locked.
     * 
     * @return bool True if the cursor is locked, false otherwise
     * 
     * @example
     * @code
     * if (Input::IsCursorLocked()) {
     *     // Cursor is locked
     * }
     * @endcode
     */
    static bool IsCursorLocked();
    
    // ----------------System methods----------------
    
    /** 
     * @brief Sets the state of a key (for internal use).
     * 
     * @param key The key code to set
     * @param pressed True if the key is pressed, false otherwise
     */
    static void SetKeyState(Key::Code key, bool pressed);

    /** 
     * @brief Sets the state of a mouse button (for internal use).
     * 
     * @param button The mouse button code to set
     * @param pressed True if the button is pressed, false otherwise
     */
    static void SetMouseButtonState(Mouse::Button button, bool pressed);

    /** 
     * @brief Sets the current mouse position (for internal use).
     * 
     * @param position The new mouse position
     */
    static void SetMousePosition(const Vector2& position);

    /** 
     * @brief Sets the mouse scroll delta (for internal use).
     * 
     * @param scroll The new mouse scroll delta
     */
    static void SetMouseScrollDelta(const Vector2& scroll);

private:
    // Internal state

    /** 
     * @var s_KeyState
     * @brief Current state of all keys.
     */
    static std::array<bool, 512> s_KeyState;

    /** 
     * @var s_KeyStatePrevious
     * @brief Previous frame state of all keys.
     */
    static std::array<bool, 512> s_KeyStatePrevious;

    /** 
     * @var s_MouseButtonState
     * @brief Current state of all mouse buttons.
     */
    static std::array<bool, 8> s_MouseButtonState;

    /** 
     * @var s_MouseButtonStatePrevious
     * @brief Previous frame state of all mouse buttons.
     */
    static std::array<bool, 8> s_MouseButtonStatePrevious;

    /** 
     * @var s_MousePosition
     * @brief Current mouse position.
     */
    static Vector2 s_MousePosition;

    /** 
     * @var s_MousePositionPrevious
     * @brief Previous frame mouse position.
     */
    static Vector2 s_MousePositionPrevious;

    /** 
     * @var s_MouseScrollDelta
     * @brief Mouse scroll delta.
     */
    static Vector2 s_MouseScrollDelta;

    /** 
     * @var s_CursorLocked
     * @brief Whether the cursor is currently locked.
     */
    static bool s_CursorLocked;
};