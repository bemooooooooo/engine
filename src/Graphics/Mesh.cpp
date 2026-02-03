#include "Mesh.h"

#include "../Core/Log.h"
#include "../Math/Matrix3.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
    : m_IndexCount(indices.size()) {
  // Create vertex buffer
  Ref<VertexBuffer> vertexBuffer = CreateRef<VertexBuffer>(
      vertices.data(), vertices.size() * sizeof(Vertex));

  // Create layout
  VertexBufferLayout layout;
  layout.Push<f32>(3);  // position
  layout.Push<f32>(3);  // normal
  layout.Push<f32>(2);  // texCoord
  layout.Push<f32>(3);  // tangent
  layout.Push<f32>(3);  // bitangent

  // Create vertex array
  m_VertexArray = CreateRef<VertexArray>();
  m_VertexArray->AddVertexBuffer(vertexBuffer, layout);

  // Create index buffer
  Ref<IndexBuffer> indexBuffer =
      CreateRef<IndexBuffer>(indices.data(), indices.size());
  m_VertexArray->SetIndexBuffer(indexBuffer);

  // Create default material
  m_Material = CreateRef<Material>();

  ENGINE_INFO << "Mesh created with " << vertices.size() << " vertices, "
              << indices.size() << " indices";
}

Mesh::~Mesh() { ENGINE_INFO << "Mesh destroyed"; }

void Mesh::Draw(const Ref<Shader>& shader, const Matrix4& transform) const {
  if (!shader) {
    ENGINE_ERROR << "Mesh::Draw: shader is null!";
    return;
  }
  
  if (m_Material) {
    m_Material->Apply(shader);
  }

  shader->SetMatrix4("u_Model", transform);
  
  // Вычисляем нормальную матрицу на CPU (оптимизация)
  Matrix3 normalMatrix = transform.GetNormalMatrix();
  shader->SetMatrix3("u_NormalMatrix", normalMatrix);
  
  if (!m_VertexArray) {
    ENGINE_ERROR << "Mesh::Draw: vertex array is null!";
    return;
  }
  
  m_VertexArray->Bind();
  Renderer::Submit(shader, m_VertexArray, transform);
}

Ref<Mesh> Mesh::CreateCube() {
    std::vector<Vertex> vertices = {
        // Front face
        { Vector3(-0.5f, -0.5f,  0.5f), Vector3(0, 0, 1), Vector2(0, 0) },
        { Vector3( 0.5f, -0.5f,  0.5f), Vector3(0, 0, 1), Vector2(1, 0) },
        { Vector3( 0.5f,  0.5f,  0.5f), Vector3(0, 0, 1), Vector2(1, 1) },
        { Vector3(-0.5f,  0.5f,  0.5f), Vector3(0, 0, 1), Vector2(0, 1) },
        
        // Back face
        { Vector3(-0.5f, -0.5f, -0.5f), Vector3(0, 0, -1), Vector2(1, 0) },
        { Vector3(-0.5f,  0.5f, -0.5f), Vector3(0, 0, -1), Vector2(1, 1) },
        { Vector3( 0.5f,  0.5f, -0.5f), Vector3(0, 0, -1), Vector2(0, 1) },
        { Vector3( 0.5f, -0.5f, -0.5f), Vector3(0, 0, -1), Vector2(0, 0) },
        
        // Top face
        { Vector3(-0.5f,  0.5f, -0.5f), Vector3(0, 1, 0), Vector2(0, 1) },
        { Vector3(-0.5f,  0.5f,  0.5f), Vector3(0, 1, 0), Vector2(0, 0) },
        { Vector3( 0.5f,  0.5f,  0.5f), Vector3(0, 1, 0), Vector2(1, 0) },
        { Vector3( 0.5f,  0.5f, -0.5f), Vector3(0, 1, 0), Vector2(1, 1) },
        
        // Bottom face
        { Vector3(-0.5f, -0.5f, -0.5f), Vector3(0, -1, 0), Vector2(1, 1) },
        { Vector3( 0.5f, -0.5f, -0.5f), Vector3(0, -1, 0), Vector2(0, 1) },
        { Vector3( 0.5f, -0.5f,  0.5f), Vector3(0, -1, 0), Vector2(0, 0) },
        { Vector3(-0.5f, -0.5f,  0.5f), Vector3(0, -1, 0), Vector2(1, 0) },
        
        // Right face
        { Vector3(0.5f, -0.5f, -0.5f), Vector3(1, 0, 0), Vector2(1, 0) },
        { Vector3(0.5f,  0.5f, -0.5f), Vector3(1, 0, 0), Vector2(1, 1) },
        { Vector3(0.5f,  0.5f,  0.5f), Vector3(1, 0, 0), Vector2(0, 1) },
        { Vector3(0.5f, -0.5f,  0.5f), Vector3(1, 0, 0), Vector2(0, 0) },
        
        // Left face
        { Vector3(-0.5f, -0.5f, -0.5f), Vector3(-1, 0, 0), Vector2(0, 0) },
        { Vector3(-0.5f, -0.5f,  0.5f), Vector3(-1, 0, 0), Vector2(1, 0) },
        { Vector3(-0.5f,  0.5f,  0.5f), Vector3(-1, 0, 0), Vector2(1, 1) },
        { Vector3(-0.5f,  0.5f, -0.5f), Vector3(-1, 0, 0), Vector2(0, 1) }
    };
    
    std::vector<u32> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Top face
        8, 9, 10, 10, 11, 8,
        // Bottom face
        12, 13, 14, 14, 15, 12,
        // Right face
        16, 17, 18, 18, 19, 16,
        // Left face
        20, 21, 22, 22, 23, 20
    };
    
    return CreateRef<Mesh>(vertices, indices);
}

