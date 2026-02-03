#pragma once
#include "Math.h"
#include "MathUtils.h"

/**
 * @struct Vector2
 * @brief A 2-dimensional vector structure for mathematics and graphics operations.
 * 
 * Vector2 represents a point or direction in 2D space using two floating-point
 * components (x, y). It supports standard vector operations including addition,
 * subtraction, scalar multiplication, normalization, and distance calculations.
 * 
 * Common uses:
 * - 2D positions and velocities
 * - Screen coordinates and UI positioning
 * - Direction vectors for 2D movement
 * - Texture coordinates (UV mapping)
 * - 2D physics calculations
 * 
 * @note All components are stored as f32 (single-precision floats) for performance.
 * @note Vector operations are defined as operator overloads for intuitive syntax.
 * 
 * @example
 * @code
 * Vector2 position(10.0f, 20.0f);
 * Vector2 velocity(5.0f, -3.0f);
 * Vector2 newPosition = position + velocity;  // (15.0f, 17.0f)
 * 
 * f32 distance = Vector2::Distance(position, newPosition);
 * Vector2 normalized = velocity.Normalized();
 * @endcode
 */
struct Vector2{
  /**
   * @var x
   * @brief The X-component (horizontal coordinate) of the vector.
   */
  f32 x;
  
  /**
   * @var y
   * @brief The Y-component (vertical coordinate) of the vector.
   */
  f32 y;

  /**
   * @brief Default constructor initializing vector to zero (0, 0).
   * 
   * Creates a zero vector where both x and y components are 0.0f.
   * Useful for initializing vectors that will be set later.
   * 
   * @example
   * @code
   * Vector2 zero;  // (0, 0)
   * @endcode
   */
  Vector2() : x(0), y(0) {}
  
  /**
   * @brief Constructor initializing vector with individual components.
   * 
   * Creates a vector with the specified x and y components.
   * 
   * @param x The X-component value
   * @param y The Y-component value
   * 
   * @example
   * @code
   * Vector2 position(10.5f, 20.3f);
   * @endcode
   */
  Vector2(f32 x, f32 y) : x(x), y(y) {}
  
  /**
   * @brief Constructor initializing vector with a scalar value for both components.
   * 
   * Creates a vector where both x and y are set to the same value.
   * Marked as explicit to prevent accidental implicit conversions from scalars.
   * 
   * @param scalar The value for both x and y components
   * 
   * @example
   * @code
   * Vector2 one(1.0f);  // (1.0f, 1.0f)
   * Vector2 scale(2.5f);  // (2.5f, 2.5f)
   * @endcode
   */
  explicit Vector2(f32 scalar) : x(scalar), y(scalar) {}

