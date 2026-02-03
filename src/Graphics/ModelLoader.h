#pragma once

#include "../Core/Base.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/**
 * @class ModelLoader
 * @brief Utility class for loading 3D models and meshes from files.
 * 
 * This class uses the Assimp library to load 3D model files in various formats
 * (e.g., OBJ, FBX, STL) and converts them into Mesh objects usable by the engine.
 * It handles parsing the model file, extracting vertex and index data, and
 * creating Mesh instances with associated materials.
 * 
 * Key features:
 * - Load single meshes or entire models with multiple meshes
 * - Support for common 3D file formats via Assimp
 * - Automatic processing of vertices, indices, normals, and texture coordinates
 * 
 * @note Requires Assimp library to be linked with the project.
 * @note Currently does not handle animations or skeletal data.
 * 
 * @example
 * @code
 * Ref<Mesh> mesh = ModelLoader::LoadMesh("path/to/model.obj");
 * std::vector<Ref<Mesh>> modelMeshes = ModelLoader::LoadModel("path/to/model.fbx");
 * @endcode
 */
class ModelLoader {
public:
    /**
     * @brief Loads a single mesh from the specified file.
     * 
     * Uses Assimp to import the model file and extract the first mesh found.
     * Converts the mesh data into a Mesh object.
     * 
     * @param filepath The path to the model file to load
     * @return Ref<Mesh> The loaded mesh object
     * 
     * @note If the file contains multiple meshes, only the first one is loaded.
     * @note Throws an exception if loading fails.
     */
    static Ref<Mesh> LoadMesh(const std::string& filepath);

    /**
     * @brief Loads an entire model from the specified file.
     * 
     * Uses Assimp to import the model file and extract all meshes found.
     * Converts each mesh into a Mesh object and returns a list of meshes.
     * 
     * @param filepath The path to the model file to load
     * @return std::vector<Ref<Mesh>> A vector of loaded mesh objects
     * 
     * @note Supports models with multiple meshes.
     * @note Throws an exception if loading fails.
     */
    static std::vector<Ref<Mesh>> LoadModel(const std::string& filepath);

private:
    /**
     * @brief Recursively processes a node in the Assimp scene graph.
     * 
     * Traverses the node hierarchy, processing each mesh and its children.
     * Converts meshes into Mesh objects and adds them to the provided list.
     * 
     * @param node The current Assimp node to process
     * @param scene The Assimp scene containing the model data
     * @param meshes The vector to store processed Mesh objects
     */
    static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Ref<Mesh>>& meshes);

    /**
     * @brief Processes an individual Assimp mesh and converts it to a Mesh object.
     * 
     * Extracts vertex positions, normals, texture coordinates, and indices
     * from the aiMesh structure and creates a Mesh instance.
     * 
     * @param mesh The Assimp mesh to process
     * @return Ref<Mesh> The created Mesh object
     */
    static Ref<Mesh> ProcessMesh(aiMesh* mesh);

    // Helper functions to load vertices and indices from an aiMesh
    static std::vector<u32> LoadIndices(aiMesh* mesh);
    static std::vector<Vertex> LoadVertices(aiMesh* mesh);
};