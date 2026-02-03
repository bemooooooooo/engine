#include "VertexArray.h"

#include <glad/gl.h>

VertexArray::VertexArray() { glGenVertexArrays(1, &m_RendererID); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

void VertexArray::Bind() const { glBindVertexArray(m_RendererID); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer,
                                  const VertexBufferLayout& layout) {
  Bind();
  vertexBuffer->Bind();

  const auto& elements = layout.GetElements();
  u64 offset = 0;

  for (u32 i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];
    glEnableVertexAttribArray(m_VertexBufferIndex);
    glVertexAttribPointer(m_VertexBufferIndex, element.count, element.type,
                          element.normalized, layout.GetStride(),
                          (const void*)offset);
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    m_VertexBufferIndex++;
  }

  m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
  Bind();
  indexBuffer->Bind();
  m_IndexBuffer = indexBuffer;
}