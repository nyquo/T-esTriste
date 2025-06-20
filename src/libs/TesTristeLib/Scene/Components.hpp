#pragma once

#include <TesTristeLib/Renderer/PerspectiveCamera.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <testristelib_export.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace TesTriste {

struct TET_EXPORT TransformComponent {
    glm::vec3 translation{ 0.0F, 0.0F, 0.0F };
    glm::vec3 rotation{ 0.0F, 0.0F, 0.0F };
    glm::vec3 scale{ 1.0F, 1.0F, 1.0F };

    TransformComponent() = default;
    TransformComponent(const TransformComponent& other) = default;
    TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
      : translation(translation)
      , rotation(rotation)
      , scale(scale) {}

    glm::mat4 getTransformMatrix() const {
        glm::mat4 quatRot = glm::toMat4(glm::quat(rotation));
        return glm::translate(glm::mat4(1.0F), translation) * quatRot * glm::scale(glm::mat4(1.0F), scale);
    }
};

}
