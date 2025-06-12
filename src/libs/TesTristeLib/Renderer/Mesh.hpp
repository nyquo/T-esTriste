#pragma once

#include <TesTristeLib/Renderer/Buffers.hpp>
#include <TesTristeLib/Renderer/Vertex.hpp>

#include <testristelib_export.h>
#include <vector>

// TODO add support for textures

namespace TesTriste {

class TET_EXPORT Mesh {
  public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices);
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) noexcept;
    ~Mesh();

  private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
};

}