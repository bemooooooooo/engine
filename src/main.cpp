#include "Core/Engine.h"
#include "Core/Log.h"
#include "Graphics/Graphics.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/GLError.h"
#include "Input/Input.h"
#include "Math/Math.h"

class TestScene : public Scene {
 public:
  TestScene() : Scene("TestScene") {}

  void Initialize() {
    ENGINE_INFO << "Initializing TestScene...";

    // Настройка камеры
    GetCamera().SetPerspective(Math::ToRadians(60.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    GetCamera().SetPosition(Vector3(0.0f, 20.0f, 100.0f));
    GetCamera().LookAt(Vector3(0.0f, 0.0f, 0.0f));
    
    ENGINE_DEBUG_LOG("Camera initialized at " << GetCamera().GetPosition().ToString());

    // Создание тестовых объектов
    CreateTestObjects();

    ENGINE_INFO << "TestScene initialized with " << GetObjects().size() << " objects";
  }

  void Update(f32 deltaTime) override {
    m_TotalTime += deltaTime;

    // Обновление анимаций
    UpdateAnimations(deltaTime);

    // Обновление камеры
    UpdateCamera(deltaTime);
  }

  void Render(const Ref<Shader>& shader) const override {
    Renderer::BeginScene(GetCamera());

    // Рендерим все объекты сцены
    for (const auto& sceneObject : GetObjects()) {
      if (sceneObject.mesh) {
        auto modelMatrix = sceneObject.transform.ToMatrix();
        sceneObject.mesh->Draw(shader, modelMatrix);
      }
    }

    Renderer::EndScene();
  }

 private:
  f32 m_TotalTime = 0.0f;
  f32 m_CameraOrbitAngle = 0.0f;

  void CreateTestObjects() {
    // 1. Центральный вращающийся куб (красный металлик)
    SceneObject mainCube("MainCube", Mesh::CreateCube());
    mainCube.transform.position = Vector3(0.0f, 0.5f, 0.0f);
    mainCube.mesh->GetMaterial()->GetProperties().albedo = Vector4(0.8f, 0.2f, 0.2f, 1.0f);
    mainCube.mesh->GetMaterial()->GetProperties().metallic = 0.8f;
    mainCube.mesh->GetMaterial()->GetProperties().roughness = 0.2f;
    AddObject(mainCube);

    // 2. Сфера (золотистая)
    SceneObject sphere("Sphere", Mesh::CreateSphere(32));
    sphere.transform.position = Vector3(3.0f, 0.5f, 0.0f);
    sphere.mesh->GetMaterial()->GetProperties().albedo = Vector4(1.0f, 0.8f, 0.4f, 1.0f);
    sphere.mesh->GetMaterial()->GetProperties().metallic = 1.0f;
    sphere.mesh->GetMaterial()->GetProperties().roughness = 0.1f;
    AddObject(sphere);

    // 3. Плоскость (пол, матовый серый)
    SceneObject floor("Floor", Mesh::CreatePlane());
    floor.transform.position = Vector3(0.0f, -1.5f, 0.0f);
    floor.transform.scale = Vector3(10.0f, 1.0f, 10.0f);
    floor.mesh->GetMaterial()->GetProperties().albedo = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
    floor.mesh->GetMaterial()->GetProperties().metallic = 0.0f;
    floor.mesh->GetMaterial()->GetProperties().roughness = 0.9f;
    AddObject(floor);

    // 4. Вращающийся куб (синий пластик)
    SceneObject rotatingCube("RotatingCube", Mesh::CreateCube());
    rotatingCube.transform.position = Vector3(-3.0f, 0.5f, 0.0f);
    rotatingCube.transform.scale = Vector3(0.7f, 0.7f, 0.7f);
    rotatingCube.mesh->GetMaterial()->GetProperties().albedo = Vector4(0.2f, 0.3f, 0.8f, 1.0f);
    rotatingCube.mesh->GetMaterial()->GetProperties().metallic = 0.0f;
    rotatingCube.mesh->GetMaterial()->GetProperties().roughness = 0.6f;
    AddObject(rotatingCube);
  }

  void UpdateAnimations(f32 deltaTime) {
    (void)deltaTime;

    // Rotate the cubes
    if (auto* rotatingCube = FindObject("RotatingCube")) {
      const f32 speed = 1.2f; // rad/s
      f32 angle = m_TotalTime * speed;
      rotatingCube->transform.rotation =
          Quaternion::FromAxisAngle(Vector3::Up, angle);
    }

    if (auto* mainCube = FindObject("MainCube")) {
      const f32 speed = 0.6f; // rad/s
      Vector3 axis = Vector3(0.3f, 1.0f, 0.2f).Normalized();
      f32 angle = m_TotalTime * speed;
      mainCube->transform.rotation =
          Quaternion::FromAxisAngle(axis, angle);
    }

    // Pulse the sphere
    if (auto* sphere = FindObject("Sphere")) {
      f32 scale = 1.0f + Math::Sin(m_TotalTime * 2.0f) * 0.2f;
      sphere->transform.scale = Vector3(scale, scale, scale);
    }
  }

// TODO поставить камеру нормально и заставить крутиться по орбите
  void UpdateCamera(f32 deltaTime) {
    // Orbit the camera around the scene center (0,0,0) in a circle.
    const f32 orbitSpeed = 0.3f; // rad/s
    const f32 radius = 8.0f;
    const f32 height = 2.0f;

    m_CameraOrbitAngle += deltaTime * orbitSpeed;
    if (m_CameraOrbitAngle > Math::TWO_PI) {
      m_CameraOrbitAngle -= Math::TWO_PI;
    }

    const Vector3 target(0.0f, 0.0f, 0.0f);
    f32 x = Math::Cos(m_CameraOrbitAngle) * radius;
    f32 z = Math::Sin(m_CameraOrbitAngle) * radius;
    Vector3 cameraPos(x, height, z);

    GetCamera().SetPosition(cameraPos);
    GetCamera().LookAt(target, Vector3::Up);
  }
};


class TestApp : public Engine {
 public:
  TestApp() : m_WireframeMode(false) {}

 protected:
  void OnInitialize() override {
    ENGINE_INFO << "=== Testing Complete Graphics Module ===";

    // Инициализация всех систем
    Renderer::Init();
    Input::Init();
    
    // Настройка OpenGL состояния
    {
      GL_CHECK("GL State Setup");
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      glClearDepth(1.0f);
    }

    // Загрузка шейдера из файлов через ResourceManager
    // Используем пути относительно рабочей директории (build/)
    m_Shader = ResourceManager::LoadShader("default", 
                                          "assets/shaders/default.vert",
                                          "assets/shaders/default.frag");
    
    // Если не удалось загрузить, пробуем альтернативные пути
    if (!m_Shader) {
      m_Shader = ResourceManager::LoadShader("default", 
                                            "../assets/shaders/default.vert",
                                            "../assets/shaders/default.frag");
    }
    if (!m_Shader) {
      ENGINE_ERROR << "Failed to load shader!";
      return;
    }

    // Создание и инициализация сцены
    m_Scene = CreateScope<TestScene>();
    m_Scene->Initialize();

    ENGINE_INFO << "=== All Systems Ready ===";
    ENGINE_INFO << "Controls:";
    ENGINE_INFO << "  SPACE - Toggle wireframe";
    ENGINE_INFO << "  R - Reset camera";
    ENGINE_INFO << "  ESC - Quit";
  }

  void OnUpdate(f32 deltaTime) override {
    // Обновление сцены
    m_Scene->Update(deltaTime);

    // Обработка ввода
    HandleInput();

    // Логирование производительности
    static f32 fpsTimer = 0.0f;
    static u32 frameCount = 0;
    fpsTimer += deltaTime;
    frameCount++;

    if (fpsTimer >= 1.0f) {
      ENGINE_INFO << "FPS: " << frameCount
                  << ", Objects: " << m_Scene.get()->GetObjects().size();
      fpsTimer = 0.0f;
      frameCount = 0;
    }
  }

  void OnRender() override {
    {
      GL_CHECK("Begin Render");
      
      // Устанавливаем цвет очистки и очищаем буферы
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    // Настраиваем шейдер
    if (!m_Shader) {
      ENGINE_ERROR << "Shader is null in OnRender!";
      return;
    }
    
    m_Shader->Bind();
    
    // Устанавливаем основные униформы
    m_Shader->SetVector3("u_ViewPos", m_Scene->GetCamera().GetPosition());
    m_Shader->SetInt("u_Wireframe", m_WireframeMode ? 1 : 0);

    // Рендерим сцену
    m_Scene->Render(m_Shader);
    
    {
      GL_CHECK("End Render");
    }
  }

  void OnShutdown() override {
    ENGINE_INFO << "=== Shutting Down ===";
    m_Scene.reset();
    ResourceManager::Shutdown();
    Renderer::Shutdown();
  }

  void OnEvent(Event& event) override {
    if (auto* resizeEvent = dynamic_cast<WindowResizeEvent*>(&event)) {
      ENGINE_INFO << "Window resized: " << resizeEvent->GetWidth() << "x"
                  << resizeEvent->GetHeight();
      Renderer::OnWindowResize(resizeEvent->GetWidth(),
                               resizeEvent->GetHeight());
    }
  }

 private:
  Scope<TestScene> m_Scene;
  Ref<Shader> m_Shader;
  bool m_WireframeMode;


  void HandleInput() {
    if (Input::GetKeyDown(Key::Space)) {
      m_WireframeMode = !m_WireframeMode;
      m_Shader->SetInt("u_Wireframe", m_WireframeMode ? 1 : 0);
      ENGINE_INFO << "Wireframe: " << (m_WireframeMode ? "ON" : "OFF");
    }

    if (Input::GetKeyDown(Key::R)) {
      m_Scene->GetCamera().SetPosition(Vector3(0.0f, 2.0f, 8.0f));
      m_Scene->GetCamera().LookAt(Vector3(0.0f, 0.0f, 0.0f));
      ENGINE_INFO << "Camera reset";
    }
  }
};

int main() {
  ENGINE_INFO << "=== Graphics Module Comprehensive Test ===";

  TestApp app;
  app.Run();

  ENGINE_INFO << "=== Test Completed ===";
  return 0;
}
