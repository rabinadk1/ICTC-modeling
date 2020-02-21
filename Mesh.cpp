#include "Mesh.hpp"
#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<uint> &indices, std::vector<Texture> &textures)
    : m_Vertices(vertices),
      m_Indices(indices),
      m_Textures(textures),
      m_IB(&indices[0], indices.size()),
      /*
      NOTE: Address of Vector and the address of first element of vector are different.
      Also, sizeof(<vector>) is the size of the vector not containing data
      */
      m_VB(&vertices[0], sizeof(Vertex) * vertices.size())
{
  SetupMesh();
}

void Mesh::SetupMesh() const
{
  VertexBufferLayout layout;

  // For Vertex Positions
  layout.Push<float>(3);

  //For Vertex Normals
  layout.Push<float>(3);

  // For Vertex Texture Coords
  layout.Push<float>(2);

  // For Vertex Tangent
  layout.Push<float>(3);

  // For Vertex Bitangent
  layout.Push<float>(3);

  m_VA.AddBuffer(m_VB, layout);
}

void Mesh::Draw(Shader &shader) const
{
  // bind appropriate textures
  uint diffuseNr = 1;
  uint specularNr = 1;
  uint normalNr = 1;
  uint heightNr = 1;

  for (uint i = 0; i < m_Textures.size(); ++i)
  {
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    const std::string textureType = m_Textures[i].GetType();

    if (textureType == "diffuse")
      number = std::to_string(diffuseNr++);
    else if (textureType == "specular")
      number = std::to_string(specularNr++); // transfer unsigned int to stream
    else if (textureType == "normal")
      number = std::to_string(normalNr++); // transfer unsigned int to stream
    else if (textureType == "height")
      number = std::to_string(heightNr++); // transfer unsigned int to stream

    shader.SetUniform(("texture_" + textureType + number).c_str(), static_cast<int>(i));
    m_Textures[i].Bind(i);
  }

  // Draw mesh
  Renderer::Draw(m_VA, m_IB);
}
