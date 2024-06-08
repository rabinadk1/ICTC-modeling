#include "Mesh.hpp"
#include "Renderer.hpp"

Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<unsigned int> &indices,
           const std::unordered_map<std::string, Texture> &texturesLoaded,
           const std::vector<std::string> &textures)
    : m_TexturesLoaded(texturesLoaded), m_Vertices(vertices),
      m_Indices(indices), m_Textures(textures),
      m_IB(&indices[0], indices.size()),
      m_VB(&vertices[0], sizeof(Vertex) * vertices.size()),
      /*
      NOTE: Address of Vector and the address of first element of vector are
      different. Also, sizeof(<vector>) is the size of the vector not containing
      data
      */
      m_VA(m_VB, generateVertextBufferLayout()) {}

void Mesh::Draw(Shader &shader) const {
  // bind appropriate textures
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;

  for (size_t i = 0; i < m_Textures.size(); ++i) {
    const Texture &texture = m_TexturesLoaded.find(m_Textures[i])->second;
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    const auto &textureType = texture.GetType();

    if (textureType == "diffuse")
      number = std::to_string(diffuseNr++);
    else if (textureType == "specular")
      number = std::to_string(specularNr++); // transfer unsigned int to stream
    else
      continue;

    shader.SetUniform(("u_Material." + textureType + number).c_str(),
                      static_cast<int>(i));
    texture.Bind(i);
  }

  // Draw mesh
  Renderer::Draw(m_VA, m_IB);
}
