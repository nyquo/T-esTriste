#pragma once

#include "TesTristeLib/Events/Event.hpp"

#include <testristelib_export.h>

namespace TesTriste {

class Window;

class TET_EXPORT Layer {
  public:
    Layer(float layerWidth = 0, float layerHeight = 0);
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

    void setLayerSize(float layerWidth, float layerHeight);

  protected:
    bool m_enabled{ true };

    float m_layerWidth;
    float m_layerHeight;
};

}
