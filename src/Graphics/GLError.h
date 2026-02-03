#pragma once
#include "../Core/Base.h"
#include "../Core/Log.h"
#include <glad/gl.h>
#include <string>

/**
 * @class GLErrorChecker
 * @brief Utility class for checking and logging OpenGL errors.
 * 
 * This class provides functionality to check for OpenGL errors after
 * OpenGL function calls. It logs any detected errors using the engine's
 * logging system. The class can be used
 * in a RAII manner to automatically check for errors when going out of scope.
 *
 * Usage:
 * @code
 * {
 *   GLErrorChecker glError("Rendering Frame");
 *  // OpenGL calls...
 * } // Errors checked automatically here
 * @endcode
 * @note This class is intended for debugging purposes and may be disabled
 * in release builds for performance.
 */
class GLErrorChecker {
public:
    /// @brief Constructs a GLErrorChecker with a context description.
    GLErrorChecker(const char* context) : m_Context(context) {
        // Clear any existing errors
        while (glGetError() != GL_NO_ERROR);
    }
    
    /// @brief Destructor that checks for OpenGL errors.
    ~GLErrorChecker() {
        CheckErrors();
    }
    
    /// @brief Checks for OpenGL errors and logs them.
    void CheckErrors() const {
        GLenum error;
        bool hasError = false;
        std::string errorMessages;
        
        while ((error = glGetError()) != GL_NO_ERROR) {
            hasError = true;
            const char* errorString = GetErrorString(error);
            if (!errorMessages.empty()) errorMessages += ", ";
            errorMessages += errorString;
        }
        
        if (hasError) {
            ENGINE_ERROR << "OpenGL Error in " << m_Context << ": " << errorMessages;
        }
    }
    
    /// @brief Converts an OpenGL error code to a human-readable string.
    static const char* GetErrorString(GLenum error) {
        switch (error) {
            case GL_NO_ERROR: return "GL_NO_ERROR";
            case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
            case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
            case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
            case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
            case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
            default: return "UNKNOWN_ERROR";
        }
    }

private:
    /// @brief Context description for error logging.
    const char* m_Context;
};

/// @brief Macro to create a GLErrorChecker for the current scope.
#define GL_CHECK(context) GLErrorChecker _gl_checker(context)

/// @brief Checks for OpenGL errors with a custom message.
inline void CheckGLError(const char* message) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        ENGINE_ERROR << message << " - GL Error: " << GLErrorChecker::GetErrorString(error);
    }
}

