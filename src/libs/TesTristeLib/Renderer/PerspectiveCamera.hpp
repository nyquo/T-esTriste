#pragma once

#include "Camera.hpp"
#include "TesTristeLib/Core/Types.hpp"

#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT PerspectiveCamera : public Camera {
  public:
    PerspectiveCamera(Size size = Size{ .width = 0, .height = 0 },
                      glm::vec3 position = glm::vec3{ 0.0F, 0.0F, 0.0F },
                      Orientation = Orientation{ .yaw = -90, .pitch = 0 });

    void setFov(double fov);
    void adustFov(double offset);

    void rotateCamera(double xOffset, double yOffset);
    void translateCamera(glm::vec3 translation);
    void translateCameraRelative(glm::vec3 translation);
    void setPosition(const glm::vec3& position);
    void lookAt(const glm::vec3& target);

    void setViewPortSize(Size size);

    glm::vec3 getPosition() override;
    glm::vec3 getDirection();

  private:
    void updateView();
    void updateProjection();

  private:
    glm::vec3 m_position;
    glm::vec3 m_front{ 0.0F, 0.0F, -1.0F };
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp{ 0.0F, 1.0F, 0.0F };

    double m_yaw;
    double m_pitch;

    double m_near{ 0.1 };
    double m_far{ 150 };

    double m_fov{ 45.0 };
    double m_minFov{ 0.0F };
    double m_maxFov{ 110.0F };

    double m_viewportWidth;
    double m_viewportHeight;

    bool m_constraintPitch{ false };
};

}
