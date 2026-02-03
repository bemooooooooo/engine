#pragma once
#include "Vector3.h"

struct Plane {
    Vector3 normal;
    f32 distance;
    
    Plane() : normal(Vector3::Up), distance(0) {}
    Plane(const Vector3& normal, f32 distance) : normal(normal.Normalized()), distance(distance) {}
    Plane(const Vector3& point, const Vector3& normal) : normal(normal.Normalized()), distance(Vector3::Dot(normal, point)) {}
    Plane(const Vector3& a, const Vector3& b, const Vector3& c) {
        normal = Vector3::Cross(b - a, c - a).Normalized();
        distance = Vector3::Dot(normal, a);
    }
    
    f32 GetDistanceToPoint(const Vector3& point) const {
        return Vector3::Dot(normal, point) - distance;
    }
    
    Vector3 ClosestPoint(const Vector3& point) const {
        return point - normal * GetDistanceToPoint(point);
    }
    
    bool GetSide(const Vector3& point) const {
        return GetDistanceToPoint(point) > 0.0f;
    }
};