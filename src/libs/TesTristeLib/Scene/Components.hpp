#pragma once

#include <TesTristeLib/Renderer/MeshManager.hpp>
#include <TesTristeLib/Renderer/PerspectiveCamera.hpp>
#include <TesTristeLib/Renderer/ShaderManager.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <testristelib_export.h>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace TesTriste {

struct TET_EXPORT TransformComponent {
    glm::vec3 translation{ 0.0F, 0.0F, 0.0F };
    glm::vec3 rotation{ 0.0F, 0.0F, 0.0F };
    glm::vec3 scale{ 1.0F, 1.0F, 1.0F };

    TransformComponent() = default;
    TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
      : translation(translation)
      , rotation(rotation)
      , scale(scale) {}

    glm::mat4 getTransformMatrix() const {
        glm::mat4 quatRot = glm::toMat4(glm::quat(rotation));
        return glm::translate(glm::mat4(1.0F), translation) * quatRot * glm::scale(glm::mat4(1.0F), scale);
    }
};

struct TET_EXPORT MeshComponent {
    MeshManager::MeshID meshID;
    ShaderManager::ShaderID shaderID;
    // unordered_map<std::string,

    MeshComponent() = delete;
    MeshComponent(MeshManager::MeshID id)
      : meshID(id) {}
};

}
