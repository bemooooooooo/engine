#pragma once
/**
 * @file Graphics.h
 * @brief Central header for the Graphics module of the MyGameEngine project.
 * 
 * This header aggregates all core graphics functionalities, including context
 * management, rendering commands, resource handling, and scene representation.
 * It serves as the main entry point for graphics-related operations within
 * the engine.
 * 
 * Key components included:
 *
 * - GraphicsContext: Manages the graphics rendering context and OpenGL state.
 *
 * - RenderCommand: Provides static methods for issuing rendering commands.
 *
 * - Renderer: High-level rendering interface for drawing scenes and objects.
 *
 * - Resource classes: Shader, Texture, VertexBuffer, IndexBuffer, VertexArray.
 *
 * - Scene representation: Scene, Camera, Mesh, Material.
 * 
 * @note This header should be included in any source file that requires
 * graphics functionality from the engine.
 * 
 * @version 1.0
 * @date 2026
 */

// Core
#include "GraphicsContext.h"
#include "RenderCommand.h"
#include "Renderer.h"
#include "Window.h"

// Resources
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

// Scene
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"