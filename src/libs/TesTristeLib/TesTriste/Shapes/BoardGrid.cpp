#include "BoardGrid.hpp"

#include <TesTristeLib/Core/Logger.hpp>

namespace TesTriste {

// TODO make a nicer board grid

BoardGrid::BoardGrid(float cellSize, unsigned int gridSideCellCount)
  : m_cellSize(cellSize)
  , m_gridSideCellCount(gridSideCellCount)
  , m_cellheight(cellSize / 8) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(verticesPerCell * gridSideCellCount * gridSideCellCount);
    indices.reserve(indicesPerCell * gridSideCellCount * gridSideCellCount);

    int cellIndex = 0;

    float gridWidth = cellSize * gridSideCellCount;

    for(float x = -(gridWidth / 2); x < gridWidth / 2; x += cellSize) {
        for(float z = -(gridWidth / 2); z < gridWidth / 2; z += cellSize) {
            addOneCell(vertices, indices, glm::vec3{ x + cellSize / 2, 0, z + cellSize / 2 }, cellIndex);
            cellIndex += 1;
        }
    }

    Mesh::reInit(std::move(vertices), std::move(indices));
}

void BoardGrid::addOneCell(std::vector<Vertex>& vertices,
                           std::vector<unsigned int>& indices,
                           glm::vec3 center,
                           unsigned int cellIndex) {
    float margin = m_cellSize / 16;

    for(float z = -m_cellSize / 2 + margin; z <= m_cellSize / 2 - margin; z += m_cellSize - 2 * margin) {
        for(float y = -m_cellheight / 2; y <= m_cellheight / 2; y += m_cellheight) {
            for(float x = -m_cellSize / 2 + margin; x <= m_cellSize / 2 - margin; x += m_cellSize - 2 * margin) {
                vertices.emplace_back(
                  glm::vec3(x + center.x, y, z + center.z), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
            }
        }
    }

    unsigned int offset = cellIndex * verticesPerCell;

    // Each face is defined by 4 vertices BL, BR, TL, TR
    const std::vector<std::vector<unsigned int>> faces = {
        { offset + 0, offset + 1, offset + 2, offset + 3 }, // Front face
        { offset + 4, offset + 5, offset + 6, offset + 7 }, // Back face
        { offset + 4, offset + 0, offset + 6, offset + 2 }, // Left face
        { offset + 1, offset + 5, offset + 3, offset + 7 }, // Right face
        { offset + 2, offset + 3, offset + 6, offset + 7 }, // Top face
        { offset + 4, offset + 5, offset + 0, offset + 1 }  // Bottom face
    };

    // Insert triangle in clockwise order for each face
    for(auto face : faces) {
        indices.push_back(face[0]);
        indices.push_back(face[2]);
        indices.push_back(face[1]);
        indices.push_back(face[2]);
        indices.push_back(face[3]);
        indices.push_back(face[1]);
    }
}
}