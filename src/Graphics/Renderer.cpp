#include "Renderer.h"
#include "../Core/Log.h"

Scope<SceneData> Renderer::s_SceneData = CreateScope<SceneData>();

void Renderer::Init() {
    RenderCommand::Init();
    
    ENGINE_INFO << "Renderer initialized";
}

void Renderer::Shutdown() {
    RenderCommand::Shutdown();
    
    ENGINE_INFO << "Renderer shutdown";
}

void Renderer::BeginScene(const Camera& camera) {
    s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {
    // Можно добавить пост-обработку здесь
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, 
                     const Matrix4& transform) {
    if (!shader) {
        ENGINE_ERROR << "Renderer::Submit: shader is null!";
        return;
    }
    
    if (!vertexArray) {
        ENGINE_ERROR << "Renderer::Submit: vertex array is null!";
        return;
    }
    
    shader->Bind();
    
    // Установка матриц
    shader->SetMatrix4("u_ViewProjection", s_SceneData->viewProjectionMatrix);
    shader->SetMatrix4("u_Model", transform);
    
    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    if (program == 0) {
        ENGINE_ERROR << "No shader program bound!";
        return;
    }
    
    vertexArray->Bind();
    auto indexBuffer = vertexArray->GetIndexBuffer();
    if (!indexBuffer) {
        ENGINE_ERROR << "Renderer::Submit: index buffer is null!";
        return;
    }
    
    auto indexCount = indexBuffer->GetCount();
    if (indexCount == 0) {
        ENGINE_WARN << "Renderer::Submit: index count is 0!";
        return;
    }
    
    RenderCommand::DrawIndexed(indexCount);
}

void Renderer::SetClearColor(const Vector4& color) {
    RenderCommand::SetClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::Clear() {
    RenderCommand::Clear();
}

void Renderer::OnWindowResize(u32 width, u32 height) {
    RenderCommand::SetViewport(0, 0, width, height);
}