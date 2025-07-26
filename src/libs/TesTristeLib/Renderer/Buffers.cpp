#include "Buffers.hpp"

#include <cassert>

namespace TesTriste {

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
  : m_elements(elements) {
    size_t offset = 0;
    m_stride = 0;
    for(auto& element : m_elements) {
        element.m_offset = offset;
        offset += element.m_count * element.m_size;
        m_stride += element.m_count * element.m_size;
    }
}

VertexBuffer::VertexBuffer(size_t size, void* data) {
    glGenBuffers(1, &m_id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
  : m_id(other.m_id)
  , m_layout(std::move(other.m_layout)) {
    other.m_id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    m_id = other.m_id;
    m_layout = std::move(other.m_layout);
    other.m_id = 0;

    return *this;
}

VertexBuffer::~VertexBuffer() {
    if(m_id != 0) {
        glDeleteBuffers(1, &m_id);
    }
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::setLayout(BufferLayout&& layout) { m_layout = std::move(layout); }

IndexBuffer::IndexBuffer(size_t count, unsigned int* indices)
  : m_count(count) {
    glGenBuffers(1, &m_id);

    // Idea taken from: https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Platform/OpenGL/OpenGLBuffer.cpp
    // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
    // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(unsigned int)), indices, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
  : m_id(other.m_id) {
    other.m_id = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

IndexBuffer::~IndexBuffer() {
    if(m_id != 0) {
        glDeleteBuffers(1, &m_id);
    }
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

void IndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

VertexArray::VertexArray() { glGenVertexArrays(1, &m_id); }

VertexArray::VertexArray(VertexArray&& other) noexcept
  : m_id(other.m_id) {
    other.m_id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

VertexArray::~VertexArray() {
    if(m_id != 0) {
        glDeleteVertexArrays(1, &m_id);
    }
}

void VertexArray::addVertexBuffer(VertexBuffer& vertexBuffer) const {
    assert(vertexBuffer.getLayout().getElements().size() > 0);

    bind();
    vertexBuffer.bind();

    unsigned int index = 0;
    size_t stride = vertexBuffer.getLayout().getStride();
    for(const auto& element : vertexBuffer.getLayout().getElements()) {
        // We consider GL_NONE to be an additional offset for unused data
        if(element.m_dataType != GL_NONE) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  static_cast<GLint>(element.m_count),
                                  element.m_dataType,
                                  static_cast<GLboolean>(element.m_normalize),
                                  static_cast<GLsizei>(stride),
                                  // NOLINTNEXTLINE(performance-no-int-to-ptr)
                                  (void*)element.m_offset);

            index++;
        }
    }
}

void VertexArray::setIndexBuffer(IndexBuffer& indexBuffer) const {
    bind();
    indexBuffer.bind();
}

void VertexArray::bind() const { glBindVertexArray(m_id); }

void VertexArray::unbind() { glBindVertexArray(0); }

}
