#pragma once

#include "TesTristeLib/Core/Types.hpp"
#include "TesTristeLib/Events/Event.hpp"

#include <testristelib_export.h>

namespace TesTriste {

class Window;

class TET_EXPORT Layer {
  public:
    Layer(Size size = Size{ .width = 800, .height = 600 });
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

    void setLayerSize(Size size);

  protected:
    bool m_enabled{ true };
    Size m_layerSize{ .width = 0, .height = 0 };
};

}
