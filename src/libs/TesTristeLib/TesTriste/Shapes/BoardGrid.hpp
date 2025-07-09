#pragma once

#include <TesTristeLib/Renderer/Mesh.hpp>

#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT BoardGrid : public Mesh {
  public:
    BoardGrid(float cellSize = 1.0F, unsigned int gridSideCellCount = 1U);
    BoardGrid(const BoardGrid& other) = delete;
    BoardGrid(BoardGrid&& other) noexcept = delete;
    BoardGrid& operator=(const BoardGrid& other) = delete;
    BoardGrid& operator=(BoardGrid&& other) noexcept = delete;

    ~BoardGrid() = default;

    float getCellHeight() const { return m_cellheight; }

  private:
    void addOneCell(std::vector<Vertex>& vertices,
                    std::vector<unsigned int>& indices,
                    glm::vec3 center,
                    unsigned int cellIndex);

  private:
    float m_cellSize;
    unsigned int m_gridSideCellCount;

    static const unsigned int verticesPerCell = 8;
    static const unsigned int indicesPerCell = 36;
    const float m_cellheight;
};

}