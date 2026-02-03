#include "Scene.h"

#include "../Core/Log.h"
#include "Renderer.h"

Scene::Scene() {
  m_Camera.SetPerspective(Math::ToRadians(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
  m_Camera.SetPosition(Vector3(0.0f, 0.0f, 5.0f));

  ENGINE_INFO << "Scene created with default camera settings.";
}

Scene::Scene(std::string name) : name{name} {
  m_Camera.SetPerspective(Math::ToRadians(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
  m_Camera.SetPosition(Vector3(0.0f, 0.0f, 5.0f));

  ENGINE_INFO << "Scene '" << name << "' created with default camera settings.";
}

Scene::~Scene() { ENGINE_INFO << "Scene destroyed."; }

void Scene::AddObject(const SceneObject& object) {
  // Проверка на дубликаты имен
  if (m_ObjectIndex.find(object.name) != m_ObjectIndex.end()) {
    ENGINE_WARN << "Object with name '" << object.name << "' already exists in scene! Skipping add.";
    return;
  }
  
  // Добавляем объект и обновляем индекс
  m_ObjectIndex[object.name] = m_Objects.size();
  m_Objects.push_back(object);
  ENGINE_INFO << "Added object to scene: " << object.name;
}

void Scene::RemoveObject(const std::string& name) {
  auto indexIt = m_ObjectIndex.find(name);
  if (indexIt == m_ObjectIndex.end()) {
    ENGINE_WARN << "Object not found in scene: " << name;
    return;
  }
  
  size_t index = indexIt->second;
  
  // Удаляем из вектора
  m_Objects.erase(m_Objects.begin() + index);
  
  // Обновляем индекс для всех объектов после удаленного
  m_ObjectIndex.erase(indexIt);
  for (auto& pair : m_ObjectIndex) {
    if (pair.second > index) {
      pair.second--;
    }
  }
  
  ENGINE_INFO << "Removed object from scene: " << name;
}

SceneObject* Scene::FindObject(const std::string& name) {
  auto it = m_ObjectIndex.find(name);
  if (it != m_ObjectIndex.end()) {
    return &m_Objects[it->second];
  }
  return nullptr;
}

void Scene::Update(f32 deltaTime) {
  for (auto& object : m_Objects) {
    if (object.onUpdate) {
      object.onUpdate(&object, deltaTime);
    }
  }
}

void Scene::Render(const Ref<Shader>& shader) const {
  if (!shader) {
    ENGINE_ERROR << "Scene::Render: shader is null!";
    return;
  }
  
  Renderer::BeginScene(m_Camera);

  for (const auto& object : m_Objects) {
    if (object.mesh) {
      ENGINE_DEBUG_LOG("Rendering " << object.name);
      object.mesh->Draw(shader, object.transform.ToMatrix());
    }
  }

  Renderer::EndScene();
}