Ref<Mesh> Mesh::CreatePlane() {
  std::vector<Vertex> vertices = {
      {Vector3(-0.5f, 0.0f, -0.5f), Vector3(0, 1, 0), Vector2(0, 0)},
      {Vector3(0.5f, 0.0f, -0.5f), Vector3(0, 1, 0), Vector2(1, 0)},
      {Vector3(0.5f, 0.0f, 0.5f), Vector3(0, 1, 0), Vector2(1, 1)},
      {Vector3(-0.5f, 0.0f, 0.5f), Vector3(0, 1, 0), Vector2(0, 1)}};

  std::vector<u32> indices = {0, 1, 2, 2, 3, 0};

  return CreateRef<Mesh>(vertices, indices);
}

Ref<Mesh> Mesh::CreateSphere(u32 segments) {
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    
    segments = Math::Max(segments, 3u);
    
    // Generate vertices
    for (u32 y = 0; y <= segments; y++) {
        for (u32 x = 0; x <= segments; x++) {
            f32 xSegment = (f32)x / (f32)segments;
            f32 ySegment = (f32)y / (f32)segments;
            
            f32 xPos = std::cos(xSegment * 2.0f * Math::PI) * std::sin(ySegment * Math::PI);
            f32 yPos = std::cos(ySegment * Math::PI);
            f32 zPos = std::sin(xSegment * 2.0f * Math::PI) * std::sin(ySegment * Math::PI);
            
            Vertex vertex;
            vertex.position = Vector3(xPos, yPos, zPos) * 0.5f;
            vertex.normal = Vector3(xPos, yPos, zPos).Normalized();
            vertex.texCoord = Vector2(xSegment, ySegment);
            
            vertices.push_back(vertex);
        }
    }
    
    // Generate indices
    for (u32 y = 0; y < segments; y++) {
        for (u32 x = 0; x < segments; x++) {
            u32 first = (y * (segments + 1)) + x;
            u32 second = first + segments + 1;
            
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    
    // Calculate tangents and bitangents
    for (size_t i = 0; i < indices.size(); i += 3) {
        u32 i0 = indices[i];
        u32 i1 = indices[i + 1];
        u32 i2 = indices[i + 2];
        
        Vertex& v0 = vertices[i0];
        Vertex& v1 = vertices[i1];
        Vertex& v2 = vertices[i2];
        
        Vector3 edge1 = v1.position - v0.position;
        Vector3 edge2 = v2.position - v0.position;
        
        Vector2 deltaUV1 = v1.texCoord - v0.texCoord;
        Vector2 deltaUV2 = v2.texCoord - v0.texCoord;
        
        f32 f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        Vector3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = tangent.Normalized();
        
        Vector3 bitangent;
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = bitangent.Normalized();
        
        v0.tangent = tangent;
        v1.tangent = tangent;
        v2.tangent = tangent;
        
        v0.bitangent = bitangent;
        v1.bitangent = bitangent;
        v2.bitangent = bitangent;
    }
    
    return CreateRef<Mesh>(vertices, indices);
}