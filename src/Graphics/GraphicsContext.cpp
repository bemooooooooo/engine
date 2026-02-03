#include "GraphicsContext.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

bool GraphicsContext::s_Initialized = false;

void GraphicsContext::Init() {
    if (s_Initialized) return;
    
    // Проверяем что Glad инициализирован
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        ENGINE_CRITICAL << "Glad not initialized!";
        return;
    }
    
    // Базовая настройка OpenGL
    EnableDepthTest(true);
    EnableBlending(true);
    // Disable culling for now while fixing matrix handedness issues.
    EnableCulling(false);
    
    // Настройка blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Настройка глубины
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);
    
    // Проверка состояния после инициализации
    GLint depthTest, depthFunc, cullFace, blendEnabled;
    glGetIntegerv(GL_DEPTH_TEST, &depthTest);
    glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
    glGetIntegerv(GL_CULL_FACE, &cullFace);
    glGetIntegerv(GL_BLEND, &blendEnabled);
    
    ENGINE_INFO << "OpenGL State after init:";
    ENGINE_INFO << "  Depth Test: " << (depthTest ? "ON" : "OFF");
    ENGINE_INFO << "  Depth Func: 0x" << std::hex << depthFunc;
    ENGINE_INFO << "  Face Culling: " << (cullFace ? "ON" : "OFF");
    ENGINE_INFO << "  Blending: " << (blendEnabled ? "ON" : "OFF");
    
    s_Initialized = true;
    
    ENGINE_INFO << "Graphics Context Initialized";
    ENGINE_INFO << "  Vendor: " << GetVendor();
    ENGINE_INFO << "  Renderer: " << GetRenderer();
    ENGINE_INFO << "  Version: " << GetVersion();
}

void GraphicsContext::Shutdown() {
    s_Initialized = false;
    ENGINE_INFO << "Graphics Context Shutdown";
}

void GraphicsContext::SetViewport(u32 x, u32 y, u32 width, u32 height) {
    glViewport(x, y, width, height);
}

void GraphicsContext::SetClearColor(f32 r, f32 g, f32 b, f32 a) {
    glClearColor(r, g, b, a);
}

void GraphicsContext::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsContext::EnableDepthTest(bool enable) {
    if (enable) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}

void GraphicsContext::EnableBlending(bool enable) {
    if (enable) glEnable(GL_BLEND);
    else glDisable(GL_BLEND);
}

void GraphicsContext::EnableCulling(bool enable) {
    if (enable) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);
}

void GraphicsContext::SetCullFace(bool cullBack) {
    glCullFace(cullBack ? GL_BACK : GL_FRONT);
}

void GraphicsContext::DrawArrays(u32 vertexCount) {
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void GraphicsContext::DrawIndexed(u32 indexCount) {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
}

const char* GraphicsContext::GetVendor() {
    return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}

const char* GraphicsContext::GetRenderer() {
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

  const char* GraphicsContext::GetVersion() {
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

// Добавляем функцию для вывода текущего состояния OpenGL
void GraphicsContext::PrintGLInfo() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    ENGINE_INFO << "OpenGL Viewport: " << viewport[0] << ", " << viewport[1] 
                << ", " << viewport[2] << ", " << viewport[3];

    GLint depthTest, depthFunc, cullFace;
    glGetIntegerv(GL_DEPTH_TEST, &depthTest);
    glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
    glGetIntegerv(GL_CULL_FACE, &cullFace);
    
    ENGINE_INFO << "Depth Test: " << (depthTest ? "ON" : "OFF")
                << ", Depth Func: 0x" << std::hex << depthFunc
                << ", Face Culling: " << (cullFace ? "ON" : "OFF");

    GLfloat depth_range[2];
    glGetFloatv(GL_DEPTH_RANGE, depth_range);
    ENGINE_INFO << "Depth Range: " << depth_range[0] << " - " << depth_range[1];
}
