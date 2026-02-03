#include "Material.h"

#include "../Core/Log.h"

Material::Material() {
  // Create default PBR shader
  const char* defaultVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 a_Position;
        layout (location = 1) in vec3 a_Normal;
        layout (location = 2) in vec2 a_TexCoord;
        layout (location = 3) in vec3 a_Tangent;
        layout (location = 4) in vec3 a_Bitangent;

        uniform mat4 u_Model;
        uniform mat4 u_ViewProjection;
        uniform mat3 u_NormalMatrix;  // Предвычисленная нормальная матрица

        out vec3 v_FragPos;
        out vec3 v_Normal;
        out vec2 v_TexCoord;
        out mat3 v_TBN;

        void main() {
            vec4 worldPos = u_Model * vec4(a_Position, 1.0);
            v_FragPos = worldPos.xyz;
            v_Normal = normalize(u_NormalMatrix * a_Normal);
            v_TexCoord = a_TexCoord;
            
            // TBN matrix для normal mapping
            vec3 T = normalize(mat3(u_Model) * a_Tangent);
            vec3 B = normalize(mat3(u_Model) * a_Bitangent);
            vec3 N = v_Normal;
            v_TBN = mat3(T, B, N);
            
            gl_Position = u_ViewProjection * worldPos;
        }
    )";

  const char* defaultFragmentShader = R"(
        #version 330 core

        struct Material {
            vec4 albedo;
            float metallic;
            float roughness;
            float ambientOcclusion;
        };

        in vec3 v_FragPos;
        in vec3 v_Normal;
        in vec2 v_TexCoord;
        in mat3 v_TBN;

        uniform Material u_Material;
        uniform vec3 u_ViewPos;
        uniform bool u_Wireframe;

        // Texture maps
        uniform sampler2D u_AlbedoMap;
        uniform sampler2D u_NormalMap;
        uniform sampler2D u_MetallicMap;
        uniform sampler2D u_RoughnessMap;
        uniform sampler2D u_AmbientOcclusionMap;

        // Texture flags
        uniform int u_HasAlbedoMap;
        uniform int u_HasNormalMap;
        uniform int u_HasMetallicMap;
        uniform int u_HasRoughnessMap;
        uniform int u_HasAOMap;

        out vec4 FragColor;

        void main() {
            if (u_Wireframe) {
                FragColor = vec4(1.0);
                return;
            }
            
            // Загружаем материал из текстур или используем значения по умолчанию
            vec4 albedo = u_HasAlbedoMap != 0 ? texture(u_AlbedoMap, v_TexCoord) : u_Material.albedo;
            float metallic = u_HasMetallicMap != 0 ? texture(u_MetallicMap, v_TexCoord).r : u_Material.metallic;
            float roughness = u_HasRoughnessMap != 0 ? texture(u_RoughnessMap, v_TexCoord).r : u_Material.roughness;
            float ambientOcclusion = u_HasAOMap != 0 ? texture(u_AmbientOcclusionMap, v_TexCoord).r : u_Material.ambientOcclusion;
            
            // Загружаем нормаль из текстуры или используем интерполированную нормаль
            vec3 normal = u_HasNormalMap != 0 ? 
                normalize(v_TBN * (texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0)) :
                normalize(v_Normal);
            
            vec3 lightPos = vec3(5.0, 5.0, 5.0);
            vec3 lightDir = normalize(lightPos - v_FragPos);
            vec3 viewDir = normalize(u_ViewPos - v_FragPos);
            
            // Ambient
            vec3 ambient = vec3(0.2) * albedo.rgb * ambientOcclusion;
            
            // Diffuse
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * albedo.rgb;
            
            // Specular
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0 * (1.0 - roughness));
            vec3 specular = spec * vec3(0.5) * metallic;
            
            // Rim lighting для подчеркивания краев
            float rim = 1.0 - max(dot(viewDir, normal), 0.0);
            rim = pow(rim, 3.0);
            vec3 rimLight = rim * vec3(0.3);
            
            vec3 result = ambient + diffuse + specular + rimLight;
            
            // Гамма-коррекция
            result = pow(result, vec3(1.0/2.2));
            
            FragColor = vec4(result, albedo.a);
        }
    )";

  m_Shader = Shader::Create(defaultVertexShader, defaultFragmentShader);
}

Material::Material(const Ref<Shader>& shader) : m_Shader(shader) {}

Material::Material(const Ref<Shader>& shader,
                   const MaterialProperties& properties)
    : m_Shader(shader), m_Properties(properties) {}

void Material::Apply(const Ref<Shader>& shader) const {
  // Set material properties
  // В GLSL нельзя устанавливать поля структуры через точечную нотацию
  // Используем отдельные униформы или устанавливаем всю структуру
  shader->SetVector4("u_Material.albedo", m_Properties.albedo);
  shader->SetFloat("u_Material.metallic", m_Properties.metallic);
  shader->SetFloat("u_Material.roughness", m_Properties.roughness);
  shader->SetFloat("u_Material.ambientOcclusion",
                   m_Properties.ambientOcclusion);

  // Альтернативный способ - установка через отдельные униформы (если шейдер их
  // поддерживает) Это более надежно, так как некоторые драйверы могут не
  // поддерживать точечную нотацию

  // Set texture uniforms and bind textures
  u32 textureSlot = 0;

  if (m_Properties.albedoMap) {
    shader->SetInt("u_HasAlbedoMap", 1);
    m_Properties.albedoMap->Bind(textureSlot);
    shader->SetInt("u_AlbedoMap", textureSlot);
    textureSlot++;
  } else {
    shader->SetInt("u_HasAlbedoMap", 0);
  }

  if (m_Properties.normalMap) {
    shader->SetInt("u_HasNormalMap", 1);
    m_Properties.normalMap->Bind(textureSlot);
    shader->SetInt("u_NormalMap", textureSlot);
    textureSlot++;
  } else {
    shader->SetInt("u_HasNormalMap", 0);
  }

  if (m_Properties.metallicMap) {
    shader->SetInt("u_HasMetallicMap", 1);
    m_Properties.metallicMap->Bind(textureSlot);
    shader->SetInt("u_MetallicMap", textureSlot);
    textureSlot++;
  } else {
    shader->SetInt("u_HasMetallicMap", 0);
  }

  if (m_Properties.roughnessMap) {
    shader->SetInt("u_HasRoughnessMap", 1);
    m_Properties.roughnessMap->Bind(textureSlot);
    shader->SetInt("u_RoughnessMap", textureSlot);
    textureSlot++;
  } else {
    shader->SetInt("u_HasRoughnessMap", 0);
  }

  if (m_Properties.ambientOcclusionMap) {
    shader->SetInt("u_HasAOMap", 1);
    m_Properties.ambientOcclusionMap->Bind(textureSlot);
    shader->SetInt("u_AmbientOcclusionMap", textureSlot);
    textureSlot++;
  } else {
    shader->SetInt("u_HasAOMap", 0);
  }
}
