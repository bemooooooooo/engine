#pragma once
#include "../Core/Base.h"
#include "../Core/Log.h"

/**
 * @class GraphicsContext
 * @brief Core graphics context management for OpenGL operations.
 * 
 * This class manages the graphics rendering context and provides a unified interface
 * for OpenGL operations. It handles viewport setup, color clearing, depth testing,
 * blending, face culling, and drawing operations.
 * 
 * All member functions are static, making GraphicsContext a static utility class.
 * This enforces a single graphics context per application, which is appropriate
 * for most game engine architectures.
 * 
 * The class is responsible for:
 * - Initializing and shutting down the graphics context
 * - Managing rendering state (viewport, clear color, depth test, etc.)
 * - Providing drawing functions
 * - Querying graphics hardware information
 * 
 * @note This class uses static methods. It should be initialized once at engine startup.
 * @note All OpenGL operations must occur after initialization.
 * 
 * @example
 * @code
 * // Initialize graphics at engine startup
 * GraphicsContext::Init();
 * GraphicsContext::SetViewport(0, 0, 800, 600);
 * GraphicsContext::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
 * 
 * // Render loop
 * while (running) {
 *     GraphicsContext::Clear();
 *     // ... draw objects ...
 *     GraphicsContext::DrawArrays(vertexCount);
 * }
 * 
 * // Cleanup at shutdown
 * GraphicsContext::Shutdown();
 * @endcode
 */
class GraphicsContext {
public:
    /**
     * @brief Initializes the graphics context.
     * 
     * Must be called once at engine startup before any graphics operations.
     * This function initializes OpenGL, loads extensions (via GLAD), and sets up
     * the initial graphics state.
     * 
     * @note This should be called right after creating a window.
     * @note Must be called before any rendering or graphics queries.
     * 
     * @example
     * @code
     * Window window(800, 600, "My Game");
     * GraphicsContext::Init();  // Initialize graphics
     * @endcode
     */
    static void Init();
    
