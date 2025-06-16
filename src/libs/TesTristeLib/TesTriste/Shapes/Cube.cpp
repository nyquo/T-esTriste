#include "Cube.hpp"

namespace TesTriste {

Cube::Cube(float size) {
    std::vector<Vertex> vertices;
    vertices.reserve(8);

    // TODO fix normals

    // Front face CCW
    vertices.emplace_back(glm::vec3(0, 0, 0), glm::vec3(-1.0F, -1.0F, -1.0F), glm::vec2(0.0F, 0.0F));
    vertices.emplace_back(glm::vec3(size, 0, 0), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertices.emplace_back(glm::vec3(size, size, 0), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertices.emplace_back(glm::vec3(0, size, 0), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    // Back face CW
    vertices.emplace_back(glm::vec3(0, 0, size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertices.emplace_back(glm::vec3(size, 0, size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertices.emplace_back(glm::vec3(size, size, size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
    vertices.emplace_back(glm::vec3(0, size, size), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));

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

    Mesh::reInit(std::move(vertices), std::move(indices));
}

}