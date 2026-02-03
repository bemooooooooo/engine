#include "Window.h"
#include "../Core/Log.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Статические переменные для GLFW
static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description) {
    ENGINE_ERROR << "GLFW Error (" << error << "): " << description;
}

Window::Window() {
    ENGINE_TRACE << "Window constructor";
}

Window::~Window() {
    Shutdown();
    ENGINE_TRACE << "Window destructor";
}

bool Window::Initialize(const WindowProps& props) {
    ENGINE_INFO << "Creating window: " << props.Title << " (" << props.Width << "x" << props.Height << ")";
    
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;
    
    // Инициализация GLFW
    if (!s_GLFWInitialized) {
        int success = glfwInit();
        if (!success) {
            ENGINE_CRITICAL << "Could not initialize GLFW!";
            return false;
        }
        
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
        ENGINE_INFO << "GLFW initialized";
    }
    
    // Настройка OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    // Создание окна
    m_Window = glfwCreateWindow(
        static_cast<int>(m_Data.Width),
        static_cast<int>(m_Data.Height),
        m_Data.Title.c_str(),
        nullptr, nullptr
    );
    
    if (!m_Window) {
        ENGINE_CRITICAL << "Failed to create GLFW window!";
        glfwTerminate();
        return false;
    }
    
    // Сделать контекст текущим
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window));
    
    // Инициализация Glad
    if (!gladLoadGL(glfwGetProcAddress)) {
        ENGINE_CRITICAL << "Failed to initialize Glad!";
        return false;
    }
    
    ENGINE_INFO << "OpenGL Info:";
    ENGINE_INFO << "  Vendor: " << glGetString(GL_VENDOR);
    ENGINE_INFO << "  Renderer: " << glGetString(GL_RENDERER);
    ENGINE_INFO << "  Version: " << glGetString(GL_VERSION);
    
    // Настройка колбэков
    SetupCallbacks();
    
    // Базовая настройка OpenGL
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    ENGINE_INFO << "Window created successfully";
    return true;
}

void Window::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_Window));
        m_Window = nullptr;
        ENGINE_INFO << "Window destroyed";
    }
}

void Window::Update() {
    // Обновляем систему ввода в начале кадра
    Input::Update();
    
    // Обрабатываем события GLFW
    glfwPollEvents();
    
    // Обновляем буферы
    glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window));
}

void Window::SetupCallbacks() {
    GLFWwindow* window = static_cast<GLFWwindow*>(m_Window);
    
    // Сохраняем указатель на данные окна для доступа в статических колбэках
    glfwSetWindowUserPointer(window, &m_Data);
    
    // Колбэк изменения размера окна
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.Width = width;
        data.Height = height;
        
        // Создаем и передаем событие через EventCallback
        WindowResizeEvent event(static_cast<u32>(width), static_cast<u32>(height));
        if (data.EventCallback) {
            data.EventCallback(event);
        }
    });
    
    // Колбэк закрытия окна
    glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        
        // Создаем и передаем событие через EventCallback
        WindowCloseEvent event;
        if (data.EventCallback) {
            data.EventCallback(event);
        }
    });
}

void Window::BeginFrame() {
    // Очистка буферов (цвета и глубины)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::EndFrame() {
    // В данный момент ничего не делаем, можно добавить:
    // - Отладочную информацию
    // - Статистику рендеринга
    // - Post-processing эффекты
}