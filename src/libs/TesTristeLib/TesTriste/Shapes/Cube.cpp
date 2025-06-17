#include "Cube.hpp"

namespace TesTriste {

Cube::Cube(float size) {
    std::vector<Vertex> vertices;
    vertices.reserve(8);

    // TODO Fix normals and texture coordinates
    for(float z = -size / 2; z <= size / 2; z += size) {
        for(float y = -size / 2; y <= size / 2; y += size) {
            for(float x = -size / 2; x <= size / 2; x += size) {
                vertices.emplace_back(glm::vec3(x, y, z), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
            }
        }
    }

    std::vector<unsigned int> indices;

    // Each face is defined by 4 vertices BL, BR, TL, TR
    const std::vector<std::vector<unsigned int>> faces = {
        { 0, 1, 2, 3 }, // Front face
        { 4, 5, 6, 7 }, // Back face
        { 4, 0, 6, 2 }, // Left face
        { 1, 5, 3, 7 }, // Right face
        { 2, 3, 6, 7 }, // Top face
        { 4, 5, 0, 1 }  // Bottom face
    };

    // Insert triangle in clockwise order for each face
    indices.reserve(faces.size() * 6);
    for(auto face : faces) {
        indices.push_back(face[0]);
        indices.push_back(face[2]);
        indices.push_back(face[1]);
        indices.push_back(face[2]);
        indices.push_back(face[3]);
        indices.push_back(face[1]);
    }

    Mesh::reInit(std::move(vertices), std::move(indices));
}

}