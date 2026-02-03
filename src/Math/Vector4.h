#pragma once
#include "Math.h"
#include "MathUtils.h"
#include "Vector3.h"

struct Vector4 {
    f32 x, y, z, w;
    
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
    explicit Vector4(f32 scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vector4(const Vector3& vec, f32 w = 1.0f) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
    
    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    
    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    
    Vector4 operator*(f32 scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    
    Vector4 operator/(f32 scalar) const {
        f32 inv = 1.0f / scalar;
        return Vector4(x * inv, y * inv, z * inv, w * inv);
    }
    
    Vector4& operator+=(const Vector4& other) {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }
    
    Vector4& operator*=(f32 scalar) {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }
    
    bool operator==(const Vector4& other) const {
        return Math::Approximately(x, other.x) && Math::Approximately(y, other.y) && 
               Math::Approximately(z, other.z) && Math::Approximately(w, other.w);
    }
    
    Vector4 operator-() const {
        return Vector4(-x, -y, -z, -w);
    }
    
    f32 operator[](int index) const {
        return (&x)[index];
    }
    
    f32& operator[](int index) {
        return (&x)[index];
    }
    
    f32 LengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    
    f32 Length() const {
        return Math::Sqrt(LengthSquared());
    }
    
    void Normalize() {
        f32 len = Length();
        if (len > Math::EPSILON) {
            f32 invLen = 1.0f / len;
            x *= invLen; y *= invLen; z *= invLen; w *= invLen;
        }
    }
    
    Vector4 Normalized() const {
        Vector4 result = *this;
        result.Normalize();
        return result;
    }
    
    Vector3 XYZ() const {
        return Vector3(x, y, z);
    }
    
    static f32 Dot(const Vector4& a, const Vector4& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }
    
    static Vector4 Lerp(const Vector4& a, const Vector4& b, f32 t) {
        return Vector4(
            Math::Lerp(a.x, b.x, t),
            Math::Lerp(a.y, b.y, t),
            Math::Lerp(a.z, b.z, t),
            Math::Lerp(a.w, b.w, t)
        );
    }
    
    Vector3 Homogenized() const {
        if (Math::NearZero(w)) return Vector3(x, y, z);
        f32 invW = 1.0f / w;
        return Vector3(x * invW, y * invW, z * invW);
    }
    
    static const Vector4 Zero;
    static const Vector4 One;
    static const Vector4 UnitX;
    static const Vector4 UnitY;
    static const Vector4 UnitZ;
    static const Vector4 UnitW;
};

inline Vector4 operator*(f32 scalar, const Vector4& vec) {
    return vec * scalar;
}

