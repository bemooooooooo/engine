#pragma once
#include "../Math/Matrix4.h"
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"

/**
 * @enum CameraType
 * @brief Enumeration of camera projection types.
 * 
 * Defines the type of projection used by the camera: Perspective or Orthographic.
 */
enum class CameraType {
    Perspective,
    Orthographic
};

/**
 * @class Camera
 * @brief A 3D camera class supporting perspective and orthographic projections.
 * 
 * The Camera class encapsulates the properties and behaviors of a virtual camera
 * in a 3D scene. It supports both perspective and orthographic projection modes,
 * allowing for flexible rendering setups. The camera maintains its position,
 * orientation, and projection parameters, and provides methods to manipulate
 * its transform and retrieve view/projection matrices.
 * 
 * Key features:
 * - Switchable between perspective and orthographic projections
 * - Position and rotation management using Vector3 and Quaternion
 * - View and projection matrix generation
 * - Utility methods for moving, rotating, and looking at targets
 * 
 * @note The camera uses a right-handed coordinate system.
 * @note The up vector is assumed to be (0, 1, 0) unless specified otherwise.
 * 
 * @example
 * @code
 * Camera camera(CameraType::Perspective, 45.0f, 16.0f/9.0f, 0.1f, 100.0f);
 * camera.SetPosition(Vector3(0, 0, 5));
 * camera.LookAt(Vector3(0, 0, 0));
 * Matrix4 viewProj = camera.GetViewProjectionMatrix();
 * @endcode
 */
class Camera {
public:
    /**
     * @brief Construct a new Camera object
     * 
     * Initializes a camera with default parameters. The default camera is a perspective camera
     * positioned at the origin, looking down the negative Z-axis.
     */
    Camera();

    /**
     * @brief Construct a new Camera object with specified parameters.
     * 
     * @param type The type of camera projection (Perspective or Orthographic)
     * @param fovOrSize For Perspective: vertical field of view in degrees; For Orthographic: size of the view volume
     * @param aspect Aspect ratio (width / height) of the viewport
     * @param near Near clipping plane distance
     * @param far Far clipping plane distance
     */
    Camera(CameraType type, f32 fovOrSize, f32 aspect, f32 near, f32 far);
    
    /**
     * @brief Sets the camera to use perspective projection.
     * 
     * @param fov Vertical field of view in degrees
     * @param aspect Aspect ratio (width / height) of the viewport
     * @param near Near clipping plane distance
     * @param far Far clipping plane distance
     */
    void SetPerspective(f32 fov, f32 aspect, f32 near, f32 far);

    /**
     * @brief Sets the camera to use orthographic projection.
     * 
     * @param size Size of the orthographic view volume (height)
     * @param aspect Aspect ratio (width / height) of the viewport
     * @param near Near clipping plane distance
     * @param far Far clipping plane distance
     */
    void SetOrthographic(f32 size, f32 aspect, f32 near, f32 far);
    
    /**
     * @brief Sets the camera's position in world space.
     * 
     * @param position The new position of the camera
     */
    void SetPosition(const Vector3& position) { m_Position = position; Recalculate(); }

    /**
     * @brief Sets the camera's rotation using a quaternion.
     * 
     * @param rotation The new rotation of the camera
     */
    void SetRotation(const Quaternion& rotation) { m_Rotation = rotation; Recalculate(); }
    
    /**
     * @brief Orients the camera to look at a target point.
     * 
     * @param target The point in world space to look at
     * @param up The up direction vector (default is (0, 1, 0))
     */
    void LookAt(const Vector3& target, const Vector3& up = Vector3::Up);
    
    /**
     * @brief Moves the camera by an offset in world space.
     * 
     * @param offset The translation vector to move the camera
     */
    void Move(const Vector3& offset);

    /**
     * @brief Rotates the camera by a quaternion rotation.
     * 
     * @param rotation The quaternion representing the rotation to apply
     */
    void Rotate(const Quaternion& rotation);
    
    // Getters

    /// @brief Gets the camera's position in world space.
    const Vector3& GetPosition() const { return m_Position; }

    /// @brief Gets the camera's rotation as a quaternion.
    const Quaternion& GetRotation() const { return m_Rotation; }
    
    /// @brief Gets the view matrix of the camera.
    const Matrix4& GetViewMatrix() const { return m_ViewMatrix; }

    /// @brief Gets the projection matrix of the camera.
    const Matrix4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

    /// @brief Gets the combined view-projection matrix of the camera.
    Matrix4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }
    
    /// @brief Gets the camera's forward, right, and up direction vectors.
    Vector3 GetForward() const { return m_Rotation * Vector3::Forward; }

    /// @brief Gets the camera's right direction vector.
    Vector3 GetRight() const { return m_Rotation * Vector3::Right; }

    /// @brief Gets the camera's up direction vector.
    Vector3 GetUp() const { return m_Rotation * Vector3::Up; }
    
    /// @brief Gets the camera type (Perspective or Orthographic).
    CameraType GetType() const { return m_Type; }

    /// @brief Gets the camera's field of view (for perspective cameras).
    f32 GetFOV() const { return m_FOV; }

    /// @brief Gets the camera's size (for orthographic cameras).
    f32 GetAspect() const { return m_Aspect; }

    /// @brief Gets the camera's near clipping plane distance.
    f32 GetNear() const { return m_Near; }

    /// @brief Gets the camera's far clipping plane distance.
    f32 GetFar() const { return m_Far; }

private:
    /// @brief Recalculates the view and projection matrices based on current parameters.
    void Recalculate();
    
    // Members

    /// @brief The type of camera projection.
    CameraType m_Type;

    /// @brief The position of the camera in world space.
    Vector3 m_Position;

    /// @brief The rotation of the camera as a quaternion.
    Quaternion m_Rotation;
    
    /// @brief Projection parameters
    f32 m_FOV;      // For perspective
    f32 m_Size;     // For orthographic
    f32 m_Aspect;
    f32 m_Near;
    f32 m_Far;
    
    /// @brief View and projection matrices
    Matrix4 m_ViewMatrix;
    
    /// @brief Projection matrix
    Matrix4 m_ProjectionMatrix;
};