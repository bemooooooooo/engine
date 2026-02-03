#include "ResourceManager.h"
#include "../Core/Log.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

std::unordered_map<std::string, Ref<Shader>> ResourceManager::s_Shaders;
std::unordered_map<std::string, Ref<Texture>> ResourceManager::s_Textures;
std::unordered_map<std::string, Ref<Mesh>> ResourceManager::s_Meshes;

Ref<Shader> ResourceManager::GetShader(const std::string& name) {
    auto it = s_Shaders.find(name);
    if (it != s_Shaders.end()) {
        return it->second;
    }
    ENGINE_WARN << "Shader '" << name << "' not found in ResourceManager";
    return nullptr;
}

Ref<Shader> ResourceManager::LoadShader(const std::string& name, 
                                        const std::string& vertexPath, 
                                        const std::string& fragmentPath) {
    // Проверяем, не загружен ли уже
    if (s_Shaders.find(name) != s_Shaders.end()) {
        ENGINE_WARN << "Shader '" << name << "' already loaded, returning existing";
        return s_Shaders[name];
    }
    
    auto shader = Shader::CreateFromFiles(vertexPath, fragmentPath);
    if (shader) {
        s_Shaders[name] = shader;
        ENGINE_INFO << "Shader '" << name << "' loaded successfully";
    } else {
        ENGINE_ERROR << "Failed to load shader '" << name << "'";
    }
    
    return shader;
}

Ref<Shader> ResourceManager::LoadShaderFromSource(const std::string& name,
                                                   const std::string& vertexSrc,
                                                   const std::string& fragmentSrc) {
    // Проверяем, не загружен ли уже
    if (s_Shaders.find(name) != s_Shaders.end()) {
        ENGINE_WARN << "Shader '" << name << "' already loaded, returning existing";
        return s_Shaders[name];
    }
    
    auto shader = Shader::Create(vertexSrc, fragmentSrc);
    if (shader) {
        s_Shaders[name] = shader;
        ENGINE_INFO << "Shader '" << name << "' loaded successfully";
    } else {
        ENGINE_ERROR << "Failed to load shader '" << name << "'";
    }
    
    return shader;
}

void ResourceManager::UnloadShader(const std::string& name) {
    auto it = s_Shaders.find(name);
    if (it != s_Shaders.end()) {
        s_Shaders.erase(it);
        ENGINE_INFO << "Shader '" << name << "' unloaded";
    } else {
        ENGINE_WARN << "Shader '" << name << "' not found for unloading";
    }
}

Ref<Texture> ResourceManager::GetTexture(const std::string& name) {
    auto it = s_Textures.find(name);
    if (it != s_Textures.end()) {
        return it->second;
    }
    ENGINE_WARN << "Texture '" << name << "' not found in ResourceManager";
    return nullptr;
}

Ref<Texture> ResourceManager::LoadTexture(const std::string& name, const std::string& path) {
    // Проверяем, не загружена ли уже
    if (s_Textures.find(name) != s_Textures.end()) {
        ENGINE_WARN << "Texture '" << name << "' already loaded, returning existing";
        return s_Textures[name];
    }
    
    auto texture = Texture::Create(path);
    if (texture) {
        s_Textures[name] = texture;
        ENGINE_INFO << "Texture '" << name << "' loaded successfully";
    } else {
        ENGINE_ERROR << "Failed to load texture '" << name << "' from " << path;
    }
    
    return texture;
}

void ResourceManager::UnloadTexture(const std::string& name) {
    auto it = s_Textures.find(name);
    if (it != s_Textures.end()) {
        s_Textures.erase(it);
        ENGINE_INFO << "Texture '" << name << "' unloaded";
    } else {
        ENGINE_WARN << "Texture '" << name << "' not found for unloading";
    }
}

Ref<Mesh> ResourceManager::GetMesh(const std::string& name) {
    auto it = s_Meshes.find(name);
    if (it != s_Meshes.end()) {
        return it->second;
    }
    ENGINE_WARN << "Mesh '" << name << "' not found in ResourceManager";
    return nullptr;
}

void ResourceManager::RegisterMesh(const std::string& name, const Ref<Mesh>& mesh) {
    if (s_Meshes.find(name) != s_Meshes.end()) {
        ENGINE_WARN << "Mesh '" << name << "' already registered, overwriting";
    }
    s_Meshes[name] = mesh;
    ENGINE_INFO << "Mesh '" << name << "' registered";
}

void ResourceManager::UnloadMesh(const std::string& name) {
    auto it = s_Meshes.find(name);
    if (it != s_Meshes.end()) {
        s_Meshes.erase(it);
        ENGINE_INFO << "Mesh '" << name << "' unloaded";
    } else {
        ENGINE_WARN << "Mesh '" << name << "' not found for unloading";
    }
}

void ResourceManager::Shutdown() {
    ENGINE_INFO << "ResourceManager shutdown - Shaders: " << s_Shaders.size() 
                << ", Textures: " << s_Textures.size() 
                << ", Meshes: " << s_Meshes.size();
    s_Shaders.clear();
    s_Textures.clear();
    s_Meshes.clear();
}

