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

    vertices.reserve(static_cast<size_t>(verticesPerCell) * gridSideCellCount * gridSideCellCount);
    indices.reserve(static_cast<size_t>(indicesPerCell) * gridSideCellCount * gridSideCellCount);

    int cellIndex = 0;

    float gridWidth = cellSize * static_cast<float>(gridSideCellCount);

    for(float x = 0; x < gridWidth; x += cellSize) {
        for(float z = 0; z < gridWidth; z += cellSize) {
            addOneCell(vertices, indices, glm::vec3{ x, 0, z }, cellIndex);
            cellIndex += 1;
        }
    }

    Mesh::reInit(std::move(vertices), std::move(indices));
}

void BoardGrid::addOneCell(std::vector<Vertex>& vertices,
                           std::vector<unsigned int>& indices,
                           glm::vec3 corner,
                           unsigned int cellIndex) const {
    float margin = m_cellSize / 16;
    float correctedCellSize = m_cellSize - 2 * margin;

    for(float z = margin; z <= correctedCellSize + margin; z += correctedCellSize) {
        for(float y = 0; y <= m_cellheight; y += m_cellheight) {
            for(float x = margin; x <= correctedCellSize + margin; x += correctedCellSize) {
                vertices.emplace_back(
                  glm::vec3(x + corner.x, y, z + corner.z), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec2(0.0F, 0.0F));
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