#pragma once

#include "TesTristeLib/Core/Layer.hpp"
#include "TesTristeLib/Events/Event.hpp"
#include "TesTristeLib/TesTristeExport.hpp"

#include <memory>
#include <vector>

namespace TesTriste {

class TET_API LayerStack {
  public:
    LayerStack(Window* window = nullptr);
    LayerStack(const LayerStack& other) = delete;
    LayerStack(LayerStack&& other) = delete;
    LayerStack operator=(const LayerStack& other) = delete;
    LayerStack operator=(LayerStack&& other) = delete;
    ~LayerStack() = default;

    void pushLayer(const std::shared_ptr<Layer>& layer);
    void pushOverlayLayer(const std::shared_ptr<Layer>& layer);
    void removeLayer(const std::shared_ptr<Layer>& layer);
    void removeOverlayLayer(const std::shared_ptr<Layer>& layer);

    void onUpdate();
    void onEvent(Event& e);

  private:
    std::vector<std::shared_ptr<Layer>> m_layers;
    std::vector<std::shared_ptr<Layer>> m_uiLayers;

    Window* m_window;
};

}
