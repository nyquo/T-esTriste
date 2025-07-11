#include "CameraMover.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace TesTriste {

CameraMover::CameraMover(const std::shared_ptr<PerspectiveCamera>& camera)
  : m_camera(camera) {}

void CameraMover::update() {
    auto* currentWindow = glfwGetCurrentContext();
    if(!Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        m_firstMouse = true;
        if(currentWindow && m_enabled) {
            glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    } else {
        if(currentWindow && m_enabled) {
            glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if(!m_camera || !m_enabled) {
        return;
    }
}

void CameraMover::setCamera(const std::shared_ptr<PerspectiveCamera>& camera) { m_camera = camera; }

void CameraMover::onEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraMover::onMouseScrolled));
    dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(CameraMover::onMouseMoved));
}

void CameraMover::enable() { m_enabled = true; }

void CameraMover::disable() { m_enabled = false; }

void CameraMover::moveCamera(float horizontalAngleOffset, float verticalOffset) {
    auto oldCameraPos = m_camera->getPosition();
    float newY = std::clamp(oldCameraPos.y + verticalOffset, m_minHeight, m_maxHeight);
    float newX = oldCameraPos.x;
    float newZ = oldCameraPos.z;

    glm::vec3 newCameraPos =
      rotateAroundPoint(glm::vec3(newX, newY, newZ), glm::vec3(0.0F, 0.0F, 0.0F), glm::radians(horizontalAngleOffset));

    m_camera->setPosition(newCameraPos);
    m_camera->lookAt(glm::vec3(0.0F, 0.0F, 0.0F));
}

glm::vec3 CameraMover::rotateAroundPoint(const glm::vec3& pointToRotate,
                                         const glm::vec3& pivot,
                                         float angleRadians,
                                         const glm::vec3& axis) {
    glm::vec3 relative = pointToRotate - pivot;
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, axis);
    glm::vec3 rotatedRelative = glm::vec3(rotationMatrix * glm::vec4(relative, 1.0));
    return pivot + rotatedRelative;
}

bool CameraMover::onMouseScrolled(MouseScrolledEvent& event) {
    if(!m_camera || !m_enabled) {
        return false;
    }
    m_camera->adustFov(-m_zoomOffset * event.getYOffset());
    return false;
}

bool CameraMover::onMouseMoved(MouseMovedEvent& event) {
    if(!m_camera || !m_enabled) {
        return false;
    }
    if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        if(m_firstMouse) {
            m_lastMouseX = event.getX();
            m_lastMouseY = event.getY();
            m_firstMouse = false;
        }
        float xOffset = m_lastMouseX - event.getX();
        float yOffset = event.getY() - m_lastMouseY;
        m_lastMouseX = event.getX();
        m_lastMouseY = event.getY();
        xOffset *= m_mouseSensitivityX;
        yOffset *= m_mouseSensitivityY;

        moveCamera(xOffset, yOffset);
    }

    return false;
}

}