#include "Piece.hpp"

namespace TesTriste {

Piece::Piece(CubePresenceMatrix&& presenceMatrix, glm::vec3 color)
  : m_presenceMatrix(std::move(presenceMatrix))
  , m_color(color) {}

}