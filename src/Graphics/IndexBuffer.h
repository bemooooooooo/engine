#pragma once
#include "../Core/Base.h"

/**
 * @class IndexBuffer
 * @brief Represents an OpenGL index buffer for indexed drawing.
 * 
 * The IndexBuffer class encapsulates the creation, binding, and management
 * of an OpenGL index buffer object (IBO). It allows for efficient rendering
 * of complex geometries by reusing vertex data through indexed drawing.
 * 
 * Key features:
 * - Creation of index buffer from an array of indices
 * - Binding and unbinding the index buffer for rendering
 * - Retrieval of the number of indices stored
 * 
 * Usage:
 * - Create an IndexBuffer with a list of indices
 * - Bind the buffer before issuing draw calls
 * - Unbind when done to avoid unintended state changes
 * 
 * @note This class assumes that an OpenGL context is active when used.
 * @note The indices are expected to be of type u32 (unsigned 32-bit integers).
 * 
 * @example
 * @code
 * u32 indices[] = {0, 1, 2, 2, 3, 0};
 * IndexBuffer ibo(indices, 6);
 * 
 * ibo.Bind();
 * GraphicsContext::DrawElements(ibo.GetCount());
 * ibo.Unbind();
 * @endcode
 */
class IndexBuffer {
public:
    /**
     * @brief Constructs an IndexBuffer from an array of indices.
     * 
     * Creates an OpenGL index buffer object and uploads the provided
     * indices to the GPU for use in indexed drawing.
     * 
     * @param indices Pointer to an array of u32 indices
     * @param count Number of indices in the array
     */
    IndexBuffer(const u32* indices, u32 count);

    /**
     * @brief Destructor that cleans up the OpenGL index buffer.
     * 
     * Deletes the OpenGL buffer object associated with this IndexBuffer
     * to free GPU resources.
     */
    ~IndexBuffer();
    
    /**
     * @brief Binds the index buffer for rendering.
     * 
     * Sets this index buffer as the current buffer to be used for
     * indexed drawing operations.
     */
    void Bind() const;

    /**
     * @brief Unbinds the index buffer.
     * 
     * Resets the current index buffer binding to zero, effectively
     * unbinding any index buffer that was previously bound.
     */
    void Unbind() const;
    
    /**
     * @brief Gets the number of indices stored in the buffer.
     * 
     * Returns the count of indices that were uploaded to the GPU
     * when the IndexBuffer was created.
     * 
     * @return u32 The number of indices
     */
    u32 GetCount() const { return m_Count; }

    /**
     * @brief Gets the OpenGL renderer ID for this index buffer.
     * 
     * Returns the internal OpenGL buffer object ID used for binding
     * and rendering.
     * 
     * @return u32 The OpenGL buffer object ID
     */
    u32 GetRendererID() const { return m_RendererID; }

private:
    /**
     * @var m_RendererID
     * @brief The OpenGL buffer object ID for this index buffer.
     */
    u32 m_RendererID;
    /**
     * @var m_Count
     * @brief The number of indices stored in this index buffer.
     */
    u32 m_Count;
};