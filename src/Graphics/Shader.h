#pragma once
#include "../Core/Base.h"
#include "../Core/Log.h"
#include "../Math/Matrix3.h"
#include "../Math/Matrix4.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include <glad/gl.h>
#include <unordered_map>

/**
 * @class Shader
 * @brief Represents an OpenGL shader program composed of vertex and fragment shaders.
 * 
 * This class encapsulates the creation, compilation, linking, and management of
 * OpenGL shader programs. It provides methods to bind/unbind the shader and set
 * uniform variables of various types (int, float, vectors, matrices).
 * 
 * Key features:
 * - Load shaders from source code strings or files
 * - Compile and link shader programs
 * - Set uniform variables with caching for performance
 * 
 * @note Requires a valid OpenGL context to function properly.
 * @note All OpenGL calls are made through this class; direct OpenGL calls should be avoided.
 * 
 * @example
 * @code
 * std::string vertexSrc = "...";   // Vertex shader source code
 * std::string fragmentSrc = "..."; // Fragment shader source code
 * Ref<Shader> shader = Shader::Create(vertexSrc, fragmentSrc);
 * 
 * shader->Bind();
 * shader->SetMatrix4("u_ViewProjection", viewProjMatrix);
 * shader->SetVector3("u_LightPosition", lightPos);
 * shader->Unbind();
 * @endcode
 */
class Shader {
public:
    /** 
     * @brief Constructs a Shader from vertex and fragment source code strings.
     * 
     * Compiles the provided vertex and fragment shader source code, links them
     * into a shader program, and prepares it for use.
     * 
     * @param vertexSrc The source code for the vertex shader
     * @param fragmentSrc The source code for the fragment shader
     */
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);

    /** 
     * @brief Destructor that cleans up the OpenGL shader program.
     */
    ~Shader();
    
    /** 
     * @brief Binds the shader program for use in rendering.
     */
    void Bind() const;

    /** 
     * @brief Unbinds the shader program.
     */
    void Unbind() const;
    
    // ----------------Uniform setters----------------
    /** 
     * @brief Sets an integer uniform variable in the shader.
     * 
     * @param name The name of the uniform variable
     * @param value The integer value to set
     */
    void SetInt(const std::string& name, i32 value);

    /** 
     * @brief Sets a float uniform variable in the shader.
     * 
     * @param name The name of the uniform variable
     * @param value The float value to set
     */
    void SetFloat(const std::string& name, f32 value);

    /** 
     * @brief Sets a Vector2 uniform variable in the shader.
     * 
     * @param name The name of the uniform variable
     * @param value The Vector2 value to set
     */
    void SetVector2(const std::string& name, const Vector2& value);

    /** 
     * @brief Sets a Vector3 uniform variable in the shader.
     * 
     * @param name The name of the uniform variable
     * @param value The Vector3 value to set
     */
    void SetVector3(const std::string& name, const Vector3& value);

    /** 
     * @brief Sets a Vector4 uniform variable in the shader.
     * 
     * @param name The name of the uniform variable
     * @param value The Vector4 value to set
     */
    void SetVector4(const std::string& name, const Vector4& value);

    /** 
     * @brief Sets a Matrix3 uniform variable in the shader.
     * 
     * @param name The name of the uniform variable
     * @param value The Matrix3 value to set
     */
    void SetMatrix3(const std::string& name, const Matrix3& value);

    /** 
     * @brief Sets a Matrix4 uniform variable in the shader.
     * 
     * @param name The name of the uniform variable
     * @param value The Matrix4 value to set
     */
    void SetMatrix4(const std::string& name, const Matrix4& value);
    
    /** 
     * @brief Gets the OpenGL renderer ID of the shader program.
     * 
     * @return u32 The OpenGL renderer ID
     */
    [[nodiscard]] u32 GetRendererID() const { return m_RendererID; }
    
    /** 
     * @brief Creates a Shader from vertex and fragment source code strings.
     * 
     * @param vertexSrc The source code for the vertex shader
     * @param fragmentSrc The source code for the fragment shader
     * @return Ref<Shader> A reference-counted pointer to the created Shader
     */
    [[nodiscard]] static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);

    /** 
     * @brief Creates a Shader by loading vertex and fragment shader source code from files.
     * 
     * @param vertexPath The file path to the vertex shader source code
     * @param fragmentPath The file path to the fragment shader source code
     * @return Ref<Shader> A reference-counted pointer to the created Shader
     */
    [[nodiscard]] static Ref<Shader> CreateFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

private:
    /** 
     * @brief Compiles a shader of the given type from source code.
     * 
     * @param type The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
     * @param source The source code of the shader
     * @return u32 The OpenGL ID of the compiled shader
     */
    u32 CompileShader(GLenum type, const std::string& source);
    
    /** 
     * @brief Retrieves the location of a uniform variable, using caching for performance.
     * 
     * @param name The name of the uniform variable
     * @return i32 The location of the uniform variable
     */
    i32 GetUniformLocation(const std::string& name);
    
    /** 
     * @var m_RendererID
     * @brief The OpenGL renderer ID for the shader program.
     */
    u32 m_RendererID;

    /** 
     * @var m_UniformLocationCache
     * @brief Cache mapping uniform variable names to their OpenGL locations.
     */
    std::unordered_map<std::string, i32> m_UniformLocationCache;
};