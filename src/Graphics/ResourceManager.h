#pragma once
#include "../Core/Base.h"
#include <unordered_map>
#include <string>

// ----------------Forward declarations----------------
class Shader;
class Texture;
class Mesh;

/**
 * @class ResourceManager
 * @brief Centralized manager for loading, storing, and accessing graphics resources.
 * 
 * This class provides static methods to load, retrieve, and unload various graphics
 * resources such as shaders, textures, and meshes. It maintains internal maps to
 * ensure that resources are loaded only once and shared across the engine.
 * 
 * Key features:
 * - Load shaders from files or source code
 * - Load textures from image files
 * - Register and retrieve meshes
 * - Automatic resource cleanup on shutdown
 * 
 * @note All methods are static; no instances of ResourceManager should be created.
 * @note Resources are identified by unique string names.
 * 
 * @example
 * @code
 * auto shader = ResourceManager::LoadShader("basic", "vertex.glsl", "fragment.glsl");
 * auto texture = ResourceManager::LoadTexture("brick", "brick_diffuse.png");
 * auto mesh = ResourceManager::GetMesh("cube");
 * @endcode
 */
class ResourceManager {
public:
    // ----------------Shader management----------------
    /** 
     * @brief Retrieves a shader by name.
     * 
     * @param name The unique name of the shader
     * @return Ref<Shader> The requested shader, or nullptr if not found
     */
    [[nodiscard]] static Ref<Shader> GetShader(const std::string& name);

    /** 
     * @brief Loads a shader from vertex and fragment file paths.
     * 
     * @param name The unique name to associate with the shader
     * @param vertexPath The file path to the vertex shader source
     * @param fragmentPath The file path to the fragment shader source
     * @return Ref<Shader> The loaded shader
     */
    [[nodiscard]] static Ref<Shader> LoadShader(const std::string& name, 
                                  const std::string& vertexPath, 
                                  const std::string& fragmentPath);

    /**
     * @brief Loads a shader from provided source code strings.
     * 
     * @param name The unique name to associate with the shader
     * @param vertexSrc The vertex shader source code
     * @param fragmentSrc The fragment shader source code
     * @return Ref<Shader> The loaded shader
     */
    [[nodiscard]] static Ref<Shader> LoadShaderFromSource(const std::string& name,
                                            const std::string& vertexSrc,
                                            const std::string& fragmentSrc);
    /** 
     * @brief Unloads a shader by name.
     * @param name The unique name of the shader to unload
     */
    static void UnloadShader(const std::string& name);
    
    // ----------------Texture management----------------
    /** 
     * @brief Retrieves a texture by name.
     * 
     * @param name The unique name of the texture
     * @return Ref<Texture> The requested texture, or nullptr if not found
     */
    [[nodiscard]] static Ref<Texture> GetTexture(const std::string& name);

    /** 
     * @brief Loads a texture from an image file.
     * 
     * @param name The unique name to associate with the texture
     * @param path The file path to the image
     * @return Ref<Texture> The loaded texture
     */
    [[nodiscard]] static Ref<Texture> LoadTexture(const std::string& name, const std::string& path);

    /** 
     * @brief Unloads a texture by name.
     * @param name The unique name of the texture to unload
     */
    static void UnloadTexture(const std::string& name);
    
    // ----------------Mesh management----------------

    /** 
     * @brief Retrieves a mesh by name.
     * 
     * @param name The unique name of the mesh
     * @return Ref<Mesh> The requested mesh, or nullptr if not found
     */
    [[nodiscard]] static Ref<Mesh> GetMesh(const std::string& name);

    /** 
     * @brief Registers a mesh with a unique name.
     * 
     * @param name The unique name to associate with the mesh
     * @param mesh The mesh to register
     */
    static void RegisterMesh(const std::string& name, const Ref<Mesh>& mesh);

    /** 
     * @brief Unloads a mesh by name.
     * @param name The unique name of the mesh to unload
     */
    static void UnloadMesh(const std::string& name);
    
    // ----------------Cleanup----------------

    /** 
     * @brief Unloads all resources and clears internal storage.
     * 
     * Should be called at engine shutdown to free all loaded resources.
     */
    static void Shutdown();
    
    // ----------------Statistics----------------

    /** 
     * @brief Gets the count of currently loaded shaders, textures, and meshes.
     * 
     * @return size_t Number of loaded shaders
     * @return size_t Number of loaded textures
     * @return size_t Number of loaded meshes
     */
    static size_t GetShaderCount() { return s_Shaders.size(); }

    /** 
     * @brief Gets the count of currently loaded textures.
     * 
     * @return size_t Number of loaded textures
     */
    static size_t GetTextureCount() { return s_Textures.size(); }

    /** 
     * @brief Gets the count of currently loaded meshes.
     * 
     * @return size_t Number of loaded meshes
     */
    static size_t GetMeshCount() { return s_Meshes.size(); }

private:
// ----------------Static resource storage----------------
    /** 
     * @var s_Shaders
     * @brief Map of loaded shaders identified by unique names.
     */
    static std::unordered_map<std::string, Ref<Shader>> s_Shaders;

    /** 
     * @var s_Textures
     * @brief Map of loaded textures identified by unique names.
     */
    static std::unordered_map<std::string, Ref<Texture>> s_Textures;

    /** 
     * @var s_Meshes
     * @brief Map of loaded meshes identified by unique names.
     */
    static std::unordered_map<std::string, Ref<Mesh>> s_Meshes;
};

