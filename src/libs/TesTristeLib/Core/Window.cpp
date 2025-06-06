#include "Window.hpp"

#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/Events/KeyEvent.hpp>
#include <TesTristeLib/Events/MouseEvent.hpp>
#include <TesTristeLib/Events/WindowEvent.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace TesTriste {

bool Window::s_glfwInitialized = false;
bool Window::s_gladInitialized = false;

Window::Window(std::string name, Size size)
  : m_name(std::move(name))
  , m_width(size.width)
  , m_height(size.height) {
    if(!s_glfwInitialized) {
        if(glfwInit() != GLFW_TRUE) {
            Logger::logError("Failed to init GLFW!");
            throw(std::runtime_error("Failed to init GLFW!"));
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    s_glfwInitialized = true;

    m_window =
      glfwCreateWindow(static_cast<int>(m_width), static_cast<int>(m_height), m_name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    glfwSetWindowSizeLimits(m_window, s_minWidth, s_minHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);

    if(!s_gladInitialized) {
        if(!(bool)gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Logger::logError("Failed to initialize GLAD");
            throw(std::runtime_error("Failed to initialize GLAD"));
        }

        // configure global opengl state
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glfwSwapInterval(0);

        s_gladInitialized = true;
    }

    glfwSetWindowUserPointer(m_window, &m_eventCallBack);

    // NOLINTBEGIN(bugprone-easily-swappable-parameters)

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        WindowCloseEvent event;
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        eventCallBack(event);
    });

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowResizeEvent event(width, height);
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        eventCallBack(event);
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        MouseScrolledEvent event(xOffset, yOffset);
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        eventCallBack(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int modifiers) {
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        if(action == GLFW_PRESS) {
            KeyPressedEvent event(key);
            eventCallBack(event);
        }

        if(action == GLFW_RELEASE) {
            KeyReleasedEvent event(key);
            eventCallBack(event);
        }

        if(action == GLFW_REPEAT) {
            KeyPressedEvent event(key, true);
            eventCallBack(event);
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y) {
        MouseMovedEvent event(x, y);
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        eventCallBack(event);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int modifiers) {
        auto eventCallBack = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
        if(action == GLFW_PRESS) {
            MouseButtonPressedEvent event(button);
            eventCallBack(event);
        }

        if(action == GLFW_RELEASE) {
            MouseButtonReleasedEvent event(button);
            eventCallBack(event);
        }
    });

    // NOLINTEND(bugprone-easily-swappable-parameters)

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}
Window::~Window() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Window::onUpdate() {
    glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();
    m_layerStack.onUpdate();
    glfwSwapBuffers(m_window);
}

void Window::onEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<TesTriste::WindowResizeEvent>(BIND_EVENT_FN(Window::onWindowResized));
    m_layerStack.onEvent(event);
}

void Window::setEventCallBack(std::function<void(Event&)> callBack) { m_eventCallBack = std::move(callBack); }

void Window::pushLayer(std::shared_ptr<Layer> layer) { m_layerStack.pushLayer(layer); }
void Window::pushOverlayLayer(std::shared_ptr<Layer> layer) { m_layerStack.pushOverlayLayer(layer); }
void Window::removeLayer(std::shared_ptr<Layer> layer) { m_layerStack.removeLayer(layer); }
void Window::removeOverlayLayer(std::shared_ptr<Layer> layer) { m_layerStack.removeOverlayLayer(layer); }

bool Window::onWindowResized(TesTriste::WindowResizeEvent& e) {
    m_width = e.getWidth();
    m_height = e.getHeight();
    return false;
}
}