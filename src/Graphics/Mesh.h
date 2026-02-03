#pragma once
#include "../Core/Base.h"
#include "VertexArray.h"
#include "Material.h"
#include "../Math/Matrix4.h"
#include "../Math/Vector2.h"
// #include "RenderCommand.h"
#include "Renderer.h"


/**
 * @struct Vertex
 * @brief Structure representing a single vertex in a mesh.
 * 
 * Contains position, normal, texture coordinates, tangent, and bitangent data.
 * Used for defining the geometry of 3D models.
 */
struct Vertex {
    /**
     * @var position
     * @brief The 3D position of the vertex.
     */
    Vector3 position;

    /**
     * @var normal
     * @brief The normal vector at the vertex, used for lighting calculations.
     */
    Vector3 normal;

    /**
     * @var texCoord
     * @brief The 2D texture coordinates (UV) for mapping textures to the vertex.
     */
    Vector2 texCoord;

    /**
     * @var tangent
     * @brief The tangent vector at the vertex, used for normal mapping.
     */
    Vector3 tangent;

    /**
     * @var bitangent
     * @brief The bitangent vector at the vertex, used for normal mapping.
     */
    Vector3 bitangent;
    
    /**
     * @brief Default constructor initializing vertex attributes.
     */
    Vertex() = default;

    /**
     * @brief Parameterized constructor to initialize vertex attributes.
     * 
     * @param pos The position of the vertex
     * @param norm The normal vector at the vertex (default is up vector)
     * @param uv The texture coordinates (default is zero vector)
     */
    Vertex(const Vector3& pos, const Vector3& norm = Vector3::Up, 
           const Vector2& uv = Vector2::Zero)
        : position(pos), normal(norm), texCoord(uv) {}
};

/**
 * @class Mesh
 * @brief Represents a 3D mesh with geometry and material.
 * 
 * Encapsulates vertex and index buffers, along with material properties.
 * Provides methods to draw the mesh using a specified shader and transformation.
 */
class Mesh {
public:
    /**
     * @brief Constructs a Mesh from given vertices and indices.
     * 
     * Creates vertex and index buffers, and sets up the vertex array object.
     * 
     * @param vertices The list of vertices defining the mesh geometry
     * @param indices The list of indices defining the mesh faces
     */
    Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices);
    /**
     * @brief Destructor for the Mesh class.
     */
    ~Mesh();
    
    /**
     * @brief Draws the mesh using the specified shader and transformation matrix.
     * 
     * Binds the material and vertex array, sets shader uniforms, and issues
     * the draw call to render the mesh.
     * 
     * @param shader The shader to use for rendering
     * @param transform The transformation matrix to apply to the mesh
     */
    void Draw(const Ref<Shader>& shader, const Matrix4& transform) const;
    
    /**
     * @brief Sets the material for the mesh.
     * 
     * @param material The material to associate with this mesh
     */
    void SetMaterial(const Ref<Material>& material) { m_Material = material; }

    /**
     * @brief Gets the material associated with the mesh.
     * 
     * @return Ref<Material> The material of the mesh
     */
    Ref<Material> GetMaterial() const { return m_Material; }
    
    /**
     * @brief Gets the vertex array of the mesh.
     * 
     * @return const Ref<VertexArray>& The vertex array object
     */
    const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
    
    /**
     * @brief Factory method to create a cube mesh.
     * 
     * @return Ref<Mesh> The created cube mesh
     */
    static Ref<Mesh> CreateCube();

    /**
     * @brief Factory method to create a plane mesh.
     * 
     * @return Ref<Mesh> The created plane mesh
     */
    static Ref<Mesh> CreatePlane();

    /**
     * @brief Factory method to create a sphere mesh.
     * 
     * @param segments The number of segments for the sphere (default is 32)
     * @return Ref<Mesh> The created sphere mesh
     */
    static Ref<Mesh> CreateSphere(u32 segments = 32);

private:
// Private members
    /**
     * @var m_VertexArray
     * @brief The vertex array object containing vertex and index buffers.
     */
    Ref<VertexArray> m_VertexArray;
    
    /**
     * @var m_Material
     * @brief The material associated with the mesh.
     */
    Ref<Material> m_Material;

    /**
     * @var m_IndexCount
     * @brief The number of indices in the mesh.
     */
    u32 m_IndexCount;
};