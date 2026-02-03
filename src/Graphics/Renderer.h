#pragma once
#include "../Core/Base.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"
#include "VertexArray.h"

/**
 * @struct SceneData
 * @brief Structure holding per-scene rendering data.
 * 
 * Contains the view-projection matrix used for transforming world coordinates
 * to clip space during rendering.
 */
struct SceneData {
    /**
     * @var viewProjectionMatrix
     * @brief The combined view and projection matrix for the current scene.
     */
    Matrix4 viewProjectionMatrix;
};

/**
 * @class Renderer
 * @brief Core rendering system for managing draw calls and scene rendering.
 * 
 * This class provides static methods for initializing the renderer, beginning
 * and ending scenes, submitting draw calls, and handling window resize events.
 * It manages the rendering state and coordinates with shaders and vertex arrays
 * to render 3D objects to the screen.
 * 
 * Key features:
 * - Scene management (begin/end)
 * - Draw call submission
 * - Clear color and buffer clearing
 * - Window resize handling
 * 
 * @note All methods are static; no instances of Renderer should be created.
 * @note Must be initialized via Init() before use.
 * 
 * @example
 * @code
 * Renderer::Init();
 * 
 * while (running) {
 *     Renderer::BeginScene(camera);
 *     
 *     Renderer::Submit(shader, vertexArray, transform);
 *     
 *     Renderer::EndScene();
 * }
 * 
 * Renderer::Shutdown();
 * @endcode
 */
class Renderer {
public:
    /**
     * @brief Initializes the rendering system.
     * 
     * Must be called once at engine startup before any rendering operations.
     * This function sets up necessary rendering state.
     * 
     * @note This should be called right after creating a window.
     * @note Must be called before any rendering.
     * 
     * @example
     * @code
     * Window window(800, 600, "My Game");
     * Renderer::Init();  // Initialize renderer
     * @endcode
     */
    static void Init();
    
    /**
     * @brief Shuts down the rendering system.
     * 
     * Called at engine shutdown to clean up rendering resources.
     * 
     * @note Should be called once when the application is closing.
     * 
     * @example
     * @code
     * Renderer::Shutdown();  // Clean up renderer
     * @endcode
     */
    static void Shutdown();
    
    /**
     * @brief Begins a new scene for rendering.
     * 
     * Sets up the scene with the provided camera, calculating the view-projection
     * matrix for transforming objects in the scene.
     * 
     * @param camera The camera defining the view and projection for the scene
     * 
     * @note Must be called before submitting any draw calls for the scene.
     * 
     * @example
     * @code
     * Renderer::BeginScene(camera);
     * // ... submit draw calls ...
     * Renderer::EndScene();
     * @endcode
     */
    static void BeginScene(const Camera& camera);

    /**
     * @brief Ends the current scene.
     * 
     * Finalizes rendering for the current scene. Should be called after all
     * draw calls have been submitted.
     * 
     * @note Must be called after BeginScene() and all submissions.
     * 
     * @example
     * @code
     * Renderer::BeginScene(camera);
     * // ... submit draw calls ...
     * Renderer::EndScene();
     * @endcode
     */
    static void EndScene();
    
    /**
     * @brief Submits a draw call to render a vertex array with a shader.
     * 
     * Binds the shader and vertex array, sets necessary uniforms, and issues
     * the draw call to render the object.
     * 
     * @param shader The shader to use for rendering
     * @param vertexArray The vertex array containing geometry data
     * @param transform The transformation matrix to apply to the object (default is identity)
     * 
     * @note Must be called between BeginScene() and EndScene().
     * 
     * @example
     * @code
     * Renderer::BeginScene(camera);
     * Renderer::Submit(shader, vertexArray, transform);
     * Renderer::EndScene();
     * @endcode
     */
    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, 
                      const Matrix4& transform = Matrix4::Identity());
    
    /**
     * @brief Sets the clear color for the framebuffer.
     * 
     * @param color The color to use when clearing the screen
     * 
     * @note Affects the color used in Clear().
     * 
     * @example
     * @code
     * Renderer::SetClearColor(Vector4(0.1f, 0.1f, 0.1f, 1.0f)); // Dark gray
     * @endcode
     */
    static void SetClearColor(const Vector4& color);

    /**
     * @brief Clears the color and depth buffers.
     * 
     * Fills the color buffer with the color set by SetClearColor() and clears
     * the depth buffer. Typically called at the start of each frame.
     * 
     * @note Must be called each frame to clear previous frame's rendering.
     * 
     * @example
     * @code
     * Renderer::Clear();  // Clear both color and depth buffers
     * @endcode
     */
    static void Clear();
    
    /**
     * @brief Handles window resize events.
     * 
     * Updates the viewport and any necessary rendering state when the window
     * size changes.
     * 
     * @param width The new width of the window
     * @param height The new height of the window
     * 
     * @note Should be called whenever the window is resized.
     * 
     * @example
     * @code
     * void OnWindowResize(u32 width, u32 height) {
     *     Renderer::OnWindowResize(width, height);
     * }
     * @endcode
     */
    static void OnWindowResize(u32 width, u32 height);

private:
    /**
     * @var s_SceneData
     * @brief Static instance holding current scene rendering data.
     */
    static Scope<SceneData> s_SceneData;
};