#pragma once
#include "../Core/Base.h"

/**
 * @class VertexBuffer
 * @brief Encapsulates an OpenGL vertex buffer object (VBO).
 * 
 * Manages the creation, binding, and data management of a vertex buffer.
 */
class VertexBuffer {
public:
    /** 
     * @brief Constructs a VertexBuffer with the given data and size.
     * 
     * @param data Pointer to the vertex data
     * @param size Size of the vertex data in bytes
     */
    VertexBuffer(const void* data, u32 size);

    /** 
     * @brief Constructs a dynamic VertexBuffer with the given size.
     * 
     * @param size Size of the vertex buffer in bytes
     */
    VertexBuffer(u32 size);

    /** 
     * @brief Destructor that cleans up the OpenGL vertex buffer.
     */
    ~VertexBuffer();
    
    /** 
     * @brief Binds the vertex buffer for rendering.
     */
    void Bind() const;

    /** 
     * @brief Unbinds the vertex buffer.
     */
    void Unbind() const;
    
    /** 
     * @brief Sets the data of the vertex buffer.
     * 
     * @param data Pointer to the new vertex data
     * @param size Size of the data in bytes
     */
    void SetData(const void* data, u32 size);
    
    /** 
     * @brief Gets the OpenGL renderer ID of the vertex buffer.
     * 
     * @return u32 The OpenGL renderer ID
     */
    [[nodiscard]] u32 GetRendererID() const { return m_RendererID; }

private:
    /** 
     * @var m_RendererID
     * @brief The OpenGL renderer ID for the vertex buffer.
     */
    u32 m_RendererID;
};