#include <unordered_map>
#include <vector>

#include <assimp/scene.h>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class Model {
private:
  std::vector<Mesh> m_Meshes;
  std::unordered_map<std::string, Texture> m_TexturesLoaded;
  const std::string m_Directory;

private:
  // processes a node in a recursive fashion. Processes each individual mesh
  // located at the node and repeats this process on its children nodes (if
  // any).
  void ProcessNode(const aiNode *const node, const aiScene *const scene);

  void ProcessMesh(const aiMesh *const mesh, const aiScene *const scene);

  /*
  checks all material textures of a given type and loads the textures if they're
  not loaded yet the required info is returned as a Texture struct.
  */
  void LoadMaterialTextures(const aiMaterial *const mat,
                            std::vector<std::string> &textures,
                            const aiTextureType type,
                            const char *const typeName);

public:
  // constructor, expects a filepath to a 3D model.
  Model(const std::string &path);

  // draws the model, and thus all its meshes
  void Draw(Shader &shader) {
    for (auto &mesh : m_Meshes)
      mesh.Draw(shader);
  }
};
