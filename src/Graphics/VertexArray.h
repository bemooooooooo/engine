#pragma once
#include <glad/gl.h>

#include <vector>

#include "../Core/Base.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

/**
 * @class VertexBufferElement
 * @brief Represents a single element in a vertex buffer layout.
 *
 * Contains the type, count, and normalization flag for a vertex attribute.
 */
struct VertexBufferElement {
  /**
   * @var type
   * @brief The OpenGL data type of the vertex attribute (e.g., GL_FLOAT).
   */
  u32 type;

  /**
   * @var count
   * @brief The number of components in the vertex attribute (e.g., 3 for vec3).
   */
  u32 count;

  /**
   * @var normalized
   * @brief Whether the attribute data should be normalized (GL_TRUE or
   * GL_FALSE).
   */
  u8 normalized;

  /**
   * @brief Gets the size in bytes of the given OpenGL data type.
   *
   * @param type The OpenGL data type (e.g., GL_FLOAT)
   * @return u32 The size in bytes of the type
   */
  static u32 GetSizeOfType(u32 type) {
    switch (type) {
      case GL_FLOAT:
        return 4;
      case GL_UNSIGNED_INT:
        return 4;
      case GL_UNSIGNED_BYTE:
        return 1;
    }
    return 0;
  }
};

/**
 * @class VertexBufferLayout
 * @brief Defines the layout of vertex attributes in a vertex buffer.
 *
 * Manages a list of VertexBufferElement instances and calculates the stride
 * (total size) of the vertex structure.
 */
class VertexBufferLayout {
 public:
  /**
   * @brief Default constructor initializing an empty layout.
   */
  VertexBufferLayout() : m_Stride(0) {}

  /**
   * @brief Templated method to add a vertex attribute to the layout.
   *
   * Specializations exist for f32, u32, and u8 types.
   *
   * @tparam T The data type of the vertex attribute
   * @param count The number of components in the attribute
   */
  template <typename T>
  void Push(u32 count) {
    static_assert(false);
  }

  /**
   * @brief Gets the list of vertex buffer elements in the layout.
   *
   * @return const std::vector<VertexBufferElement>& The list of elements
   */
  const std::vector<VertexBufferElement>& GetElements() const {
    return m_Elements;
  }

  /**
   * @brief Gets the stride (total size) of the vertex structure in bytes.
   *
   * @return u32 The stride in bytes
   */
  u32 GetStride() const { return m_Stride; }

 private:
  /**
   * @var m_Elements
   * @brief The list of vertex buffer elements defining the layout.
   */
  std::vector<VertexBufferElement> m_Elements;

  /**
   * @var m_Stride
   * @brief The total size of the vertex structure in bytes.
   */
  u32 m_Stride;
};

/**
 * @brief Specialization for float (f32) vertex attributes.
 *
 * @param count The number of float components
 */
template <>
inline void VertexBufferLayout::Push<f32>(u32 count) {
  m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
  m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

/**
 * @brief Specialization for unsigned int (u32) vertex attributes.
 *
 * @param count The number of unsigned int components
 */
template <>
inline void VertexBufferLayout::Push<u32>(u32 count) {
  m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
  m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

/**
 * @brief Specialization for unsigned byte (u8) vertex attributes.
 *
 * @param count The number of unsigned byte components
 */
template <>
inline void VertexBufferLayout::Push<u8>(u32 count) {
  m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
  m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

/**
 * @class VertexArray
 * @brief Encapsulates an OpenGL vertex array object (VAO).
 *
 * Manages the binding of vertex buffers and index buffers for rendering.
 */
class VertexArray {
 public:
  /**
   * @brief Constructs a new VertexArray and generates the OpenGL VAO.
   */
  VertexArray();

  /**
   * @brief Destructor that cleans up the OpenGL VAO.
   */
  ~VertexArray();

  /**
   * @brief Binds the vertex array for rendering.
   */
  void Bind() const;

  /**
   * @brief Unbinds the vertex array.
   */
  void Unbind() const;

  /**
   * @brief Adds a vertex buffer to the vertex array with the specified layout.
   *
   * Configures the vertex attribute pointers based on the provided layout.
   *
   * @param vertexBuffer The vertex buffer to add
   * @param layout The layout describing the vertex attributes
   */
  void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer,
                       const VertexBufferLayout& layout);

  /**
   * @brief Sets the index buffer for the vertex array.
   *
   * @param indexBuffer The index buffer to set
   */
  void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

  /**
   * @brief Gets the OpenGL renderer ID of the vertex array.
   *
   * @return u32 The OpenGL renderer ID
   */
  const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const {
    return m_VertexBuffers;
  }

  /**
   * @brief Gets the index buffer associated with the vertex array.
   *
   * @return const Ref<IndexBuffer>& The index buffer
   */
  const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

 private:
  /**
   * @var m_RendererID
   * @brief The OpenGL renderer ID for the vertex array object.
   */
  u32 m_RendererID;

  /**
   * @var m_VertexBufferIndex
   * @brief The current index for vertex buffer attributes.
   */
  u32 m_VertexBufferIndex = 0;

  /**
   * @var m_VertexBuffers
   * @brief The list of vertex buffers associated with this vertex array.
   */
  std::vector<Ref<VertexBuffer>> m_VertexBuffers;

  /**
   * @var m_IndexBuffer
   * @brief The index buffer associated with this vertex array.
   */
  Ref<IndexBuffer> m_IndexBuffer;
};