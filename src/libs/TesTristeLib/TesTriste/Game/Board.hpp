#pragma once

#include <TesTristeLib/TesTriste/Game/Piece.hpp>

#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT Board {
  public:
    static constexpr int s_width = 15;
    static constexpr int s_height = 40;

    // 3D matrix that represent the presence of piece on the board
    // array[width][height][width] -> array[x][y][z]
    using BoardPresenceMatrix = std::array<std::array<std::array<bool, s_width>, s_height>, s_width>;

    Board() = default;
    Board(const Board& other) = delete;
    Board(Board&& other) = delete;
    Board& operator=(const Board& other) = delete;
    Board& operator=(Board&& other) = delete;

  private:
    BoardPresenceMatrix m_presenceMatrix;
};

}