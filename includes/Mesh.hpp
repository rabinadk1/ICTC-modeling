#include <string>
#include <unordered_map>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};

class Mesh {
private:
  // Mesh Data
  const std::unordered_map<std::string, Texture> &m_TexturesLoaded;
  const std::vector<Vertex> &m_Vertices;
  const std::vector<uint> &m_Indices;
  const std::vector<std::string> m_Textures;
  const IndexBuffer m_IB;
  const VertexBuffer m_VB;
  const VertexArray m_VA;

public:
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices,
       const std::unordered_map<std::string, Texture> &texturesLoaded,
       const std::vector<std::string> &textures);
  void Draw(Shader &shader) const;

private:
  inline static VertexBufferLayout generateVertextBufferLayout() {
    VertexBufferLayout layout;

    // For Vertex Positions
    layout.Push<float>(3);

    // For Vertex Normals
    layout.Push<float>(3);

    // For Vertex Texture Coords
    layout.Push<float>(2);

    // For Vertex Tangent
    layout.Push<float>(3);

    // For Vertex Bitangent
    layout.Push<float>(3);

    return layout;
  }
};
