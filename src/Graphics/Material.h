#pragma once
#include "../Core/Base.h"
#include "Texture.h"
#include "Shader.h"
#include "../Math/Vector4.h"

/**
 * @struct MaterialProperties
 * @brief Encapsulates the properties and texture maps of a material.
 * 
 * This structure holds the various properties that define the appearance
 * of a material in the rendering engine, including color, metallicity,
 * roughness, ambient occlusion, and associated texture maps.
 * 
 * Properties:
 * - albedo: Base color of the material
 * - metallic: Metallic factor (0.0 = non-metal, 1.0 = metal)
 * - roughness: Surface roughness (0.0 = smooth, 1.0 = rough)
 * - ambientOcclusion: Ambient occlusion factor
 * 
 * Texture Maps:
 * - albedoMap: Texture for base color
 * - normalMap: Texture for surface normals
 * - metallicMap: Texture for metallic values
 * - roughnessMap: Texture for roughness values
 * - ambientOcclusionMap: Texture for ambient occlusion values
 * 
 * @example
 * @code
 * MaterialProperties props;
 * props.albedo = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // Red color
 * props.metallic = 0.5f;
 * props.roughness = 0.2f;
 * props.albedoMap = CreateRef<Texture>("textures/wood_albedo.png");
 * @endcode
 */
struct MaterialProperties {
    // Default material properties
    Vector4 albedo = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    f32 metallic = 0.0f;
    f32 roughness = 0.5f;
    f32 ambientOcclusion = 1.0f;
    
    // Texture maps
    Ref<Texture> albedoMap;
    Ref<Texture> normalMap;
    Ref<Texture> metallicMap;
    Ref<Texture> roughnessMap;
    Ref<Texture> ambientOcclusionMap;
};

/**
 * @class Material
 * @brief Represents a material with properties and associated shader.
 * 
 * The Material class encapsulates the visual properties of a surface,
 * including its shader and material properties such as color, metallicity,
 * roughness, and texture maps. It provides methods to apply these properties
 * to a shader for rendering.
 * 
 * Key features:
 * - Holds a reference to a Shader used for rendering
 * - Contains MaterialProperties defining appearance
 * - Method to apply properties to the shader uniforms
 * 
 * @example
 * @code
 * Ref<Shader> shader = Shader::CreateFromFiles("vertex.glsl", "fragment.glsl");
 * Ref<Material> material = Material::Create(shader);
 * 
 * material->GetProperties().albedo = Vector4(0.8f, 0.2f, 0.2f, 1.0f); // Reddish color
 * material->GetProperties().metallic = 0.9f;
 * 
 * // In render loop
 * material->Apply(shader);
 * @endcode
 */
class Material {
public:

    /**
     * @brief Constructs a Material with default properties and no shader.
     */
    Material();

    /**
     * @brief Constructs a Material with the specified shader.
     * 
     * @param shader Reference to the Shader to be used by this material
     */
    Material(const Ref<Shader>& shader);

    /**
     * @brief Constructs a Material with the specified shader and properties.
     * 
     * @param shader Reference to the Shader to be used by this material
     * @param properties Material properties to initialize with
     */
    Material(const Ref<Shader>& shader, const MaterialProperties& properties);
    
    /**
     * @brief Applies the material properties to the given shader.
     * 
     * Sets the shader uniforms based on the material's properties and
     * binds any associated texture maps.
     * 
     * @param shader The shader to which the material properties will be applied
     */
    void Apply(const Ref<Shader>& shader) const;
    
    /**
     * @brief Sets the shader used by this material.
     * 
     * @param shader Reference to the Shader to set
     */
    void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }

    /**
     * @brief Gets the shader used by this material.
     * 
     * @return Ref<Shader> Reference to the current Shader
     */
    Ref<Shader> GetShader() const { return m_Shader; }
    
    /**
     * @brief Gets the material properties.
     * 
     * @return MaterialProperties& Reference to the material properties
     */
    MaterialProperties& GetProperties() { return m_Properties; }

    /**
     * @brief Gets the material properties (const version).
     * 
     * @return const MaterialProperties& Const reference to the material properties
     */
    const MaterialProperties& GetProperties() const { return m_Properties; }
    
    /**
     * @brief Factory method to create a Material with default properties.
     * 
     * @return Ref<Material> Reference to the newly created Material
     */
    static Ref<Material> Create();

    /**
     * @brief Factory method to create a Material with the specified shader.
     * 
     * @param shader Reference to the Shader to be used by the new material
     * @return Ref<Material> Reference to the newly created Material
     */
    static Ref<Material> Create(const Ref<Shader>& shader);

private:
    /**
     * @var m_Shader
     * @brief The shader associated with this material.
     */
    Ref<Shader> m_Shader;

    /**
     * @var m_Properties
     * @brief The properties defining the material's appearance.
     */
    MaterialProperties m_Properties;
};
