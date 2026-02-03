#include "Camera.h"
#include "../Core/Log.h"

Camera::Camera() 
    : m_Type(CameraType::Perspective),
      m_Position(Vector3::Zero),
      m_Rotation(Quaternion::Identity()),
      m_FOV(Math::ToRadians(60.0f)),
      m_Size(5.0f),
      m_Aspect(16.0f / 9.0f),
      m_Near(0.1f),
      m_Far(1000.0f) {
    Recalculate();
}

Camera::Camera(CameraType type, f32 fovOrSize, f32 aspect, f32 near, f32 far)
    : m_Type(type),
      m_Position(Vector3::Zero),
      m_Rotation(Quaternion::Identity()),
      m_Aspect(aspect),
      m_Near(near),
      m_Far(far) {
    
    if (type == CameraType::Perspective) {
        m_FOV = fovOrSize;
    } else {
        m_Size = fovOrSize;
    }
    
    Recalculate();
}

void Camera::SetPerspective(f32 fov, f32 aspect, f32 near, f32 far) {
    // Валидация параметров
    if (fov <= 0.0f || fov >= Math::PI) {
        ENGINE_ERROR << "Invalid FOV: " << fov << " (must be between 0 and PI)";
        return;
    }
    if (aspect <= 0.0f) {
        ENGINE_ERROR << "Invalid aspect ratio: " << aspect << " (must be > 0)";
        return;
    }
    if (near <= 0.0f) {
        ENGINE_ERROR << "Invalid near plane: " << near << " (must be > 0)";
        return;
    }
    if (far <= near) {
        ENGINE_ERROR << "Invalid far plane: " << far << " (must be > near: " << near << ")";
        return;
    }
    
    m_Type = CameraType::Perspective;
    m_FOV = fov;
    m_Aspect = aspect;
    m_Near = near;
    m_Far = far;
    Recalculate();
}

void Camera::SetOrthographic(f32 size, f32 aspect, f32 near, f32 far) {
    // Валидация параметров
    if (size <= 0.0f) {
        ENGINE_ERROR << "Invalid orthographic size: " << size << " (must be > 0)";
        return;
    }
    if (aspect <= 0.0f) {
        ENGINE_ERROR << "Invalid aspect ratio: " << aspect << " (must be > 0)";
        return;
    }
    if (near <= 0.0f) {
        ENGINE_ERROR << "Invalid near plane: " << near << " (must be > 0)";
        return;
    }
    if (far <= near) {
        ENGINE_ERROR << "Invalid far plane: " << far << " (must be > near: " << near << ")";
        return;
    }
    
    m_Type = CameraType::Orthographic;
    m_Size = size;
    m_Aspect = aspect;
    m_Near = near;
    m_Far = far;
    Recalculate();
}

void Camera::LookAt(const Vector3& target, const Vector3& up) {
    m_Rotation = Quaternion::LookRotation(target - m_Position, up);
    Recalculate();
}

void Camera::Move(const Vector3& offset) {
    m_Position += offset;
    Recalculate();
}

void Camera::Rotate(const Quaternion& rotation) {
    m_Rotation = rotation * m_Rotation;
    Recalculate();
}

void Camera::Recalculate() {
    // Recalculate view matrix
    Vector3 forward = GetForward();
    Vector3 up = GetUp();
    
    m_ViewMatrix = Matrix4::LookAt(m_Position, m_Position + forward, up);
    
    // Recalculate projection matrix
    if (m_Type == CameraType::Perspective) {
        m_ProjectionMatrix = Matrix4::Perspective(m_FOV, m_Aspect, m_Near, m_Far);
    } else {
        f32 right = m_Size * m_Aspect * 0.5f;
        f32 left = -right;
        f32 top = m_Size * 0.5f;
        f32 bottom = -top;
        m_ProjectionMatrix = Matrix4::Orthographic(left, right, bottom, top, m_Near, m_Far);
    }
}