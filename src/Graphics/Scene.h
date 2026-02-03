#pragma once
#include "../Core/Base.h"
#include "../Math/Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include <functional>
#include <unordered_map>

/**
 * @struct SceneObject
 * @brief Represents an object within a 3D scene, containing a mesh and transform.
 * 
 * SceneObject encapsulates a 3D mesh along with its transformation (position,
 * rotation, scale) in the scene. It also supports an optional update callback
 * for per-frame logic.
 * 
 * Key features:
 * - Holds a reference to a Mesh for rendering
 * - Contains a Transform for spatial positioning
 * - Supports an onUpdate callback for custom behavior each frame
 * 
 * @note The onUpdate function, if set, is called during the scene's Update()
 *       method with the delta time parameter.
 * 
 * @example
 * @code
 * Ref<Mesh> cubeMesh = ResourceManager::GetMesh("cube");
 * SceneObject cubeObject("Cube", cubeMesh);
 * cubeObject.transform.position = Vector3(0, 0, 0);
 * cubeObject.onUpdate = [](SceneObject* obj, f32 deltaTime) {
 *     obj->transform.rotation.y += 90.0f * deltaTime; // Rotate 90 degrees per second
 * };
 * @endcode
 */
struct SceneObject {
  /** 
   * @var name
   * @brief The unique name identifier for the scene object.
   */
  std::string name;

  /** 
   * @var mesh
   * @brief The mesh associated with this scene object.
   */
  Ref<Mesh> mesh;

  /** 
   * @var transform
   * @brief The transformation (position, rotation, scale) of the object in the scene.
   */
  Transform transform;

  /** 
   * @var onUpdate
   * @brief Optional callback function called each frame during scene update.
   * 
   * This function, if set, is invoked during the Scene's Update() method,
   * allowing for custom per-frame logic such as animations or behavior.
   * The function receives a pointer to the SceneObject and the delta time
   * since the last frame.
   * 
   * @note Set to nullptr by default (no update).
   * 
   * @example
   * @code
   * sceneObject.onUpdate = [](SceneObject* obj, f32 deltaTime) {
   *     obj->transform.position.x += 1.0f * deltaTime; // Move right at 1 unit/sec
   * };
   * @endcode
   */
  std::function<void (SceneObject*, f32)> onUpdate = nullptr;

  /** 
   * @brief Constructs a SceneObject with the given name, mesh, and optional transform.
   * 
   * @param name The unique name for the scene object
   * @param mesh The mesh to associate with this object
   * @param transform The initial transform of the object (default is identity)
   */
  SceneObject(const std::string& name, const Ref<Mesh>& mesh,
              const Transform& transform = Transform())
      : name(name), mesh(mesh), transform(transform) {}
};  // struct SceneObject

/**
 * @class Scene
 * @brief Represents a 3D scene containing multiple scene objects and a camera.
 * 
 * The Scene class manages a collection of SceneObject instances, allowing for
 * adding, removing, updating, and rendering them. It also holds a Camera to
 * define the viewpoint for rendering the scene.
 * 
 * Key features:
 * - Add and remove SceneObjects by name
 * - Find SceneObjects by name
 * - Update all objects each frame, invoking their onUpdate callbacks
 * - Render all objects using a specified shader
 * - Manage a Camera for the scene
 * 
 * @note The Scene does not handle physics or collision; it focuses on object
 *       management and rendering.
 * 
 * @example
 * @code
 * Scene scene("SampleScene");
 * Ref<Mesh> cubeMesh = ResourceManager::GetMesh("cube");
 * SceneObject cubeObject("Cube", cubeMesh);
 * scene.AddObject(cubeObject);
 * 
 * // In the main loop
 * f32 deltaTime = timer.Tick();
 * scene.Update(deltaTime);
 * scene.Render(shader);
 * @endcode
 */
class Scene {
 public:
 /** 
   * @brief Constructs a new Scene with an optional name.
   * 
   * @param name The name of the scene (default is empty string)
   */
  Scene();
  /** 
   * @brief Constructs a new Scene with the specified name.
   * 
   * @param name The name of the scene
   */
  Scene(std::string name);
  /** 
   * @brief Destructor for the Scene class.
   */
  ~Scene();

  /** 
   * @brief Adds a SceneObject to the scene.
   * 
   * @param object The SceneObject to add
   */
  void AddObject(const SceneObject& object);

  /** 
   * @brief Removes a SceneObject from the scene by name.
   * 
   * @param name The name of the SceneObject to remove
   */
  void RemoveObject(const std::string& name);

  /** 
   * @brief Finds a SceneObject in the scene by name.
   * 
   * @param name The name of the SceneObject to find
   * @return SceneObject* Pointer to the found SceneObject, or nullptr if not found
   */
  [[nodiscard]] SceneObject* FindObject(const std::string& name);

  /** 
   * @brief Updates all SceneObjects in the scene.
   * 
   * Calls the onUpdate callback for each object if it is set.
   * 
   * @param deltaTime The time elapsed since the last update in seconds
   */
  virtual void Update(f32 deltaTime);

  /** 
   * @brief Renders all SceneObjects in the scene using the specified shader.
   * 
   * @param shader The shader to use for rendering the objects
   */
  virtual void Render(const Ref<Shader>& shader) const;

  /** 
   * @brief Gets the list of SceneObjects in the scene.
   * 
   * @return const std::vector<SceneObject>& Constant reference to the vector of SceneObjects
   */
  const std::vector<SceneObject>& GetObjects() const { return m_Objects; }

  /** 
   * @brief Gets the list of SceneObjects in the scene.
   * 
   * @return std::vector<SceneObject>& Reference to the vector of SceneObjects
   */
  std::vector<SceneObject>& GetObjects() { return m_Objects; }

  /** 
   * @brief Sets the camera for the scene.
   * 
   * @param camera The Camera to set
   */
  void SetCamera(const Camera& camera) { m_Camera = camera; }

  /** 
   * @brief Gets the camera of the scene.
   * 
   * @return Camera& Reference to the scene's Camera
   */
  Camera& GetCamera() { return m_Camera; }

  /** 
   * @brief Gets the camera of the scene.
   * 
   * @return const Camera& Constant reference to the scene's Camera
   */
  const Camera& GetCamera() const { return m_Camera; }

 private:
 /** 
   * @var name
   * @brief The name of the scene.
   */
  std::string name;

  /** 
   * @var m_Objects
   * @brief The list of SceneObjects contained in the scene.
   */
  std::vector<SceneObject> m_Objects;
  
  /** 
   * @var m_ObjectIndex
   * @brief A map for quick lookup of SceneObjects by name.
   */
  std::unordered_map<std::string, size_t> m_ObjectIndex;

  /** 
   * @var m_Camera
   * @brief The camera used to view the scene.
   */
  Camera m_Camera;
};  // class Scene
