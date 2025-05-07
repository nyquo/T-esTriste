#pragma once

#include "TesTristeLib/Events/Event.hpp"
#include "TesTristeLib/TesTristeExport.hpp"

namespace TesTriste {

class Window;

class TET_API Layer {
  public:
    Layer() = default;
    Layer(const Layer& other) = delete;
    Layer(Layer&& other) = delete;
    Layer operator=(const Layer& other) = delete;
    Layer operator=(Layer&& other) = delete;
    virtual ~Layer() = default;

    virtual void onEvent(Event& event) {};
    virtual void onUpdate() {};
    virtual void onImGuiRender() {};

    [[nodiscard]] bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }

  protected:
    bool m_enabled{ true };
};

}
