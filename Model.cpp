#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include "Model.hpp"

Model::Model(const std::string &path)
    : m_Directory(path.substr(0, path.find_last_of("/"))) // retrieve the directory path of the filepath
{
  // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
  // read file via ASSIMP
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

  // check for errors
  if (!scene or scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  // process ASSIMP's root node recursively
  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(const aiNode *node, const aiScene *scene)
{
  // process each mesh located at the current node
  for (uint i = 0; i < node->mNumMeshes; ++i)
  {
    // the node object only contains indices to index the actual objects in the scene.
    // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
    const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    ProcessMesh(mesh, scene);
  }
  // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
  for (uint i = 0; i < node->mNumChildren; ++i)
    ProcessNode(node->mChildren[i], scene);
}

void Model::ProcessMesh(const aiMesh *mesh, const aiScene *scene)
{
  // data to fill
  std::vector<Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);
  std::vector<uint> indices;
  std::vector<std::string> textures;

  // Walk through each of the mesh's vertices
  for (uint i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex vertex;
    // aiVector is container for positions and normals of assimp which cannot be converted to glm::vec3 directly
    // For positions
    aiVector3D aiVector = mesh->mVertices[i];
    vertex.Position = glm::vec3(aiVector.x, aiVector.y, aiVector.z);

    // For normals
    aiVector = mesh->mNormals[i];
    vertex.Normal = glm::vec3(aiVector.x, aiVector.y, aiVector.z);

    // For tangent
    aiVector = mesh->mTangents[i];
    vertex.Tangent = glm::vec3(aiVector.x, aiVector.y, aiVector.z);

    // For bitangent
    aiVector = mesh->mBitangents[i];
    vertex.Bitangent = glm::vec3(aiVector.x, aiVector.y, aiVector.z);

    // Texture Coordinates
    if (mesh->mTextureCoords[0]) // check if the mesh contains texture cooordinates
    {
      /*
      A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
      use models where a vertex can have multiple texture coordinates so we always take the first set (0).
      */
      aiVector = mesh->mTextureCoords[0][i];
      vertex.TexCoords = glm::vec2(aiVector.x, aiVector.y);
    }
    else
      vertex.TexCoords = glm::vec2(0.f);

    vertices.push_back(vertex);
  }

  // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
  for (uint i = 0; i < mesh->mNumFaces; ++i)
  {
    const aiFace &face = mesh->mFaces[i];
    // retrieve all indices of the face and store them in the indices vector
    for (uint j = 0; j < face.mNumIndices; ++j)
      indices.push_back(face.mIndices[j]);
  }

  //process materials
  const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  /*
    we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    Same applies to other texture as the following list summarizes:
    diffuse: texture_diffuseN
    specular: texture_specularN
    normal: texture_normalN
    */

  //Diffuse Maps
  LoadMaterialTextures(material, textures, aiTextureType_DIFFUSE, "diffuse");

  // Specular Maps
  LoadMaterialTextures(material, textures, aiTextureType_SPECULAR, "specular");

  // Normal maps
  // LoadMaterialTextures(material, textures, aiTextureType_HEIGHT, "normal");

  // Height maps
  // LoadMaterialTextures(material, textures, aiTextureType_AMBIENT, "height");

  // return a mesh object created from the extracted mesh data
  m_Meshes.emplace_back(vertices, indices, m_TexturesLoaded, textures);
}

void Model::LoadMaterialTextures(const aiMaterial *mat, std::vector<std::string> &textures, aiTextureType type, const char *typeName)
{
  for (uint i = 0; i < mat->GetTextureCount(type); ++i)
  {
    aiString path;
    mat->GetTexture(type, i, &path);
    std::string pathString = path.C_Str();
    const auto loadedTexture = m_TexturesLoaded.find(pathString);
    if (loadedTexture == m_TexturesLoaded.end())
    {
      m_TexturesLoaded.emplace(std::piecewise_construct,
                               std::forward_as_tuple(pathString),
                               std::forward_as_tuple(m_Directory + '/' + pathString,
                                                     typeName));
    }
    textures.push_back(pathString);
  }
}
