#pragma once

#include <TesTristeLib/TesTriste/Game/Piece.hpp>

#include <glm/glm.hpp>
#include <testristelib_export.h>
#include <vector>

namespace TesTriste {

class PiecePool {
  public:
    PiecePool() {
        std::srand(std::time({}));

        // clang-format off
        m_pieces.emplace_back(std::vector<glm::uvec3>{
            { 0, 0, 0 },
            { 1, 0, 0 },
            { 2, 0, 0 },
            { 3, 0, 0 },
            { 4, 0, 0 }
        });

        /*m_pieces.emplace_back(std::vector<glm::uvec3>{
            { 0, 0, 0 },
            { 1, 0, 0 },
            { 2, 0, 0 },
            { 3, 0, 0 },
            { 0, 1, 0 }
        });

        m_pieces.emplace_back(std::vector<glm::uvec3>{
            { 1, 0, 1 },
            { 1, 1, 1 },
            { 1, 1, 0 },
            { 1, 1, 2 },
            { 0, 1, 1 },
            { 2, 1, 1 }
        });

        m_pieces.emplace_back(std::vector<glm::uvec3>{
            { 0, 0, 0 },
            { 1, 0, 0 },
            { 2, 0, 0 },
            { 0, 0, 1 },
            { 1, 0, 1 },
            { 2, 0, 1 }
        });*/
        // clang-format on
    }
    PiecePool(const PiecePool& other) = delete;
    PiecePool(PiecePool&& other) noexcept = delete;
    PiecePool& operator=(const PiecePool& other) = delete;
    PiecePool& operator=(PiecePool&& other) noexcept = delete;

    const Piece& getRandomPiece() const {
        if(m_pieces.empty()) {
            throw std::runtime_error("No pieces available in the pool");
        }
        size_t randomIndex = std::rand() % m_pieces.size();
        return m_pieces[randomIndex];
    }

  private:
    std::vector<Piece> m_pieces;
};

class ColorPool {
  public:
    ColorPool() {
        m_colors.emplace_back(1.0F, 0.0F, 0.0F);                       // Red
        m_colors.emplace_back(0.0F, 1.0F, 0.0F);                       // Green
        m_colors.emplace_back(0.0F, 0.0F, 1.0F);                       // Blue
        m_colors.emplace_back(1.0F, 1.0F, 0.0F);                       // Yellow
        m_colors.emplace_back(0.901960784F, 0.423529412, 0.749019608); // Purple-ish
        m_colors.emplace_back(1.0F, 1.0F, 1.0F);                       // White
    }
    ColorPool(const ColorPool& other) = delete;
    ColorPool(ColorPool&& other) noexcept = delete;
    ColorPool& operator=(const ColorPool& other) = delete;
    ColorPool& operator=(ColorPool&& other) noexcept = delete;

    const glm::vec3& getRandomColor() const {
        if(m_colors.empty()) {
            throw std::runtime_error("No colors available in the pool");
        }
        size_t randomIndex = std::rand() % m_colors.size();
        return m_colors[randomIndex];
    }

  private:
    std::vector<glm::vec3> m_colors;
};

}