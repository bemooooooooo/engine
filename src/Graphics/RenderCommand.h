#pragma once
#include "../Core/Base.h"
#include "GraphicsContext.h"

/**
 * @class RenderCommand
 * @brief Static wrapper class for issuing rendering commands.
 * 
 * This class provides a simplified interface for common rendering operations
 * by wrapping the GraphicsContext static methods. It serves as a higher-level
 * abstraction for issuing draw calls and managing rendering state.
 * 
 * All methods are static, allowing direct access without instantiation.
 * 
 * Key features:
 * - Viewport setup
 * - Clear color and buffer clearing
 * - Drawing operations (arrays and indexed)
 * - Depth testing and blending control
 * 
 * @note This class relies on GraphicsContext for actual OpenGL calls.
 * @note Must be initialized via Init() before use.
 * 
 * @example
 * @code
 * RenderCommand::Init();
 * RenderCommand::SetViewport(0, 0, 800, 600);
 * RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
 * 
 * while (running) {
 *     RenderCommand::Clear();
 *     RenderCommand::DrawIndexed(indexCount);
 * }
 * 
 * RenderCommand::Shutdown();
 * @endcode
 */
class RenderCommand {
public:
    /**
     * @brief Initializes the rendering command system.
     * 
     * Must be called once at engine startup before any rendering commands are issued.
     * This function initializes the underlying GraphicsContext.
     * 
     * @note This should be called right after creating a window.
     * @note Must be called before any rendering commands.
     * 
     * @example
     * @code
     * Window window(800, 600, "My Game");
     * RenderCommand::Init();  // Initialize rendering commands
     * @endcode
     */
    static void Init() { GraphicsContext::Init(); }

    /**
     * @brief Shuts down the rendering command system.
     * 
     * Called at engine shutdown to clean up rendering resources.
     * This function shuts down the underlying GraphicsContext.
     * 
     * @note Must be called before destroying the window.
     * @note No rendering commands should occur after shutdown.
     * 
     * @example
     * @code
     * // At engine shutdown
     * RenderCommand::Shutdown();
     * window.Destroy();
     * @endcode
     */
    static void Shutdown() { GraphicsContext::Shutdown(); }
    
    /**
     * @brief Sets the viewport dimensions and position.
     * 
     * Defines the rectangular region of the render target (screen/framebuffer)
     * where rendering will occur. The viewport is specified in pixel coordinates.
     * 
     * @param x The left edge of the viewport in pixels (usually 0)
     * @param y The bottom edge of the viewport in pixels (usually 0)
     * @param width The width of the viewport in pixels
     * @param height The height of the viewport in pixels
     * 
     * @note The origin (0, 0) is typically at the bottom-left in OpenGL.
     * @note Viewport can be smaller than the window for split-screen or UI areas.
     * 
     * @example
     * @code
     * // Set full viewport for 800x600 window
     * RenderCommand::SetViewport(0, 0, 800, 600);
     * 
     * // Set viewport for quarter of the screen (top-left)
     * RenderCommand::SetViewport(0, 300, 400, 300);
     * @endcode
     */
    static void SetViewport(u32 x, u32 y, u32 width, u32 height) {
        GraphicsContext::SetViewport(x, y, width, height);
    }
    
    /**
     * @brief Sets the color used for clearing the color buffer.
     * 
     * Specifies the RGBA color that will be used when Clear() is called to
     * clear the color buffer. This is typically called once per frame with
     * the background color.
     * 
     * @param r Red component (0.0f to 1.0f)
     * @param g Green component (0.0f to 1.0f)
     * @param b Blue component (0.0f to 1.0f)
     * @param a Alpha component (0.0f to 1.0f)
     * 
     * @note Values are clamped to [0.0, 1.0] internally by OpenGL.
     * 
     * @example
     * @code
     * // Black background
     * RenderCommand::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
     * 
     * // Gray background
     * RenderCommand::SetClearColor(0.5f, 0.5f, 0.5f, 1.0f);
     * 
     * // Sky blue
     * RenderCommand::SetClearColor(0.53f, 0.81f, 0.92f, 1.0f);
     * @endcode
     */
    static void SetClearColor(f32 r, f32 g, f32 b, f32 a) {
        GraphicsContext::SetClearColor(r, g, b, a);
    }
    
    /**
     * @brief Clears the color and depth buffers.
     * 
     * Fills the color buffer with the color set by SetClearColor() and clears
     * the depth buffer. This is typically called at the start of each frame to
     * prepare for new rendering.
     * 
     * @note Must be called each frame to clear previous frame's rendering.
     * @note The depth buffer is always cleared regardless of depth test state.
     * 
     * @example
     * @code
     * while (running) {
     *     RenderCommand::Clear();  // Clear both color and depth
     *     // ... render scene ...
     * }
     * @endcode
     */
    static void Clear() {
        GraphicsContext::Clear();
    }
    
    /**
     * @brief Issues a draw call for non-indexed geometry.
     * 
     * Renders primitives from array data. This is typically used for drawing
     * simple shapes or when index buffers are not needed.
     * 
     * @param vertexCount Number of vertices to render
     * 
     * @note Requires bound vertex array.
     * @note Less efficient than indexed drawing for complex meshes.
     * 
     * @example
     * @code
     * VAO.Bind();
     * RenderCommand::DrawArrays(vertexCount);
     * @endcode
     */
    static void DrawArrays(u32 vertexCount) {
        GraphicsContext::DrawArrays(vertexCount);
    }
    
    /**
     * @brief Issues a draw call for indexed geometry.
     * 
     * Renders primitives using an index buffer. This is the preferred method
     * for drawing complex meshes where vertices are shared between triangles.
     * 
     * @param indexCount Number of indices to render
     * 
     * @note Requires bound vertex array and index buffer.
     * 
     * @example
     * @code
     * VAO.Bind();
     * IndexBuffer.Bind();
     * RenderCommand::DrawIndexed(indexCount);
     * @endcode
     */
    static void DrawIndexed(u32 indexCount) {
        GraphicsContext::DrawIndexed(indexCount);
    }
    
    /**
     * @brief Enables or disables depth testing.
     * 
     * When enabled, the depth buffer is used to determine fragment visibility
     * based on depth values. This prevents rendering artifacts in 3D scenes.
     * 
     * @param enable True to enable depth testing, false to disable
     * 
     * @note Depth testing is typically enabled for 3D rendering.
     * 
     * @example
     * @code
     * RenderCommand::EnableDepthTest(true);  // Enable depth testing
     * 
     * // For UI rendering
     * RenderCommand::EnableDepthTest(false); // Disable depth testing
     * @endcode
     */
    static void EnableDepthTest(bool enable) {
        GraphicsContext::EnableDepthTest(enable);
    }
    
    /**
     * @brief Enables or disables blending.
     * 
     * When enabled, blending is used to combine fragment colors with the
     * framebuffer based on alpha values. This is essential for rendering
     * transparent or semi-transparent objects.
     * 
     * @param enable True to enable blending, false to disable
     * 
     * @note Blending is typically enabled when rendering transparent objects.
     * 
     * @example
     * @code
     * RenderCommand::EnableBlending(true);  // Enable blending for transparency
     * 
     * // For opaque objects
     * RenderCommand::EnableBlending(false); // Disable blending
     * @endcode
     */
    static void EnableBlending(bool enable) {
        GraphicsContext::EnableBlending(enable);
    }
};