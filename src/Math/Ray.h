#pragma once
#include "Vector3.h"

struct Ray {
  Vector3 origin;
  Vector3 direction;

  Ray() : origin(Vector3::Zero), direction(Vector3::Forward) {}
  Ray(const Vector3& origin, const Vector3& direction)
      : origin(origin), direction(direction.Normalized()) {}

  Vector3 GetPoint(f32 distance) const { return origin + direction * distance; }

  bool IntersectsPlane(const Vector3& planePoint, const Vector3& planeNormal,
                       f32& distance) const {
    f32 denom = Vector3::Dot(direction, planeNormal);
    if (Math::NearZero(denom)) {
      return false;
    }

    distance = Vector3::Dot(planePoint - origin, planeNormal) / denom;
    return distance >= 0.0f;
  }
};