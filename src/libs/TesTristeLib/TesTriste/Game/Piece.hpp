#pragma once

#include <array>
#include <glm/glm.hpp>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT Piece {
  public:
    constexpr static int s_maxSize = 5;
    using CubePresenceMatrix = std::array<std::array<std::array<bool, s_maxSize>, s_maxSize>, s_maxSize>;

    Piece(CubePresenceMatrix&& presenceMatrix, glm::vec3 color);

    CubePresenceMatrix getPresenceMatrix() const { return m_presenceMatrix; }
    glm::vec3 getColor() const { return m_color; }

  private:
    CubePresenceMatrix m_presenceMatrix;
    glm::vec3 m_color;
};

}