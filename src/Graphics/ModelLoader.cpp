#include "ModelLoader.h"

#include "../Core/Log.h"

Ref<Mesh> ModelLoader::LoadMesh(const std::string& filepath) {
  auto meshes = LoadModel(filepath);
  return meshes.empty() ? nullptr : meshes[0];
}

std::vector<Ref<Mesh>> ModelLoader::LoadModel(const std::string& filepath) {
  std::vector<Ref<Mesh>> meshes;

  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs |
                                      aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    ENGINE_ERROR << "Assimp error: " << importer.GetErrorString();
    return meshes;
  }

  ProcessNode(scene->mRootNode, scene, meshes);
  ENGINE_INFO << "Loaded model: " << filepath << " (" << meshes.size()
              << " meshes)";

  return meshes;
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene,
                              std::vector<Ref<Mesh>>& meshes) {
  // Process each mesh located at the current node
  for (u32 i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(ProcessMesh(mesh));
  }

  // Process each of the children nodes
  for (u32 i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene, meshes);
  }
}

Ref<Mesh> ModelLoader::ProcessMesh(aiMesh* mesh) {
  std::vector<Vertex> vertices = LoadVertices(mesh);
  std::vector<u32> indices = LoadIndices(mesh);
  return CreateRef<Mesh>(vertices, indices);
}

std::vector<Vertex> ModelLoader::LoadVertices(aiMesh* mesh) {
  std::vector<Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);

  for (u32 i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    // Position
    vertex.position.x = mesh->mVertices[i].x;
    vertex.position.y = mesh->mVertices[i].y;
    vertex.position.z = mesh->mVertices[i].z;

    // Normal
    if (mesh->HasNormals()) {
      vertex.normal.x = mesh->mNormals[i].x;
      vertex.normal.y = mesh->mNormals[i].y;
      vertex.normal.z = mesh->mNormals[i].z;
    }

    // Texture coordinates
    if (mesh->mTextureCoords[0]) {
      vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
      vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
    }

    // Tangents and bitangents
    if (mesh->HasTangentsAndBitangents()) {
      vertex.tangent.x = mesh->mTangents[i].x;
      vertex.tangent.y = mesh->mTangents[i].y;
      vertex.tangent.z = mesh->mTangents[i].z;

      vertex.bitangent.x = mesh->mBitangents[i].x;
      vertex.bitangent.y = mesh->mBitangents[i].y;
      vertex.bitangent.z = mesh->mBitangents[i].z;
    }

    vertices.push_back(vertex);
  }

  return vertices;
}

std::vector<u32> ModelLoader::LoadIndices(aiMesh* mesh) {
  std::vector<u32> indices;

  for (u32 i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (u32 j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  return indices;
}