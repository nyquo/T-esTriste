
#include "PerspectiveCamera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace TesTriste {

PerspectiveCamera::PerspectiveCamera(Size size, glm::vec3 position, Orientation orientation)
  : m_viewportWidth(static_cast<double>(size.width))
  , m_viewportHeight(static_cast<double>(size.height))
  , m_position(position)
  , m_yaw(orientation.yaw)
  , m_pitch(orientation.pitch) {
    updateView();
    updateProjection();
}

void PerspectiveCamera::setFov(double fov) {
    m_fov = std::max(m_minFov, std::min(m_maxFov, fov));
    updateProjection();
}

void PerspectiveCamera::adustFov(double offset) { setFov(m_fov + offset); }

void PerspectiveCamera::rotateCamera(double xOffset, double yOffset) {
    m_yaw += xOffset;
    m_pitch += yOffset;

    if(m_constraintPitch) {
        static const double minYaw = -89.0;
        static const double minPitch = 89.0;
        m_pitch = std::max(minYaw, std::min(minPitch, m_pitch));
    }

    updateView();
}

void PerspectiveCamera::translateCamera(glm::vec3 translation) {
    m_position += translation;
    updateView();
}

void PerspectiveCamera::translateCameraRelative(glm::vec3 translation) {
    float cameraYPos = m_position.y;
    m_position += (translation.z * m_front);
    m_position += (translation.x * glm::normalize(glm::cross(m_front, m_up)));

    // prevent x/z movement to make y changes
    m_position.y = cameraYPos;
    m_position.y += translation.y;
    updateView();
}

void PerspectiveCamera::setViewPortSize(Size size) {
    m_viewportWidth = static_cast<double>(size.width);
    m_viewportHeight = static_cast<double>(size.height);
    updateProjection();
}

glm::vec3 PerspectiveCamera::getPosition() { return m_position; }

glm::vec3 PerspectiveCamera::getDirection() { return m_front; }

void PerspectiveCamera::setPosition(const glm::vec3& position) {
    m_position = position;
    updateView();
}

void PerspectiveCamera::lookAt(const glm::vec3& target) {
    m_front = glm::normalize(target - m_position);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    m_yaw = glm::degrees(atan2(m_front.z, m_front.x));
    m_pitch = glm::degrees(asin(m_front.y));

    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}

void PerspectiveCamera::updateView() {
    glm::vec3 direction;
    direction.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    direction.y = static_cast<float>(sin(glm::radians(m_pitch)));
    direction.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    m_front = glm::normalize(direction);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}

void PerspectiveCamera::updateProjection() {
    m_projection = glm::perspective(glm::radians(m_fov), m_viewportWidth / m_viewportHeight, m_near, m_far);
}

}
