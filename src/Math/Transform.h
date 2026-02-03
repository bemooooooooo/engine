#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4.h"

class Transform {
public:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
    
    Transform() : position(Vector3::Zero), rotation(Quaternion::Identity()), scale(Vector3::One) {}
    
    Matrix4 ToMatrix() const {
        Matrix4 translation = Matrix4::Translation(position);
        Matrix4 rotationMat = rotation.ToMatrix();
        Matrix4 scaleMat = Matrix4::Scale(scale);
        
        return translation * rotationMat * scaleMat;
    }
    
    Vector3 Forward() const {
        return rotation * Vector3::Forward;
    }
    
    Vector3 Right() const {
        return rotation * Vector3::Right;
    }
    
    Vector3 Up() const {
        return rotation * Vector3::Up;
    }
    
    void LookAt(const Vector3& target, const Vector3& up = Vector3::Up) {
        rotation = Quaternion::LookRotation(target - position, up);
    }
    
    Transform operator*(const Transform& other) const {
        Transform result;
        result.position = position + rotation * (other.position * scale);
        result.rotation = rotation * other.rotation;
        result.scale = scale * other.scale;
        return result;
    }
    
    static Transform Lerp(const Transform& a, const Transform& b, f32 t) {
        Transform result;
        result.position = Vector3::Lerp(a.position, b.position, t);
        result.rotation = Quaternion::Slerp(a.rotation, b.rotation, t);
        result.scale = Vector3::Lerp(a.scale, b.scale, t);
        return result;
    }
};