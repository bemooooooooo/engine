#include "Input.h"
#include "../Core/Log.h"
#include <array>

// Статические переменные
std::array<bool, 512> Input::s_KeyState;
std::array<bool, 512> Input::s_KeyStatePrevious;
std::array<bool, 8> Input::s_MouseButtonState;
std::array<bool, 8> Input::s_MouseButtonStatePrevious;
Vector2 Input::s_MousePosition;
Vector2 Input::s_MousePositionPrevious;
Vector2 Input::s_MouseScrollDelta;
bool Input::s_CursorLocked = false;

void Input::Init() {
    // Инициализация всех состояний как "не нажато"
    s_KeyState.fill(false);
    s_KeyStatePrevious.fill(false);
    s_MouseButtonState.fill(false);
    s_MouseButtonStatePrevious.fill(false);
    s_MousePosition = Vector2::Zero;
    s_MousePositionPrevious = Vector2::Zero;
    s_MouseScrollDelta = Vector2::Zero;
    
    ENGINE_INFO << "Input system initialized";
}

void Input::Update() {
    // Сохраняем предыдущие состояния
    s_KeyStatePrevious = s_KeyState;
    s_MouseButtonStatePrevious = s_MouseButtonState;
    s_MousePositionPrevious = s_MousePosition;
    
    // Сбрасываем дельту скролла (она обновляется каждый кадр)
    s_MouseScrollDelta = Vector2::Zero;
}

// === КЛАВИАТУРА ===
bool Input::GetKey(Key::Code key) {
    return s_KeyState[static_cast<int>(key)];
}

bool Input::GetKeyDown(Key::Code key) {
    int keyCode = static_cast<int>(key);
    return s_KeyState[keyCode] && !s_KeyStatePrevious[keyCode];
}

bool Input::GetKeyUp(Key::Code key) {
    int keyCode = static_cast<int>(key);
    return !s_KeyState[keyCode] && s_KeyStatePrevious[keyCode];
}

// === МЫШЬ ===
bool Input::GetMouseButton(Mouse::Button button) {
    return s_MouseButtonState[static_cast<int>(button)];
}

bool Input::GetMouseButtonDown(Mouse::Button button) {
    int buttonCode = static_cast<int>(button);
    return s_MouseButtonState[buttonCode] && !s_MouseButtonStatePrevious[buttonCode];
}

bool Input::GetMouseButtonUp(Mouse::Button button) {
    int buttonCode = static_cast<int>(button);
    return !s_MouseButtonState[buttonCode] && s_MouseButtonStatePrevious[buttonCode];
}

Vector2 Input::GetMousePosition() {
    return s_MousePosition;
}

Vector2 Input::GetMouseDelta() {
    return s_MousePosition - s_MousePositionPrevious;
}

Vector2 Input::GetMouseScrollDelta() {
    return s_MouseScrollDelta;
}

// === ДРУГОЕ ===
void Input::SetCursorMode(bool locked) {
    s_CursorLocked = locked;
    // Здесь будет вызов GLFW для реальной блокировки курсора
}

bool Input::IsCursorLocked() {
    return s_CursorLocked;
}

// === СИСТЕМНЫЕ МЕТОДЫ ===
void Input::SetKeyState(Key::Code key, bool pressed) {
    s_KeyState[static_cast<int>(key)] = pressed;
}

void Input::SetMouseButtonState(Mouse::Button button, bool pressed) {
    s_MouseButtonState[static_cast<int>(button)] = pressed;
}

void Input::SetMousePosition(const Vector2& position) {
    s_MousePosition = position;
}

void Input::SetMouseScrollDelta(const Vector2& scroll) {
    s_MouseScrollDelta = scroll;
}