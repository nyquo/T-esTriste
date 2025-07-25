#pragma once

#include "TesTristeLib/Core/LayerStack.hpp"
#include "TesTristeLib/Core/Types.hpp"
#include "TesTristeLib/Core/gl.hpp"
#include "TesTristeLib/Events/Event.hpp"
#include "TesTristeLib/Events/WindowEvent.hpp"

#include <string>
#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT Window {
  public:
    Window(std::string name = "New Window", Size size = Size{ .width = 800, .height = 800 });
    Window(const Window& other) = delete;
    Window(Window&& other) = delete;
    Window operator=(const Window& other) = delete;
    Window operator=(Window&& other) = delete;
    virtual ~Window();

    void onUpdate();
    void onEvent(Event& event);

    void setEventCallBack(std::function<void(Event&)> callBack);

    unsigned int getWidth() const { return m_size.width; }
    unsigned int getHeight() const { return m_size.height; }
    GLFWwindow* getWindow() { return m_window; }

    void pushLayer(const std::shared_ptr<Layer>& layer);
    void pushOverlayLayer(const std::shared_ptr<Layer>& layer);
    void removeLayer(const std::shared_ptr<Layer>& layer);
    void removeOverlayLayer(const std::shared_ptr<Layer>& layer);

  private:
    bool onWindowResized(TesTriste::WindowResizeEvent& event);

  private:
    static constexpr Size s_minSize{ .width = 400, .height = 300 };
    static constexpr glm::vec4 s_defaultBackgroundColor{ 0.1f, 0.1f, 0.1f, 1.0f };

    static bool s_glfwInitialized;
    static bool s_gladInitialized;
    std::function<void(Event&)> m_eventCallBack;
    std::string m_name;
    Size m_size;
    GLFWwindow* m_window;
    LayerStack m_layerStack{ this };
};

}