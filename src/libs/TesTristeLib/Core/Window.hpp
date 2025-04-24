#pragma once

#include <TesTristeLib/Core/gl.hpp>
#include <TesTristeLib/Events/Event.hpp>
#include <TesTristeLib/TesTristeExport.hpp>

#include <string>

namespace TesTriste {

struct Size {
    unsigned int width;
    unsigned int height;
};

class TET_API Window {
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

    GLFWwindow* getWindow() { return m_window; }

  private:
    static constexpr unsigned int s_minWidth{ 400 };
    static constexpr unsigned int s_minHeight{ 300 };

    static bool s_glfwInitialized;
    static bool s_gladInitialized;
    std::function<void(Event&)> m_eventCallBack;

    std::string m_name;
    unsigned int m_width;
    unsigned int m_height;

    GLFWwindow* m_window;
};

}