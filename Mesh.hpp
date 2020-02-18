#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

struct Vertex
{
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

class Mesh
{
private:
  // Mesh Data
  const std::vector<Vertex> m_Vertices;
  const std::vector<uint> m_Indices;
  const std::vector<Texture> m_Textures;
  const VertexArray m_VA;
  const IndexBuffer m_IB;

private:
  void SetupMesh() const;

public:
  Mesh(std::vector<Vertex> &vertices, std::vector<uint> &indices, std::vector<Texture> &textures);
  void Draw(Shader &shader) const;
};
