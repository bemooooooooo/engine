#pragma once
#include <cmath>
#include <limits>

#include "../Core/Base.h"

/**
 * @namespace Math
 * @brief Mathematical constants and utility functions for the game engine.
 * 
 * This namespace provides a collection of mathematical constants, functions,
 * and utilities commonly used in game development. It includes definitions
 * for pi, conversion factors between degrees and radians, and various
 * helper functions for clamping, interpolation, and common math operations.
 * 
 * Key features:
 *
 * - Mathematical constants (PI, TWO_PI, DEG2RAD, RAD2DEG)
 *
 * - Clamping and interpolation functions
 *
 * - Angle conversion utilities
 *
 * - Common math functions (sqrt, sin, cos, etc.)
 *
 * - Utility functions for range checking and power-of-two calculations
 * 
 * @note All floating-point operations use f32 (single-precision) for performance.
 * 
 * @example
 * @code
 * f32 angleRad = Math::ToRadians(90.0f);  // Convert degrees to radians
 * f32 clampedValue = Math::Clamp(value, 0.0f, 1.0f);  // Clamp value between 0 and 1
 * f32 lerpedValue = Math::Lerp(start, end, t);  // Linear interpolation
 * @endcode
 */
namespace Math {

/**
   * @var PI
   * @brief Mathematical constant π (pi) approximately equal to 3.14159.
   */
constexpr f32 PI = 3.14159265358979323846f;
/**
   * @var TWO_PI
   * @brief Mathematical constant 2π (two times pi) approximately equal to 6.28318.
   */
constexpr f32 TWO_PI = 6.28318530717958647692f;

/**
   * @var HALF_PI
   * @brief Mathematical constant π/2 (half pi) approximately equal to 1.5708.
   */
constexpr f32 HALF_PI = 1.57079632679489661923f;

/**
   * @var DEG2RAD
   * @brief Conversion factor from degrees to radians.
   */
constexpr f32 DEG2RAD = PI / 180.0f;

/**
   * @var RAD2DEG
   * @brief Conversion factor from radians to degrees.
   */
constexpr f32 RAD2DEG = 180.0f / PI;

/**
   * @var EPSILON
   * @brief Small value used for floating-point comparisons.
   */
constexpr f32 EPSILON = 1.0e-6f;

/**
 * @brief Clamps a value between a minimum and maximum.
 * 
 * Returns the value limited to the specified range [min, max].
 * If value < min, returns min. If value > max, returns max.
 * Otherwise, returns value.
 * 
 * @tparam T The type of the value (must support comparison operators)
 * @param value The value to clamp
 * @param min The minimum allowable value
 * @param max The maximum allowable value
 * @return T The clamped value within [min, max]
 * 
 * @example
 * @code
 * f32 clamped = Math::Clamp(5.0f, 0.0f, 3.0f);  // clamped = 3.0f
 * f32 clamped2 = Math::Clamp(-1.0f, 0.0f, 3.0f); // clamped2 = 0.0f
 * f32 clamped3 = Math::Clamp(2.0f, 0.0f, 3.0f);  // clamped3 = 2.0f
 * @endcode
 */
template <typename T>
constexpr T Clamp(T value, T min, T max) {
  return (value < min) ? min : (value > max) ? max : value;
}

/**
 * @brief Linearly interpolates between two values.
 * 
 * Performs linear interpolation (lerp) between a and b using parameter t.
 * Formula: result = a + (b - a) * t
 * 
 * @tparam T The type of the values (must support arithmetic operators)
 * @param a The start value (t = 0)
 * @param b The end value (t = 1)
 * @param t The interpolation parameter (0.0 to 1.0)
 * @return T The interpolated value
 * 
 * Parameter t behavior:
 * - t = 0.0: Returns a
 * - t = 1.0: Returns b
 * - t < 0.0 or t > 1.0: Extrapolates beyond a and b
 * 
 * @example
 * @code
 * f32 result = Math::Lerp(0.0f, 10.0f, 0.5f);  // result = 5.0f
 * f32 result2 = Math::Lerp(20.0f, 30.0f, 0.25f); // result2 = 22.5f
 * @endcode
 */
template <typename T>
constexpr T Lerp(T a, T b, f32 t) {
  return a + t * (b - a);
}

/**
 * @brief Converts degrees to radians.
 * 
 * Multiplies the degree value by the DEG2RAD conversion factor.
 * 
 * @param degrees Angle in degrees
 * @return f32 Angle in radians
 * 
 * @example
 * @code
 * f32 radians = Math::ToRadians(180.0f);  // radians = π
 * @endcode
 */
inline f32 ToRadians(f32 degrees) { return degrees * DEG2RAD; }

/**
 * @brief Converts radians to degrees.
 * 
 * Multiplies the radian value by the RAD2DEG conversion factor.
 * 
 * @param radians Angle in radians
 * @return f32 Angle in degrees
 * 
 * @example
 * @code
 * f32 degrees = Math::ToDegrees(Math::PI);  // degrees = 180.0f
 * @endcode
 */
inline f32 ToDegrees(f32 radians) { return radians * RAD2DEG; }

/**
 * @brief Checks if two floating-point numbers are approximately equal.
 * 
 * Compares two f32 values within a small epsilon tolerance to account for
 * floating-point precision errors.
 * 
 * @param a The first floating-point value
 * @param b The second floating-point value
 * @param epsilon The tolerance for comparison (default is EPSILON)
 * @return bool True if values are approximately equal, false otherwise
 * 
 * @example
 * @code
 * bool equal = Math::Approximately(0.1f + 0.2f, 0.3f);  // equal = true
 * @endcode
 */
inline bool NearZero(f32 value, f32 epsilon = EPSILON) {
  return std::fabs(value) <= epsilon;
}

/**
 * @brief Checks if two floating-point numbers are approximately equal.
 * 
 * Compares two f32 values within a small epsilon tolerance to account for
 * floating-point precision errors.
 * 
 * @param a The first floating-point value
 * @param b The second floating-point value
 * @param epsilon The tolerance for comparison (default is EPSILON)
 * @return bool True if values are approximately equal, false otherwise
 * 
 * @example
 * @code
 * bool equal = Math::Approximately(0.1f + 0.2f, 0.3f);  // equal = true
 * @endcode
 */
inline f32 Sqrt(f32 v) { return std::sqrt(v); }

// ----------------Trigonometric functions----------------

/**
 * @brief Sin function.
 * 
 * @param angleRad angle in radians
 * @return f32 Sine of the angle
 */
inline f32 Sin(f32 angleRad) { return std::sin(angleRad); }

/**
 * @brief Cos function.
 * 
 * @param angleRad angle in radians
 * @return f32 Cosine of the angle
 */
inline f32 Cos(f32 angleRad) { return std::cos(angleRad); }

/**
 * @brief Tan function.
 * 
 * @param angleRad angle in radians
 * @return f32 Tangent of the angle
 */
inline f32 Tan(f32 angleRad) { return std::tan(angleRad); }

// ----------------Other common math functions----------------

/**
 * @brief Arc sine function.
 * 
 * @param value Input value
 * @return f32 Arc sine of the value in radians
 */
inline f32 ASin(f32 value) { return std::asin(value); }

/**
 * @brief Arc cosine function.
 * 
 * @param value Input value
 * @return f32 Arc cosine of the value in radians
 */
inline f32 ACos(f32 value) { return std::acos(value); }

/**
 * @brief Arc tangent function.
 * 
 * @param value Input value
 * @return f32 Arc tangent of the value in radians
 */
inline f32 ATan(f32 value) { return std::atan(value); }

/**
 * @brief Arc tangent function with two arguments.
 * 
 * @param y Y coordinate
 * @param x X coordinate
 * @return f32 Arc tangent of y/x in radians
 */
inline f32 ATan2(f32 y, f32 x) { return std::atan2(y, x); }

// ----------------Exponential and logarithmic functions----------------

/**
 * @brief Exponential function.
 * 
 * @param value Input value
 * @return f32 e raised to the power of value
 */
inline f32 Exp(f32 value) { return std::exp(value); }

/**
 * @brief Natural logarithm function.
 * 
 * @param value Input value
 * @return f32 Natural logarithm of the value
 */
inline f32 Log(f32 value) { return std::log(value); }

/**
 * @brief Base-10 logarithm function.
 * 
 * @param value Input value
 * @return f32 Base-10 logarithm of the value
 */
inline f32 Log10(f32 value) { return std::log10(value); }

// ----------------Rounding functions----------------

/**
 * @brief Ceiling function.
 * 
 * @param value Input value
 * @return f32 Smallest integer not less than value
 */
inline f32 Ceil(f32 value) { return std::ceil(value); }

/**
 * @brief Floor function.
 * 
 * @param value Input value
 * @return f32 Largest integer not greater than value
 */
inline f32 Floor(f32 value) { return std::floor(value); }

/**
 * @brief Round function.
 * 
 * @param value Input value
 * @return f32 Nearest integer to value
 */
inline f32 Round(f32 value) { return std::round(value); }

/**
 * @brief Fractional part function.
 * 
 * @param value Input value
 * @return f32 Fractional part of value
 */
inline f32 Fmod(f32 x, f32 y) { return std::fmod(x, y); }

// ----------------Utility functions----------------

/**
 * @brief Sign function.
 * 
 * @tparam T The type of the value (must support comparison operators)
 * @param value The value to check
 * @return T -1 if value < 0, 1 if value > 0, 0 if value == 0
 * 
 * @example
 * @code
 * i32 sign1 = Math::Sign(-10);  // sign1 = -1
 * i32 sign2 = Math::Sign(5);    // sign2 = 1
 * i32 sign3 = Math::Sign(0);    // sign3 = 0
 * @endcode
 */
template <typename T>
constexpr T Sign(T value) {
  return (T(0) < value) - (value < T(0));
}

/**
 * @brief Smoothstep function for smooth interpolation.
 * 
 * Performs smooth Hermite interpolation between 0 and 1 when x is in [edge0, edge1].
 * The function returns 0 if x <= edge0 and 1 if x >= edge1.
 * The interpolation has zero first derivatives at the endpoints.
 * 
 * @param edge0 The lower edge of the transition
 * @param edge1 The upper edge of the transition
 * @param x The input value to interpolate
 * @return f32 The smoothly interpolated value between 0 and 1
 * @example
 * @code
 * f32 smooth = Math::Smoothstep(0.0f, 1.0f, 0.5f);  // smooth = 0.5f
 * @endcode
 */
inline f32 Smoothstep(f32 edge0, f32 edge1, f32 x) {
  x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  return x * x * (3.0f - 2.0f * x);
}

/**
 * @brief Smootherstep function for even smoother interpolation.
 * 
 * Performs smoother Hermite interpolation between 0 and 1 when x is in [edge0, edge1].
 * The function returns 0 if x <= edge0 and 1 if x >= edge1.
 * The interpolation has zero first and second derivatives at the endpoints.
 * 
 * @param edge0 The lower edge of the transition
 * @param edge1 The upper edge of the transition
 * @param x The input value to interpolate
 * @return f32 The smoothly interpolated value between 0 and 1
 * 
 * @example
 * @code
 * f32 smoother = Math::Smootherstep(0.0f, 1.0f, 0.5f);  // smoother = 0.5f
 * @endcode
 */
inline f32 Smootherstep(f32 edge0, f32 edge1, f32 x) {
  x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  return x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f);
}

