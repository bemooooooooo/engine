#pragma once
#include "Math.h"
#include "Vector3.h"
#include "Matrix4.h"

struct Quaternion {
    f32 x, y, z, w;
    
    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
    
    // Базовые операции
    Quaternion operator+(const Quaternion& other) const {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }
    
    Quaternion operator*(f32 scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    
    Vector3 operator*(const Vector3& vec) const {
        Quaternion p(vec.x, vec.y, vec.z, 0.0f);
        Quaternion result = (*this) * p * Conjugate();
        return Vector3(result.x, result.y, result.z);
    }
    
    Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }
    
    bool operator==(const Quaternion& other) const {
        return Math::Approximately(x, other.x) && Math::Approximately(y, other.y) && 
               Math::Approximately(z, other.z) && Math::Approximately(w, other.w);
    }
    
    // Методы
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
    
    Quaternion Normalized() const {
        Quaternion result = *this;
        result.Normalize();
        return result;
    }
    
    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }
    
    Quaternion Inverse() const {
        f32 lenSq = LengthSquared();
        if (Math::NearZero(lenSq)) return Identity();
        return Conjugate() * (1.0f / lenSq);
    }
    
    f32 Dot(const Quaternion& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    
    Matrix4 ToMatrix() const {
        Matrix4 result;
        
        f32 xx = x * x, yy = y * y, zz = z * z;
        f32 xy = x * y, xz = x * z, xw = x * w;
        f32 yz = y * z, yw = y * w, zw = z * w;
        
        result.m[0] = 1.0f - 2.0f * (yy + zz);
        result.m[1] = 2.0f * (xy - zw);
        result.m[2] = 2.0f * (xz + yw);
        
        result.m[4] = 2.0f * (xy + zw);
        result.m[5] = 1.0f - 2.0f * (xx + zz);
        result.m[6] = 2.0f * (yz - xw);
        
        result.m[8] = 2.0f * (xz - yw);
        result.m[9] = 2.0f * (yz + xw);
        result.m[10] = 1.0f - 2.0f * (xx + yy);
        
        return result;
    }
    
    // Статические методы
    static Quaternion Identity() {
        return Quaternion(0, 0, 0, 1);
    }
    
    static Quaternion FromAxisAngle(const Vector3& axis, f32 angle) {
        f32 halfAngle = angle * 0.5f;
        f32 s = Math::Sin(halfAngle);
        Vector3 n = axis.Normalized();
        
        return Quaternion(n.x * s, n.y * s, n.z * s, Math::Cos(halfAngle));
    }
    
    static Quaternion FromEuler(const Vector3& euler) {
        f32 halfX = euler.x * 0.5f;
        f32 halfY = euler.y * 0.5f;
        f32 halfZ = euler.z * 0.5f;
        
        f32 cx = Math::Cos(halfX), sx = Math::Sin(halfX);
        f32 cy = Math::Cos(halfY), sy = Math::Sin(halfY);
        f32 cz = Math::Cos(halfZ), sz = Math::Sin(halfZ);
        
        return Quaternion(
            sx * cy * cz - cx * sy * sz,
            cx * sy * cz + sx * cy * sz,
            cx * cy * sz - sx * sy * cz,
            cx * cy * cz + sx * sy * sz
        );
    }
    
    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, f32 t) {
        t = Math::Clamp(t, 0.0f, 1.0f);
        return (a * (1.0f - t) + b * t).Normalized();
    }
    
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, f32 t) {
        t = Math::Clamp(t, 0.0f, 1.0f);
        
        f32 cosHalfTheta = a.Dot(b);
        Quaternion b2 = b;
        
        if (cosHalfTheta < 0.0f) {
            b2 = b * -1.0f;
            cosHalfTheta = -cosHalfTheta;
        }
        
        if (cosHalfTheta > 0.9999f) {
            return Lerp(a, b2, t);
        }
        
        f32 halfTheta = Math::ACos(cosHalfTheta);
        f32 sinHalfTheta = Math::Sqrt(1.0f - cosHalfTheta * cosHalfTheta);
        
        if (Math::NearZero(sinHalfTheta)) {
            return Lerp(a, b2, t);
        }
        
        f32 ratioA = Math::Sin((1.0f - t) * halfTheta) / sinHalfTheta;
        f32 ratioB = Math::Sin(t * halfTheta) / sinHalfTheta;
        
        return (a * ratioA + b2 * ratioB).Normalized();
    }
    
    static Quaternion LookRotation(const Vector3& forward, const Vector3& up = Vector3::Up) {
        Vector3 f = forward.Normalized();
        Vector3 u = up.Normalized();
        Vector3 r = Vector3::Cross(u, f);
        u = Vector3::Cross(f, r);
        
        Matrix4 m;
        m.m[0] = r.x; m.m[1] = r.y; m.m[2] = r.z;
        m.m[4] = u.x; m.m[5] = u.y; m.m[6] = u.z;
        m.m[8] = f.x; m.m[9] = f.y; m.m[10] = f.z;
        
        return FromRotationMatrix(m);
    }
    
    static Quaternion FromRotationMatrix(const Matrix4& matrix) {
    Quaternion result;
    f32 trace = matrix(0, 0) + matrix(1, 1) + matrix(2, 2);
    
    if (trace > 0.0f) {
        f32 s = 0.5f / Math::Sqrt(trace + 1.0f);
        result.w = 0.25f / s;
        result.x = (matrix(2, 1) - matrix(1, 2)) * s;
        result.y = (matrix(0, 2) - matrix(2, 0)) * s;
        result.z = (matrix(1, 0) - matrix(0, 1)) * s;
    }
    else {
        if (matrix(0, 0) > matrix(1, 1) && matrix(0, 0) > matrix(2, 2)) {
            f32 s = 2.0f * Math::Sqrt(1.0f + matrix(0, 0) - matrix(1, 1) - matrix(2, 2));
            result.w = (matrix(2, 1) - matrix(1, 2)) / s;
            result.x = 0.25f * s;
            result.y = (matrix(0, 1) + matrix(1, 0)) / s;
            result.z = (matrix(0, 2) + matrix(2, 0)) / s;
        }
        else if (matrix(1, 1) > matrix(2, 2)) {
            f32 s = 2.0f * Math::Sqrt(1.0f + matrix(1, 1) - matrix(0, 0) - matrix(2, 2));
            result.w = (matrix(0, 2) - matrix(2, 0)) / s;
            result.x = (matrix(0, 1) + matrix(1, 0)) / s;
            result.y = 0.25f * s;
            result.z = (matrix(1, 2) + matrix(2, 1)) / s;
        }
        else {
            f32 s = 2.0f * Math::Sqrt(1.0f + matrix(2, 2) - matrix(0, 0) - matrix(1, 1));
            result.w = (matrix(1, 0) - matrix(0, 1)) / s;
            result.x = (matrix(0, 2) + matrix(2, 0)) / s;
            result.y = (matrix(1, 2) + matrix(2, 1)) / s;
            result.z = 0.25f * s;
        }
    }
    
    return result.Normalized();
}
};