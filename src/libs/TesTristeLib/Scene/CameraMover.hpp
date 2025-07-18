#pragma once

#include <TesTristeLib/Core/Input.hpp>
#include <TesTristeLib/Events/Event.hpp>
#include <TesTristeLib/Events/MouseEvent.hpp>
#include <TesTristeLib/Renderer/PerspectiveCamera.hpp>

#include <glm/glm.hpp>
#include <memory>
#include <testristelib_export.h>

namespace TesTriste {

// TODO Init CenterPoint distance and min/max in constuctor
class TET_EXPORT CameraMover {
  public:
    CameraMover() = default;
    CameraMover(const std::shared_ptr<PerspectiveCamera>& camera);
    CameraMover(const CameraMover& other) = delete;
    CameraMover(CameraMover&& other) = delete;
    CameraMover operator=(const CameraMover& other) = delete;
    CameraMover operator=(CameraMover&& other) = delete;
    virtual ~CameraMover() = default;

    void onEvent(Event& e);

    void setCamera(const std::shared_ptr<PerspectiveCamera>& camera);
    void update();
    void enable();
    void disable();
    void moveCamera(float horizontalAngleOffset, float verticalOffset);

  private:
    glm::vec3 rotateAroundPoint(const glm::vec3& pointToRotate,
                                const glm::vec3& pivot,
                                float angleRadians,
                                const glm::vec3& axis = glm::vec3(0.0F, 1.0F, 0.0F));

    bool onMouseScrolled(MouseScrolledEvent& event);
    bool onMouseMoved(MouseMovedEvent& event);

  private:
    std::shared_ptr<PerspectiveCamera> m_camera;

    float m_zoomOffset{ 2.0 };
    bool m_enabled{ true };
    float m_lastMouseX{ 0 };
    float m_lastMouseY{ 0 };
    bool m_firstMouse{ true };
    float m_mouseSensitivityX{ 0.2 };
    float m_mouseSensitivityY{ 0.02 };

    float m_centerPointDistance{ 10.0F };
    float m_minHeight{ 10.0F };
    float m_maxHeight{ 30.0F };
};

}