#pragma once

#include <glm/glm.hpp>
#include <testristelib_export.h>
#include <vector>

namespace TesTriste {

class TET_EXPORT Piece {
  public:
    Piece(std::vector<glm::uvec3> cubePositions) {
        if(cubePositions.empty()) {
            return;
        }
        size_t minX = cubePositions[0].x;
        size_t maxX = cubePositions[0].x;
        size_t minY = cubePositions[0].y;
        size_t maxY = cubePositions[0].y;
        size_t minZ = cubePositions[0].z;
        size_t maxZ = cubePositions[0].z;

        for(const auto& pos : cubePositions) {
            if(pos.x < minX)
                minX = pos.x;
            if(pos.x > maxX)
                maxX = pos.x;
            if(pos.y < minY)
                minY = pos.y;
            if(pos.y > maxY)
                maxY = pos.y;
            if(pos.z < minZ)
                minZ = pos.z;
            if(pos.z > maxZ)
                maxZ = pos.z;
        }

        m_width = maxX - minX + 1;
        m_height = maxY - minY + 1;
        m_depth = maxZ - minZ + 1;

        m_cubePositions = std::move(cubePositions);
    }
    Piece(const Piece& other) = default;
    Piece(Piece&& other) noexcept = default;
    Piece& operator=(const Piece& other) = default;
    Piece& operator=(Piece&& other) noexcept = default;

    std::vector<glm::uvec3> getCubePositions() const { return m_cubePositions; }
    size_t getWidth() const { return m_width; }
    size_t getHeight() const { return m_height; }
    size_t getDepth() const { return m_depth; }

  private:
    std::vector<glm::uvec3> m_cubePositions;
    size_t m_width{ 0 };
    size_t m_height{ 0 };
    size_t m_depth{ 0 };
};

}