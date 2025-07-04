#pragma once

#include <TesTristeLib/Renderer/Mesh.hpp>

#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT BoardGrid : public Mesh {
  public:
    BoardGrid(float cellSize = 1.0F, unsigned int gridWidth = 1U);
    BoardGrid(const BoardGrid& other) = delete;
    BoardGrid(BoardGrid&& other) noexcept = delete;
    BoardGrid& operator=(const BoardGrid& other) = delete;
    BoardGrid& operator=(BoardGrid&& other) noexcept = delete;

    ~BoardGrid() = default;

  private:
    void drawOneCell(glm::vec3 center);
    float m_cellSize;
    unsigned int m_gridWidth;
};

}