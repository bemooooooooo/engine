#pragma once

/**
 * @file Base.h
 * @brief Core base definitions, type aliases, and utility macros for the game engine.
 * 
 * This header provides fundamental types, memory management utilities, and cross-platform
 * compatibility definitions for the MyGameEngine project. It serves as the foundation that
 * all other engine modules depend on.
 * 
 * Key features:
 * - Cross-platform compatibility macros
 * - Fixed-size integer type aliases (i8, i16, i32, i64, u8, u16, u32, u64)
 * - Floating-point type aliases (f32, f64)
 * - Smart pointer utilities (Scope, Ref) with factory functions
 * - Debug logging macros that compile out in release builds
 * 
 * @author bemooooooooo
 * @version 1.0
 * @date 2026
 */

/**
 * @brief Cross-platform export macro for DLL/shared library definitions.
 * 
 * On Windows platforms, this macro expands to __declspec(dllexport) to properly
 * export symbols from a dynamic library. On other platforms (Linux, macOS), it
 * expands to nothing, allowing the code to work seamlessly across different OS.
 * 
 * Usage:
 * @code
 * class ENGINE_API MyPublicClass { ... };
 * @endcode
 */
#ifdef _WIN32
    #define ENGINE_API __declspec(dllexport)
#else
    #define ENGINE_API __attribute__((visibility("default")))
#endif

// Compiler-specific attributes
#ifdef _MSC_VER
    #define ENGINE_FORCE_INLINE __forceinline
    #define ENGINE_DEPRECATED(msg) __declspec(deprecated(msg))
    #define ENGINE_PACKED __pragma(pack(push, 1))
    #define ENGINE_PACKED_END __pragma(pack(pop))
#elif defined(__GNUC__) || defined(__clang__)
    #define ENGINE_FORCE_INLINE inline __attribute__((always_inline))
    #define ENGINE_DEPRECATED(msg) __attribute__((deprecated(msg)))
    #define ENGINE_PACKED __attribute__((packed))
    #define ENGINE_PACKED_END
#else
    #define ENGINE_FORCE_INLINE inline
    #define ENGINE_DEPRECATED(msg)
    #define ENGINE_PACKED
    #define ENGINE_PACKED_END
#endif

/**
 * @defgroup BaseTypes Fundamental Type Aliases
 * @{
 * 
 * Standard type aliases for fixed-size integer and floating-point types.
 * These provide a consistent interface across different platforms and compilers
 * for numerical computations throughout the engine.
 */

// Base types
#include <cstdint>

/**
 * @typedef i8
 * @brief Signed 8-bit integer type (range: -128 to 127)
 */
using i8 = int8_t;

/**
 * @typedef i16
 * @brief Signed 16-bit integer type (range: -32768 to 32767)
 */
using i16 = int16_t;

/**
 * @typedef i32
 * @brief Signed 32-bit integer type (range: -2147483648 to 2147483647)
 */
using i32 = int32_t;

/**
 * @typedef i64
 * @brief Signed 64-bit integer type (range: -9223372036854775808 to 9223372036854775807)
 */
using i64 = int64_t;

/**
 * @typedef u8
 * @brief Unsigned 8-bit integer type (range: 0 to 255)
 */
using u8 = uint8_t;

/**
 * @typedef u16
 * @brief Unsigned 16-bit integer type (range: 0 to 65535)
 */
using u16 = uint16_t;

/**
 * @typedef u32
 * @brief Unsigned 32-bit integer type (range: 0 to 4294967295)
 */
using u32 = uint32_t;

/**
 * @typedef u64
 * @brief Unsigned 64-bit integer type (range: 0 to 18446744073709551615)
 */
using u64 = uint64_t;

/**
 * @typedef f32
 * @brief 32-bit floating-point type (standard C++ float)
 * 
 * Provides single-precision floating-point calculations, commonly used for
 * 3D graphics computations, physics calculations, and transformations.
 */
using f32 = float;

/**
 * @typedef f64
 * @brief 64-bit floating-point type (standard C++ double)
 * 
 * Provides double-precision floating-point calculations for high-precision
 * mathematical operations where accuracy is critical.
 */
using f64 = double;

/** @} */

/**
 * @defgroup MemoryManagement Smart Pointer and Memory Utilities
 * @{
 * 
 * Template aliases and factory functions for managing dynamic memory using
 * C++11 smart pointers. These ensure proper resource management and prevent
 * memory leaks by automating cleanup. Use Scope for exclusive ownership and
 * Ref for shared ownership scenarios.
 */

#include <memory>

/**
 * @typedef Scope<T>
 * @brief Type alias for std::unique_ptr<T>
 * 
 * Represents exclusive ownership semantics. Use Scope when an object should be
 * owned by a single entity and deleted when that owner is destroyed. Prevents
 * memory leaks through automatic cleanup and eliminates the need for manual delete.
 * 
 * @tparam T The type of object to manage
 * 
 * @example
 * @code
 * Scope<Camera> camera = CreateScope<Camera>();
 * // Camera is automatically deleted when camera goes out of scope
 * @endcode
 */
