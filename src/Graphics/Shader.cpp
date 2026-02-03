#include "Shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    u32 vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    u32 fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);
    
    // Check for linking errors
    i32 success;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog);
        ENGINE_ERROR << "Shader program linking failed: " << infoLog;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    ENGINE_INFO << "Shader created (ID: " << m_RendererID << ")";
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
    ENGINE_INFO << "Shader destroyed (ID: " << m_RendererID << ")";
}

u32 Shader::CompileShader(GLenum type, const std::string& source) {
    u32 shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    // Check for compilation errors
    i32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        ENGINE_ERROR << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") 
                    << " shader compilation failed: " << infoLog;
    }
    
    return shader;
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

i32 Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    
    i32 location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        ENGINE_WARN << "Uniform '" << name << "' doesn't exist!";
    }
    
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetInt(const std::string& name, i32 value) {
    i32 location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::SetFloat(const std::string& name, f32 value) {
    i32 location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::SetVector2(const std::string& name, const Vector2& value) {
    i32 location = GetUniformLocation(name);
    if (location != -1) {
        glUniform2f(location, value.x, value.y);
    }
}

void Shader::SetVector3(const std::string& name, const Vector3& value) {
    i32 location = GetUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void Shader::SetVector4(const std::string& name, const Vector4& value) {
    i32 location = GetUniformLocation(name);
    if (location != -1) {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
}

void Shader::SetMatrix3(const std::string& name, const Matrix3& value) {
    i32 location = GetUniformLocation(name);
    if (location != -1) {
        Matrix3 transposed = value.Transposed();
        glUniformMatrix3fv(location, 1, GL_FALSE, transposed.m);
    }
}

void Shader::SetMatrix4(const std::string& name, const Matrix4& value) {
    i32 location = GetUniformLocation(name);
    if (location != -1) {
        Matrix4 transposed = value.Transposed();
        glUniformMatrix4fv(location, 1, GL_FALSE, transposed.m);
    }
}

Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
    return CreateRef<Shader>(vertexSrc, fragmentSrc);
}

Ref<Shader> Shader::CreateFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);
    
    if (!vertexFile.is_open() || !fragmentFile.is_open()) {
        ENGINE_ERROR << "Failed to open shader files: " << vertexPath << ", " << fragmentPath;
        return nullptr;
    }
    
    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();
    
    return Create(vertexStream.str(), fragmentStream.str());
}
