#pragma once

#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/Core/gl.hpp>
#include <TesTristeLib/Io/stb_image.h>
#include <TesTristeLib/Renderer/Buffers.hpp>

#include <array>
#include <filesystem>
#include <memory>
#include <testristelib_export.h>
#include <vector>

namespace TesTriste {

class CubeMap {
  private:
    unsigned int m_id{ 0 };
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    std::unique_ptr<VertexArray> m_vertexArray;

    size_t m_indicesCount{ 0 };

  public:
    CubeMap(std::vector<std::filesystem::path> faces) {
        if(faces.size() != 6) {
            Logger::logError("CubeMap requires exactly 6 faces.");
        }

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
        int width, height, nbChannels;
        for(unsigned int i = 0; i < faces.size(); ++i) {
            unsigned char* data = stbi_load(faces[i].string().c_str(), &width, &height, &nbChannels, 0);

            if(data) {
                glTexImage2D(
                  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            } else {
                Logger::logError("Failed to load texture at path: ", faces[i].string());
            }
            stbi_image_free(data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // clang-format off
        std::vector<float> vertices = {
            -1.0f,  1.0f, -1.0f,  // 0
            -1.0f, -1.0f, -1.0f,  // 1
            1.0f, -1.0f, -1.0f,  // 2
            1.0f,  1.0f, -1.0f,  // 3
            -1.0f,  1.0f,  1.0f,  // 4
            -1.0f, -1.0f,  1.0f,  // 5
            1.0f, -1.0f,  1.0f,  // 6
            1.0f,  1.0f,  1.0f   // 7
        };
        
        std::vector<unsigned int> indices = {
            // Back face
            0, 1, 2,
            2, 3, 0,
            // Front face
            4, 5, 6,
            6, 7, 4,
            // Left face
            4, 5, 1,
            1, 0, 4,
            // Right face
            3, 2, 6,
            6, 7, 3,
            // Top face
            4, 0, 3,
            3, 7, 4,
            // Bottom face
            1, 5, 6,
            6, 2, 1
        };

        // clang-format on

        m_indicesCount = indices.size();

        m_vertexBuffer = std::make_unique<VertexBuffer>(sizeof(float) * vertices.size(), (void*)vertices.data());
        m_indexBuffer = std::make_unique<IndexBuffer>(indices.size(), indices.data());
        m_vertexArray = std::make_unique<VertexArray>();
        BufferLayout layout{ BufferElement(GL_FLOAT, 3, false, sizeof(float)) };
        m_vertexBuffer->setLayout(std::move(layout));
        m_vertexArray->addVertexBuffer(*m_vertexBuffer);
        m_vertexArray->setIndexBuffer(*m_indexBuffer);
        glBindVertexArray(0);
    }
    CubeMap(const CubeMap& other) = delete;
    CubeMap(CubeMap&& other) = delete;
    CubeMap& operator=(const CubeMap& other) = delete;
    CubeMap& operator=(CubeMap&& other) = delete;
    ~CubeMap() {
        if(m_id != 0) {
            glDeleteTextures(1, &m_id);
        }
    }

    // TODO rename or move func like bind
    void bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_id); }
    void unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }
    void draw() const {
        m_vertexArray->bind();
        bind();
        glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
        unbind();
        m_vertexArray->unbind();
    }
};
}