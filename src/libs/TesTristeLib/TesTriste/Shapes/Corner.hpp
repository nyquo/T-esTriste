#pragma once

#include <TesTristeLib/Renderer/Mesh.hpp>

#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT Corner : public Mesh {
  public:
    Corner(float size = 1.0F) {
        const size_t nbVertices{ 22 };
        std::vector<Vertex> vertices;
        vertices.reserve(nbVertices);

        // 0
        vertices.emplace_back(glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(size, 0.0F, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(0.0F, size, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(size, size, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(0.0F, 2 * size, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(size, 2 * size, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(2 * size, 2 * size, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(2 * size, size, 0.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));

        // 8
        vertices.emplace_back(glm::vec3(0.0F, 2 * size, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(0.0F, 2 * size, 2 * -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(size, 2 * size, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(size, 2 * size, 2 * -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(2 * size, 2 * size, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));

        // 13
        vertices.emplace_back(glm::vec3(0.0F, 0.0F, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(0.0F, size, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(0.0F, 2 * size, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(0.0F, size, 2 * -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(0.0F, 2 * size, 2 * -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));

        // 18
        vertices.emplace_back(glm::vec3(size, 0.0F, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(2 * size, size, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
        vertices.emplace_back(glm::vec3(size, size, 2 * -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));

        // 21
        vertices.emplace_back(glm::vec3(size, size, -size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));

        // const size_t nbIndices{}
        std::vector<unsigned int> indices;

        // Each face is defined by 4 vertices BL, BR, TL, TR
        const std::vector<std::vector<unsigned int>> faces = {
            { 0, 1, 2, 3 },     { 2, 3, 4, 5 },     { 3, 7, 5, 6 },    { 4, 5, 8, 10 },   { 5, 6, 10, 12 },
            { 8, 10, 9, 11 },   { 13, 0, 14, 2 },   { 14, 2, 8, 4 },   { 16, 14, 17, 8 }, { 1, 18, 3, 21 },
            { 13, 18, 0, 1 },   { 18, 13, 21, 14 }, { 21, 19, 3, 7 },  { 7, 19, 6, 12 },  { 19, 21, 12, 10 },
            { 21, 14, 20, 16 }, { 21, 20, 10, 11 }, { 20, 16, 11, 17 }
        };

        // Insert triangle in clockwise order for each face
        indices.reserve(faces.size() * faces.at(0).size());
        for(auto face : faces) {
            indices.push_back(face[0]);
            indices.push_back(face[2]);
            indices.push_back(face[1]);
            indices.push_back(face[2]);
            indices.push_back(face[3]);
            indices.push_back(face[1]);
        }

        Mesh::reInit(std::move(vertices), std::move(indices));
        m_size = glm::vec3(2 * size, 2 * size, 2 * size);
    }
    Corner(const Corner& other) = delete;
    Corner(Corner&& other) noexcept = delete;
    Corner& operator=(const Corner& other) = delete;
    Corner& operator=(Corner&& other) noexcept = delete;

    ~Corner() = default;
};

}