#include "Texture.h"
#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(u32 width, u32 height) 
    : m_Width(width), m_Height(height) {
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;
    CreateInternal();
}

Texture::Texture(const std::string& filepath) 
    : m_Filepath(filepath) {
    stbi_set_flip_vertically_on_load(1);
    
    i32 width, height, channels;
    stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        ENGINE_ERROR << "Failed to load texture: " << filepath;
        return;
    }
    
    m_Width = width;
    m_Height = height;
    
    if (channels == 4) {
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;
    } else if (channels == 3) {
        m_InternalFormat = GL_RGB8;
        m_DataFormat = GL_RGB;
    }
    
    CreateInternal();
    
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, 
                 m_DataFormat, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    m_IsLoaded = true;
    
    ENGINE_INFO << "Texture loaded: " << filepath << " (" << m_Width << "x" << m_Height << ")";
}

Texture::~Texture() {
    glDeleteTextures(1, &m_RendererID);
}

void Texture::CreateInternal() {
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::Bind(u32 slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetData(void* data, u32 size) {
    u32 bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    if (size != m_Width * m_Height * bpp) {
        ENGINE_ERROR << "Data size must match texture dimensions!";
        return;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, 
                 m_DataFormat, GL_UNSIGNED_BYTE, data);
}

Ref<Texture> Texture::Create(u32 width, u32 height) {
    return CreateRef<Texture>(width, height);
}

Ref<Texture> Texture::Create(const std::string& filepath) {
    return CreateRef<Texture>(filepath);
}