    /**
     * @brief Shuts down the graphics context.
     * 
     * Called at engine shutdown to clean up graphics resources and properly
     * close the graphics context. This should be called before window destruction.
     * 
     * @note Must be called before destroying the window.
     * @note No graphics operations should occur after shutdown.
     * 
     * @example
     * @code
     * // At engine shutdown
     * GraphicsContext::Shutdown();
     * window.Destroy();
     * @endcode
     */
    static void Shutdown();
    
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
     * GraphicsContext::SetViewport(0, 0, 800, 600);
     * 
     * // Set viewport for quarter of the screen (top-left)
     * GraphicsContext::SetViewport(0, 300, 400, 300);
     * @endcode
     */
    static void SetViewport(u32 x, u32 y, u32 width, u32 height);
    
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
     * GraphicsContext::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
     * 
     * // Gray background
     * GraphicsContext::SetClearColor(0.5f, 0.5f, 0.5f, 1.0f);
     * 
     * // Sky blue
     * GraphicsContext::SetClearColor(0.53f, 0.81f, 0.92f, 1.0f);
     * @endcode
     */
    static void SetClearColor(f32 r, f32 g, f32 b, f32 a);
    
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
     *     GraphicsContext::Clear();  // Clear both color and depth
     *     // ... render scene ...
     * }
     * @endcode
     */
    static void Clear();
    
    /**
     * @brief Enables or disables depth testing.
     * 
     * Depth testing compares the depth (Z value) of fragments to determine if they
     * should be rendered. When enabled, only the closest fragment at each pixel
     * is rendered, preventing far objects from appearing in front of near ones.
     * 
     * @param enable True to enable depth testing, false to disable
     * 
     * @note Enabled by default in most graphics engines.
     * @note Must be enabled for proper 3D rendering.
     * @note Disable for 2D rendering or UI layers.
     * 
     * @example
     * @code
     * // Enable for 3D rendering
     * GraphicsContext::EnableDepthTest(true);
     * 
     * // Disable for 2D UI overlays
     * GraphicsContext::EnableDepthTest(false);
     * RenderUI();
     * GraphicsContext::EnableDepthTest(true);
     * @endcode
     */
    static void EnableDepthTest(bool enable);
    
    /**
     * @brief Enables or disables color blending.
     * 
     * Blending allows semi-transparent rendering by combining the color being rendered
     * with the existing color in the framebuffer. Essential for transparency, particles,
     * and UI rendering.
     * 
     * @param enable True to enable blending, false to disable
     * 
     * @note When enabled, uses standard alpha blending (src_alpha, one_minus_src_alpha).
     * @note Enable for transparent objects, particles, and UI elements.
     * 
     * @example
     * @code
     * // Enable for transparent objects
     * GraphicsContext::EnableBlending(true);
     * RenderParticles();
     * RenderUI();
     * 
     * // Disable for opaque geometry
     * GraphicsContext::EnableBlending(false);
     * RenderScene();
     * @endcode
     */
    static void EnableBlending(bool enable);
    
    /**
     * @brief Enables or disables face culling.
     * 
     * Face culling eliminates back-facing triangles from rendering, reducing the
     * number of fragments that need to be processed. Improves performance for
     * closed meshes where only front faces should be visible.
     * 
     * @param enable True to enable culling, false to disable
     * 
     * @note When enabled, back faces are culled by default (set via SetCullFace).
     * @note Improves performance by skipping invisible back-facing triangles.
     * @note Should be enabled for typical opaque geometry.
     * 
     * @example
     * @code
     * // Enable culling for opaque objects
     * GraphicsContext::EnableCulling(true);
     * RenderScene();
     * 
     * // Disable for transparent objects or single-sided geometry
     * GraphicsContext::EnableCulling(false);
     * RenderTransparent();
     * @endcode
     */
    static void EnableCulling(bool enable);
    
    /**
     * @brief Sets which face to cull (back or front).
     * 
     * Determines whether back faces or front faces are culled when face culling
     * is enabled. Back face culling is the standard for most graphics applications.
     * 
     * @param cullBack True to cull back faces (standard), false to cull front faces
     * 
     * @note Face direction is determined by vertex winding order (counter-clockwise = front).
     * @note Only effective when EnableCulling(true) has been called.
     * 
     * @example
     * @code
     * // Standard setup: cull back faces
     * GraphicsContext::EnableCulling(true);
     * GraphicsContext::SetCullFace(true);  // Cull back faces
     * 
     * // For special case: cull front faces instead
     * GraphicsContext::SetCullFace(false);  // Cull front faces
     * @endcode
     */
    static void SetCullFace(bool cullBack);
    
    /**
     * @brief Draws vertices as a list using vertex arrays.
     * 
     * Renders vertices from the bound vertex array without indices.
     * Vertices are drawn in order (0, 1, 2, ..., vertexCount-1).
     * 
     * @param vertexCount Number of vertices to render
     * 
     * @note Requires a bound vertex array with position data.
     * @note Less efficient than indexed drawing for meshes with shared vertices.
     * 
     * @example
     * @code
     * // Draw a triangle
     * VAO.Bind();
     * GraphicsContext::DrawArrays(3);
     * 
     * // Draw a quad
     * VAO.Bind();
     * GraphicsContext::DrawArrays(4);
     * @endcode
     */
    static void DrawArrays(u32 vertexCount);
    
    /**
     * @brief Draws vertices using an index buffer.
     * 
     * Renders vertices according to indices in the bound index buffer.
     * More efficient than DrawArrays() for meshes where vertices are shared
     * between multiple triangles.
     * 
     * @param indexCount Number of indices to render
     * 
     * @note Requires bound vertex array and index buffer.
     * @note Preferred method for rendering meshes and models.
     * 
     * @example
     * @code
     * VAO.Bind();
     * IndexBuffer.Bind();
     * GraphicsContext::DrawIndexed(mesh.GetIndexCount());
     * @endcode
     */
    static void DrawIndexed(u32 indexCount);
    
    /**
     * @brief Gets the graphics hardware vendor name.
     * 
     * Returns a string identifying the GPU manufacturer (e.g., "NVIDIA", "AMD", "Intel").
     * 
     * @return const char* Vendor name string, typically from GL_VENDOR
     * 
     * @example
     * @code
     * const char* vendor = GraphicsContext::GetVendor();
     * ENGINE_INFO << "GPU Vendor: " << vendor;
     * @endcode
     */
    static const char* GetVendor();
    
    /**
     * @brief Gets the graphics hardware renderer name.
     * 
     * Returns a string identifying the specific GPU model
     * (e.g., "GeForce RTX 3080", "Radeon RX 6900 XT").
     * 
     * @return const char* Renderer name string, typically from GL_RENDERER
     * 
     * @example
     * @code
     * const char* renderer = GraphicsContext::GetRenderer();
     * ENGINE_INFO << "GPU Model: " << renderer;
     * @endcode
     */
    static const char* GetRenderer();
    
    /**
     * @brief Gets the OpenGL version string.
     * 
     * Returns the OpenGL version supported by the graphics context
     * (e.g., "4.6.0", "3.3.0").
     * 
     * @return const char* Version string, typically from GL_VERSION
     * 
     * @example
     * @code
     * const char* version = GraphicsContext::GetVersion();
     * ENGINE_INFO << "OpenGL Version: " << version;
     * @endcode
     */
    static const char* GetVersion();

    /**
     * @brief Prints detailed graphics hardware information to the log.
     * 
     * Outputs vendor, renderer model, OpenGL version, and other graphics
     * capability information. Useful for debugging and logging at startup.
     * 
     * @note This is a convenience function that logs the output of GetVendor(),
     *       GetRenderer(), and GetVersion().
     * 
     * @example
     * @code
     * GraphicsContext::Init();
     * GraphicsContext::PrintGLInfo();  // Log graphics info
     * @endcode
     */
    static void PrintGLInfo();
    
private:
    /**
     * @var s_Initialized
     * @brief Static flag indicating whether the graphics context is initialized.
     * 
     * Used to track initialization state and prevent double-initialization or
     * operations before proper setup.
     */
    static bool s_Initialized;
};