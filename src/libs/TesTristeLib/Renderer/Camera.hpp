#pragma once

#include <glm/glm.hpp>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT Camera {
  public:
    Camera(const glm::mat4& projection = glm::mat4(1.0F), const glm::mat4& view = glm::mat4(1.0F))
      : m_projection(projection)
      , m_view(view) {}
    virtual ~Camera() = default;

    [[nodiscard]] virtual glm::vec3 getPosition() = 0;
    [[nodiscard]] glm::mat4 getProjection() const { return m_projection; }
    [[nodiscard]] glm::mat4 getView() const { return m_view; }

  protected:
    glm::mat4 m_projection;
    glm::mat4 m_view;
};

}
