#pragma once

#include "Math.h"
#include "MathUtils.h"
#include <string>

struct Vector3 {
  f32 x, y, z;

  std::string ToString() const {
    return "{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}";
  }

  Vector3() : x(0), y(0), z(0) {}
  Vector3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
  explicit Vector3(f32 scalar) : x(scalar), y(scalar), z(scalar) {}

  Vector3 operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  Vector3 operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  Vector3 operator*(const Vector3& other) const {
    return Vector3(x * other.x, y * other.y, z * other.z);
  }

  Vector3 operator/(const Vector3& other) const {
    return Vector3(x / other.x, y / other.y, z / other.z);
  }

  Vector3 operator*(f32 scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  Vector3 operator/(f32 scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
  }

  Vector3& operator+=(const Vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  Vector3& operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  Vector3& operator*=(const Vector3& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  Vector3& operator/=(const Vector3& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
  }

  Vector3& operator*=(f32 scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  Vector3& operator/=(f32 scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  f32 LengthSquared() const {
    return x * x + y * y + z * z;
  }

  f32 Length() const {
    return Math::Sqrt(LengthSquared());
  }

  void Normalize() {
    f32 len = Length();
    if (Math::NearZero(len)) return;
    (*this) /= len;
  }

  Vector3 Normalized() const {
    Vector3 result = *this;
    result.Normalize();
    return result;
  }

  bool operator==(const Vector3& other) const {
    return Math::Approximately(x, other.x) &&
           Math::Approximately(y, other.y) &&
           Math::Approximately(z, other.z);
  }

  bool operator!=(const Vector3& other) const {
    return !(*this == other);
  }

  bool isNormalized() const {
    return Math::Approximately(LengthSquared(), 1.0f);
  }

  bool isZero() const {
    return Math::NearZero(x) && Math::NearZero(y) && Math::NearZero(z);
  }

  Vector3 operator-() const {
    return Vector3(-x, -y, -z);
  }

  f32 operator[](u32 index) const {
    return (&x)[index];
  }

  f32& operator[](u32 index) {
    return (&x)[index];
  }

  static f32 Dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  static Vector3 Cross(const Vector3& a, const Vector3& b) {
    return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
  }

  static Vector3 Lerp(const Vector3& a, const Vector3& b, f32 t) {
    return Vector3(
        Math::Lerp(a.x, b.x, t),
        Math::Lerp(a.y, b.y, t),
        Math::Lerp(a.z, b.z, t)
    );
  }

  static Vector3 LerpClamped(const Vector3& a, const Vector3& b, f32 t) {
    t = Math::Clamp(t, 0.0f, 1.0f);
    return Lerp(a, b, t);
  }

  static Vector3 QuadraticLerp(const Vector3& a, const Vector3& b, const Vector3& c, f32 t) {
    Vector3 ab = Lerp(a, b, t);
    Vector3 bc = Lerp(b, c, t);
    return Lerp(ab, bc, t);
  }

  static Vector3 Project(const Vector3& a, const Vector3& b) {
    f32 dot = Dot(a,b);
    f32 lenSq = b.LengthSquared();
    if(Math::NearZero(lenSq)) return Vector3::Zero;
    return b * (dot / lenSq);
  }

  static Vector3 Reject(const Vector3& incident, const Vector3& normal) {
    f32 dot = Dot(incident, normal);
    return incident - normal * (2.0f*dot);
  }

  static f32 Distance(const Vector3& a, const Vector3& b) {
    return (a - b).Length();
  }

  static f32 DistanceSquared(const Vector3& a, const Vector3& b) {
    return (a - b).LengthSquared();
  }

  static const Vector3 Zero;
  static const Vector3 One;
  static const Vector3 Up;
  static const Vector3 Down;
  static const Vector3 Left;
  static const Vector3 Right;
  static const Vector3 Forward;
  static const Vector3 Backward;
  static const Vector3 UnitX;
  static const Vector3 UnitY;
  static const Vector3 UnitZ;
};

inline Vector3 operator*(f32 scalar, const Vector3& vec) {
  return vec*scalar;
}


