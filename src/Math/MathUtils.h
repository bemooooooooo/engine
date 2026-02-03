#pragma once
#include "Math.h"

/**
 * @namespace Math
 * @brief Additional mathematical utility functions for the game engine.
 * 
 * This namespace extends the core Math namespace with additional utility functions
 * commonly used in game development. It includes functions for minimum/maximum,
 * absolute value, power calculations, approximate comparisons, and various
 * interpolation techniques.
 * 
 * Key features:
 *
 * - Min/Max functions for comparing two values
 *
 * - Absolute value function
 *
 * - Power function for exponentiation
 *
 * - Approximate equality checks for floating-point numbers
 *
 * - Clamped linear interpolation
 *
 * - Quadratic interpolation
 *
 * - Angle wrapping utility
 * 
 * @note All floating-point operations use f32 (single-precision) for performance.
 * 
 * @example
 * @code
 * f32 minValue = Math::Min(3.5f, 2.1f);  // minValue = 2.1f
 * f32 absValue = Math::Abs(-4.2f);       // absValue = 4.2f
 * f32 powerValue = Math::Pow(2.0f, 3.0f); // powerValue = 8.0f
 * bool equal = Math::Approximately(0.1f + 0.2f, 0.3f);  // equal = true
 * @endcode
 */
namespace Math{

  /**
   * @brief Returns the minimum of two values.
   * 
   * @param a The first value
   * @param b The second value
   * @return T The minimum value
   * 
   * @example
   * @code
   * f32 minValue = Math::Min(3.5f, 2.1f);  // minValue = 2.1f
   * @endcode
   */
  template<typename T>
  constexpr T Min(T a, T b){
    return (a < b) ? a : b;
  }

  /**
   * @brief Returns the maximum of two values.
   * 
   * @param a The first value
   * @param b The second value
   * @return T The maximum value
   * 
   * @example
   * @code
   * f32 maxValue = Math::Max(3.5f, 2.1f);  // maxValue = 3.5f
   * @endcode
   */
  template<typename T>
  constexpr T Max(T a, T b){
    return (a > b) ? a : b;
  }

  /**
   * @brief Returns the absolute value of a number.
   * 
   * @param value The value to get the absolute of
   * @return T The absolute value
   * 
   * @example
   * @code
   * f32 absValue = Math::Abs(-4.2f);       // absValue = 4.2f
   * @endcode
   */
  template<typename T>
  constexpr T Abs(T value){
    return (value < T(0)) ? -value : value;
  }

  /**
   * @brief Raises a base to the power of an exponent.
   * 
   * @param base The base value
   * @param exponent The exponent value
   * @return f32 The result of base raised to the exponent
   * 
   * @example
   * @code
   * f32 powerValue = Math::Pow(2.0f, 3.0f); // powerValue = 8.0f
   * @endcode
   */
  inline f32 Pow(f32 base, f32 exponent){
    return std::pow(base, exponent);
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
  inline bool Approximately(f32 a, f32 b, f32 epsilon = EPSILON){
    return Abs(a - b) <= epsilon;
  }

  /**
   * @brief Clamped linear interpolation between two values.
   * 
   * Performs linear interpolation (lerp) between a and b, clamping the
   * interpolation parameter t to the range [0.0, 1.0].
   * 
   * @param a The start value (t = 0)
   * @param b The end value (t = 1)
   * @param t The interpolation parameter
   * @return T The interpolated value
   * 
   * @example
   * @code
   * f32 result = Math::LerpClamped(0.0f, 10.0f, 1.5f); // result = 10.0f
   * @endcode
   */
  template<typename T>
  constexpr T LerpClamped(T a, T b, f32 t){
    t = Clamp(t, 0.0f, 1.0f);
    return Lerp(a, b, t);
  }

  /**
   * @brief Quadratic interpolation among three values.
   * 
   * Performs quadratic interpolation using three control points a, b, and c.
   * 
   * @param a The first control point
   * @param b The second control point
   * @param c The third control point
   * @param t The interpolation parameter
   * @return T The interpolated value
   * 
   * @example
   * @code
   * f32 result = Math::QuadraticLerp(0.0f, 5.0f, 10.0f, 0.5f); // result = 5.0f
   * @endcode
   */
  template<typename T>
  constexpr T QuadraticLerp(T a, T b, T c, f32 t){
    T ab = Lerp(a, b, t);
    T bc = Lerp(b, c, t);
    return Lerp(ab, bc, t);
  }

  /**
   * @brief Wraps an angle in radians to the range [-π, π].
   * 
   * @param angle The input angle in radians
   * @return f32 The wrapped angle in the range [-π, π]
   * 
   * @example
   * @code
   * f32 wrappedAngle = Math::WarpAngle(4.0f); // wrappedAngle ≈ -2.28319f
   * @endcode
   */
  template<typename T>
  constexpr f32 WarpAngle(T angle){
    angle = std::fmod(angle, TWO_PI);
    if(angle < PI) angle -= TWO_PI;
    if(angle > PI) angle += TWO_PI;
    return angle;
  }

}; // namespace Math