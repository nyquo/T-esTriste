#include "Application.hpp"

namespace TesTriste {

Application::Application(std::string mainWindowName)
  : m_mainWindow(std::make_unique<Window>(std::move(mainWindowName))) {
    m_mainWindow->setEventCallBack(BIND_EVENT_FN(Application::onEvent));
}

Application::~Application() { glfwTerminate(); }

void Application::onEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

    m_mainWindow->onEvent(event);
}

void Application::run() {
    while(m_continueRunning && !glfwWindowShouldClose(m_mainWindow->getWindow())) {
        m_mainWindow->onUpdate();
    }
}

bool Application::onWindowClose(WindowCloseEvent& event) {
    m_continueRunning = false;
    return false;
}
}