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