/**
 * @brief Remaps a value from one range to another.
 * 
 * Maps a value from the range [fromMin, fromMax] to the range [toMin, toMax].
 * 
 * @param value The input value to remap
 * @param fromMin The lower bound of the input range
 * @param fromMax The upper bound of the input range
 * @param toMin The lower bound of the output range
 * @param toMax The upper bound of the output range
 * @return f32 The remapped value in the output range
 * 
 * @example
 * @code
 * f32 remapped = Math::Remap(5.0f, 0.0f, 10.0f, 100.0f, 200.0f);  // remapped = 150.0f
 * @endcode
 */
inline f32 Remap(f32 value, f32 fromMin, f32 fromMax, f32 toMin, f32 toMax) {
  return toMin + (value - fromMin) * (toMax - toMin) / (fromMax - fromMin);
}

/**
 * @brief Check if value is in range [min, max]
 * 
 * @param value The value to check
 * @param min The minimum bound
 * @param max The maximum bound
 * @return bool True if value is in range, false otherwise
 * @example
 * @code
 * bool inRange = Math::InRange(5, 0, 10);  // inRange = true
 * @endcode
 */
template <typename T>
constexpr bool InRange(T value, T min, T max) {
  return value >= min && value <= max;
}

/**
 * @brief Check if value is in exclusive range (min, max)
 * 
 * @param value The value to check
 * @param min The minimum bound
 * @param max The maximum bound
 * @return bool True if value is in exclusive range, false otherwise
 * @example
 * @code
 * bool inRangeExclusive = Math::InRangeExclusive(5, 0, 10);  // inRangeExclusive = true
 * @endcode
 */
template <typename T>
constexpr bool InRangeExclusive(T value, T min, T max) {
  return value > min && value < max;
}

/**
 * @brief Converts a floating-point number to an integer by rounding.
 * 
 * @param value Input floating-point value
 * @return i32 Rounded integer value
 */
inline i32 FloatToInt(f32 value) { return static_cast<i32>(Round(value)); }

/**
 * @brief Checks if a value is a power of two.
 * 
 * @param value The value to check
 * @return bool True if value is a power of two, false otherwise
 * @example
 * @code
 * bool isPOT = Math::IsPowerOfTwo(16);  // isPOT = true
 * @endcode
 */
template <typename T>
constexpr bool IsPowerOfTwo(T value) {
  return value != 0 && (value & (value - 1)) == 0;
}

/**
 * @brief Calculates the next power of two greater than or equal to value.
 * 
 * @param value The input value
 * @return T The next power of two
 * @example
 * @code
 * u32 nextPOT = Math::NextPowerOfTwo(20);  // nextPOT = 32
 * @endcode
 */
template <typename T>
constexpr T NextPowerOfTwo(T value) {
  if (value <= 0) return 1;
  value--;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  return value + 1;
}
};  // namespace Math
