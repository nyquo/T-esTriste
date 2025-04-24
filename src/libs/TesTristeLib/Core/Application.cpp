#include "Application.hpp"

namespace TesTriste {

Application::Application(std::string mainWindowName)
  : m_mainWindow(std::make_unique<Window>("The app")) {
    m_mainWindow->setEventCallBack(BIND_EVENT_FN(Application::onEvent));
}

Application::~Application() { glfwTerminate(); }

void Application::onEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

    m_mainWindow->onEvent(e);
}

void Application::run() {
    while(m_continueRunning && !glfwWindowShouldClose(m_mainWindow->getWindow())) {
        m_mainWindow->onUpdate();
    }
}

bool Application::onWindowClose(WindowCloseEvent& e) {
    m_continueRunning = false;
    return false;
}
}