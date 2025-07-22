#pragma once

#include <TesTristeLib/Renderer/Buffers.hpp>
#include <TesTristeLib/Renderer/Vertex.hpp>

#include <memory>
#include <testristelib_export.h>
#include <vector>

// TODO add support for textures

namespace TesTriste {

class TET_EXPORT Mesh {
  public:
    Mesh() = default;
    // TODO mabe take directly the data here instead of vectors?
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices);
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) noexcept;
    virtual ~Mesh();

    size_t getIndicesCount() const { return m_indicesCount; }
    void bind() const;

  protected:
    void reInit(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices);

  private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    std::unique_ptr<VertexArray> m_vertexArray;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;

    size_t m_indicesCount{ 0 };
};

}