template <typename T>
using Scope = std::unique_ptr<T>;

/**
 * @brief Factory function to create a unique_ptr with RAII semantics.
 * 
 * Constructs an object of type T on the heap and returns it wrapped in a
 * unique_ptr. This function simplifies the creation of scoped objects with
 * automatic cleanup and prevents accidental memory leaks. Perfect-forwarding
 * is used to support any constructor signature.
 * 
 * @tparam T The type of object to create
 * @tparam Args Parameter types for the constructor
 * @param args Constructor arguments to forward to T's constructor
 * @return Scope<T> A unique_ptr managing the newly created object
 * 
 * @example
 * @code
 * auto renderer = CreateScope<Renderer>(800, 600);
 * // Object is automatically deleted when renderer goes out of scope
 * @endcode
 */
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

/**
 * @typedef Ref<T>
 * @brief Type alias for std::shared_ptr<T>
 * 
 * Represents shared ownership semantics. Use Ref when multiple entities need
 * to own and access an object. The object is deleted automatically when the
 * last reference is released. Internally uses reference counting to track ownership.
 * 
 * @tparam T The type of object to manage
 * 
 * @example
 * @code
 * Ref<Texture> texture = CreateRef<Texture>("assets/image.png");
 * auto copy = texture; // Both share ownership
 * // Texture is deleted when both references are released
 * @endcode
 */
template <typename T>
using Ref = std::shared_ptr<T>;

/**
 * @brief Factory function to create a shared_ptr with RAII semantics.
 * 
 * Constructs an object of type T on the heap and returns it wrapped in a
 * shared_ptr. This function simplifies the creation of reference-counted
 * objects and is useful when multiple systems need to share ownership. Uses
 * perfect-forwarding to support any constructor signature.
 * 
 * @tparam T The type of object to create
 * @tparam Args Parameter types for the constructor
 * @param args Constructor arguments to forward to T's constructor
 * @return Ref<T> A shared_ptr managing the newly created object
 * 
 * @example
 * @code
 * auto model = CreateRef<Model>("assets/model.obj");
 * auto reference = model; // Both share ownership
 * // Object is deleted when all references are released
 * @endcode
 */
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

/** @} */

/**
 * @defgroup DebugMacros Debug Logging Macros
 * @{
 * 
 * Conditional compilation macros for debug logging that provide zero-overhead
 * debugging in release builds. These macros are completely eliminated during
 * compilation when ENGINE_DEBUG_BUILD is not defined, ensuring zero performance impact.
 */

/**
 * @def ENGINE_DEBUG_LOG
 * @brief Logs a message in debug builds only with zero overhead in release builds.
 * 
 * In debug builds (when ENGINE_DEBUG_BUILD is defined and non-zero), this macro
 * outputs the given expression to the engine's logging system via ENGINE_INFO.
 * In release builds, this macro compiles to a no-op instruction that has virtually
 * no performance cost.
 * 
 * @param x The message or expression to log
 * 
 * @note Requires ENGINE_INFO to be defined as a logging stream (e.g., logger stream)
 * 
 * @example
 * @code
 * ENGINE_DEBUG_LOG("Player position: " << playerPos.x << ", " << playerPos.y);
 * ENGINE_DEBUG_LOG("Loading asset: " << assetPath);
 * @endcode
 */
#if defined(ENGINE_DEBUG_BUILD) && ENGINE_DEBUG_BUILD
  #define ENGINE_DEBUG_LOG(x) ENGINE_INFO << x
#else
  #define ENGINE_DEBUG_LOG(x) ((void)0)
#endif

/**
 * @def ENGINE_DEBUG_LOG_IF
 * @brief Conditionally logs a message in debug builds only if a condition is met.
 * 
 * In debug builds, this macro evaluates the condition and logs the expression only
 * if the condition evaluates to true. In release builds, this macro is completely
 * eliminated by the preprocessor, resulting in zero runtime cost.
 * 
 * This is useful for conditional logging that should only appear under specific
 * runtime circumstances, such as logging only when values are out of expected ranges
 * or when certain error conditions are detected.
 * 
 * @param cond The condition to evaluate (true = log, false = skip)
 * @param x The message or expression to log if condition is true
 * 
 * @note Requires ENGINE_INFO to be defined as a logging stream
 * 
 * @example
 * @code
 * ENGINE_DEBUG_LOG_IF(health < maxHealth, "Health reduced to: " << health);
 * ENGINE_DEBUG_LOG_IF(fps < 30, "Low frame rate detected: " << fps);
 * ENGINE_DEBUG_LOG_IF(ptr == nullptr, "Null pointer encountered!");
 * @endcode
 */
#if defined(ENGINE_DEBUG_BUILD) && ENGINE_DEBUG_BUILD
  #define ENGINE_DEBUG_LOG_IF(cond, x) if (cond) { ENGINE_INFO << x; }
#else
  #define ENGINE_DEBUG_LOG_IF(cond, x) ((void)0)
#endif

/** @} */