#pragma once

#include <glm/glm.hpp>
#include <testristelib_export.h>

namespace TesTriste {

using Color = glm::vec3;

struct TET_EXPORT Size {
    unsigned int width;
    unsigned int height;
};

struct TET_EXPORT Orientation {
    double yaw;
    double pitch;
};

}
