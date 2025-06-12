#include "Mesh.hpp"

namespace TesTriste {

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices)
  : m_vertices(std::move(vertices))
  , m_indices(std::move(indices))
  , m_vertexBuffer(m_vertices.size() * sizeof(Vertex), m_vertices.data())
  , m_indexBuffer(m_indices.size(), m_indices.data()) {
    BufferLayout layout{ BufferElement(GL_FLOAT, 3, false, sizeof(float)),
                         BufferElement(GL_FLOAT, 3, false, sizeof(float)),
                         BufferElement(GL_FLOAT, 2, false, sizeof(float)) };
    m_vertexBuffer.setLayout(std::move(layout));
    m_vertexArray.addVertexBuffer(m_vertexBuffer);
    m_vertexArray.setIndexBuffer(m_indexBuffer);

    // TODO maybe delete vertices and indices after creating buffers?
    glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& other) noexcept
  : m_vertices(std::move(other.m_vertices))
  , m_indices(std::move(other.m_indices))
  , m_vertexBuffer(std::move(other.m_vertexBuffer))
  , m_indexBuffer(std::move(other.m_indexBuffer))
  , m_vertexArray(std::move(other.m_vertexArray)) {}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    m_vertices = std::move(other.m_vertices);
    m_indices = std::move(other.m_indices);
    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_indexBuffer = std::move(other.m_indexBuffer);
    m_vertexArray = std::move(other.m_vertexArray);

    return *this;
}

Mesh::~Mesh() {}

}