  /**
   * @brief Vector addition operator.
   * 
   * Adds two vectors component-wise, returning a new vector.
   * Does not modify either operand.
   * 
   * @param other The vector to add to this vector
   * @return Vector2 A new vector with summed components
   * 
   * @example
   * @code
   * Vector2 a(1, 2);
   * Vector2 b(3, 4);
   * Vector2 c = a + b;  // (4, 6)
   * @endcode
   */
  Vector2 operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
  }

  /**
   * @brief Vector subtraction operator.
   * 
   * Subtracts another vector from this vector component-wise.
   * Does not modify either operand.
   * 
   * @param other The vector to subtract from this vector
   * @return Vector2 A new vector with subtracted components
   * 
   * @example
   * @code
   * Vector2 a(5, 7);
   * Vector2 b(2, 3);
   * Vector2 c = a - b;  // (3, 4)
   * @endcode
   */
  Vector2 operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
  }

  /**
   * @brief Vector component-wise multiplication operator.
   * 
   * Multiplies two vectors component-wise (NOT dot product).
   * Each component is multiplied independently.
   * 
   * @param other The vector to multiply with this vector
   * @return Vector2 A new vector with multiplied components
   * 
   * @note This is component-wise multiplication, not dot product. Use Dot() for dot product.
   * 
   * @example
   * @code
   * Vector2 a(2, 3);
   * Vector2 b(4, 5);
   * Vector2 c = a * b;  // (8, 15) - NOT dot product
   * @endcode
   */
  Vector2 operator*(const Vector2& other) const {
    return Vector2(x * other.x, y * other.y);
  }

  /**
   * @brief Vector component-wise division operator.
   * 
   * Divides this vector by another vector component-wise.
   * Each component is divided independently.
   * 
   * @param other The vector to divide this vector by
   * @return Vector2 A new vector with divided components
   * 
   * @warning Does not check for division by zero. Ensure other's components are non-zero.
   * 
   * @example
   * @code
   * Vector2 a(10, 20);
   * Vector2 b(2, 5);
   * Vector2 c = a / b;  // (5, 4)
   * @endcode
   */
  Vector2 operator/(const Vector2& other) const {
    return Vector2(x / other.x, y / other.y);
  }

  /**
   * @brief Scalar multiplication operator (vector * scalar).
   * 
   * Multiplies each component of the vector by a scalar value.
   * This is the primary way to scale a vector.
   * 
   * @param scalar The scalar value to multiply with
   * @return Vector2 A new vector with scaled components
   * 
   * @example
   * @code
   * Vector2 v(2, 3);
   * Vector2 scaled = v * 2.5f;  // (5, 7.5)
   * @endcode
   */
  Vector2 operator*(f32 scalar) const {
    return Vector2(x * scalar, y * scalar);
  }

  /**
   * @brief Scalar division operator (vector / scalar).
   * 
   * Divides each component of the vector by a scalar value.
   * Used to scale down a vector or normalize using a custom denominator.
   * 
   * @param scalar The scalar value to divide by
   * @return Vector2 A new vector with divided components
   * 
   * @warning Does not check for division by zero. Ensure scalar is non-zero.
   * 
   * @example
   * @code
   * Vector2 v(10, 20);
   * Vector2 scaled = v / 2.0f;  // (5, 10)
   * @endcode
   */
  Vector2 operator/(f32 scalar) const {
    return Vector2(x / scalar, y / scalar);
  }

  /**
   * @brief In-place vector addition operator.
   * 
   * Adds another vector to this vector, modifying it in-place.
   * Returns a reference to this vector for chaining operations.
   * 
   * @param other The vector to add
   * @return Vector2& Reference to this modified vector
   * 
   * @example
   * @code
   * Vector2 v(1, 2);
   * v += Vector2(3, 4);  // v is now (4, 6)
   * @endcode
   */
  Vector2& operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * @brief In-place vector subtraction operator.
   * 
   * Subtracts another vector from this vector, modifying it in-place.
   * Returns a reference to this vector for chaining operations.
   * 
   * @param other The vector to subtract
   * @return Vector2& Reference to this modified vector
   * 
   * @example
   * @code
   * Vector2 v(5, 7);
   * v -= Vector2(2, 3);  // v is now (3, 4)
   * @endcode
   */
  Vector2& operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * @brief In-place component-wise multiplication operator.
   * 
   * Multiplies this vector by another vector component-wise, modifying it in-place.
   * 
   * @param other The vector to multiply with
   * @return Vector2& Reference to this modified vector
   * 
   * @example
   * @code
   * Vector2 v(2, 3);
   * v *= Vector2(4, 5);  // v is now (8, 15)
   * @endcode
   */
  Vector2& operator*=(const Vector2& other) {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  /**
   * @brief In-place component-wise division operator.
   * 
   * Divides this vector by another vector component-wise, modifying it in-place.
   * 
   * @param other The vector to divide by
   * @return Vector2& Reference to this modified vector
   * 
   * @warning Does not check for division by zero.
   * 
   * @example
   * @code
   * Vector2 v(10, 20);
   * v /= Vector2(2, 5);  // v is now (5, 4)
   * @endcode
   */
  Vector2& operator/=(const Vector2& other) {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
   * @brief In-place scalar multiplication operator.
   * 
   * Multiplies this vector by a scalar value, modifying it in-place.
   * 
   * @param scalar The scalar to multiply by
   * @return Vector2& Reference to this modified vector
   * 
   * @example
   * @code
   * Vector2 v(2, 3);
   * v *= 2.0f;  // v is now (4, 6)
   * @endcode
   */
  Vector2& operator*=(f32 scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  /**
   * @brief In-place scalar division operator.
   * 
   * Divides this vector by a scalar value, modifying it in-place.
   * 
   * @param scalar The scalar to divide by
   * @return Vector2& Reference to this modified vector
   * 
   * @warning Does not check for division by zero.
   * 
   * @example
   * @code
   * Vector2 v(10, 20);
   * v /= 2.0f;  // v is now (5, 10)
   * @endcode
   */
  Vector2& operator/=(f32 scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  /**
   * @brief Equality comparison operator with floating-point tolerance.
   * 
   * Compares two vectors for equality using approximate comparison to handle
   * floating-point precision issues. Uses Math::Approximately() internally.
   * 
   * @param other The vector to compare with
   * @return bool True if vectors are approximately equal, false otherwise
   * 
   * @note Uses floating-point tolerance, not exact equality.
   * 
   * @example
   * @code
   * Vector2 a(1.0f, 2.0f);
   * Vector2 b(1.000001f, 2.000001f);
   * if (a == b) {  // True due to approximate comparison
   *     // Vectors are considered equal
   * }
   * @endcode
   */
  bool operator==(const Vector2& other) const {
    return Math::Approximately(x, other.x) && Math::Approximately(y, other.y);
  }

  /**
   * @brief Inequality comparison operator with floating-point tolerance.
   * 
   * Compares two vectors for inequality using approximate comparison.
   * Returns the opposite of the equality operator.
   * 
   * @param other The vector to compare with
   * @return bool True if vectors are not approximately equal, false otherwise
   * 
   * @example
   * @code
   * Vector2 a(1.0f, 2.0f);
   * Vector2 b(5.0f, 6.0f);
   * if (a != b) {
   *     // Vectors are different
   * }
   * @endcode
   */
  bool operator!=(const Vector2& other) const {
    return !(*this == other);
  }

  /**
   * @brief Unary negation operator (negates both components).
   * 
   * Returns a new vector with both components negated.
   * Useful for reversing direction vectors.
   * 
   * @return Vector2 A new vector with negated components
   * 
   * @example
   * @code
   * Vector2 v(3, -4);
   * Vector2 neg = -v;  // (-3, 4)
   * @endcode
   */
  Vector2 operator-() const {
    return Vector2(-x, -y);
  }

  /**
   * @brief Array-style access operator (const version).
   * 
   * Allows accessing vector components by index: 0 = x, 1 = y.
   * This is the const version and returns by value.
   * 
   * @param index Component index (0 = x, 1 = y)
   * @return f32 The component value
   * 
   * @warning No bounds checking. Index must be 0 or 1.
   * 
   * @example
   * @code
   * Vector2 v(10, 20);
   * f32 x = v[0];  // 10
   * f32 y = v[1];  // 20
   * @endcode
   */
  f32 operator[](u32 index) const {
    return (&x)[index];
  }

  /**
   * @brief Array-style access operator (mutable version).
   * 
   * Allows accessing and modifying vector components by index: 0 = x, 1 = y.
   * This is the mutable version and returns by reference.
   * 
   * @param index Component index (0 = x, 1 = y)
   * @return f32& Reference to the component
   * 
   * @warning No bounds checking. Index must be 0 or 1.
   * 
   * @example
   * @code
   * Vector2 v(10, 20);
   * v[0] = 30;  // v is now (30, 20)
   * v[1] += 5;  // v is now (30, 25)
   * @endcode
   */
  f32& operator[](u32 index) {
    return (&x)[index];
  }

  /**
   * @brief Calculates the length (magnitude) of this vector.
   * 
   * Computes the Euclidean length of the vector using the formula:
   * length = sqrt(x² + y²)
   * 
   * @return f32 The length of the vector
   * 
   * @note This involves a square root operation. For comparisons, use LengthSquared() instead.
   * 
   * @example
   * @code
   * Vector2 v(3, 4);
   * f32 len = v.Length();  // 5.0
   * @endcode
   */
  f32 Length() const {
    return Math::Sqrt(x * x + y * y);
  }

  /**
   * @brief Calculates the squared length (magnitude squared) of this vector.
   * 
   * Computes x² + y² without the expensive square root operation.
   * Useful for distance comparisons where exact values aren't needed.
   * 
   * @return f32 The squared length of the vector
   * 
   * @note Faster than Length() since it avoids the square root. Use for comparisons.
   * 
   * @example
   * @code
   * Vector2 v(3, 4);
   * f32 lenSq = v.LengthSquared();  // 25.0
   * 
   * // Distance comparison without sqrt
   * if (v.LengthSquared() > maxDistanceSq) {
   *     // Too far away
   * }
   * @endcode
   */
  f32 LengthSquared() const {
    return x * x + y * y;
  }

  /**
   * @brief Normalizes this vector in-place.
   * 
   * Scales the vector so its length becomes 1.0 (unit vector).
   * The direction remains the same; only the magnitude changes.
   * If the vector is zero-length, it remains unchanged.
   * 
   * @note Modifies this vector in-place.
   * @note Handles zero-length vectors gracefully.
   * 
   * @example
   * @code
   * Vector2 v(3, 4);
   * v.Normalize();  // v is now (0.6, 0.8) with length 1.0
   * @endcode
   */
  void Normalize() {
    f32 len = Length();
    if (Math::NearZero(len)) {
      x = 0.0f;
      y = 0.0f;
    } else {
      x /= len;
      y /= len;
    }
  }

  /**
   * @brief Returns a normalized copy of this vector.
   * 
   * Creates and returns a new unit vector in the same direction as this vector.
   * This vector is not modified. If the vector is zero-length, returns zero vector.
   * 
   * @return Vector2 A new unit vector in the same direction
   * 
   * @note Does not modify this vector; creates a new one.
   * @note Handles zero-length vectors gracefully.
   * 
   * @example
   * @code
   * Vector2 v(3, 4);
   * Vector2 normalized = v.Normalized();  // (0.6, 0.8)
   * assert(Math::Approximately(normalized.Length(), 1.0f));
   * @endcode
   */
  Vector2 Normalized() const {
    f32 len = Length();
    if (Math::NearZero(len)) {
      return Vector2(0.0f, 0.0f);
    } else {
      return Vector2(x / len, y / len);
    }
  }

  /**
   * @brief Checks if this vector is normalized (length approximately 1.0).
   * 
   * Tests whether the vector is a unit vector using floating-point tolerance.
   * 
   * @return bool True if length is approximately 1.0, false otherwise
   * 
   * @example
   * @code
   * Vector2 v(1, 0);
   * assert(v.isNormalized());  // True
   * 
   * Vector2 w(2, 0);
   * assert(!w.isNormalized());  // False
   * @endcode
   */
  bool isNormalized() const {
    return Math::Approximately(LengthSquared(), 1.0f);
  }

  /**
   * @brief Checks if this vector is zero vector (approximately zero length).
   * 
   * Tests whether the vector has zero length using floating-point tolerance.
   * 
   * @return bool True if length is approximately zero, false otherwise
   * 
   * @example
   * @code
   * Vector2 zero(0, 0);
   * assert(zero.isZero());  // True
   * 
   * Vector2 v(1, 1);
   * assert(!v.isZero());  // False
   * @endcode
   */
  bool isZero() const {
    return Math::NearZero(LengthSquared());
  }

  /**
   * @brief Calculates the dot product of two vectors.
   * 
   * The dot product (scalar product) is: a·b = a.x*b.x + a.y*b.y
   * Mathematically, a·b = |a| * |b| * cos(θ), where θ is the angle between vectors.
   * 
   * @param a The first vector
   * @param b The second vector
   * @return f32 The dot product result
   * 
   * Interpretation of dot product:
   * - Positive: vectors point in similar directions (angle < 90°)
   * - Zero: vectors are perpendicular (angle = 90°)
   * - Negative: vectors point in opposite directions (angle > 90°)
   * 
   * @example
   * @code
   * Vector2 a(1, 0);
   * Vector2 b(0, 1);
   * f32 dot = Vector2::Dot(a, b);  // 0 (perpendicular)
   * 
   * Vector2 c(2, 0);
   * f32 dot2 = Vector2::Dot(a, c);  // 2 (same direction)
   * @endcode
   */
  static f32 Dot(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;
  }

  /**
   * @brief Calculates the distance between two points.
   * 
   * Computes the Euclidean distance between two vectors treated as points in 2D space.
   * Formula: distance = |(a - b)|
   * 
   * @param a The first point
   * @param b The second point
   * @return f32 The distance between the points
   * 
   * @note This is expensive due to square root. Use DistanceSquared() for comparisons.
   * 
   * @example
   * @code
   * Vector2 a(0, 0);
   * Vector2 b(3, 4);
   * f32 dist = Vector2::Distance(a, b);  // 5.0
   * @endcode
   */
  static f32 Distance(const Vector2& a, const Vector2& b) {
    return (a - b).Length();
  }

  /**
   * @brief Calculates the squared distance between two points.
   * 
   * Computes the squared distance between two vectors without expensive square root.
   * Useful for distance comparisons where relative ordering is what matters.
   * 
   * @param a The first point
   * @param b The second point
   * @return f32 The squared distance between the points
   * 
   * @note Much faster than Distance(). Use for comparisons and sorting.
   * 
   * @example
   * @code
   * Vector2 a(0, 0);
   * Vector2 b(3, 4);
   * f32 distSq = Vector2::DistanceSquared(a, b);  // 25.0
   * 
   * // Find closest point without sqrt
   * if (Vector2::DistanceSquared(enemy, player) < maxRangeSq) {
   *     // Enemy is in range
   * }
   * @endcode
   */
  static f32 DistanceSquared(const Vector2& a, const Vector2& b) {
    return (a - b).LengthSquared();
  }

  /**
   * @brief Linearly interpolates between two vectors.
   * 
   * Performs linear interpolation (lerp) between vectors a and b.
   * Formula: result = a + (b - a) * t = a * (1 - t) + b * t
   * 
   * @param a The start vector (t = 0)
   * @param b The end vector (t = 1)
   * @param t The interpolation parameter (0.0 to 1.0)
   * @return Vector2 The interpolated vector
   * 
   * Parameter t behavior:
   * - t = 0.0: Returns a
   * - t = 0.5: Returns midpoint
   * - t = 1.0: Returns b
   * - t < 0.0 or t > 1.0: Extrapolates (useful for some techniques)
   * 
   * @example
   * @code
   * Vector2 start(0, 0);
   * Vector2 end(10, 10);
   * 
   * Vector2 halfway = Vector2::Lerp(start, end, 0.5f);  // (5, 5)
   * Vector2 quarter = Vector2::Lerp(start, end, 0.25f);  // (2.5, 2.5)
   * @endcode
   */
  static Vector2 Lerp(const Vector2& a, const Vector2& b, f32 t) {
    return Vector2(
        Math::Lerp(a.x, b.x, t),
        Math::Lerp(a.y, b.y, t)
    );
  }

  /**
   * @var Zero
   * @brief Constant zero vector (0, 0).
   */
  static const Vector2 Zero;
  
  /**
   * @var One
   * @brief Constant unit vector (1, 1).
   */
  static const Vector2 One;
  
  /**
   * @var UnitX
   * @brief Constant unit vector along X axis (1, 0).
   */
  static const Vector2 UnitX;
  
  /**
   * @var UnitY
   * @brief Constant unit vector along Y axis (0, 1).
   */
  static const Vector2 UnitY;
};

/**
 * @brief Scalar multiplication operator (scalar * vector).
 * 
 * Multiplies a scalar by a vector, scaling each component.
 * This allows writing either `v * 2.0f` or `2.0f * v` with the same result.
 * 
 * @param scalar The scalar to multiply with
 * @param vec The vector to scale
 * @return Vector2 A new scaled vector
 * 
 * @example
 * @code
 * Vector2 v(2, 3);
 * Vector2 result1 = v * 2.5f;      // (5, 7.5)
 * Vector2 result2 = 2.5f * v;      // (5, 7.5) - same result
 * @endcode
 */
inline Vector2 operator*(f32 scalar, const Vector2& vec) {
  return vec*scalar;
}

