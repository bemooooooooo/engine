#pragma once
#include "../Core/Base.h"
#include "../Core/Log.h"

/**
 * @class Texture
 * @brief Represents a 2D texture resource in OpenGL.
 * 
 * This class encapsulates the creation, management, and usage of 2D textures
 * in OpenGL. It provides methods to load textures from image files, create
 * empty textures, bind/unbind them for rendering, and set texture data.
 * 
 * Key features:
 * - Load textures from image files (e.g., PNG, JPEG)
 * - Create empty textures with specified width and height
 * - Bind and unbind textures to texture units
 * - Set texture data dynamically
 * 
 * @note Requires a valid OpenGL context to function properly.
 * @note All OpenGL calls are made through this class; direct OpenGL calls should be avoided.
 * 
 * @example
 * @code
 * // Create a texture from file
 * Ref<Texture> texture = Texture::Create("path/to/texture.png");
 * 
 * // Create an empty texture
 * Ref<Texture> emptyTexture = Texture::Create(512, 512);
 * 
 * // Bind the texture for use
 * texture->Bind(0);  // Bind to texture unit 0
 * 
 * // Set texture data
 * void* data = ...; // Pointer to pixel data
 * texture->SetData(data, width * height * 4); // Assuming RGBA format
 * 
 * // Unbind the texture
 * texture->Unbind();
 * @endcode
 */
class Texture {
public:
    /** 
     * @brief Constructs a Texture with the given width and height.
     * 
     * Creates an empty texture with the specified dimensions.
     * 
     * @param width The width of the texture in pixels
     * @param height The height of the texture in pixels
     */
    Texture(u32 width, u32 height);

    /** 
     * @brief Constructs a Texture by loading from the specified file path.
     * 
     * Loads the texture image from the given file and creates the OpenGL texture.
     * 
     * @param filepath The path to the image file to load
     */
    Texture(const std::string& filepath);

    /** 
     * @brief Destructor that cleans up the OpenGL texture resource.
     */
    ~Texture();
    
    /** 
     * @brief Binds the texture to the specified texture unit.
     * 
     * @param slot The texture unit slot to bind to (default is 0)
     */
    void Bind(u32 slot = 0) const;

    /** 
     * @brief Unbinds the texture from the current texture unit.
     */
    void Unbind() const;
    
    /** 
     * @brief Sets the texture data from a raw data pointer.
     * 
     * Uploads the provided pixel data to the texture.
     * 
     * @param data Pointer to the pixel data
     * @param size Size of the data in bytes
     */
    void SetData(void* data, u32 size);
    
    /** 
     * @brief Gets the width of the texture in pixels.
     * 
     * @return u32 The width of the texture
     */
    u32 GetWidth() const { return m_Width; }

    /** 
     * @brief Gets the height of the texture in pixels.
     * 
     * @return u32 The height of the texture
     */
    u32 GetHeight() const { return m_Height; }

    /** 
     * @brief Gets the OpenGL renderer ID of the texture.
     * 
     * @return u32 The OpenGL renderer ID
     */
    [[nodiscard]] u32 GetRendererID() const { return m_RendererID; }
    
    /** 
     * @brief Checks if the texture was successfully loaded.
     * 
     * @return true If the texture is loaded and valid
     * @return false If the texture failed to load
     */
    bool IsLoaded() const { return m_IsLoaded; }
    
    /** 
     * @brief Factory method to create a Texture.
     * 
     * Overloaded to create a texture either from dimensions or from a file path.
     * 
     * @param width The width of the texture in pixels
     * @param height The height of the texture in pixels
     * @return Ref<Texture> The created empty texture
     * 
     * @param filepath The path to the image file to load
     * @return Ref<Texture> The created texture loaded from file
     */
    static Ref<Texture> Create(u32 width, u32 height);

    /** 
     * @brief Factory method to create a Texture from a file.
     * 
     * @param filepath The path to the image file to load
     * @return Ref<Texture> The created texture loaded from file
     */
    static Ref<Texture> Create(const std::string& filepath);

private:
    /** 
     * @brief Internal method to create the OpenGL texture resource.
     */
    void CreateInternal();
    
    /** 
     * @var m_RendererID
     * @brief The OpenGL renderer ID for the texture.
     */
    u32 m_RendererID;

    /** 
     * @var m_Width
     * @brief The width of the texture in pixels.
     */
    u32 m_Width, m_Height;

    /** 
     * @var m_InternalFormat
     * @brief The internal format of the texture (e.g., GL_RGBA8).
     * @var m_DataFormat
     * @brief The data format of the texture (e.g., GL_RGBA).
     */
    u32 m_InternalFormat, m_DataFormat;

    /** 
     * @var m_Filepath
     * @brief The file path from which the texture was loaded.
     */
    std::string m_Filepath;

    /** 
     * @var m_IsLoaded
     * @brief Flag indicating whether the texture was successfully loaded.
     */
    bool m_IsLoaded = false;
};