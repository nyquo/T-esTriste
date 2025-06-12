#pragma once

#include <glm/glm.hpp>
#include <testristelib_export.h>

namespace TesTriste {

struct TET_EXPORT Vertex {
    Vertex(const glm::vec3& pos = glm::vec3(0.0F, 0.0F, 0.0F),
           const glm::vec3& norm = glm::vec3(0.0F, 0.0F, 1.0F),
           const glm::vec2& tex = glm::vec2(0.0F, 0.0F))
      : position(pos)
      , normal(norm)
      , texCoord(tex) {}

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

}