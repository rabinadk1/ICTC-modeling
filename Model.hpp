#include <vector>
#include <unordered_map>

#include <assimp/scene.h>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class Model
{
private:
  std::vector<Mesh> m_Meshes;
  const std::string m_Directory;
  std::unordered_map<std::string, Texture> m_TexturesLoaded;

private:
  // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
  void ProcessNode(const aiNode *node, const aiScene *scene);

  void ProcessMesh(const aiMesh *mesh, const aiScene *scene);

  /*
  checks all material textures of a given type and loads the textures if they're not loaded yet
  the required info is returned as a Texture struct.
  */
  void LoadMaterialTextures(const aiMaterial *mat, std::vector<std::string> &textures, aiTextureType type, const char *typeName);

public:
  // constructor, expects a filepath to a 3D model.
  Model(const std::string &path);

  // draws the model, and thus all its meshes
  void Draw(Shader &shader)
  {
    for (auto &mesh : m_Meshes)
      mesh.Draw(shader);
  }
};
