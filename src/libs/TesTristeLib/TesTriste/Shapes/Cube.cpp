#include "Cube.hpp"

namespace TesTriste {

Cube::Cube(float size) {
    std::vector<Vertex> vertex;
    vertex.reserve(8);

    // TODO fix normals

    // Front face CCW
    vertex.emplace_back(glm::vec3(0, 0, 0), glm::vec3(-1.0F, -1.0F, -1.0F), glm::vec2(0.0F, 0.0F));
    vertex.emplace_back(glm::vec3(size, 0, 0), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertex.emplace_back(glm::vec3(size, size, 0), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertex.emplace_back(glm::vec3(0, size, 0), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    // Back face CW
    vertex.emplace_back(glm::vec3(0, 0, size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertex.emplace_back(glm::vec3(size, 0, size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertex.emplace_back(glm::vec3(size, size, size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertex.emplace_back(glm::vec3(0, size, size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));

    std::vector<unsigned int> indices;
    indices.reserve(36);

    // All in CW order

    // Front face
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);
    // Back face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(7);
    // Left face
    indices.push_back(0);
    indices.push_back(7);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(7);
    // Right face
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(1);
    indices.push_back(6);
    indices.push_back(5);
    // Top face
    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(6);
    indices.push_back(3);
    indices.push_back(6);
    indices.push_back(2);
    // Bottom face
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(4);

    Mesh::upload(vertex,
                 BufferLayout{ BufferElement(GL_FLOAT, 3, false, sizeof(float) * 3),
                               BufferElement(GL_FLOAT, 3, false, sizeof(float) * 3),
                               BufferElement(GL_FLOAT, 2, false, sizeof(float) * 2) },
                 indices);
}

}