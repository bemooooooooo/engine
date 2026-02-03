# Шейдеры, материалы и текстуры в движке

Ниже описаны правила написания шейдеров для движка, а также то, как устроены материалы и текстуры на стороне C++.

## 1) Требования к вершинному шейдеру

Движок формирует `VertexBufferLayout` с таким порядком атрибутов (см. `src/Graphics/Mesh.cpp`):

1. `layout(location = 0) in vec3 a_Position;`
2. `layout(location = 1) in vec3 a_Normal;`
3. `layout(location = 2) in vec2 a_TexCoord;`
4. `layout(location = 3) in vec3 a_Tangent;`
5. `layout(location = 4) in vec3 a_Bitangent;`

Если ваш вершинный шейдер использует другой порядок или типы, это нужно синхронизировать с `VertexBufferLayout` в C++.

### Обязательные uniform

Движок выставляет следующие матрицы/данные (см. `src/Graphics/Renderer.cpp` и `src/Graphics/Mesh.cpp`):

- `uniform mat4 u_Model;` — модельная матрица.
- `uniform mat4 u_ViewProjection;` — матрица вида * проекции.
- `uniform mat3 u_NormalMatrix;` — нормальная матрица (вычисляется на CPU).

Если ваш вершинный шейдер не объявляет эти uniform под теми же именами, обновление из движка не подхватится.

### Рекомендуемые varying (выходы из VS)

Если вы используете стандартный фрагментный шейдер `assets/shaders/default.frag`, он ожидает:

- `out vec3 v_FragPos;`
- `out vec3 v_Normal;`
- `out vec2 v_TexCoord;`
- `out mat3 v_TBN;` — матрица для normal mapping.

## 2) Требования к фрагментному шейдеру

### Материал (uniform struct)

Движок выставляет значения из `MaterialProperties` через `Material::Apply`:

```
struct Material {
    vec4 albedo;
    float metallic;
    float roughness;
    float ambientOcclusion;
};
uniform Material u_Material;
```

Имена полей должны совпадать: `u_Material.albedo`, `u_Material.metallic`, `u_Material.roughness`, `u_Material.ambientOcclusion`.

### Текстуры и флаги

Шейдеры должны поддерживать следующие uniform-ы:

- `uniform sampler2D u_AlbedoMap;`
- `uniform sampler2D u_NormalMap;`
- `uniform sampler2D u_MetallicMap;`
- `uniform sampler2D u_RoughnessMap;`
- `uniform sampler2D u_AmbientOcclusionMap;`

И флаги наличия:

- `uniform int u_HasAlbedoMap;`
- `uniform int u_HasNormalMap;`
- `uniform int u_HasMetallicMap;`
- `uniform int u_HasRoughnessMap;`
- `uniform int u_HasAOMap;`

`Material::Apply` в любом случае вызывает `SetInt` для этих имен. Если вы пишете кастомный шейдер и хотите убрать часть uniform-ов, нужно либо:

1. Оставить эти имена в шейдере (даже если не используете), или
2. Изменить `Material::Apply` под свои нужды.

### Дополнительные uniform

Стандартный `default.frag` использует:

- `uniform vec3 u_ViewPos;` — позиция камеры.
- `uniform bool u_Wireframe;` — режим отладки.

Их устанавливает `src/main.cpp`.

## 3) Скелет шейдера под текущий движок

```glsl
// vertex.glsl
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform mat3 u_NormalMatrix;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out mat3 v_TBN;

void main() {
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    v_FragPos = worldPos.xyz;
    v_Normal = normalize(u_NormalMatrix * a_Normal);
    v_TexCoord = a_TexCoord;

    vec3 T = normalize(mat3(u_Model) * a_Tangent);
    vec3 B = normalize(mat3(u_Model) * a_Bitangent);
    vec3 N = v_Normal;
    v_TBN = mat3(T, B, N);

    gl_Position = u_ViewProjection * worldPos;
}
```

```glsl
// fragment.glsl
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

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_AmbientOcclusionMap;

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

    vec4 albedo = (u_HasAlbedoMap != 0) ? texture(u_AlbedoMap, v_TexCoord) : u_Material.albedo;
    float metallic = (u_HasMetallicMap != 0) ? texture(u_MetallicMap, v_TexCoord).r : u_Material.metallic;
    float roughness = (u_HasRoughnessMap != 0) ? texture(u_RoughnessMap, v_TexCoord).r : u_Material.roughness;
    float ao = (u_HasAOMap != 0) ? texture(u_AmbientOcclusionMap, v_TexCoord).r : u_Material.ambientOcclusion;

    vec3 normal = (u_HasNormalMap != 0)
        ? normalize(v_TBN * (texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0))
        : normalize(v_Normal);

    // Lighting (упрощенная модель)
    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    vec3 lightDir = normalize(lightPos - v_FragPos);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    vec3 ambient = vec3(0.2) * albedo.rgb * ao;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * albedo.rgb;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0 * (1.0 - roughness));
    vec3 specular = spec * vec3(0.5) * metallic;

    vec3 result = ambient + diffuse + specular;
    result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, albedo.a);
}
```

## 4) Материалы в C++

`MaterialProperties` определяет параметры и карты текстур (см. `src/Graphics/Material.h`):

- `albedo` — базовый цвет.
- `metallic` — металлическость (0..1).
- `roughness` — шероховатость (0..1).
- `ambientOcclusion` — затенение (0..1).
- `albedoMap`, `normalMap`, `metallicMap`, `roughnessMap`, `ambientOcclusionMap`.

`Material::Apply`:

- Устанавливает `u_Material.*` в шейдер.
- Привязывает карты к текстурным слотам в порядке:
  1. albedo
  2. normal
  3. metallic
  4. roughness
  5. ambient occlusion
- Для каждой карты выставляет соответствующий `u_Has*`.

### Пример создания материала

```cpp
Ref<Material> material = Material::Create();
auto& props = material->GetProperties();
props.albedo = Vector4(0.8f, 0.2f, 0.2f, 1.0f);
props.metallic = 0.8f;
props.roughness = 0.2f;

props.albedoMap = Texture::Create("assets/textures/wood_albedo.png");
props.normalMap = Texture::Create("assets/textures/wood_normal.png");
props.metallicMap = Texture::Create("assets/textures/wood_metallic.png");
props.roughnessMap = Texture::Create("assets/textures/wood_roughness.png");
props.ambientOcclusionMap = Texture::Create("assets/textures/wood_ao.png");
```

## 5) Текстуры

Текстуры создаются через `Texture::Create(path)` или `ResourceManager::LoadTexture(name, path)`:

```cpp
auto albedo = Texture::Create("assets/textures/brick_albedo.png");
auto normal = Texture::Create("assets/textures/brick_normal.png");
```

`Texture::Bind(slot)` вызывается внутри `Material::Apply`, поэтому вручную биндинг делать не нужно.

### Примечания по каналам

- `metallic`, `roughness`, `ambientOcclusion` берутся из `r`-канала текстуры.
- normal map ожидается в tangent space (RGB -> XYZ).

## 6) Как подключить шейдер

Пример загрузки из файлов (см. `src/main.cpp`):

```cpp
auto shader = ResourceManager::LoadShader(
    "default",
    "assets/shaders/default.vert",
    "assets/shaders/default.frag"
);
```

Если шейдер не загрузился — в `main.cpp` есть fallback на путь `../assets/...` при запуске из `build/`.

