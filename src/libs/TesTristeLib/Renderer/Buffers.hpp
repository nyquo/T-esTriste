#pragma once

#include "TesTristeLib/Core/gl.hpp"

#include <testristelib_export.h>
#include <vector>

namespace TesTriste {

struct TET_EXPORT BufferElement {
    BufferElement(unsigned int dataType, size_t count, bool normalize, size_t size)
      : m_dataType(dataType)
      , m_count(count)
      , m_normalize(normalize)
      , m_size(size)
      , m_offset(0) {}
    unsigned int m_dataType;
    size_t m_count;
    bool m_normalize;
    size_t m_size;
    size_t m_offset;
};

class TET_EXPORT BufferLayout {
  public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    const std::vector<BufferElement>& getElements() const { return m_elements; }
    size_t getStride() const { return m_stride; }

  private:
    std::vector<BufferElement> m_elements;
    size_t m_stride{ 0 };
};

class TET_EXPORT VertexBuffer {
  public:
    VertexBuffer(size_t size, void* data);
    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;
    ~VertexBuffer();

    void bind() const;
    static void unbind();

    const BufferLayout& getLayout() const { return m_layout; }
    void setLayout(BufferLayout&& layout);

  private:
    unsigned int m_id{ 0 };
    BufferLayout m_layout;
};

class TET_EXPORT IndexBuffer {
  public:
    IndexBuffer(size_t count, unsigned int* indices);
    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;
    ~IndexBuffer();

    void bind() const;
    static void unbind();

    size_t getCount() const { return m_count; }

  private:
    unsigned int m_id{ 0 };
    size_t m_count;
};

class TET_EXPORT VertexArray {
  public:
    VertexArray();
    VertexArray(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(const VertexArray& other) = delete;
    VertexArray& operator=(VertexArray&& other) noexcept;
    ~VertexArray();

    void addVertexBuffer(VertexBuffer& vertexBuffer) const;
    void setIndexBuffer(IndexBuffer& indexBuffer) const;

    void bind() const;
    static void unbind();

  private:
    unsigned int m_id{ 0 };
};

}
